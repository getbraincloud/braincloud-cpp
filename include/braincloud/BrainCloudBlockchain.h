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

    class BrainCloudBlockchain
    {
    public:
        BrainCloudBlockchain(BrainCloudClient *client);

        /**
         * Retrieves the blockchain items owned by the caller.
         *
         * Service Name - Blockchain
         * Service Operation - GET_BLOCKCHAITEMS
         *
         * @param integrationID  The blockchain integration id.
         *                          Currently only 'default' is supported.
         * @param contextJson    Optional. Reserved for future use.
         * @param callback          The callback handler
         */
        void GetBlockchainItems(const std::string &integrationID, const std::string &contextJson, IServerCallback *callback = NULL);

        /**
         * Retrieves the uniqs owned by the caller.
         *
         * Service Name - Blockchain
         * Service Operation - GET_UNIQS
         *
         * @param integrationID  The blockchain integration id.
         *                          Currently only 'default' is supported.
         * @param contextJson    Optional. Reserved for future use.
         * @param callback          The callback handler
         */
        void GetUniqs(const std::string &integrationID, const std::string &contextJson, IServerCallback *callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
