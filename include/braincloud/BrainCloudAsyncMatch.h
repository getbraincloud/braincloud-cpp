// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include "braincloud/BrainCloudTypes.h"
#include <string>

namespace BrainCloud
{

    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudAsyncMatch
    {
    public:
        BrainCloudAsyncMatch(BrainCloudClient *client);

        /**
         * Creates an instance of an asynchronous match.
         *
         * Service Name - asyncMatch
         * Service Operation - CREATE
         *
         * @param jsonOpponentIds  JSON string identifying the opponent platform and id for this match.
         *
         * Platforms are identified as:
         * BC - a brainCloud profile id
         * FB - a Facebook id
         *
         * An exmaple of this string would be:
         * [
         *     {
         *         "platform": "BC",
         *         "id": "some-braincloud-profile"
         *     },
         *     {
         *         "platform": "FB",
         *         "id": "some-facebook-id"
         *     }
         * ]
         *
         * @param pushNotificationMessage Optional push notification message to send to the other party.
         *  Refer to the Push Notification functions for the syntax required.
         * @param callback Optional instance of IServerCallback to call when the server response is received.
         */
        void createMatch(
            const char *jsonOpponentIds,
            const char *pushNotificationMessage = NULL,
            IServerCallback *callback = NULL);

        /**
         * Creates an instance of an asynchronous match with an initial turn.
         *
         * Service Name - asyncMatch
         * Service Operation - CREATE
         *
         * @param jsonOpponentIds  JSON string identifying the opponent platform and id for this match.
         *
         * Platforms are identified as:
         * BC - a brainCloud profile id
         * FB - a Facebook id
         *
         * An exmaple of this string would be:
         * [
         *     {
         *         "platform": "BC",
         *         "id": "some-braincloud-profile"
         *     },
         *     {
         *         "platform": "FB",
         *         "id": "some-facebook-id"
         *     }
         * ]
         *
         * @param jsonMatchState    JSON string blob provided by the caller
         * @param pushNotificationMessage Optional push notification message to send to the other party.
         *      Refer to the Push Notification functions for the syntax required.
         * @param nextPlayer Optionally, force the next player player to be a specific player
         * @param jsonSummary Optional JSON string defining what the other player will see as a summary of the game when listing their games
         * @param callback Optional instance of IServerCallback to call when the server response is received.
         */
        void createMatchWithInitialTurn(
            const char *jsonOpponentIds,
            const char *jsonMatchState,
            const char *pushNotificationMessage = NULL,
            const char *nextPlayer = NULL,
            const char *jsonSummary = NULL,
            IServerCallback *callback = NULL);

        /**
         * Submits a turn for the given match.
         *
         * Service Name - asyncMatch
         * Service Operation - SUBMIT_TURN
         *
         * @param ownerId Match owner identfier
         * @param matchId Match identifier
         * @param version Game state version to ensure turns are submitted once and in order
         * @param jsonMatchState JSON string provided by the caller
         * @param pushNotificationMessage Optional push notification message to send to the other party.
         *          Refer to the Push Notification functions for the syntax required.
         * @param nextPlayer Optionally, force the next player player to be a specific player
         * @param jsonSummary Optional JSON string that other players will see as a summary of the game when listing their games
         * @param jsonStatistics Optional JSON string blob provided by the caller
         * @param callback Optional instance of IServerCallback to call when the server response is received.
         */
        void submitTurn(
            const char *ownerId,
            const char *matchId,
            uint64_t version,
            const char *jsonMatchState,
            const char *pushNotificationMessage = NULL,
            const char *nextPlayer = NULL,
            const char *jsonSummary = NULL,
            const char *jsonStatistics = NULL,
            IServerCallback *callback = NULL);

        /**
         * Allows the current player in the game to overwrite the matchState and
         * statistics without completing their turn or adding to matchHistory.
         *
         * Service Name - asyncMatch
         * Service Operation - UPDATE_MATCH_STATE_CURRENT_TURN
         *
         * @param ownerId   Match owner identifier
         * @param matchId   Match identifier
         * @param version Game state version being updated, to ensure data integrity
         * @param jsonMatchState JSON string provided by the caller Required.
         * @param jsonStatistics Optional JSON string provided by the caller.
         * @param callback Optional instance of IServerCallback to call when the server response is received.
         */
        void updateMatchStateCurrentTurn(const char *ownerId, const char *matchId, uint64_t version, const char *jsonMatchState, const char *jsonStatistics, IServerCallback *callback = NULL);

        /**
         * Allows the current player (only) to update Summary data without having to submit a whole turn.
         *
         * Service Name - asyncMatch
         * Service Operation - UPDATE_SUMMARY
         *
         * @param ownerId Match owner identfier
         * @param matchId Match identifier
         * @param version Game state version to ensure turns are submitted once and in order
         * @param jsonSummary JSON string that other players will see as a summary of the game when listing their games
         * @param callback Optional instance of IServerCallback to call when the server response is received.
         */
        void updateMatchSummaryData(
            const char *ownerId,
            const char *matchId,
            uint64_t version,
            const char *jsonSummary,
            IServerCallback *callback = NULL);

        /**
         * Marks the given match as complete.
         *
         * Service Name - asyncMatch
         * Service Operation - COMPLETE
         *
         * @param ownerId Match owner identifier
         * @param matchId Match identifier
         * @param callback Optional instance of IServerCallback to call when the server response is received.
         */
        void completeMatch(const char *ownerId, const char *matchId, IServerCallback *callback = NULL);

        /**
         * Returns the current state of the given match.
         *
         * Service Name - asyncMatch
         * Service Operation - READ_MATCH
         *
         * @param ownerId   Match owner identifier
         * @param matchId   Match identifier
         * @param callback  Optional instance of IServerCallback to call when the server response is received.
         */
        void readMatch(const char *ownerId, const char *matchId, IServerCallback *callback = NULL);

        /**
         * Returns the match history of the given match.
         *
         * Service Name - asyncMatch
         * Service Operation - READ_MATCH_HISTORY
         *
         * @param ownerId   Match owner identifier
         * @param matchId   Match identifier
         * @param callback  Optional instance of IServerCallback to call when the server response is received.
         */
        void readMatchHistory(const char *ownerId, const char *matchId, IServerCallback *callback = NULL);

        /**
         * Returns all matches that are NOT in a COMPLETE state for which the player is involved.
         *
         * Service Name - asyncMatch
         * Service Operation - FIND_MATCHES
         *
         * @param callback  Optional instance of IServerCallback to call when the server response is received.
         */
        void findMatches(IServerCallback *callback);

        /**
         * Returns all matches that are in a COMPLETE state for which the player is involved.
         *
         * Service Name - asyncMatch
         * Service Operation - FIND_MATCHES_COMPLETED
         *
         * @param callback  Optional instance of IServerCallback to call when the server response is received.
         */
        void findCompleteMatches(IServerCallback *callback = NULL);

        /**
         * Marks the given match as abandoned.
         *
         * Service Name - asyncMatch
         * Service Operation - ABANDON
         *
         * @param ownerId   Match owner identifier
         * @param matchId   Match identifier
         * @param callback  Optional instance of IServerCallback to call when the server response is received.
         */
        void abandonMatch(const char *ownerId, const char *matchId, IServerCallback *callback = NULL);

        /**
         * Removes the match and match history from the server. DEBUG ONLY, in production it is recommended
         *   the user leave it as completed.
         *
         * Service Name - asyncMatch
         * Service Operation - DELETE_MATCH
         *
         * @param ownerId   Match owner identifier
         * @param matchId   Match identifier
         * @param callback  Optional instance of IServerCallback to call when the server response is received.
         */
        void deleteMatch(const char *ownerId, const char *matchId, IServerCallback *callback = NULL);

        /**
         * Marks the given match as complete. This call can send a notification message.
         *
         * Service Name - asyncMatch
         * Service Operation - COMPLETE_MATCH_WITH_SUMMARY_DATA
         *
         * @param ownerId   Match owner identifier
         * @param matchId   Match identifier
         * @param pushContent Optional push notification message to send to the other party when completing the match.
         * @param summary Optional JSON string summary that other players will see when listing their games
         * @param callback  Optional instance of IServerCallback to call when the server response is received.
         */
        void completeMatchWithSummaryData(const char *ownerId, const char *matchId, const char *pushContent, const std::string &summary, IServerCallback *callback = NULL);

        /**
         * Marks the given match as abandoned. This call can send a notification message.
         *
         * Service Name - asyncMatch
         * Service Operation - ABANDON_MATCH_WITH_SUMMARY_DATA
         *
         * @param ownerId   Match owner identifier
         * @param matchId   Match identifier
         * @param pushContent Optional push notification message to send to the other party when abandoning the match.
         * @param summary Optional JSON string summary that other players will see when listing their games
         * @param callback  Optional instance of IServerCallback to call when the server response is received.
         */
        void abandonMatchWithSummaryData(const char *ownerId, const char *matchId, const char *pushContent, const std::string &summary, IServerCallback *callback = NULL);

    private:
        void createMatchInternal(
            const char *jsonOpponentIds,
            const char *jsonMatchState,
            const char *pushNotificationMessage,
            const char *matchId,
            const char *nextPlayer,
            const char *jsonSummary,
            IServerCallback *callback);

        BrainCloudClient *m_client;
    };
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
