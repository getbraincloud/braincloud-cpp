// Copyright 2016 bitHeads, Inc. All Rights Reserved.

#pragma once

#include <string>
#include <vector>
#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud
{
	enum SocialLeaderboardType {
		HIGH_VALUE,
		CUMULATIVE,
		LAST_VALUE,
		LOW_VALUE
	};

	enum RotationType {
		NEVER,
		DAILY,
		WEEKLY,
		MONTHLY,
		YEARLY
	};

	enum FetchType {
		HIGHEST_RANKED
	};

	enum SortOrder {
		HIGH_TO_LOW,
		LOW_TO_HIGH
	};

	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudSocialLeaderboard
	{
	public:
		BrainCloudSocialLeaderboard(BrainCloudClient* in_client);

		/**
		 * Method returns the social leaderboard. A player's social leaderboard is
		 * comprised of players who are recognized as being your friend.
		 * For now, this applies solely to Facebook connected players who are
		 * friends with the logged in player (who also must be Facebook connected).
		 * In the future this will expand to other identification means (such as
		 * Game Center, Google circles etc).
		 *
		 * Leaderboards entries contain the player's score and optionally, some user-defined
		 * data associated with the score. The currently logged in player will also
		 * be returned in the social leaderboard.
		 *
		 * Note: If no friends have played the game, the bestScore, createdAt, updatedAt
		 * will contain NULL.
		 *
		 * @param in_leaderboardId The id of the leaderboard to retrieve
		 * @param in_replaceName If true, the currently logged in player's name will be replaced
		 * by the string "You".
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 */
		void getSocialLeaderboard(const char * in_leaderboardId, bool in_replaceName, IServerCallback * in_callback = NULL);

		/**
		 * Reads multiple social leaderboards.
		 *
		 * @param in_leaderboardIds Collection of leaderboard IDs.
		 * @param in_leaderboardResultCount Maximum count of entries to return for each leaderboard.
		 * @param in_replaceName If true, the currently logged in player's name will be replaced
		 * by the string "You".
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getMultiSocialLeaderboard(const std::vector<std::string> & in_leaderboardIds, int in_leaderboardResultCount, bool in_replaceName, IServerCallback * in_callback = NULL);


	
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
		 * @param in_leaderboardId The id of the leaderboard to retrieve.
		 * @param in_sort Sort key Sort order of page.
		 * @param in_startIndex The index at which to start the page.
		 * @param in_endIndex The index at which to end the page.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardPage(
			const char * in_leaderboardId,
			SortOrder in_sortOrder,
			int in_startIndex,
			int in_endIndex,
			IServerCallback * in_callback = NULL);


		/**
		 * Method returns a page of global leaderboard results.
		 * By using a non-current version id, the user can retrieve a historical leaderboard.
		 * See GetGlobalLeaderboardVersions method to retrieve the version id.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GetGlobalLeaderboardPage
		 *
		 * @param in_leaderboardId The id of the leaderboard to retrieve.
		 * @param in_sort Sort key Sort order of page.
		 * @param in_startIndex The index at which to start the page.
		 * @param in_endIndex The index at which to end the page.
		 * @param in_versionId The historical version to retrieve.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardPageByVersion(
			const char * in_leaderboardId,
			SortOrder in_sortOrder,
			int in_startIndex,
			int in_endIndex,
			int in_versionId,
			IServerCallback * in_callback = NULL);

	
		/**
		 * Method returns a view of global leaderboard results that centers on the current player.
		 *
		 * Leaderboards entries contain the player's score and optionally, some user-defined
		 * data associated with the score.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GetGlobalLeaderboardView
		 *
		 * @param in_leaderboardId The id of the leaderboard to retrieve.
		 * @param in_sort Sort key Sort order of page.
		 * @param in_beforeCount The count of number of players before the current player to include.
		 * @param in_afterCount The count of number of players after the current player to include.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardView(const char * in_leaderboardId, SortOrder in_sortOrder, int in_beforeCount, int in_afterCount, IServerCallback * in_callback = NULL);

		/**
		 * Method returns a view of global leaderboard results that centers on the current player.
		 * By using a non-current version id, the user can retrieve a historical leaderboard.
		 * See GetGlobalLeaderboardVersions method to retrieve the version id.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GetGlobalLeaderboardView
		 *
		 * @param in_leaderboardId The id of the leaderboard to retrieve.
		 * @param in_sort Sort key Sort order of page.
		 * @param in_beforeCount The count of number of players before the current player to include.
		 * @param in_afterCount The count of number of players after the current player to include.
		 * @param in_versionId The historical version to retrieve.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardViewByVersion(const char * in_leaderboardId, SortOrder in_sortOrder, int in_beforeCount, int in_afterCount, int in_versionId, IServerCallback * in_callback = NULL);

		/** Gets the global leaderboard versions.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - GetGlobalLeaderboardVersions
		 *
		 * @param in_leaderboardId The leaderboard
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardVersions(const char * in_leaderboardId, IServerCallback * in_callback = NULL);

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
		 * @param in_leaderboardId The leaderboard to post to
		 * @param in_score The score to post
		 * @param in_data Optional user-defined data to post with the score
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void postScoreToLeaderboard(const char * in_leaderboardId, int64_t in_score, const std::string& in_jsonOtherData, IServerCallback * in_callback = NULL);

		/**
		 * Post the players score to the given social leaderboard.
		 * Pass leaderboard config data to dynamically create if necessary.
		 * You can optionally send a user-defined json string of data
		 * with the posted score. This string could include information
		 * relevant to the posted score.
		 *
		 * Service Name - SocialLeaderboard
		 * Service Operation - PostScoreDynamic
		 *
		 * @param in_leaderboardId The leaderboard to post to
		 * @param in_score The score to post
		 * @param in_data Optional user-defined data to post with the score
		 * @param in_leaderboardType leaderboard type
		 * @param in_rotationType Type of rotation
		 * @param in_rotationReset Date to start rotation calculations
		 * @param in_retainedCount How many rotations to keep
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void postScoreToDynamicLeaderboard(
			const char * in_leaderboardId,
			int64_t in_score,
			const std::string& in_jsonData,
			SocialLeaderboardType in_leaderboardType,
			RotationType in_rotationType, struct tm* in_rotationReset,
			int in_retainedCount,
			IServerCallback * in_callback = NULL);

		/**
		* Post the players score to the given social leaderboard.
		* Pass leaderboard config data to dynamically create if necessary.
		* You can optionally send a user-defined json string of data
		* with the posted score. This string could include information
		* relevant to the posted score.
		*
		* Service Name - SocialLeaderboard
		* Service Operation - PostScoreDynamic
		*
		* @param in_leaderboardId The leaderboard to post to
		* @param in_score The score to post
		* @param in_data Optional user-defined data to post with the score
		* @param in_leaderboardType leaderboard type
		* @param in_rotationReset Date to start rotation calculations
		* @param in_retainedCount How many rotations to keep
		* @param in_numDaysToRotate How many days between each rotation
		* @param in_callback The method to be invoked when the server response is received
		*/
		void postScoreToDynamicLeaderboardDays(
			const char * in_leaderboardId,
			int64_t in_score,
			const std::string& in_jsonData,
			SocialLeaderboardType in_leaderboardType,
			struct tm* in_rotationReset,
			int32_t in_retainedCount,
			int32_t in_numDaysToRotate,
			IServerCallback * in_callback = NULL);

		/**
		* Removes a player's score from the leaderboard
		*
		* Service Name - leaderboard
		* Service Operation - REMOVE_PLAYER_SCORE
		*
		* @param in_leaderboardId The leaderboard ID
		* @param in_versionId The version of the leaderboard. Use -1 to specifiy the currently active leaderboard version
		* @param in_callback The method to be invoked when the server response is received
		*/
		void removePlayerScore(const char * in_leaderboardId, int32_t in_versionId, IServerCallback * in_callback = NULL);
		
		/**
		 * Method to retrieve the player's completed tournaments for the game.
		 * Arguments:
		 *  leaderboardId: Name of the leaderboard
		 *  replaceName: True to replace the player's name with "You"; false otherwise.
		 *  callback: client-side callback for the results
		 */
		void playerTournamentReward(const char * in_eventName, uint64_t in_multiplier, IServerCallback * in_callback = NULL);

		/**
		 * Retrieve the social leaderboard for a group.
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_GROUP_SOCIAL_LEADERBOARD
		 *
		 * @param in_leaderboardId The leaderboard to retrieve
		 * @param in_groupId The ID of the group
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getGroupSocialLeaderboard(const char * in_leaderboardId, const char * in_groupId, IServerCallback * in_callback = NULL);

		/**
		 * Retrieve the social leaderboard for a list of players.
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_PLAYERS_SOCIAL_LEADERBOARD
		 *
		 * @param in_leaderboardId The leaderboard to retrieve
		 * @param in_profileIds The IDs of the players
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getPlayersSocialLeaderboard(const char * in_leaderboardId, std::vector<std::string> in_profileIds, IServerCallback * in_callback = NULL);

		/**
		 * Retrieve a list of all leaderboards
		 *
		 * Service Name - leaderboard
		 * Service Operation - LIST_ALL_LEADERBOARDS
		 *
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void listAllLeaderboards(IServerCallback * in_callback = NULL);

		/**
		 * Gets the number of entries in a global leaderboard
		 *
		 * Service Name - leaderboard
		 * Service Operation - GET_GLOBAL_LEADERBOARD_ENTRY_COUNT
		 *
		 * @param in_leaderboardId The leaderboard ID
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getGlobalLeaderboardEntryCount(const char * in_leaderboardId, IServerCallback * in_callback = NULL);

		/**
		* Gets the number of entries in a global leaderboard
		*
		* Service Name - leaderboard
		* Service Operation - GET_GLOBAL_LEADERBOARD_ENTRY_COUNT
		*
		* @param in_leaderboardId The leaderboard ID
		* @param in_versionId The version of the leaderboard. Use -1 for current.
		* @param in_callback The method to be invoked when the server response is received
		*/
		void getGlobalLeaderboardEntryCountByVersion(const char * in_leaderboardId, int32_t in_versionId, IServerCallback * in_callback = NULL);

		/**
		* Gets a player's score from a leaderboard
		*
		* Service Name - leaderboard
		* Service Operation - GET_PLAYER_SCORE
		*
		* @param in_leaderboardId The leaderboard ID
		* @param in_versionId The version of the leaderboard. Use -1 for current.
		* @param in_callback The method to be invoked when the server response is received
		*/
		void getPlayerScore(const char * in_leaderboardId, int32_t in_versionId, IServerCallback * in_callback = NULL);

		/**
		* Gets a player's score from multiple leaderboards
		*
		* Service Name - leaderboard
		* Service Operation - GET_PLAYER_SCORES_FROM_LEADERBOARDS
		*
		* @param in_leaderboardId A collection of leaderboardIds to retrieve scores from
		* @param in_callback The method to be invoked when the server response is received
		*/
		void getPlayerScoresFromLeaderboards(const std::vector<std::string> & in_leaderboardIds, IServerCallback * in_callback = NULL);

		static std::string leaderboardTypeToString(SocialLeaderboardType type);
		static std::string leaderboardRotationTypeToString(RotationType type);
		static std::string sortOrderToString(SortOrder in_sortOrder);

	private:
		BrainCloudClient * m_client;
	};
}
