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

    /**
     * This class is used to directly update the user's statistics registered on the server.
     *  However the preferred mechanism is to use "PlayerStatisticsEvents" via the
     *  BrainCloud.triggerStatisticsEvent() method. Using that method allows achivements,
     *  experience, and other award to be triggered in a manner set up on the server.
     */
    class BrainCloudPlayerStatistics
    {
    public:
        BrainCloudPlayerStatistics(BrainCloudClient *client);

        /**
         * Read all available user statistics.
         *
         * Service Name - PlayerStatistics
         * Service Operation - Read
         *
         * @param callback The method to be invoked when the server response is received
         */
        void readAllUserStats(IServerCallback *callback = NULL);

        /**
         * Reads a subset of user statistics as defined by the input collection.
         *
         * Service Name - PlayerStatistics
         * Service Operation - ReadSubset
         *
         * @param statistics A collection containing the subset of statistics to read:
         * ex. [ "pantaloons", "minions" ]
         * @param callback The method to be invoked when the server response is received
         */
        void readUserStatsSubset(const std::vector<std::string> &statistics, IServerCallback *callback = NULL);

        /**
         * Method retrieves the user statistics for the given category.
         *
         * Service Name - PlayerStatistics
         * Service Operation - READ_FOR_CATEGORY
         *
         * @param category The user statistics category
         * @param callback Method to be invoked when the server response is received.
         */
        void readUserStatsForCategory(const char *category, IServerCallback *callback = NULL);

        /**
         * Reset all of the statistics for this user back to their initial value.
         *
         * Service Name - PlayerStatistics
         * Service Operation - Reset
         *
         * @param callback The method to be invoked when the server response is received
         */
        void resetAllUserStats(IServerCallback *callback = NULL);

        /**
         * Atomically increment (or decrement) user statistics.
         * Any rewards that are triggered from user statistic increments
         * will be considered. User statistics are defined through the brainCloud portal.
         * Note also that the "xpCapped" property is returned (true/false depending on whether
         * the xp cap is turned on and whether the user has hit it).
         *
         * Service Name - PlayerStatistics
         * Service Operation - Update
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
         * @param callback The method to be invoked when the server response is received
         */
        void incrementUserStats(const std::string &jsonData, IServerCallback *callback = NULL);

        /**
         * Returns JSON representing the next experience level for the user.
         *
         * Service Name - PlayerStatistics
         * Service Operation - ReadNextXpLevel
         *
         * @param callback The method to be invoked when the server response is received
         */
        void getNextExperienceLevel(IServerCallback *callback);

        /**
         * Increments the user's experience. If the user goes up a level,
         * the new level details will be returned along with a list of rewards.
         *
         * Service Name - PlayerStatistics
         * Service Operation - UpdateIncrement
         *
         * @param xpValue The amount to increase the user's experience by
         * @param callback The method to be invoked when the server response is received
         */
        void incrementExperiencePoints(int xpValue, IServerCallback *callback);

        /**
         * Sets the user's experience to an absolute value. Note that this
         * is simply a set and will not reward the user if their level changes
         * as a result.
         *
         * Service Name - PlayerStatistics
         * Service Operation - SetXpPoints
         *
         * @param xpValue The amount to set the the user's experience to
         * @param callback The method to be invoked when the server response is received
         */
        void setExperiencePoints(int xpValue, IServerCallback *callback);

        /**
         * Apply statistics grammar to a partial set of statistics.
         *
         * Service Name - PlayerStatistics
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
        void processStatistics(const std::string &jsonData, IServerCallback *callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
