// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudPresence.h"
#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/JsonUtil.h"

namespace BrainCloud
{
	BrainCloudPresence::BrainCloudPresence(BrainCloudClient *client)
		: m_client(client)
	{
	}

	void BrainCloudPresence::forcePush(IServerCallback *callback)
	{
		Json::Value message;

		ServerCall *sc = new ServerCall(ServiceName::Presence, ServiceOperation::ForcePush, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudPresence::getPresenceOfFriends(const std::string &platform, bool includeOffline, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PresencePlatform.getValue()] = platform;
		message[OperationParam::PresenceIncludeOffline.getValue()] = includeOffline;

		ServerCall *sc = new ServerCall(ServiceName::Presence, ServiceOperation::GetPresenceOfFriends, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudPresence::getPresenceOfGroup(const std::string &groupId, bool includeOffline, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PresenceGroupId.getValue()] = groupId;
		message[OperationParam::PresenceIncludeOffline.getValue()] = includeOffline;

		ServerCall *sc = new ServerCall(ServiceName::Presence, ServiceOperation::GetPresenceOfGroup, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudPresence::getPresenceOfUsers(const std::vector<std::string> &profileIds, bool includeOffline, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PresenceProfileIds.getValue()] = JsonUtil::stringVectorToJson(profileIds);
		message[OperationParam::PresenceIncludeOffline.getValue()] = includeOffline;

		ServerCall *sc = new ServerCall(ServiceName::Presence, ServiceOperation::GetPresenceOfUsers, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudPresence::registerListenersForFriends(const std::string &platform, bool bidirectional, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PresencePlatform.getValue()] = platform;
		message[OperationParam::PresenceBidirectional.getValue()] = bidirectional;

		ServerCall *sc = new ServerCall(ServiceName::Presence, ServiceOperation::RegisterListenersForFriends, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudPresence::registerListenersForGroup(const std::string &groupId, bool bidirectional, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PresenceGroupId.getValue()] = groupId;
		message[OperationParam::PresenceBidirectional.getValue()] = bidirectional;

		ServerCall *sc = new ServerCall(ServiceName::Presence, ServiceOperation::RegisterListenersForGroup, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudPresence::registerListenersForProfiles(const std::vector<std::string> &profileIds, bool bidirectional, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PresenceProfileIds.getValue()] = JsonUtil::stringVectorToJson(profileIds);
		message[OperationParam::PresenceBidirectional.getValue()] = bidirectional;

		ServerCall *sc = new ServerCall(ServiceName::Presence, ServiceOperation::RegisterListenersForProfiles, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudPresence::setVisibility(bool visible, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PresenceVisible.getValue()] = visible;

		ServerCall *sc = new ServerCall(ServiceName::Presence, ServiceOperation::SetVisibility, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudPresence::stopListening(IServerCallback *callback)
	{
		Json::Value message;

		ServerCall *sc = new ServerCall(ServiceName::Presence, ServiceOperation::StopListening, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudPresence::updateActivity(const std::string &jsonActivity, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PresenceActivity.getValue()] = JsonUtil::jsonStringToValue(jsonActivity.c_str());

		ServerCall *sc = new ServerCall(ServiceName::Presence, ServiceOperation::UpdateActivity, message, callback);
		m_client->sendRequest(sc);
	}
};
