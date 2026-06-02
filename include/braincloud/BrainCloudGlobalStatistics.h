// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>
#include <vector>
#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud
{
    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudGlobalStatistics
    {
    public:
        BrainCloudGlobalStatistics(BrainCloudClient* client);

        /**
         * Method returns all of the global statistics.
         *
         * Service Name - globalGameStatistics
         * Service Operation - READ
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readAllGlobalStats(IServerCallback * callback = NULL);

        /**
         * Reads a subset of global statistics as defined by the input collection.
         *
         * Service Name - globalGameStatistics
         * Service Operation - READ_SUBSET
         *
         * @param statistics A collection containing the statistics to read:
         * [
         *   "Level01_TimesBeaten",
         *   "Level02_TimesBeaten"
         * ]
         * @param callback Method to be invoked when the server response is received.
         */
        void readGlobalStatsSubset(const std::vector<std::string> & statistics, IServerCallback * callback = NULL);

        /**
         * Method retrieves the global statistics for the given category.
         *
         * Service Name - globalGameStatistics
         * Service Operation - READ_FOR_CATEGORY
         *
         * @param category The global statistics category
         * @param callback Method to be invoked when the server response is received.
         */
        void readGlobalStatsForCategory(const char * category, IServerCallback * callback = NULL);

        /**
         * Atomically increment (or decrement) global statistics.
         * Global statistics are defined through the brainCloud portal.
         *
         * Service Name - globalGameStatistics
         * Service Operation - UPDATE_INCREMENT
         *
         * @param jsonData The JSON encoded data to be sent to the server as follows:
         * {
         *   stat1: 10,
         *   stat2: -5.5,
         * }
         * would increment stat1 by 10 and decrement stat2 by 5.5.
         * For the full statistics grammer see the api.braincloudservers.com site.
         * There are many more complex operations supported such as:
         * {
         *   stat1:INC_TO_LIMIT#9#30
         * }
         * which increments stat1 by 9 up to a limit of 30.
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void incrementGlobalGameStat(const std::string& jsonData, IServerCallback * callback = NULL);

		/**
		* Apply statistics grammar to a partial set of statistics.
		*
		* Service Name - globalGameStatistics
		* Service Operation - PROCESS_STATISTICS
		*
		* @param jsonData The JSON format is as follows:
		* {
		*     "DEAD_CATS": "RESET",
		*     "LIVES_LEFT": "SET#9",
		*     "MICE_KILLED": "INC#2",
		*     "DOG_SCARE_BONUS_POINTS": "INC#10",
		*     "TREES_CLIMBED": 1
		* }
		* @param callback Method to be invoked when the server response is received.
		*/
		void processStatistics(const std::string& jsonData, IServerCallback * callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
