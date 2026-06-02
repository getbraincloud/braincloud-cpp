// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <stddef.h>
#include <string>

namespace BrainCloud {

    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudDataStream
    {
    public:
        BrainCloudDataStream(BrainCloudClient* client);

        /**
         * Creates custom data stream page event
         *
         * Service Name - dataStream
         * Service Operation - CUSTOM_PAGE_EVENT
         *
         * @param eventName Name of event
         * @param eventProperties Properties of event
         */
        void customPageEvent(const char * eventName, const char * jsonEventProperties, IServerCallback * callback = NULL);

        /**
         * Creates custom data stream screen event
         *
         * @param eventName Name of event
         * @param eventProperties Properties of event
         */
        void customScreenEvent(const char * eventName, const char * jsonEventProperties, IServerCallback * callback = NULL);

        /**
         * Creates custom data stream track event
         *
         * @param eventName Name of event
         * @param eventProperties Properties of event
         */
        void customTrackEvent(const char * eventName, const char * jsonEventProperties, IServerCallback * callback = NULL);

        /**
         * Send crash report
         *
         * @param crashType
         * @param errorMsg
         * @param crashJson
         * @param crashLog
         * @param userName
         * @param userEmail
         * @param userNotes
         * @param userSubmitted
         */
        void submitCrashReport(const char * crashType, const char * errorMsg, const std::string&  crashJson, const char * crashLog, const char * username, const char * userEmail, const char * userNotes, bool userSubmitted, IServerCallback * callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
