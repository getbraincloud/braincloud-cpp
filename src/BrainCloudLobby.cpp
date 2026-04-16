// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudLobby.h"
#include "braincloud/BrainCloudClient.h"
#include "braincloud/reason_codes.h"
#include "braincloud/ServerCall.h"
#include "braincloud/http_codes.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/IPinger.h"
#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/JsonUtil.h"

#include <algorithm>
#include <iostream>
#include <memory>

static const int MAX_PING_CALLS = 4;
static const int MAX_PING_CALLS_PARRALLEL = 2;

static Json::Value serializePingData(const std::map<std::string, int> &pingData)
{
	Json::Value jsonPingData;
	for (const auto &kv : pingData)
	{
		jsonPingData[kv.first] = kv.second;
	}
	return jsonPingData;
}

namespace BrainCloud
{
	BrainCloudLobby::GetRegionsForLobbiesCallback::GetRegionsForLobbiesCallback(BrainCloudLobby *pBrainCloudLobby)
		: m_pBrainCloudLobby(pBrainCloudLobby), m_pExternalCallback(nullptr)
	{
	}

	void BrainCloudLobby::GetRegionsForLobbiesCallback::setExternalCallback(IServerCallback *callback)
	{
		m_pExternalCallback = callback;
	}

	void BrainCloudLobby::GetRegionsForLobbiesCallback::serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
	{
		m_pBrainCloudLobby->m_pingRegionsThread.stop(); // Make sure current ping process is halted so we dont accidentally fill new data inside m_pingData
		m_pBrainCloudLobby->m_pingData = {};			// Clear previous pings
		m_pBrainCloudLobby->m_pingRegions = {};			// Clear previous pings

		Json::Reader reader;
		Json::Value json;
		if (reader.parse(jsonData, json))
		{
			const auto &jsonRegionPingData = json["data"]["regionPingData"];
			if (!jsonRegionPingData.isNull())
			{
				auto regions = jsonRegionPingData.getMemberNames();
				for (const auto &regionName : regions)
				{
					const auto &jsonRegion = jsonRegionPingData[regionName];
					const auto &jsonType = jsonRegion["type"];
					const auto &target = jsonRegion["target"];
					const auto &targetTcp = jsonRegion["targetTcp"];
					if (target.isString() && jsonType.isString() && jsonType.asString() == "PING")
					{
						// Append the TCP relay port so the pinger tests the actual relay
						// endpoint rather than defaulting to port 80 (which may be firewalled).
						std::string pingTarget = target.asString();
						if (targetTcp.isString() && !targetTcp.asString().empty())
							pingTarget += ":" + targetTcp.asString();
						m_pBrainCloudLobby->m_pingRegions[regionName] = pingTarget;
					}
				}
			}
		}

		// Call user callback
		if (m_pExternalCallback)
		{
			m_pExternalCallback->serverCallback(serviceName, serviceOperation, jsonData);
		}
	}

	void BrainCloudLobby::GetRegionsForLobbiesCallback::serverError(ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string &jsonError)
	{
		if (m_pExternalCallback)
		{
			m_pExternalCallback->serverError(serviceName, serviceOperation, statusCode, reasonCode, jsonError);
		}
	}

	BrainCloudLobby::PingRegionsThread::PingRegionsThread(BrainCloudLobby *pBrainCloudLobby)
		: m_pBrainCloudLobby(pBrainCloudLobby)
	{
		m_isRunning = false;
	}

	BrainCloudLobby::PingRegionsThread::~PingRegionsThread()
	{
		stop();
	}

	bool BrainCloudLobby::PingRegionsThread::isRunning() const
	{
		return m_isRunning;
	}

