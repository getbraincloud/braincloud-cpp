// Copyright 2016 bitHeads, Inc. All Rights Reserved.

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
        BrainCloudPlaybackStream(BrainCloudClient* in_client);

        /**
        * Starts a stream
        *
        * Service Name - PlaybackStream
        * Service Operation - StartStream
        *
        * @param in_targetPlayerId The player to start a stream with
        * @param in_includeSharedData Whether to include shared data in the stream
        * @param in_callback The method to be invoked when the server response is received
        */
        void startStream(const char * in_targetPlayerId, bool in_includeSharedData, IServerCallback * in_callback = NULL);

        /**
        * Reads a stream
        *
        * Service Name - PlaybackStream
        * Service Operation - ReadStream
        *
        * @param in_playbackStreamId Identifies the stream to read
        * @param in_callback The method to be invoked when the server response is received
        */
        void readStream(const char * in_playbackStreamId, IServerCallback * in_callback = NULL);

        /**
        * Ends a stream
        *
        * Service Name - PlaybackStream
        * Service Operation - EndStream
        *
        * @param in_playbackStreamId Identifies the stream to read
        * @param in_callback The method to be invoked when the server response is received
        */
        void endStream(const char * in_playbackStreamId, IServerCallback * in_callback = NULL);

        /**
        * Deletes a stream
        *
        * Service Name - PlaybackStream
        * Service Operation - DeleteStream
        *
        * @param in_playbackStreamId Identifies the stream to read
        * @param in_callback The method to be invoked when the server response is received
        */
        void deleteStream(const char * in_playbackStreamId, IServerCallback * in_callback = NULL);

        /**
        * Adds a stream event
        *
        * Service Name - PlaybackStream
        * Service Operation - AddEvent
        *
        * @param in_playbackStreamId Identifies the stream to read
        * @param in_jsonEventData Describes the event
        * @param in_jsonSummary Current summary data as of this event
        * @param in_callback The method to be invoked when the server response is received
        */
        void addEvent(const char * in_playbackStreamId, const char * in_jsonEventData, const char * in_jsonSummary, IServerCallback * in_callback = NULL);

        /**
        * Gets recent stream summaries for initiating player
        *
        * Service Name - PlaybackStream
        * Service Operation - GetRecentStreamsForInitiatingPlayer
        *
        * @param targetPlayerId The player that started the stream
        * @param maxNumStreams The max number of streams to query
        * @param callback The callback.
        */
        void getRecentStreamsForInitiatingPlayer(const char * in_initiatingPlayerId, int in_maxNumStreams, IServerCallback * in_callback = NULL);

        /**
        * Gets recent stream summaries for target player
        *
        * Service Name - PlaybackStream
        * Service Operation - GetRecentStreamsForTargetPlayer
        *
        * @param targetPlayerId The player that was target of the stream
        * @param maxNumStreams The max number of streams to query
        * @param callback The callback.
        */
        void getRecentStreamsForTargetPlayer(const char * in_targetPlayerId, int in_maxNumStreams, IServerCallback * in_callback = NULL);

        /**
         * Protects a playback stream from being purged (but not deleted) for the given number of days (from now).
         * If the number of days given is less than the normal purge interval days (from createdAt), the longer protection date is applied.
         * Can only be called by users involved in the playback stream.
         *
         * Service Name - PlaybackStream
         * Service Operation - PROTECT_STREAM_UNTIL
         *
         * @param in_playbackStreamId Identifies the stream to protect
         * @param in_numDays The number of days the stream is to be protected (from now)
         * @param in_callback The method to be invoked when the server response is received
         */
        void protectStreamUntil(const char *in_playbackStreamId, int in_numDays, IServerCallback *in_callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
