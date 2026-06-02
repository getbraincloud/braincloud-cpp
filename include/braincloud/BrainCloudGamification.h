// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>
#include <vector>
#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud {

    class BrainCloudClient;
    class IServerCallback;

    class BrainCloudGamification {

    public:
        BrainCloudGamification(BrainCloudClient* client);

        /**
         * Method retrieves all gamification data for the player.
         *
         * Service Name - Gamification
         * Service Operation - READ
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readAllGamification(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         * Method retrieves all milestones defined for the game.
         *
         * Service Name - Gamification
         * Service Operation - READ_MILESTONES
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readMilestones(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         * Read all of the achievements defined for the game.
         *
         * Service Name - Gamification
         * Service Operation - READ_ACHIEVEMENTS
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readAchievements(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         * Method returns all defined xp levels and any rewards associated
         * with those xp levels.
         *
         * Service Name - Gamification
         * Service Operation - READ_XP_LEVELS
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readXpLevelsMetaData(IServerCallback * callback = NULL);

        /**
        * Method retrives the list of achieved achievements.
        *
        * Service Name - Gamification
        * Service Operation - READ_ACHIEVED_ACHIEVEMENTS
        *
        * @param callback Method to be invoked when the server response is received.
        */
        void readAchievedAchievements(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         * Method retrieves the list of completed milestones.
         *
         * Service Name - Gamification
         * Service Operation - READ_COMPLETED_MILESTONES
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readCompletedMilestones(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         * Method retrieves the list of in progress milestones
         *
         * Service Name - Gamification
         * Service Operation - READ_IN_PROGRESS_MILESTONES
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readInProgressMilestones(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
        * Method retrieves milestones of the given category.
        *
        * Service Name - Gamification
        * Service Operation - READ_MILESTONES_BY_CATEGORY
        *
        * @param category The milestone category
        * @param callback Method to be invoked when the server response is received.
        */
        void readMilestonesByCategory(const char * category, bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
        * Method will award the achievements specified.
        *
        * Service Name - Gamification
        * Service Operation - AWARD_ACHIEVEMENTS
        *
        * @param achievementIds Collection of achievement ids to award
        * @param callback Method to be invoked when the server response is received.
        */
        void awardAchievements(const std::vector<std::string> & achievements, IServerCallback * callback = NULL);

        /**
         * Method retrieves all of the quests defined for the game.
         *
         * Service Name - Gamification
         * Service Operation - READ_QUESTS
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readQuests(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         *  Method returns all completed quests.
         *
         * Service Name - Gamification
         * Service Operation - READ_COMPLETED_QUESTS
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readCompletedQuests(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         *  Method returns quests that are in progress.
         *
         * Service Name - Gamification
         * Service Operation - READ_IN_PROGRESS_QUESTS
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readInProgressQuests(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         *  Method returns quests that have not been started.
         *
         * Service Name - Gamification
         * Service Operation - READ_NOT_STARTED_QUESTS
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readNotStartedQuests(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         *  Method returns quests with a status.
         *
         * Service Name - Gamification
         * Service Operation - READ_QUESTS_WITH_STATUS
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readQuestsWithStatus(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         *  Method returns quests with a basic percentage.
         *
         * Service Name - Gamification
         * Service Operation - READ_QUESTS_WITH_BASIC_PERCENTAGE
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readQuestsWithBasicPercentage(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         *  Method returns quests with a complex percentage.
         *
         * Service Name - Gamification
         * Service Operation - READ_QUESTS_WITH_COMPLEX_PERCENTAGE
         *
         * @param callback Method to be invoked when the server response is received.
         */
        void readQuestsWithComplexPercentage(bool includeMetaData = false, IServerCallback * callback = NULL);

        /**
         * Method returns quests for the given category.
         *
         * Service Name - Gamification
         * Service Operation - READ_QUESTS_BY_CATEGORY
         *
         * @param category The quest category
         * @param callback Method to be invoked when the server response is received.
         */
        void readQuestsByCategory(const char * category, bool includeMetaData = false, IServerCallback * callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
