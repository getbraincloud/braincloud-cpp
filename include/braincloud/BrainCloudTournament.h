// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include <string>
#include "braincloud/BrainCloudTypes.h"
#include "braincloud/BrainCloudSocialLeaderboard.h"

namespace BrainCloud
{
	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudTournament
	{
	public:
		BrainCloudTournament(BrainCloudClient *client);

		/**
		 * Processes any outstanding rewards for the given player
		 *
		 * Service Name - tournament
		 * Service Operation - CLAIM_TOURNAMENT_REWARD
		 *
		 * @param leaderboardId The leaderboard for the tournament
		 * @param versionId Version of the tournament. Use -1 for the latest version.
		 * @param callback The method to be invoked when the server response is received
		 */
		void claimTournamentReward(const char *leaderboardId, int32_t versionId, IServerCallback *callback = NULL);

		/**
		 * Get the status of a division
		 *
		 * Service Name - tournament
		 * Service Operation - GET_DIVISION_INFO
		 *
		 * @param divSetId The id for the division
		 * @param callback The method to be invoked when the server response is received
		 */
		void getDivisionInfo(const std::string &divSetId, IServerCallback *callback = NULL);

		/**
		 * Returns list of player's recently active divisions
		 *
		 * Service Name - tournament
		 * Service Operation - GET_MY_DIVISIONS
		 *
		 * @param callback The method to be invoked when the server response is received
		 */
		void getMyDivisions(IServerCallback *callback = NULL);

		/**
		 * Get tournament status associated with a leaderboard
		 *
		 * Service Name - tournament
		 * Service Operation - GET_TOURNAMENT_STATUS
		 *
		 * @param leaderboardId The leaderboard for the tournament
		 * @param versionId Version of the tournament. Use -1 for the latest version.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getTournamentStatus(const char *leaderboardId, int32_t versionId, IServerCallback *callback = NULL);

		/**
		 * Join the specified division.
		 * If joining requires a fee, it is possible to fail at joining the division
		 *
		 * Service Name - tournament
		 * Service Operation - JOIN_DIVISION
		 *
		 * @param divSetId The id for the division
		 * @param tournamentCode Tournament to join
		 * @param initialScore The initial score for players first joining a tournament
		 *						 Usually 0, unless leaderboard is LOW_VALUE
		 * @param callback The method to be invoked when the server response is received
		 */
		void joinDivision(const std::string &divSetId, const std::string &tournamentCode, int64_t initialScore, IServerCallback *callback = NULL);

		/**
		 * Join the specified tournament.
		 * Any entry fees will be automatically collected.
		 *
		 * Service Name - tournament
		 * Service Operation - JOIN_TOURNAMENT
		 *
		 * @param leaderboardId The leaderboard for the tournament
		 * @param tournamentCode Tournament to join
		 * @param initialScore The initial score for players first joining a tournament
		 *						  Usually 0, unless leaderboard is LOW_VALUE
		 * @param callback The method to be invoked when the server response is received
		 */
		void joinTournament(const char *leaderboardId, const char *tournamentCode, int64_t initialScore, IServerCallback *callback = NULL);

		/**
		 * Removes player from division instance
		 * Also removes division instance from player's division list
		 *
		 * Service Name - tournament
		 * Service Operation - LEAVE_DIVISION_INSTANCE
		 *
		 * @param leaderboardId The leaderboard for the tournament
		 * @param callback The method to be invoked when the server response is received
		 */
		void leaveDivisionInstance(const std::string &leaderboardId, IServerCallback *callback = NULL);

		/**
		 * Removes player's score from tournament leaderboard
		 *
		 * Service Name - tournament
		 * Service Operation - LEAVE_TOURNAMENT
		 *
		 * @param leaderboardId The leaderboard for the tournament
		 * @param callback The method to be invoked when the server response is received
		 */
		void leaveTournament(const char *leaderboardId, IServerCallback *callback = NULL);

		/**
		 * Post the users score to the leaderboard - UTC time
		 *
		 * Service Name - tournament
		 * Service Operation - POST_TOURNAMENT_SCORE
		 *
		 * @param leaderboardId The leaderboard for the tournament
		 * @param score The score to post
		 * @param jsonData Optional data attached to the leaderboard entry
		 * @param roundStartedTimeUTC Time the user started the match resulting in the score being posted in UTC. Use UTC time in milliseconds since epoch
		 * @param callback The method to be invoked when the server response is received
		 */
		void postTournamentScoreUTC(const char *leaderboardId, int64_t score, const std::string &jsonData, int64_t roundStartedTimeUTC, IServerCallback *callback = NULL);

		/**
		 * Post the users score to the leaderboard - UTC time
		 *
		 * Service Name - tournament
		 * Service Operation - POST_TOURNAMENT_SCORE_WITH_RESULTS
		 *
		 * @param leaderboardId The leaderboard for the tournament
		 * @param score The score to post
		 * @param jsonData Optional data attached to the leaderboard entry
		 * @param roundStartedTimeUTC Time the user started the match resulting in the score being posted in UTC. Use UTC time in milliseconds since epoch
		 * @param sort Sort key Sort order of page.
		 * @param beforeCount The count of number of players before the current player to include.
		 * @param afterCount The count of number of players after the current player to include.
		 * @param initialScore The initial score for players first joining a tournament
		 *						 Usually 0, unless leaderboard is LOW_VALUE
		 * @param callback The method to be invoked when the server response is received
		 */
		void postTournamentScoreWithResultsUTC(
			const char *leaderboardId,
			int64_t score,
			const std::string &jsonData,
			int64_t roundStartedTimeUTC,
			SortOrder sort,
			int32_t beforeCount,
			int32_t afterCount,
			int64_t initialScore,
			IServerCallback *callback = NULL);

		/**
		 * Returns the user's expected reward based on the current scores
		 *
		 * Service Name - tournament
		 * Service Operation - VIEW_CURRENT_REWARD
		 *
		 * @param leaderboardId The leaderboard for the tournament
		 * @param callback The method to be invoked when the server response is received
		 */
		void viewCurrentReward(const char *leaderboardId, IServerCallback *callback = NULL);

		/**
		 * Returns the user's reward from a finished tournament
		 *
		 * Service Name - tournament
		 * Service Operation - VIEW_REWARD
		 *
		 * @param leaderboardId The leaderboard for the tournament
		 * @param versionId Version of the tournament. Use -1 for the latest version.
		 * @param callback The method to be invoked when the server response is received
		 */
		void viewReward(const char *leaderboardId, int32_t versionId, IServerCallback *callback = NULL);

	private:
		BrainCloudClient *m_client;
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
