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

    class BrainCloudPlaybackStream
    {
    public:
        BrainCloudPlaybackStream(BrainCloudClient* client);

        /**
        * Starts a stream
        *
        * Service Name - PlaybackStream
        * Service Operation - START_STREAM
        *
        * @param targetPlayerId The player to start a stream with
        * @param includeSharedData Whether to include shared data in the stream
        * @param callback The method to be invoked when the server response is received
        */
        void startStream(const char * targetPlayerId, bool includeSharedData, IServerCallback * callback = NULL);

        /**
        * Reads a stream
        *
        * Service Name - PlaybackStream
        * Service Operation - READ_STREAM
        *
        * @param playbackStreamId Identifies the stream to read
        * @param callback The method to be invoked when the server response is received
        */
        void readStream(const char * playbackStreamId, IServerCallback * callback = NULL);

        /**
        * Ends a stream
        *
        * Service Name - PlaybackStream
        * Service Operation - END_STREAM
        *
        * @param playbackStreamId Identifies the stream to read
        * @param callback The method to be invoked when the server response is received
        */
        void endStream(const char * playbackStreamId, IServerCallback * callback = NULL);

        /**
        * Deletes a stream
        *
        * Service Name - PlaybackStream
        * Service Operation - DELETE_STREAM
        *
        * @param playbackStreamId Identifies the stream to read
        * @param callback The method to be invoked when the server response is received
        */
        void deleteStream(const char * playbackStreamId, IServerCallback * callback = NULL);

        /**
        * Adds a stream event
        *
        * Service Name - PlaybackStream
        * Service Operation - ADD_EVENT
        *
        * @param playbackStreamId Identifies the stream to read
        * @param jsonEventData Describes the event
        * @param jsonSummary Current summary data as of this event
        * @param callback The method to be invoked when the server response is received
        */
        void addEvent(const char * playbackStreamId, const char * jsonEventData, const char * jsonSummary, IServerCallback * callback = NULL);

        /**
        * Gets recent stream summaries for initiating player
        *
        * Service Name - PlaybackStream
        * Service Operation - GET_RECENT_STREAMS_FOR_INITIATING_PLAYER
        *
        * @param targetPlayerId The player that started the stream
        * @param maxNumStreams The max number of streams to query
        * @param callback The callback.
        */
        void getRecentStreamsForInitiatingPlayer(const char * initiatingPlayerId, int maxNumStreams, IServerCallback * callback = NULL);

        /**
        * Gets recent stream summaries for target player
        *
        * Service Name - PlaybackStream
        * Service Operation - GET_RECENT_STREAMS_FOR_TARGET_PLAYER
        *
        * @param targetPlayerId The player that was target of the stream
        * @param maxNumStreams The max number of streams to query
        * @param callback The callback.
        */
        void getRecentStreamsForTargetPlayer(const char * targetPlayerId, int maxNumStreams, IServerCallback * callback = NULL);

        /**
         * Protects a playback stream from being purged (but not deleted) for the given number of days (from now).
         * If the number of days given is less than the normal purge interval days (from createdAt), the longer protection date is applied.
         * Can only be called by users involved in the playback stream.
         *
         * Service Name - PlaybackStream
         * Service Operation - PROTECT_STREAM_UNTIL
         *
         * @param playbackStreamId Identifies the stream to protect
         * @param numDays The number of days the stream is to be protected (from now)
         * @param callback The method to be invoked when the server response is received
         */
        void protectStreamUntil(const char *playbackStreamId, int numDays, IServerCallback *callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
