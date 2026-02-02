// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudAsyncMatch.h"

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
	BrainCloudAsyncMatch::BrainCloudAsyncMatch(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudAsyncMatch::createMatch(const char *jsonOpponentIds,
										   const char *pushNotificationMessage,
										   IServerCallback *callback)
	{
		createMatchInternal(jsonOpponentIds,
							NULL,
							pushNotificationMessage,
							NULL,
							NULL,
							NULL,
							callback);
	}

	void BrainCloudAsyncMatch::createMatchWithInitialTurn(const char *jsonOpponentIds,
														  const char *jsonMatchState,
														  const char *pushNotificationMessage,
														  const char *nextPlayer,
														  const char *jsonSummary,
														  IServerCallback *callback)
	{
		createMatchInternal(jsonOpponentIds,
							StringUtil::IsOptionalParameterValid(jsonMatchState) ? jsonMatchState : "{}",
							pushNotificationMessage,
							NULL,
							nextPlayer,
							jsonSummary,
							callback);
	}

	void BrainCloudAsyncMatch::submitTurn(
		const char *ownerId,
		const char *matchId,
		uint64_t version,
		const char *jsonMatchState,
		const char *pushNotificationMessage,
		const char *nextPlayer,
		const char *jsonSummary,
		const char *jsonStatistics,
		IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServiceOwnerId.getValue()] = ownerId;
		message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;
		message[OperationParam::AsyncMatchServiceVersion.getValue()] = (Json::UInt64)version;

		if (StringUtil::IsOptionalParameterValid(jsonMatchState))
		{
			message[OperationParam::AsyncMatchServiceMatchState.getValue()] = JsonUtil::jsonStringToValue(jsonMatchState);
		}
		if (StringUtil::IsOptionalParameterValid(nextPlayer))
		{
			message[OperationParam::AsyncMatchServiceMatchStatus.getValue()][OperationParam::AsyncMatchServiceCurrentPlayer.getValue()] = nextPlayer;
		}
		if (StringUtil::IsOptionalParameterValid(jsonSummary))
		{
			message[OperationParam::AsyncMatchServiceMatchSummary.getValue()] = JsonUtil::jsonStringToValue(jsonSummary);
		}
		if (StringUtil::IsOptionalParameterValid(jsonStatistics))
		{
			message[OperationParam::AsyncMatchServiceStatistics.getValue()] = JsonUtil::jsonStringToValue(jsonStatistics);
		}
		if (StringUtil::IsOptionalParameterValid(pushNotificationMessage))
		{
			message[OperationParam::AsyncMatchServicePushMessage.getValue()] = pushNotificationMessage;
		}

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::SubmitTurn, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::updateMatchStateCurrentTurn(
		const char *ownerId,
		const char *matchId,
		uint64_t version,
		const char *jsonMatchState,
		const char *jsonStatistics,
		IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServiceOwnerId.getValue()] = ownerId;
		message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;
		message[OperationParam::AsyncMatchServiceVersion.getValue()] = (Json::UInt64)version;

		if (StringUtil::IsOptionalParameterValid(jsonMatchState))
		{
			message[OperationParam::AsyncMatchServiceMatchState.getValue()] = JsonUtil::jsonStringToValue(jsonMatchState);
		}
		if (StringUtil::IsOptionalParameterValid(jsonStatistics))
		{
			message[OperationParam::AsyncMatchServiceStatistics.getValue()] = JsonUtil::jsonStringToValue(jsonStatistics);
		}

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::UpdateMatchStateCurrentTurn, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::updateMatchSummaryData(const char *ownerId, const char *matchId, uint64_t version, const char *jsonSummary, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServiceOwnerId.getValue()] = ownerId;
		message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;
		message[OperationParam::AsyncMatchServiceVersion.getValue()] = (Json::UInt64)version;
		if (StringUtil::IsOptionalParameterValid(jsonSummary))
		{
			message[OperationParam::AsyncMatchServiceMatchSummary.getValue()] = JsonUtil::jsonStringToValue(jsonSummary);
		}

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::UpdateMatchSummary, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::completeMatch(const char *ownerId, const char *matchId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServiceOwnerId.getValue()] = ownerId;
		message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::Complete, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::readMatch(const char *ownerId, const char *matchId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServiceOwnerId.getValue()] = ownerId;
		message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::ReadMatch, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::readMatchHistory(const char *ownerId, const char *matchId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServiceOwnerId.getValue()] = ownerId;
		message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::ReadMatchHistory, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::findMatches(IServerCallback *callback)
	{
		Json::Value message;

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::FindMatches, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::findCompleteMatches(IServerCallback *callback)
	{
		Json::Value message;

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::FindMatchesCompleted, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::abandonMatch(const char *ownerId, const char *matchId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServiceOwnerId.getValue()] = ownerId;
		message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::Abandon, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::deleteMatch(const char *ownerId, const char *matchId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServiceOwnerId.getValue()] = ownerId;
		message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::DeleteMatch, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::createMatchInternal(const char *jsonOpponentIds,
												   const char *jsonMatchState,
												   const char *pushNotificationMessage,
												   const char *matchId,
												   const char *nextPlayer,
												   const char *jsonSummary,
												   IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServicePlayers.getValue()] = JsonUtil::jsonStringToValue(jsonOpponentIds);
		if (StringUtil::IsOptionalParameterValid(jsonMatchState))
		{
			message[OperationParam::AsyncMatchServiceMatchState.getValue()] = JsonUtil::jsonStringToValue(jsonMatchState);
		}
		if (StringUtil::IsOptionalParameterValid(matchId))
		{
			message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;
		}
		if (StringUtil::IsOptionalParameterValid(nextPlayer))
		{
			message[OperationParam::AsyncMatchServiceMatchStatus.getValue()][OperationParam::AsyncMatchServiceCurrentPlayer.getValue()] = nextPlayer;
		}
		if (StringUtil::IsOptionalParameterValid(jsonSummary))
		{
			message[OperationParam::AsyncMatchServiceMatchSummary.getValue()] = JsonUtil::jsonStringToValue(jsonSummary);
		}
		if (StringUtil::IsOptionalParameterValid(pushNotificationMessage))
		{
			message[OperationParam::AsyncMatchServicePushMessage.getValue()] = pushNotificationMessage;
		}

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::Create, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::completeMatchWithSummaryData(const char *ownerId, const char *matchId, const char *pushContent, const std::string &summary, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServiceOwnerId.getValue()] = ownerId;
		message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;
		if (pushContent != NULL)
		{
			message[OperationParam::AsyncMatchServicePushMessage.getValue()] = pushContent;
		}
		message[OperationParam::AsyncMatchServiceMatchSummary.getValue()] = JsonUtil::jsonStringToValue(summary);

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::CompleteMatchWithSummaryData, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudAsyncMatch::abandonMatchWithSummaryData(const char *ownerId, const char *matchId, const char *pushContent, const std::string &summary, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AsyncMatchServiceOwnerId.getValue()] = ownerId;
		message[OperationParam::AsyncMatchServiceMatchId.getValue()] = matchId;
		if (pushContent != NULL)
		{
			message[OperationParam::AsyncMatchServicePushMessage.getValue()] = pushContent;
		}
		message[OperationParam::AsyncMatchServiceMatchSummary.getValue()] = JsonUtil::jsonStringToValue(summary);

		ServerCall *sc = new ServerCall(ServiceName::AsyncMatch, ServiceOperation::AbandonMatchWithSummaryData, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

} // end namespace
