// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudMatchmaking.h"

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
    BrainCloudMatchmaking::BrainCloudMatchmaking(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudMatchmaking::read(IServerCallback *callback)
    {
        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::Read, Json::nullValue, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::setPlayerRating(int32_t playerRating, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::MatchMakingServicePlayerRating.getValue()] = Json::Value(playerRating);

        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::SetPlayerRating, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::resetPlayerRating(IServerCallback *callback)
    {
        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::ResetPlayerRating, Json::nullValue, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::incrementPlayerRating(int32_t increment, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::MatchMakingServicePlayerRating.getValue()] = increment;

        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::IncrementPlayerRating, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::decrementPlayerRating(int32_t decrement, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::MatchMakingServicePlayerRating.getValue()] = decrement;

        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::DecrementPlayerRating, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::turnShieldOn(IServerCallback *callback)
    {
        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::ShieldOn, Json::nullValue, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::turnShieldOnFor(int32_t minutes, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::MatchMakingServiceMinutes.getValue()] = minutes;

        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::ShieldOnFor, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::incrementShieldOnFor(int32_t minutes, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::MatchMakingServiceMinutes.getValue()] = minutes;

        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::IncrementShieldOnFor, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::turnShieldOff(IServerCallback *callback)
    {
        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::ShieldOff, Json::nullValue, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::getShieldExpiry(const char *playerId, IServerCallback *callback)
    {
        Json::Value message;
        if (StringUtil::IsOptionalParameterValid(playerId))
        {
            message[OperationParam::MatchMakingServicePlayerId.getValue()] = playerId;
        }

        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::GetShieldExpiry, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::findPlayers(int32_t rangeDelta, int32_t numMatches, IServerCallback *callback)
    {
        findPlayersWithAttributes(rangeDelta, numMatches, "", callback);
    }

    void BrainCloudMatchmaking::findPlayersWithAttributes(int32_t rangeDelta, int32_t numMatches, std::string jsonAttributes, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::MatchMakingServiceRangeDelta.getValue()] = rangeDelta;
        message[OperationParam::MatchMakingServiceNumMatches.getValue()] = numMatches;

        if (StringUtil::IsOptionalParameterValid(jsonAttributes))
        {
            Json::Value jsonData = JsonUtil::jsonStringToValue(jsonAttributes);
            message[OperationParam::MatchMakingServiceAttributes.getValue()] = jsonData;
        }

        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::FindPlayers, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::findPlayersUsingFilter(int32_t rangeDelta, int32_t numMatches, std::string jsonExtraParms, IServerCallback *callback)
    {
        findPlayersWithAttributesUsingFilter(rangeDelta, numMatches, "", jsonExtraParms, callback);
    }

    void BrainCloudMatchmaking::findPlayersWithAttributesUsingFilter(
        int32_t rangeDelta,
        int32_t numMatches,
        std::string jsonAttributes,
        std::string jsonExtraParms,
        IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::MatchMakingServiceRangeDelta.getValue()] = rangeDelta;
        message[OperationParam::MatchMakingServiceNumMatches.getValue()] = numMatches;

        if (StringUtil::IsOptionalParameterValid(jsonAttributes))
        {
            Json::Value jsonData = JsonUtil::jsonStringToValue(jsonAttributes);
            message[OperationParam::MatchMakingServiceAttributes.getValue()] = jsonData;
        }

        if (StringUtil::IsOptionalParameterValid(jsonExtraParms))
        {
            Json::Value jsonData = JsonUtil::jsonStringToValue(jsonExtraParms);
            message[OperationParam::MatchMakingServiceExtraParams.getValue()] = jsonData;
        }

        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::FindPlayersUsingFilter, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::enableMatchMaking(IServerCallback *callback)
    {
        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::EnableMatchMaking, Json::nullValue, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudMatchmaking::disableMatchMaking(IServerCallback *callback)
    {
        ServerCall *sc = new ServerCall(ServiceName::MatchMaking, ServiceOperation::DisableMatchMaking, Json::nullValue, callback);
        m_client->sendRequest(sc);
    }
}