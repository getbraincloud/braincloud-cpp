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

    class BrainCloudMatchmaking
    {
    public:
        BrainCloudMatchmaking(BrainCloudClient *client);

        /**
         * Read match making record
         *
         * Service Name - MatchMaking
         * Service Operation - Read
         *
         * @param callback The method to be invoked when the server response is received
         */
        void read(IServerCallback *callback = NULL);

        /**
         * Sets player rating
         *
         * Service Name - MatchMaking
         * Service Operation - SetPlayerRating
         *
         * @param playerRating The new player rating.
         * @param callback The method to be invoked when the server response is received
         */
        void setPlayerRating(int32_t playerRating, IServerCallback *callback = NULL);

        /**
         * Resets player rating
         *
         * Service Name - MatchMaking
         * Service Operation - ResetPlayerRating
         *
         * @param callback The method to be invoked when the server response is received
         */
        void resetPlayerRating(IServerCallback *callback = NULL);

        /**
         * Increments player rating
         *
         * Service Name - MatchMaking
         * Service Operation - IncrementPlayerRating
         *
         * @param increment The increment amount
         * @param callback The method to be invoked when the server response is received
         */
        void incrementPlayerRating(int32_t increment, IServerCallback *callback = NULL);

        /**
         * Decrements player rating
         *
         * Service Name - MatchMaking
         * Service Operation - DecrementPlayerRating
         *
         * @param decrement The decrement amount
         * @param callback The method to be invoked when the server response is received
         */
        void decrementPlayerRating(int32_t decrement, IServerCallback *callback = NULL);

        /**
         * Turns shield on
         *
         * Service Name - MatchMaking
         * Service Operation - ShieldOn
         *
         * @param callback The method to be invoked when the server response is received
         */
        void turnShieldOn(IServerCallback *callback = NULL);

        /**
         * Turns shield on for the specified number of minutes
         *
         * Service Name - MatchMaking
         * Service Operation - ShieldOnFor
         *
         * @param minutes Number of minutes to turn the shield on for
         * @param callback The method to be invoked when the server response is received
         */
        void turnShieldOnFor(int32_t minutes, IServerCallback *callback = NULL);

        /**
         * Increases the shield on time by specified number of minutes
         *
         * Service Name - MatchMaking
         * Service Operation - IncrementShieldOnFor
         *
         * @param minutes Number of minutes to increase the shield time for
         * @param callback The method to be invoked when the server response is received
         */
        void incrementShieldOnFor(int32_t minutes, IServerCallback *callback = NULL);

        /**
         * Turns shield off
         *
         * Service Name - MatchMaking
         * Service Operation - ShieldOff
         *
         * @param callback The method to be invoked when the server response is received
         */
        void turnShieldOff(IServerCallback *callback = NULL);

        /**
         * Gets the shield expiry for the given player id. Passing in a null player id
         * will return the shield expiry for the current player. The value returned is
         * the time in UTC millis when the shield will expire.
         *
         * Service Name - MatchMaking
         * Service Operation - GetShieldExpiry
         *
         * @param playerId The player id or use null to retrieve for the current player
         * @param callback The method to be invoked when the server response is received
         */
        void getShieldExpiry(const char *playerId, IServerCallback *callback = NULL);

        /**
         * Finds matchmaking enabled players
         *
         * Service Name - MatchMaking
         * Service Operation - FIND_PLAYERS
         *
         * @param rangeDelta The range delta
         * @param numMatches The maximum number of matches to return
         * @param callback The method to be invoked when the server response is received
         */
        void findPlayers(int32_t rangeDelta, int32_t numMatches, IServerCallback *callback = NULL);

        /**
         * Finds matchmaking enabled players with additional attributes
         *
         * Service Name - MatchMaking
         * Service Operation - FIND_PLAYERS
         *
         * @param rangeDelta The range delta
         * @param numMatches The maximum number of matches to return
         * @param jsonAttributes Attributes match criteria
         * @param callback The method to be invoked when the server response is received
         */
        void findPlayersWithAttributes(int32_t rangeDelta, int32_t numMatches, std::string jsonAttributes, IServerCallback *callback = NULL);

        /**
         * Finds matchmaking enabled players
         *
         * Service Name - MatchMaking
         * Service Operation - FIND_PLAYERS_USING_FILTER
         *
         * @param rangeDelta The range delta
         * @param numMatches The maximum number of matches to return
         * @param jsonExtraParms Parameters to pass to the CloudCode filter script
         * @param callback The method to be invoked when the server response is received
         */
        void findPlayersUsingFilter(int32_t rangeDelta, int32_t numMatches, std::string jsonExtraParms, IServerCallback *callback = NULL);

        /**
         * Finds matchmaking enabled players using a cloud code filter
         * and additional attributes
         *
         * Service Name - MatchMaking
         * Service Operation - FIND_PLAYERS_USING_FILTER
         *
         * @param rangeDelta The range delta
         * @param numMatches The maximum number of matches to return
         * @param jsonAttributes Attributes match criteria
         * @param jsonExtraParms Parameters to pass to the CloudCode filter script
         * @param callback The method to be invoked when the server response is received
         */
        void findPlayersWithAttributesUsingFilter(
            int32_t rangeDelta,
            int32_t numMatches,
            std::string jsonAttributes,
            std::string jsonExtraParms,
            IServerCallback *callback = NULL);

        /**
         * Enables Match Making for the Player
         *
         * Service Name - MatchMaking
         * Service Operation - EnableMatchMaking
         *
         * @param callback The method to be invoked when the server response is received
         */
        void enableMatchMaking(IServerCallback *callback = NULL);

        /**
         * Disables Match Making for the Player
         *
         * Service Name - MatchMaking
         * Service Operation - EnableMatchMaking
         *
         * @param callback The method to be invoked when the server response is received
         */
        void disableMatchMaking(IServerCallback *callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
