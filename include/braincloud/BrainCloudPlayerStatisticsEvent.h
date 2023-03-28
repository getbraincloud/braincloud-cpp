// Copyright 2016 bitHeads, Inc. All Rights Reserved.

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
        BrainCloudPlayerStatisticsEvent(BrainCloudClient* in_client);

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
         * Service Name - PlayerStatisticsEvent
         * Service Operation - Trigger
         *
         * @see BrainCloudPlayerStatistics
         *
         * @param in_callback The method to be invoked when the server response is received
         */
        void triggerStatsEvent(const char *in_eventName, int32_t in_eventMultiplier,
                               IServerCallback *in_callback = NULL);

        /**
         * See documentation for TriggerStatisticsEvent for more
         * documentation.
         *
         * Service Name - PlayerStatisticsEvent
         * Service Operation - TriggerMultiple
         *
         * @param in_jsonData
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
        void triggerStatsEvents(const std::string &in_jsonData, IServerCallback *in_callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
