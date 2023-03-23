// Copyright 2016 bitHeads, Inc. All Rights Reserved.

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
        BrainCloudTime(BrainCloudClient* in_client);

        /**
         * Method returns the server time in UTC. This is in UNIX millis time format.
         * For instance 1396378241893 represents 2014-04-01 2:50:41.893 in GMT-4.
         *
         * Service Name - Time
         * Service Operation - Read
         *
         * Server API reference: ServiceName.Time, ServiceOperation.Read
         *
         * @param in_callback The method to be invoked when the server response is received
         */
        void readServerTime( IServerCallback * in_callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
