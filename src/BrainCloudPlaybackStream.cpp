// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudPlaybackStream.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/JsonUtil.h"

namespace BrainCloud
{
    BrainCloudPlaybackStream::BrainCloudPlaybackStream(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudPlaybackStream::startStream(const char *targetPlayerId, bool includeSharedData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlaybackStreamServiceTargetPlayerId.getValue()] = targetPlayerId;
        message[OperationParam::PlaybackStreamServiceIncludeSharedData.getValue()] = includeSharedData;

        ServerCall *sc = new ServerCall(ServiceName::PlaybackStream, ServiceOperation::StartStream, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlaybackStream::readStream(const char *playbackStreamId, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlaybackStreamServicePlaybackStreamId.getValue()] = playbackStreamId;

        ServerCall *sc = new ServerCall(ServiceName::PlaybackStream, ServiceOperation::ReadStream, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlaybackStream::endStream(const char *playbackStreamId, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlaybackStreamServicePlaybackStreamId.getValue()] = playbackStreamId;

        ServerCall *sc = new ServerCall(ServiceName::PlaybackStream, ServiceOperation::EndStream, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlaybackStream::deleteStream(const char *playbackStreamId, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlaybackStreamServicePlaybackStreamId.getValue()] = playbackStreamId;

        ServerCall *sc = new ServerCall(ServiceName::PlaybackStream, ServiceOperation::DeleteStream, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlaybackStream::addEvent(const char *playbackStreamId, const char *jsonEventData, const char *jsonSummary, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlaybackStreamServicePlaybackStreamId.getValue()] = playbackStreamId;
        message[OperationParam::PlaybackStreamServiceEventData.getValue()] = JsonUtil::jsonStringToValue(jsonEventData);
        message[OperationParam::PlaybackStreamServiceSummary.getValue()] = JsonUtil::jsonStringToValue(jsonSummary);

        ServerCall *sc = new ServerCall(ServiceName::PlaybackStream, ServiceOperation::AddEvent, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlaybackStream::getRecentStreamsForInitiatingPlayer(const char *initiatingPlayerId, int maxNumStreams, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlaybackStreamServiceInitiatingPlayerId.getValue()] = initiatingPlayerId;
        message[OperationParam::PlaybackMaxNumberOfStreams.getValue()] = maxNumStreams;

        ServerCall *sc = new ServerCall(ServiceName::PlaybackStream, ServiceOperation::GetRecentStreamsForInitiatingPlayer, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlaybackStream::getRecentStreamsForTargetPlayer(const char *targetPlayerId, int maxNumStreams, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlaybackStreamServiceTargetPlayerId.getValue()] = targetPlayerId;
        message[OperationParam::PlaybackMaxNumberOfStreams.getValue()] = maxNumStreams;

        ServerCall *sc = new ServerCall(ServiceName::PlaybackStream, ServiceOperation::GetRecentStreamsForTargetPlayer, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlaybackStream::protectStreamUntil(const char *playbackStreamId, int numDays, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlaybackStreamServicePlaybackStreamId.getValue()] = playbackStreamId;
        data[OperationParam::PlaybackStreamServiceNumDays.getValue()] = numDays;

        ServerCall *sc = new ServerCall(ServiceName::PlaybackStream, ServiceOperation::ProtectStreamUntil, data, callback);
        m_client->sendRequest(sc);
    }
}
