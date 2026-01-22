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
        BrainCloudPlayerStatisticsEvent(BrainCloudClient *in_client);

        /**
         * Trigger a server-side event that will update the user's statistics.
         * This may cause one or more awards to be sent back to the user,
         * such as achievements, experience, or other rewards. Achievements
         * will be sent by this client library to the appropriate awards service
         * (e.g., Apple Game Center, Google Play Games, etc.).
         *
         * This mechanism supersedes the PlayerStatisticsService API methods,
         * which only update raw statistics without triggering rewards.
         *
         * Service Name - PlayerStatisticsEvent
         * Service Operation - Trigger
         *
         * @param in_eventName Name of the statistics event to trigger.
         * @param in_eventMultiplier Optional multiplier to apply to the event.
         * @param in_callback Callback invoked when the server response is received.
         *                    Defaults to nullptr if no callback is needed.
         * @see BrainCloudPlayerStatistics
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
         * @param in_callback The method to be invoked when the server response is received
         */
        void triggerStatsEvents(const std::string &in_jsonData, IServerCallback *in_callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
