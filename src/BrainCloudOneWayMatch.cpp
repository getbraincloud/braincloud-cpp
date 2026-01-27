// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudOneWayMatch.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

namespace BrainCloud
{
    BrainCloudOneWayMatch::BrainCloudOneWayMatch(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudOneWayMatch::startMatch(const char *otherPlayerId, int32_t rangeDelta, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::OneWayMatchServicePlayerId.getValue()] = otherPlayerId;
        message[OperationParam::OneWayMatchServiceRangeDelta.getValue()] = rangeDelta;

        ServerCall *sc = new ServerCall(ServiceName::OneWayMatch, ServiceOperation::StartMatch, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudOneWayMatch::cancelMatch(const char *playbackStreamId, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::OneWayMatchServicePlaybackStreamId.getValue()] = playbackStreamId;

        ServerCall *sc = new ServerCall(ServiceName::OneWayMatch, ServiceOperation::CancelMatch, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudOneWayMatch::completeMatch(const char *playbackStreamId, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::OneWayMatchServicePlaybackStreamId.getValue()] = playbackStreamId;

        ServerCall *sc = new ServerCall(ServiceName::OneWayMatch, ServiceOperation::CompleteMatch, message, callback);
        m_client->sendRequest(sc);
    }
}