	void BrainCloudLobby::PingRegionsThread::stop()
	{
		m_isRunning = false;
		m_condition.notify_one();
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

	class ActivePing final
	{
	public:
		ActivePing(std::string region, std::string url, std::mutex *pMutex, std::condition_variable *pCondition, BrainCloudClient *pClient)
			: m_region(region)
		{
			m_ping = -1; // -1 means it's still going

			std::thread thread([this, url, pMutex, pCondition, pClient]
							   {
				// Create our cross-platform pinger helper
				auto pPinger = std::unique_ptr<IPinger>(IPinger::create(pClient));

				// Ping as many times as required (MAX_PING_CALLS)
				std::vector<int> pings;
				auto fullURL = "http://" + url;
				for (int i = 0; i < MAX_PING_CALLS; ++i)
				{
					pings.push_back(pPinger->ping(fullURL));
				}

				// Sort results from faster to slowest
				std::sort(pings.begin(), pings.end());

				// Calculate the average, minus the slowest one (MAX_PING_CALLS - 1)
				int pingResult = 0;
				for (int i = 0; i < MAX_PING_CALLS - 1; ++i)
				{
					pingResult += pings[i];
				}
				pingResult /= MAX_PING_CALLS - 1;

				// Notify
				m_ping = pingResult;
				std::unique_lock<std::mutex> lock(*pMutex);
				pCondition->notify_one(); });
			thread.detach();
		}

		bool isFinished() const
		{
			return m_ping != -1;
		}

		int getPing() const
		{
			return m_ping;
		}

		const std::string &getRegion() const
		{
			return m_region;
		}

	private:
		std::string m_region;
		std::atomic<int> m_ping;
	};

	void BrainCloudLobby::PingRegionsThread::start(const std::map<std::string, std::string> &pingRegions)
	{
		m_isRunning = true;

		m_thread = std::thread([this](std::map<std::string, std::string> regionsToPing)
							   {
			std::vector<std::shared_ptr<ActivePing>> activePings;
			std::map<std::string, int> pingData;
			std::unique_lock<std::mutex> lock(m_mutex);
			while (m_isRunning)
			{
				// Make sure we have the desired active pings count in parrallel
				while (!regionsToPing.empty() && activePings.size() < MAX_PING_CALLS_PARRALLEL)
				{
					auto it = regionsToPing.begin();
					auto regionName = it->first;
					auto regionURL = it->second;
					regionsToPing.erase(it);
					activePings.push_back(std::make_shared<ActivePing>(regionName, regionURL, &m_mutex, &m_condition, m_pBrainCloudLobby->m_client));
				}

				// Check for completed active pings
				for (auto it = activePings.begin(); it != activePings.end();)
				{
					auto pActivePing = *it;
					if (pActivePing->isFinished())
					{
						if (m_pBrainCloudLobby->_loggingEnabled)
						{
							std::cout << "#PING " << pActivePing->getRegion() << " = " << pActivePing->getPing() << std::endl;
						}
						pingData[pActivePing->getRegion()] = pActivePing->getPing();
						// Publish partial result so the app can poll for per-region progress
						{
							std::lock_guard<std::mutex> dataLock(m_pBrainCloudLobby->m_pingDataMutex);
							m_pBrainCloudLobby->m_pingData = pingData;
						}
						it = activePings.erase(it);
						continue;
					}
					++it;
				}

				// Check if we completed all the regions
				if (regionsToPing.empty() && activePings.empty())
				{
					{
						std::lock_guard<std::mutex> dataLock(m_pBrainCloudLobby->m_pingDataMutex);
						m_pBrainCloudLobby->m_pingData = pingData;
					}
					m_isRunning = false;
					break;
				}
				
				// Otherwise, wait for an active ping to complete
				if (!activePings.empty())
				{
					m_condition.wait(lock);
				}
			} }, pingRegions);
	}

	BrainCloudLobby::BrainCloudLobby(BrainCloudClient *client)
		: m_client(client), m_getRegionsForLobbiesCallback(this), m_pingRegionsThread(this), m_pingCallback(nullptr)
	{
		_loggingEnabled = false;
	}

	void BrainCloudLobby::enableLogging(bool shouldEnable)
	{
		_loggingEnabled = shouldEnable;
	}

	void BrainCloudLobby::getRegionsForLobbies(const std::vector<std::string> &roomTypes, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyTypes.getValue()] = JsonUtil::stringVectorToJson(roomTypes);

		m_getRegionsForLobbiesCallback.setExternalCallback(callback);
		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::GetRegionsForLobbies, message, &m_getRegionsForLobbiesCallback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::pingRegions(IServerCallback *callback)
	{
		if (m_pingRegions.empty())
		{
			if (callback)
			{
				m_errorCallbackQueue.push_back({callback, ServiceName::Lobby, ServiceOperation::PingRegions, HTTP_BAD_REQUEST, MISSING_REQUIRED_PARAMETER, "Required message parameter 'pingData' is missing. Please ensure PingData exists by first calling GetRegionsForLobbies and PingRegions, and waiting for response before proceeding."});
			}
			return;
		}
		if (!m_pingRegionsThread.isRunning())
		{
			m_pingCallback = callback;
			m_pingRegionsThread.stop();
			m_pingRegionsThread.start(m_pingRegions);
		}
		else if (callback)
		{
			m_errorCallbackQueue.push_back({callback, ServiceName::Lobby, ServiceOperation::PingRegions, HTTP_BAD_REQUEST, MISSING_REQUIRED_PARAMETER, "'pingRegions' is already running. Please wait for callback before calling this again."});
		}
	}

	void BrainCloudLobby::runPingCallbacks()
	{
		// pingRegions callback
		if (!m_pingRegionsThread.isRunning() && m_pingCallback)
		{
			auto dataStr = serializePingData(m_pingData).toStyledString();
			if (_loggingEnabled)
			{
				std::cout << "#PING RESULTS " << dataStr << std::endl;
			}
			m_pingCallback->serverCallback(ServiceName::Lobby, ServiceOperation::PingRegions, dataStr);
			m_pingCallback = nullptr;
		}

		// Trigger delayed events
		for (auto &evt : m_errorCallbackQueue)
		{
			evt.callback->serverError(evt.serviceName, evt.serviceOperation, evt.statusCode, evt.reasonCode, evt.jsonError);
		}
		m_errorCallbackQueue.clear();
	}

	std::map<std::string, int> BrainCloudLobby::getPingData() const
	{
		std::lock_guard<std::mutex> lock(m_pingDataMutex);
		return m_pingData;
	}

	void BrainCloudLobby::createLobby(const std::string &lobbyType, int rating, const std::vector<std::string> &otherUserCxIds, bool isReady, const std::string &extraJson, const std::string &teamCode, const std::string &jsonSettings, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyType.getValue()] = lobbyType;
		message[OperationParam::Rating.getValue()] = rating;
		message[OperationParam::OtherUserCxIds.getValue()] = JsonUtil::stringVectorToJson(otherUserCxIds);
		message[OperationParam::IsReady.getValue()] = isReady;
		message[OperationParam::ExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson.c_str());
		message[OperationParam::TeamCode.getValue()] = teamCode;
		message[OperationParam::Settings.getValue()] = JsonUtil::jsonStringToValue(jsonSettings.c_str());

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::CreateLobby, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::createLobbyWithPingData(const std::string &lobbyType, int rating, const std::vector<std::string> &otherUserCxIds, bool isReady, const std::string &extraJson, const std::string &teamCode, const std::string &jsonSettings, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyType.getValue()] = lobbyType;
		message[OperationParam::Rating.getValue()] = rating;
		message[OperationParam::OtherUserCxIds.getValue()] = JsonUtil::stringVectorToJson(otherUserCxIds);
		message[OperationParam::IsReady.getValue()] = isReady;
		message[OperationParam::ExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson.c_str());
		message[OperationParam::TeamCode.getValue()] = teamCode;
		message[OperationParam::Settings.getValue()] = JsonUtil::jsonStringToValue(jsonSettings.c_str());

		attachPingDataAndSend(ServiceName::Lobby, ServiceOperation::CreateLobbyWithPingData, message, callback);
	}

