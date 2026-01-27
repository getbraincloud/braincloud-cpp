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

    class BrainCloudOneWayMatch
    {
    public:
        BrainCloudOneWayMatch(BrainCloudClient *client);

        /**
         * Starts a match
         *
         * Service Name - OneWayMatch
         * Service Operation - StartMatch
         *
         * @param otherPlayerId The player to start a match with
         * @param rangeDelta The range delta used for the initial match search
         * @param callback The method to be invoked when the server response is received
         */
        void startMatch(const char *otherPlayerId, int32_t rangeDelta, IServerCallback *callback = NULL);

        /**
         * Cancels a match
         *
         * Service Name - OneWayMatch
         * Service Operation - CancelMatch
         *
         * @param playbackStreamId The playback stream id returned in the start match
         * @param callback The method to be invoked when the server response is received
         */
        void cancelMatch(const char *playbackStreamId, IServerCallback *callback = NULL);

        /**
         * Completes a match
         *
         * Service Name - OneWayMatch
         * Service Operation - CompleteMatch
         *
         * @param playbackStreamId The playback stream id returned in the initial start match
         * @param callback The method to be invoked when the server response is received
         */
        void completeMatch(const char *playbackStreamId, IServerCallback *callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
