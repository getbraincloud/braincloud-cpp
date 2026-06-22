// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>

#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud {

    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudBlockchain
    {
    public:
        BrainCloudBlockchain(BrainCloudClient* client);


        /**
         * @brief Retrieves the blockchain items owned by the caller.
         * Service Name - blockchain
         * Service Operation - GET_BLOCKCHAIN_ITEMS
         *
         */
        void GetBlockchainItems(const std::string& integrationID ,const std::string& contextJson, IServerCallback * callback = NULL);
        /**
         * @brief Retrieves the uniqs owned by the caller.
         *
         * Service Name - blockchain
         * Service Operation - GET_UNIQS
         */
        void GetUniqs(const std::string& integrationID, const std::string& contextJson, IServerCallback * callback = NULL);

        
    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