	void BrainCloudLobby::findLobby(const std::string &lobbyType, int rating, int maxSteps, const std::string &jsonAlgo, const std::string &jsonFilter, const std::vector<std::string> &otherUserCxIds, bool isReady, const std::string &extraJson, const std::string &teamCode, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyType.getValue()] = lobbyType;
		message[OperationParam::Rating.getValue()] = rating;
		message[OperationParam::MaxSteps.getValue()] = maxSteps;
		message[OperationParam::Algo.getValue()] = JsonUtil::jsonStringToValue(jsonAlgo.c_str());
		message[OperationParam::Filter.getValue()] = JsonUtil::jsonStringToValue(jsonFilter.c_str());
		message[OperationParam::OtherUserCxIds.getValue()] = JsonUtil::stringVectorToJson(otherUserCxIds);
		message[OperationParam::IsReady.getValue()] = isReady;
		message[OperationParam::ExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson.c_str());
		message[OperationParam::TeamCode.getValue()] = teamCode;

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::FindLobby, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::findLobbyWithPingData(const std::string &lobbyType, int rating, int maxSteps, const std::string &jsonAlgo, const std::string &jsonFilter, const std::vector<std::string> &otherUserCxIds, bool isReady, const std::string &extraJson, const std::string &teamCode, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyType.getValue()] = lobbyType;
		message[OperationParam::Rating.getValue()] = rating;
		message[OperationParam::MaxSteps.getValue()] = maxSteps;
		message[OperationParam::Algo.getValue()] = JsonUtil::jsonStringToValue(jsonAlgo.c_str());
		message[OperationParam::Filter.getValue()] = JsonUtil::jsonStringToValue(jsonFilter.c_str());
		message[OperationParam::OtherUserCxIds.getValue()] = JsonUtil::stringVectorToJson(otherUserCxIds);
		message[OperationParam::IsReady.getValue()] = isReady;
		message[OperationParam::ExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson.c_str());
		message[OperationParam::TeamCode.getValue()] = teamCode;

