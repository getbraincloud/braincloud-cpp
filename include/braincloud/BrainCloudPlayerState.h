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

    class BrainCloudPlayerState
    {
    public:
        BrainCloudPlayerState(BrainCloudClient *client);

        /**
         * Read the state of the currently logged in user.
         * This method returns a JSON object describing most of the
         * user's data: entities, statistics, level, currency.
         * Apps will typically call this method after authenticating to get an
         * up-to-date view of the user's data.
         *
         * Service Name - playerState
         * Service Operation - READ
         *
         * @param callback The method to be invoked when the server response is received
         */
        void readUserState(IServerCallback *callback, const char *entityTypeFilter = NULL);

        /**
         * Completely deletes the user record and all data fully owned
         * by the user. After calling this method, the user will need
         * to re-authenticate and create a new profile.
         * This is mostly used for debugging/qa.
         *
         * Service Name - playerState
         * Service Operation - FULL_PLAYER_RESET
         *
         * @param callback The method to be invoked when the server response is received
         */
        void deleteUser(IServerCallback *callback = NULL);

        /**
         * This method will delete *most* data for the currently logged in user.
         * Data which is not deleted includes: currency, credentials, and
         * purchase transactions. ResetUser is different from DeleteUser in that
         * the user record will continue to exist after the reset (so the user
         * does not need to re-authenticate).
         *
         * Service Name - playerState
         * Service Operation - GAME_DATA_RESET
         *
         * @param callback The method to be invoked when the server response is received
         */
        void resetUserState(IServerCallback *callback = NULL);

        /**
         * Logs user out of server.
         *
         * Service Name - playerState
         * Service Operation - LOGOUT
         *
         * @param callback The method to be invoked when the server response is received
         */
        void logout(IServerCallback *callback = NULL);

        /**
         * Sets the user's name.
         *
         * Service Name - playerState
         * Service Operation - UPDATE_NAME
         *
         * @param userName The name of the user
         * @param callback The method to be invoked when the server response is received
         */
        void updateUserName(const char *userName, IServerCallback *callback = NULL);

        /**
         * Updates the "friend summary data" associated with the logged in user.
         * Some operations will return this summary data. For instance the social
         * leaderboards will return the player's score in the leaderboard along
         * with the friend summary data. Generally this data is used to provide
         * a quick overview of the player without requiring a separate API call
         * to read their public stats or entity data.
         *
         * Service Name - playerState
         * Service Operation - UPDATE_SUMMARY
         *
         * @param jsonSummaryData A JSON string defining the summary data.
         * For example:
         * {
         *   "xp":123,
         *   "level":12,
         *   "highScore":45123
         * }
         * @param callback Method to be invoked when the server response is received.
         */
        void updateSummaryFriendData(const char *jsonSummaryData, IServerCallback *callback = NULL);

        /**
         * Retrieve the user's attributes.
         *
         * Service Name - playerState
         * Service Operation - GET_ATTRIBUTES
         *
         * @param callback The method to be invoked when the server response is received
         */
        void getAttributes(IServerCallback *callback = NULL);

        /**
         * Update user's attributes.
         *
         * Service Name - playerState
         * Service Operation - UPDATE_ATTRIBUTES
         *
         * @param jsonAttributes Single layer json string that is a set of key-value pairs
         * @param wipeExisting Whether to wipe existing attributes prior to update.
         * @param callback The method to be invoked when the server response is received
         */
        void updateAttributes(const std::string &jsonAttributes, bool wipeExisting, IServerCallback *callback = NULL);

        /**
         * Remove user's attributes.
         *
         * Service Name - playerState
         * Service Operation - REMOVE_ATTRIBUTES
         *
         * @param attributeNames Collection of attribute names.
         * @param callback The method to be invoked when the server response is received
         */
        void removeAttributes(const std::vector<std::string> &attributeNames, IServerCallback *callback = NULL);

        /**
         * Sets the user's timezone offset.
         *
         * Service Name - playerState
         * Service Operation - UPDATE_TIMEZONE_OFFSET
         *
         * @param timeZoneOffset The numeric timezone offset of the user
         * @param callback The method to be invoked when the server response is received
         */
        void updateTimeZoneOffset(int32_t timeZoneOffset, IServerCallback *callback = NULL);

        /**
         * Update user's language code preference on their profile.
         *
         * Service Name - playerState
         * Service Operation - UPDATE_LANGUAGE_CODE
         *
         * @param languageCode A string-based language code
         * @param callback The method to be invoked when the server response is received
         */
        void updateLanguageCode(const std::string &languageCode, IServerCallback *callback = NULL);

        /**
         * Update User picture URL.
         *
         * Service Name - playerState
         * Service Operation - UPDATE_PICTURE_URL
         *
         * @param pictureUrl URL to apply
         * @param callback The method to be invoked when the server response is received
         */
        void updateUserPictureUrl(const char *pictureUrl, IServerCallback *callback = NULL);

        /**
         * Update the user's contact email.
         * Note this is unrelated to email authentication.
         *
         * Service Name - playerState
         * Service Operation - UPDATE_CONTACT_EMAIL
         *
         * @param contactEmail Updated email
         * @param callback The method to be invoked when the server response is received
         */
        void updateContactEmail(const char *contactEmail, IServerCallback *callback = NULL);

        /**
         * Delete's the specified status
         *
         * Service Name - playerState
         * Service Operation - CLEAR_USER_STATUS
         *
         * @param statusName Updated email
         * @param callback The method to be invoked when the server response is received
         */
        void clearUserStatus(const char *statusName, IServerCallback *callback = NULL);

        /**
         * Stack user's statuses
         *
         * Service Name - playerState
         * Service Operation - EXTEND_USER_STATUS
         *
         * @param statusName Updated email
         * @param additionalSecs Add time to existing expiry time.
         * @param details Json String to add additional details.
         * @param callback The method to be invoked when the server response is received
         */
        void extendUserStatus(const char *statusName, int32_t additionalSecs, const std::string &details, IServerCallback *callback = NULL);

        /**
         * Get user status
         *
         * Service Name - playerState
         * Service Operation - GET_USER_STATUS
         *
         * @param statusName Updated email
         * @param callback The method to be invoked when the server response is received
         */
        void getUserStatus(const char *statusName, IServerCallback *callback = NULL);

        /**
         * Set timed status for a user
         *
         * Service Name - playerState
         * Service Operation - SET_USER_STATUS
         *
         * @param statusName Updated email
         * @param durationSecs An active duration.
         * @param details Json String to add additional details.
         * @param callback The method to be invoked when the server response is received
         */
        void setUserStatus(const char *statusName, int32_t durationSecs, const std::string &details, IServerCallback *callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
