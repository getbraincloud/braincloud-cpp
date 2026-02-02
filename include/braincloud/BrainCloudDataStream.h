// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include <stddef.h>
#include <string>

namespace BrainCloud
{

    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudDataStream
    {
    public:
        BrainCloudDataStream(BrainCloudClient *client);

        /**
         * Creates custom data stream page event
         *
         * @param eventName Name of event
         * @param eventProperties Properties of event
         * @param callback The method to be invoked when the server response is received
         */
        void customPageEvent(const char *eventName, const char *jsonEventProperties, IServerCallback *callback = NULL);

        /**
         * Creates custom data stream screen event
         *
         * @param eventName Name of event
         * @param eventProperties Properties of event
         * @param callback The method to be invoked when the server response is received
         */
        void customScreenEvent(const char *eventName, const char *jsonEventProperties, IServerCallback *callback = NULL);

        /**
         * Creates custom data stream track event
         *
         * @param eventName Name of event
         * @param eventProperties Properties of event
         * @param callback The method to be invoked when the server response is received
         */
        void customTrackEvent(const char *eventName, const char *jsonEventProperties, IServerCallback *callback = NULL);

        /**
         * Send crash report
         *
         * @param crashType Identifies the crash category. Developer-defined, can be anything.
         * @param errorMsg  Short message describing the crash.
         * @param crashJson Exception data.
         * @param crashLog  Client log up until the crash (if available.)
         * @param userName  Name provided by the user (if provided.)
         * @param userEmail Email address to respond to (if provided.)
         * @param userNotes Notes provided by the user (if provided.)
         * @param userSubmitted User submitted flag.
         * @param callback The callback handler
         */
        void submitCrashReport(const char *crashType, const char *errorMsg, const std::string &crashJson, const char *crashLog, const char *username, const char *userEmail, const char *userNotes, bool userSubmitted, IServerCallback *callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
