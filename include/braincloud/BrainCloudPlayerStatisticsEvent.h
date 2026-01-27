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
        BrainCloudPlayerStatisticsEvent(BrainCloudClient *client);

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
         * @param eventName Name of the statistics event to trigger.
         * @param eventMultiplier Optional multiplier to apply to the event.
         * @param callback Callback invoked when the server response is received.
         *                    Defaults to nullptr if no callback is needed.
         * @see BrainCloudPlayerStatistics
         */
        void triggerStatsEvent(const char *eventName, int32_t eventMultiplier,
                               IServerCallback *callback = NULL);

        /**
         * See documentation for TriggerStatisticsEvent for more
         * documentation.
         *
         * Service Name - PlayerStatisticsEvent
         * Service Operation - TriggerMultiple
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
         * @param callback The method to be invoked when the server response is received
         */
        void triggerStatsEvents(const std::string &jsonData, IServerCallback *callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
