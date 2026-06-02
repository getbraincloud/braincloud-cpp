// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>
#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud
{
    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudPlayerStatisticsEvent
    {
    public:
        BrainCloudPlayerStatisticsEvent(BrainCloudClient* client);

        /**
         * Trigger an event server side that will increase the user's statistics.
         * This may cause one or more awards to be sent back to the user -
         * could be achievements, experience, etc. Achievements will be sent by this
         * client library to the appropriate awards service (Apple Game Center, etc).
         *
         * This mechanism supercedes the PlayerStatisticsService API methods, since
         * PlayerStatisticsService API method only update the raw statistics without
         * triggering the rewards.
         *
         * Service Name - playerStatisticsEvent
         * Service Operation - TRIGGER
         *
         * @see BrainCloudPlayerStatistics
         *
         * @param callback The method to be invoked when the server response is received
         */
        void triggerStatsEvent(const char *eventName, int32_t eventMultiplier,
                               IServerCallback *callback = NULL);

        /**
         * See documentation for TriggerStatisticsEvent for more
         * documentation.
         *
         * Service Name - playerStatisticsEvent
         * Service Operation - TRIGGER_MULTIPLE
         *
         * @param jsonData
         *   [
         *     {
         *       "eventName": "event1",
         *       "eventMultiplier": 1
         *     },
         *     {
         *       "eventName": "event2",
         *       "eventMultiplier": 1
         *     }
         *   ]
         */
        void triggerStatsEvents(const std::string &jsonData, IServerCallback *callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