		attachPingDataAndSend(ServiceName::Lobby, ServiceOperation::FindLobbyWithPingData, message, callback);
	}

	void BrainCloudLobby::attachPingDataAndSend(ServiceName serviceName, ServiceOperation serviceOperation, Json::Value &data, IServerCallback *callback)
	{
		if (m_pingData.empty())
		{
			if (callback)
			{
				m_errorCallbackQueue.push_back({callback, serviceName, serviceOperation, HTTP_BAD_REQUEST, MISSING_REQUIRED_PARAMETER, "Required message parameter 'pingData' is missing. Please ensure PingData exists by first calling GetRegionsForLobbies and PingRegions, and waiting for response before proceeding."});
			}
			return;
		}

		data[OperationParam::PingData.getValue()] = serializePingData(m_pingData);

		ServerCall *sc = new ServerCall(serviceName, serviceOperation, data, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::findOrCreateLobby(const std::string &lobbyType, int rating, int maxSteps, const std::string &jsonAlgo, const std::string &jsonFilter, const std::vector<std::string> &otherUserCxIds, const std::string &jsonSettings, bool isReady, const std::string &extraJson, const std::string &teamCode, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyType.getValue()] = lobbyType;
		message[OperationParam::Rating.getValue()] = rating;
		message[OperationParam::MaxSteps.getValue()] = maxSteps;
		message[OperationParam::Algo.getValue()] = JsonUtil::jsonStringToValue(jsonAlgo.c_str());
		message[OperationParam::Filter.getValue()] = JsonUtil::jsonStringToValue(jsonFilter.c_str());
		message[OperationParam::OtherUserCxIds.getValue()] = JsonUtil::stringVectorToJson(otherUserCxIds);
		message[OperationParam::Settings.getValue()] = JsonUtil::jsonStringToValue(jsonSettings.c_str());
		message[OperationParam::IsReady.getValue()] = isReady;
		message[OperationParam::ExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson.c_str());
		message[OperationParam::TeamCode.getValue()] = teamCode;

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::FindOrCreateLobby, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::findOrCreateLobbyWithPingData(const std::string &lobbyType, int rating, int maxSteps, const std::string &jsonAlgo, const std::string &jsonFilter, const std::vector<std::string> &otherUserCxIds, const std::string &jsonSettings, bool isReady, const std::string &extraJson, const std::string &teamCode, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyType.getValue()] = lobbyType;
		message[OperationParam::Rating.getValue()] = rating;
		message[OperationParam::MaxSteps.getValue()] = maxSteps;
		message[OperationParam::Algo.getValue()] = JsonUtil::jsonStringToValue(jsonAlgo.c_str());
		message[OperationParam::Filter.getValue()] = JsonUtil::jsonStringToValue(jsonFilter.c_str());
		message[OperationParam::OtherUserCxIds.getValue()] = JsonUtil::stringVectorToJson(otherUserCxIds);
		message[OperationParam::Settings.getValue()] = JsonUtil::jsonStringToValue(jsonSettings.c_str());
		message[OperationParam::IsReady.getValue()] = isReady;
		message[OperationParam::ExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson.c_str());
		message[OperationParam::TeamCode.getValue()] = teamCode;

		attachPingDataAndSend(ServiceName::Lobby, ServiceOperation::FindOrCreateLobbyWithPingData, message, callback);
	}

	void BrainCloudLobby::getLobbyData(const std::string &lobbyId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyId.getValue()] = lobbyId;

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::GetLobbyData, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::getLobbyInstances(const std::string &lobbyType, const std::string &criteriaJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyType.getValue()] = lobbyType;
		message[OperationParam::LobbyCriteria.getValue()] = JsonUtil::jsonStringToValue(criteriaJson.c_str());
		;

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::GetLobbyInstances, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::getLobbyInstancesWithPingData(const std::string &lobbyType, const std::string &criteriaJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyType.getValue()] = lobbyType;
		message[OperationParam::LobbyCriteria.getValue()] = JsonUtil::jsonStringToValue(criteriaJson.c_str());
		;

		attachPingDataAndSend(ServiceName::Lobby, ServiceOperation::GetLobbyInstancesWithPingData, message, callback);
	}

	void BrainCloudLobby::joinLobby(const std::string lobbyId, bool isReady, const std::string &extraJson, std::string teamCode,
									const std::vector<std::string> &otherUserCxIds, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyId.getValue()] = lobbyId;
		message[OperationParam::IsReady.getValue()] = isReady;
		message[OperationParam::ExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson);
		message[OperationParam::TeamCode.getValue()] = teamCode;
		message[OperationParam::OtherUserCxIds.getValue()] = JsonUtil::stringVectorToJson(otherUserCxIds);

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::JoinLobby, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::joinLobbyWithPingData(const std::string lobbyId, bool isReady, const std::string &extraJson, std::string teamCode,
												const std::vector<std::string> &otherUserCxIds, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyId.getValue()] = lobbyId;
		message[OperationParam::IsReady.getValue()] = isReady;
		message[OperationParam::ExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson);
		message[OperationParam::TeamCode.getValue()] = teamCode;
		message[OperationParam::OtherUserCxIds.getValue()] = JsonUtil::stringVectorToJson(otherUserCxIds);

		attachPingDataAndSend(ServiceName::Lobby, ServiceOperation::JoinLobbyWithPingData, message, callback);
	}

	void BrainCloudLobby::leaveLobby(const std::string &lobbyId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyId.getValue()] = lobbyId;

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::LeaveLobby, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::removeMember(const std::string &lobbyId, const std::string &cxId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyId.getValue()] = lobbyId;
		message[OperationParam::CxId.getValue()] = cxId;

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::RemoveMember, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::sendSignal(const std::string &lobbyId, const std::string &jsonSignalData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyId.getValue()] = lobbyId;
		message[OperationParam::SignalData.getValue()] = JsonUtil::jsonStringToValue(jsonSignalData.c_str());

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::SendSignal, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::switchTeam(const std::string &lobbyId, const std::string &toTeamCode, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyId.getValue()] = lobbyId;
		message[OperationParam::ToTeamCode.getValue()] = toTeamCode;

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::SwitchTeam, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::updateReady(const std::string &lobbyId, bool isReady, const std::string &extraJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyId.getValue()] = lobbyId;
		message[OperationParam::IsReady.getValue()] = isReady;
		message[OperationParam::ExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson.c_str());

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::UpdateReady, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudLobby::updateSettings(const std::string &lobbyId, const std::string &jsonSettings, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyId.getValue()] = lobbyId;
		message[OperationParam::Settings.getValue()] = JsonUtil::jsonStringToValue(jsonSettings.c_str());

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::UpdateSettings, message, callback);
		m_client->sendRequest(sc);
	}

	/// <summary>
	/// Cancel this members Find, Join and Searching of Lobbies
	/// </summary>
	void BrainCloudLobby::cancelFindRequest(const std::string &lobbyType, std::string entryId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LobbyType.getValue()] = lobbyType;
		message[OperationParam::EntryId.getValue()] = entryId;

		ServerCall *sc = new ServerCall(ServiceName::Lobby, ServiceOperation::CancelFindRequest, message, callback);
		m_client->sendRequest(sc);
	}
};
