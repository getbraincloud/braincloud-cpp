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
	enum SocialLeaderboardType
	{
		HIGH_VALUE,
		CUMULATIVE,
		LAST_VALUE,
		LOW_VALUE
	};

	enum RotationType
	{
		NEVER,
		DAILY,
		WEEKLY,
		MONTHLY,
		YEARLY
	};

	enum FetchType
	{
		HIGHEST_RANKED
	};

	enum SortOrder
	{
		HIGH_TO_LOW,
		LOW_TO_HIGH
	};

	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudSocialLeaderboard
	{
	public:
		BrainCloudSocialLeaderboard(BrainCloudClient *client);

		/**
		 * Method returns the social leaderboard. A player's social leaderboard is
		 * comprised of players who are recognized as being your friend.
		 *
		 * The getSocialLeaderboard will retrieve all friends from all friend platforms, so
		 * - all external friends (Facebook, Steam, PlaystationNetwork)
		 * - all internal friends (brainCloud)
		 * - plus "self".
		 *
		 * Leaderboards entries contain the player's score and optionally, some user-defined
		 * data associated with the score. The currently logged in player will also
		 * be returned in the social leaderboard.
		 *
		 * Note: If no friends have played the game, the bestScore, createdAt, updatedAt
		 * will contain NULL.
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve
		 * @param replaceName If true, the currently logged in player's name will be replaced
		 * by the string "You".
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void getSocialLeaderboard(const char *leaderboardId, bool replaceName, IServerCallback *callback = NULL);

		/**
		 * Method returns the social leaderboard. A player's social leaderboard is
		 * comprised of players who are recognized as being your friend.
		 *
		 * This method returns the same data as getSocialLeaderboard, but it will not return an error if the leaderboard is not found.
		 *
		 * The method will retrieve all friends from all friend platforms, so
		 * - all external friends (Facebook, Steam, PlaystationNetwork)
		 * - all internal friends (brainCloud)
		 * - plus "self".
		 *
		 * Leaderboards entries contain the player's score and optionally, some user-defined
		 * data associated with the score. The currently logged in player will also
		 * be returned in the social leaderboard.
		 *
		 * Note: If no friends have played the game, the bestScore, createdAt, updatedAt
		 * will contain NULL.
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve
		 * @param replaceName If true, the currently logged in player's name will be replaced
		 * by the string "You".
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void getSocialLeaderboardIfExists(const char *leaderboardId, bool replaceName, IServerCallback *callback = NULL);

		/**
		 * Method returns the social leaderboard by its version. A player's social leaderboard is
		 * comprised of players who are recognized as being your friend.
		 *
		 * The getSocialLeaderboard will retrieve all friends from all friend platforms, so
		 * - all external friends (Facebook, Steam, PlaystationNetwork)
		 * - all internal friends (brainCloud)
		 * - plus "self".
		 *
		 * Leaderboards entries contain the player's score and optionally, some user-defined
		 * data associated with the score. The currently logged in player will also
		 * be returned in the social leaderboard.
		 *
		 * Note: If no friends have played the game, the bestScore, createdAt, updatedAt
		 * will contain NULL.
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve
		 * @param replaceName If true, the currently logged in player's name will be replaced
		 * by the string "You".
		 * @param versionId the version of the leaderboard
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void getSocialLeaderboardByVersion(const char *leaderboardId, bool replaceName, int versionId, IServerCallback *callback = NULL);

		/**
		 * Method returns the social leaderboard by its version. A player's social leaderboard is
		 * comprised of players who are recognized as being your friend.
		 *
		 * This method returns the same data as getSocialLeaderboardByVersion, but it will not return an error if the leaderboard is not found.
		 *
		 * The method will retrieve all friends from all friend platforms, so
		 * - all external friends (Facebook, Steam, PlaystationNetwork)
		 * - all internal friends (brainCloud)
		 * - plus "self".
		 *
		 * Leaderboards entries contain the player's score and optionally, some user-defined
		 * data associated with the score. The currently logged in player will also
		 * be returned in the social leaderboard.
		 *
		 * Note: If no friends have played the game, the bestScore, createdAt, updatedAt
		 * will contain NULL.
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve
		 * @param replaceName If true, the currently logged in player's name will be replaced
		 * by the string "You".
		 * @param versionId the version of the leaderboard
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void getSocialLeaderboardByVersionIfExists(const char *leaderboardId, bool replaceName, int versionId, IServerCallback *callback = NULL);

		/**
		 * Reads multiple social leaderboards.
		 *
		 * @param leaderboardIds Collection of leaderboard IDs.
		 * @param leaderboardResultCount Maximum count of entries to return for each leaderboard.
		 * @param replaceName If true, the currently logged in player's name will be replaced
		 * by the string "You".
		 * @param callback The method to be invoked when the server response is received
		 */
		void getMultiSocialLeaderboard(const std::vector<std::string> &leaderboardIds, int leaderboardResultCount, bool replaceName, IServerCallback *callback = NULL);

		/**
		 * Method returns a page of global leaderboard results.
		 *
		 * Leaderboards entries contain the player's score and optionally, some user-defined
		 * data associated with the score.
		 *
		 * Note: This method allows the client to retrieve pages from within the global leaderboard list
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GetGlobalLeaderboardPage
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve.
		 * @param sort Sort key Sort order of page.
		 * @param startIndex The index at which to start the page.
		 * @param endIndex The index at which to end the page.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardPage(
			const char *leaderboardId,
			SortOrder sortOrder,
			int startIndex,
			int endIndex,
			IServerCallback *callback = NULL);

		/**
		 * Method returns a page of global leaderboard results.
		 * Returns the same data as getGlobalLeaderboardPage, but does not return an error if the leaderboard does not exist.
		 *
		 * Leaderboards entries contain the player's score and optionally, some user-defined
		 * data associated with the score.
		 *
		 * Note: This method allows the client to retrieve pages from within the global leaderboard list
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GET_GLOBAL_LEADERBOARD_PAGE_IF_EXISTS
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve.
		 * @param sort Sort key Sort order of page.
		 * @param startIndex The index at which to start the page.
		 * @param endIndex The index at which to end the page.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardPageIfExists(
			const char *leaderboardId,
			SortOrder sortOrder,
			int startIndex,
			int endIndex,
			IServerCallback *callback = NULL);

		/**
		 * Method returns a page of global leaderboard results.
		 * By using a non-current version id, the user can retrieve a historical leaderboard.
		 * See GetGlobalLeaderboardVersions method to retrieve the version id.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GET_GLOBAL_LEADERBOARD_PAGE_BY_VERSION
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve.
		 * @param sort Sort key Sort order of page.
		 * @param startIndex The index at which to start the page.
		 * @param endIndex The index at which to end the page.
		 * @param versionId The historical version to retrieve.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardPageByVersion(
			const char *leaderboardId,
			SortOrder sortOrder,
			int startIndex,
			int endIndex,
			int versionId,
			IServerCallback *callback = NULL);

		/**
		 * Method returns a page of global leaderboard results.
		 * By using a non-current version id, the user can retrieve a historical leaderboard.
		 * See GetGlobalLeaderboardVersions method to retrieve the version id.
		 *
		 * This method returns the same data as getGlobalLeaderboardPageByVersion, but it will not return an error if the leaderboard does not exist
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GET_GLOBAL_LEADERBOARD_PAGE_BY_VERSION_IF_EXISTS
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve.
		 * @param sort Sort key Sort order of page.
		 * @param startIndex The index at which to start the page.
		 * @param endIndex The index at which to end the page.
		 * @param versionId The historical version to retrieve.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardPageByVersionIfExists(
			const char *leaderboardId,
			SortOrder sortOrder,
			int startIndex,
			int endIndex,
			int versionId,
			IServerCallback *callback = NULL);

		/**
		 * Method returns a view of global leaderboard results that centers on the current player.
		 *
		 * Leaderboards entries contain the player's score and optionally, some user-defined
		 * data associated with the score.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GET_GLOBAL_LEADERBOARD_VIEW
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve.
		 * @param sort Sort key Sort order of page.
		 * @param beforeCount The count of number of players before the current player to include.
		 * @param afterCount The count of number of players after the current player to include.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardView(const char *leaderboardId, SortOrder sortOrder, int beforeCount, int afterCount, IServerCallback *callback = NULL);

		/**
		 * Method returns a view of global leaderboard results that centers on the current player.
		 * Returns the same data as getGlobalLeaderboardView, but will not return an error if the leaderboard does not exist.
		 *
		 * Leaderboards entries contain the player's score and optionally, some user-defined
		 * data associated with the score.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GET_GLOBAL_LEADERBOARD_VIEW_IF_EXISTS
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve.
		 * @param sort Sort key Sort order of page.
		 * @param beforeCount The count of number of players before the current player to include.
		 * @param afterCount The count of number of players after the current player to include.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardViewIfExists(const char *leaderboardId, SortOrder sortOrder, int beforeCount, int afterCount, IServerCallback *callback = NULL);

		/**
		 * Method returns a view of global leaderboard results that centers on the current player.
		 * By using a non-current version id, the user can retrieve a historical leaderboard.
		 * See GetGlobalLeaderboardVersions method to retrieve the version id.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GET_GLOBAL_LEADERBOARD_VIEW_BY_VERSION
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve.
		 * @param sort Sort key Sort order of page.
		 * @param beforeCount The count of number of players before the current player to include.
		 * @param afterCount The count of number of players after the current player to include.
		 * @param versionId The historical version to retrieve.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardViewByVersion(const char *leaderboardId, SortOrder sortOrder, int beforeCount, int afterCount, int versionId, IServerCallback *callback = NULL);

		/**
		 * Method returns a view of global leaderboard results that centers on the current player.
		 * By using a non-current version id, the user can retrieve a historical leaderboard.
		 * See GetGlobalLeaderboardVersions method to retrieve the version id.
		 *
		 * This method returns the same data as getGlobalLeaderboardViewByVersion, but it will not return an error if the leaderboard does not exist.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GET_GLOBAL_LEADERBOARD_VIEW_IF_EXISTS
		 *
		 * @param leaderboardId The id of the leaderboard to retrieve.
		 * @param sort Sort key Sort order of page.
		 * @param beforeCount The count of number of players before the current player to include.
		 * @param afterCount The count of number of players after the current player to include.
		 * @param versionId The historical version to retrieve.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardViewByVersionIfExists(const char *leaderboardId, SortOrder sortOrder, int beforeCount, int afterCount, int versionId, IServerCallback *callback = NULL);

		/** Gets the global leaderboard versions.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GetGlobalLeaderboardVersions
		 *
		 * @param leaderboardId The leaderboard
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardVersions(const char *leaderboardId, IServerCallback *callback = NULL);

		/**
		 * Post the players score to the given social leaderboard.
		 * You can optionally send a user-defined json string of data
		 * with the posted score. This string could include information
		 * relevant to the posted score.
		 *
		 * Note that the behaviour of posting a score can be modified in
		 * the brainCloud portal. By default, the server will only keep
		 * the player's best score.
		 *
		 * @param leaderboardId The leaderboard to post to
		 * @param score The score to post
		 * @param data Optional user-defined data to post with the score
		 * @param callback The method to be invoked when the server response is received
		 */
		void postScoreToLeaderboard(const char *leaderboardId, int64_t score, const std::string &jsonOtherData, IServerCallback *callback = NULL);

		/**
		 * Post the player's score to the given social leaderboard,
		 * dynamically creating the leaderboard if it does not exist yet.
		 * To create new leaderboard, configJson must specify leaderboardType, rotationType, resetAt, and retainedCount, at a minimum, with support to optionally specify an expiry in minutes.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - POST_SCORE_DYNAMIC_USING_CONFIG
		 *
		 * @param leaderboardId The leaderboard to post to.
		 * @param score A score to post.
		 * @param scoreData Optional user-defined data to post with the score.
		 * @param configJson Configuration for the leaderboard if it does not exist yet, specified as JSON object.
		 *                      Configuration fields supported are:
		 *                          'leaderboardType': Required. Type of leaderboard. Valid values are:
		 *                              'LAST_VALUE',
		 *                              'HIGH_VALUE',
		 *                              'LOW_VALUE',
		 *                              'CUMULATIVE',
		 *                              'ARCADE_HIGH',
		 *                              'ARCADE_LOW';
		 *                          'rotationType': Required. Type of rotation. Valid values are:
		 *                              'NEVER',
		 *                              'DAILY',
		 *                              'DAYS',
		 *                              'WEEKLY',
		 *                              'MONTHLY',
		 *                              'YEARLY';
		 *                          'numDaysToRotate': Required if 'DAYS' rotation type, with valid values between 2 and 14; otherwise, null;
		 *                          'resetAt': UTC timestamp, in milliseconds, at which to rotate the period. Always null if 'NEVER' rotation type;
		 *                          'retainedCount': Required. Number of rotations (versions) of the leaderboard to retain;
		 *                          'expireInMins': Optional. Duration, in minutes, before the leaderboard is to automatically expire.
		 * @param callback The method to be invoked when the server response is received.
		 */
		void postScoreToDynamicLeaderboardUsingConfig(
			const char *leaderboardId,
			int64_t score,
			const std::string &scoreData,
			const std::string &configJson,
			IServerCallback *callback = NULL);

		/**
		 * Post the players score to the given social leaderboard.
		 * Pass leaderboard config data to dynamically create if necessary.
		 * You can optionally send a user-defined json string of data
		 * with the posted score. This string could include information
		 * relevant to the posted score. Uses UTC time in milliseconds since epoch
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - PostScoreDynamic
		 *
		 * @param leaderboardId The leaderboard to post to
		 * @param score The score to post
		 * @param data Optional user-defined data to post with the score
		 * @param leaderboardType leaderboard type
		 * @param rotationType Type of rotation
		 * @param rotationResetUTC Date to start rotation calculations. uses UTC time in milliseconds since epoch
		 * @param retainedCount How many rotations to keep
		 * @param callback The method to be invoked when the server response is received
		 */
		void postScoreToDynamicLeaderboardUTC(
			const char *leaderboardId,
			int64_t score,
			const std::string &jsonData,
			SocialLeaderboardType leaderboardType,
			RotationType rotationType,
			int64_t rotationResetUTC,
			int retainedCount,
			IServerCallback *callback = NULL);

		/**
		 * Post the players score to the given social leaderboard.
		 * Pass leaderboard config data to dynamically create if necessary.
		 * You can optionally send a user-defined json string of data
		 * with the posted score. This string could include information
		 * relevant to the posted score. uses UTC time in milliseconds since epoch
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - PostScoreDynamic
		 *
		 * @param leaderboardId The leaderboard to post to
		 * @param score The score to post
		 * @param data Optional user-defined data to post with the score
		 * @param leaderboardType leaderboard type
		 * @param rotationResetUTC Date to start rotation calculations, uses UTC time in milliseconds since epoch
		 * @param retainedCount How many rotations to keep
		 * @param numDaysToRotate How many days between each rotation
		 * @param callback The method to be invoked when the server response is received
		 */
		void postScoreToDynamicLeaderboardDaysUTC(
			const char *leaderboardId,
			int64_t score,
			const std::string &jsonData,
			SocialLeaderboardType leaderboardType,
			int64_t rotationResetUTC,
			int32_t retainedCount,
			int32_t numDaysToRotate,
			IServerCallback *callback = NULL);

		/**
		 * Removes a player's score from the leaderboard
		 *
		 * Service Name - leaderboard
		 * Service Operation - REMOVE_PLAYER_SCORE
		 *
		 * @param leaderboardId The leaderboard ID
		 * @param versionId The version of the leaderboard. Use -1 to specifiy the currently active leaderboard version
		 * @param callback The method to be invoked when the server response is received
		 */
		void removePlayerScore(const char *leaderboardId, int32_t versionId, IServerCallback *callback = NULL);

		/**
		 * Method to retrieve the player's completed tournaments for the game.
		 * Arguments:
		 *  leaderboardId: Name of the leaderboard
		 *  replaceName: True to replace the player's name with "You"; false otherwise.
		 *  callback: client-side callback for the results
		 */
		void playerTournamentReward(const char *eventName, uint64_t multiplier, IServerCallback *callback = NULL);

		/**
		 * Retrieve the social leaderboard for a group.
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_GROUP_SOCIAL_LEADERBOARD
		 *
		 * @param leaderboardId The leaderboard to retrieve
		 * @param groupId The ID of the group
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGroupSocialLeaderboard(const char *leaderboardId, const char *groupId, IServerCallback *callback = NULL);

		/**
		 * Retrieve the social leaderboard for a group by its version.
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_PLAYERS_SOCIAL_LEADERBOARD_BY_VERSION
		 *
		 * @param leaderboardId The leaderboard to retrieve
		 * @param groupId The ID of the group
		 * @param versionId the version of the leaderboard
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGroupSocialLeaderboardByVersion(const char *leaderboardId, const char *groupId, int versionId, IServerCallback *callback = NULL);

		/**
		 * Retrieve the social leaderboard for a list of players.
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_PLAYERS_SOCIAL_LEADERBOARD
		 *
		 * @param leaderboardId The leaderboard to retrieve
		 * @param profileIds The IDs of the players
		 * @param callback The method to be invoked when the server response is received
		 */
		void getPlayersSocialLeaderboard(const char *leaderboardId, std::vector<std::string> profileIds, IServerCallback *callback = NULL);

		/**
		 * Retrieve the social leaderboard for a list of players.
		 * This method returns the same data as getPlayersSocialLeaderboard, but it will not return an error if the leaderboard is not found.
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_PLAYERS_SOCIAL_LEADERBOARD_IF_EXISTS
		 *
		 * @param leaderboardId The leaderboard to retrieve
		 * @param profileIds The IDs of the players
		 * @param callback The method to be invoked when the server response is received
		 */
		void getPlayersSocialLeaderboardIfExists(const char *leaderboardId, std::vector<std::string> profileIds, IServerCallback *callback = NULL);

		/**
		 * Retrieve the social leaderboard for a list of players by its version.
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_PLAYERS_SOCIAL_LEADERBOARD
		 *
		 * @param leaderboardId The leaderboard to retrieve
		 * @param profileIds The IDs of the players
		 * @param callback The method to be invoked when the server response is received
		 */
		void getPlayersSocialLeaderboardByVersion(const char *leaderboardId, std::vector<std::string> profileIds, int versionId, IServerCallback *callback = NULL);

		/**
		 * Retrieve the social leaderboard for a list of players by its version.
		 * This method returns the same data as getPlayersSocialLeaderboardByVersion, but it will not return an error if the leaderboard is not found.
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_PLAYERS_SOCIAL_LEADERBOARD
		 *
		 * @param leaderboardId The leaderboard to retrieve
		 * @param profileIds The IDs of the players
		 * @param callback The method to be invoked when the server response is received
		 */
		void getPlayersSocialLeaderboardByVersionIfExists(const char *leaderboardId, std::vector<std::string> profileIds, int versionId, IServerCallback *callback = NULL);

		/**
		 * Retrieve a list of all leaderboards
		 *
		 * Service Name - leaderboard
		 * Service Operation - LIST_ALL_LEADERBOARDS
		 *
		 * @param callback The method to be invoked when the server response is received
		 */
		void listAllLeaderboards(IServerCallback *callback = NULL);

		/**
		 * Gets the number of entries in a global leaderboard
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_GLOBAL_LEADERBOARD_ENTRY_COUNT
		 *
		 * @param leaderboardId The leaderboard ID
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardEntryCount(const char *leaderboardId, IServerCallback *callback = NULL);

		/**
		 * Gets the number of entries in a global leaderboard
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_GLOBAL_LEADERBOARD_ENTRY_COUNT
		 *
		 * @param leaderboardId The leaderboard ID
		 * @param versionId The version of the leaderboard. Use -1 for current.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardEntryCountByVersion(const char *leaderboardId, int32_t versionId, IServerCallback *callback = NULL);

		/**
		 * Gets a player's score from a leaderboard
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_PLAYER_SCORE
		 *
		 * @param leaderboardId The leaderboard ID
		 * @param versionId The version of the leaderboard. Use -1 for current.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getPlayerScore(const char *leaderboardId, int32_t versionId, IServerCallback *callback = NULL);

		/**
		 * Gets a player's score from a leaderboard
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_PLAYER_SCORE
		 *
		 * @param leaderboardId The leaderboard ID
		 * @param versionId The version of the leaderboard. Use -1 for current.
		 * @param maxResults The max number of returned results
		 * @param callback The method to be invoked when the server response is received
		 */
		void getPlayerScores(const char *leaderboardId, int32_t versionId, int32_t maxResults, IServerCallback *callback = NULL);

		/**
		 * Gets a player's score from multiple leaderboards
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_PLAYER_SCORES_FROM_LEADERBOARDS
		 *
		 * @param type A collection of leaderboardIds to retrieve scores from
		 * @param callback The method to be invoked when the server response is received
		 */
		void getPlayerScoresFromLeaderboards(const std::vector<std::string> &leaderboardIds, IServerCallback *callback = NULL);

		static std::string leaderboardTypeToString(SocialLeaderboardType type);
		static std::string leaderboardRotationTypeToString(RotationType type);
		static std::string sortOrderToString(SortOrder sortOrder);

		/**
		 * Posts score to groups leaderboard - Note the user must be a member of the group
		 *
		 * Service Name - leaderboard
		 * Service Operation - POST_GROUP_SCORE
		 *
		 * @param leaderboardId A collection of leaderboardIds to retrieve scores from
		 * @param groupId the groups Id
		 * @param score the score you wish to post
		 * @param jsonData extra json Data
		 * @param callback The method to be invoked when the server response is received
		 */
		void postScoreToGroupLeaderboard(const char *leaderboardId, const char *groupId, int32_t score, const std::string &jsonData, IServerCallback *callback = NULL);

		/**
		 * Posts score to groups leaderboard - Note the user must be a member of the group
		 *
		 * Service Name - leaderboard
		 * Service Operation - POST_GROUP_SCORE
		 *
		 * @param leaderboardId A collection of leaderboardIds to retrieve scores from
		 * @param groupId the groups Id
		 * @param score the score you wish to post
		 * @param jsonData extra json Data
		 * @param callback The method to be invoked when the server response is received
		 */
		void postScoreToDynamicGroupLeaderboard(const char *leaderboardId, const char *groupId, int32_t score, const std::string &jsonData, IServerCallback *callback = NULL);

		/**
		 * Posts score to group leaderbopard and dynamically creates if necessary. leaderboardType, rotationReset, retainedCount and rotationType are required. uses UTC time in milliseconds since epoch
		 *
		 * Service Name - leaderboard
		 * Service Operation - POST_GROUP_SCORE_DYNAMIC
		 *
		 * @param leaderboardId the leaderboard to post to
		 * @param groupId the group's id
		 * @param score the score to post
		 * @param data optional user defined datat to post with scor
		 * @param leaderboardType type of leaderboard
		 * @param rotationType type of rotation
		 * @param rotationResetUTC uses UTC time in milliseconds since epoch
		 * @param retainedCount how long to keep rotation
		 * @param callback The method to be invoked when the server response is received
		 */
		void postScoreToDynamicGroupLeaderboardUTC(const char *leaderboardId, const char *groupId, int32_t score, const std::string &jsonData, const char *leaderboardType,
												   const char *rotationType, int64_t rotationResetUTC, int32_t retainedCount, IServerCallback *callback = NULL);

		/**
		 * Posts score to group leaderbopard and dynamically creates if necessary. leaderboardType, rotationReset, retainedCount and rotationType are required. uses UTC time in milliseconds since epoch
		 *
		 * Service Name - leaderboard
		 * Service Operation - POST_GROUP_SCORE_DYNAMIC
		 *
		 * @param leaderboardId the leaderboard to post to
		 * @param groupId the group's id
		 * @param score the score to post
		 * @param data optional user defined datat to post with scor
		 * @param leaderboardType type of leaderboard
		 * @param rotationResetUTC uses UTC time in milliseconds since epoch
		 * @param retainedCount how long to keep rotation
		 * @param numDaysToRotate How many days between each rotation
		 * @param callback The method to be invoked when the server response is received
		 */
		void postScoreToDynamicGroupLeaderboardDaysUTC(const char *leaderboardId, const char *groupId, int32_t score, const std::string &jsonData, const char *leaderboardType,
													   int64_t rotationResetUTC, int32_t retainedCount, int32_t numDaysToRotate, IServerCallback *callback = NULL);

		/**
		 * Post the group's score to the given social leaderboard, dynamically creating the group leaderboard if it does not exist yet.
		 * To create new leaderboard, configJson must specify leaderboardType, rotationType, resetAt, and retainedCount, at a minimum, with support to optionally specify an expiry in minutes.
		 *
		 * Service Name - Leaderboard
		 * Service Operation - POST_GROUP_SCORE_DYNAMIC_USING_CONFIG
		 *
		 * @param leaderboard The leaderboard to post to
		 * @param groupId The ID of the group
		 * @param score A score to post
		 * @param configJson Configuration for the group leaderboard if it does not
		 *                      exist yet, specified as JSON object.
		 *                      Configuration fields supported are:
		 *                          leaderboardType': Required. Type of leaderboard.
		 *                              Valid values are:
		 *                                  'LAST_VALUE',
		 *                                  'HIGH_VALUE',
		 *                                  'LOW_VALUE',
		 *                                  'CUMULATIVE',
		 *                                  'ARCADE_HIGH',
		 *                                  'ARCADE_LOW';
		 *                          'rotationType': Required. Type ofrotation.
		 *                              Valid values are:
		 *                                  'NEVER',
		 *                                  'DAILY',
		 *                                  'DAYS',
		 *                                  'WEEKLY',
		 *                                  'MONTHLY',
		 *                                  'YEARLY';
		 *                          'numDaysToRotate': Required if 'DAYS' rotation type, with valid values between 2 and 14; otherwise, null;
		 *                          'resetAt': UTC timestamp, in milliseconds, at which to rotate the period. Always null if 'NEVER' rotation type;
		 *                          'retainedCount': Required. Number of rotations (versions) of the leaderboard to retain;
		 *                          'expireInMins': Optional. Duration, in minutes, before the leaderboard is to automatically expire.
		 * @param callback The method to be invoked when the server response is received
		 */
		void postScoreToDynamicGroupLeaderboardUsingConfig(const char *leaderboardId, const char *groupId, int32_t score, const std::string &scoreData, const std::string &configJson, IServerCallback *callback);

		/**
		 * Removes score from group leaderboard
		 *
		 * Service Name - leaderboard
		 * Service Operation - REMOVE_GROUP_SCORE
		 *
		 * @param leaderboardId A collection of leaderboardIds to retrieve scores from
		 * @param groupId the groups Id
		 * @param versionId the score you wish to post
		 * @param callback The method to be invoked when the server response is received
		 */
		void removeGroupScore(const char *leaderboardId, const char *groupId, int32_t versionId, IServerCallback *callback = NULL);

		/**
		 * Retrieve a view of the group leaderboard surrounding the current group.
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_GROUP_LEADERBOARD_VIEW
		 *
		 * @param leaderboardId A collection of leaderboardIds to retrieve scores from
		 * @param groupId the groups Id
		 * @param sortOrder the sort order
		 * @param beforeCount count of players before current player to include
		 * @param afterCount count of players after current player to include
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGroupLeaderboardView(const char *leaderboardId, const char *groupId, SortOrder sortOrder, int32_t beforeCount, int32_t afterCount, IServerCallback *callback = NULL);

		/**
		 * Retrieve a view of the group leaderboard surrounding the current group by the version
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_GROUP_LEADERBOARD_VIEW
		 *
		 * @param leaderboardId A collection of leaderboardIds to retrieve scores from
		 * @param groupId the groups Id
		 * @param versionId the version
		 * @param sortOrder the sort order
		 * @param beforeCount count of players before current player to include
		 * @param afterCount count of players after current player to include
		 * @param callback The method to be invoked when the server response is received
		 */
		void getGroupLeaderboardViewByVersion(const char *leaderboardId, const char *groupId, int32_t versionId, SortOrder sortOrder, int32_t beforeCount, int32_t afterCount, IServerCallback *callback = NULL);

	private:
		BrainCloudClient *m_client;
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
