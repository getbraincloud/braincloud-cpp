// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>

namespace BrainCloud
{
    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudTime
    {
    public:
        BrainCloudTime(BrainCloudClient* client);

        /**
         * Method returns the server time in UTC. This is in UNIX millis time format.
         * For instance 1396378241893 represents 2014-04-01 2:50:41.893 in GMT-4.
         *
         * Service Name - time
         * Service Operation - READ
         *
         * Server API reference: ServiceName.Time, ServiceOperation.Read
         *
         * @param callback The method to be invoked when the server response is received
         */
        void readServerTime( IServerCallback * callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
