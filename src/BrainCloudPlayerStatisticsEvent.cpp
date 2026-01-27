// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudPlayerStatisticsEvent.h"

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
    BrainCloudPlayerStatisticsEvent::BrainCloudPlayerStatisticsEvent(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudPlayerStatisticsEvent::triggerStatsEvent(const char *eventName, int32_t eventMultiplier,
                                                            IServerCallback *callback)
    {
        Json::Value message;

        message[OperationParam::PlayerStatisticEventServiceEventName.getValue()] = eventName;
        message[OperationParam::PlayerStatisticEventServiceEventMultiplier.getValue()] = eventMultiplier;

        ServerCall *sc = new ServerCall(ServiceName::PlayerStatisticsEvent, ServiceOperation::Trigger, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerStatisticsEvent::triggerStatsEvents(const std::string &jsonData,
                                                             IServerCallback *callback)
    {
        Json::Value message;

        message[OperationParam::PlayerStatisticEventServiceEvents.getValue()] = JsonUtil::jsonStringToValue(jsonData);

        ServerCall *sc = new ServerCall(ServiceName::PlayerStatisticsEvent, ServiceOperation::TriggerMultiple, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }
}