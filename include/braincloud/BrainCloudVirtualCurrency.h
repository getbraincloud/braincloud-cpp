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

    class BrainCloudVirtualCurrency
    {
    public:
        BrainCloudVirtualCurrency(BrainCloudClient* client);

        /**
         * @warning Method is recommended to be used in Cloud Code only for security
         * If you need to use it client side, enable 'Allow Currency Calls from Client' on the brainCloud dashboard
         */
        void awardCurrency(const std::string& currencyType, int32_t amount, IServerCallback * callback = NULL);

        /**
         * @warning Method is recommended to be used in Cloud Code only for security
         * If you need to use it client side, enable 'Allow Currency Calls from Client' on the brainCloud dashboard
         */
        void consumeCurrency(const std::string& currencyType, int32_t amount, IServerCallback * callback = NULL);

        /**
        * Retrieve the user's currency account. Optional parameters: vcId (if retrieving all currencies).
        *
        * Service Name - VirtualCurrency
        * Service Operation - GET_PLAYER_VC
        *
        * @param vcId
        * @param callback The method to be invoked when the server response is received
        */
        void getCurrency(const char* vcId, IServerCallback * callback = NULL);

        /**
        * Retrieve the parent user's currency account. Optional parameters: vcId (if retrieving all currencies).
        *
        * Service Name - VirtualCurrency
        * Service Operation - GET_PARENT_VC
        *
        * @param vcId
        * @param levelName
        * @param callback The method to be invoked when the server response is received
        */
        void getParentCurrency(const char* vcId, const std::string& levelName, IServerCallback * callback = NULL);

        /**
        * Retrieve the peer user's currency account. Optional parameters: vcId (if retrieving all currencies).
        *
        * Service Name - VirtualCurrency
        * Service Operation - GET_PEER_VC
        *
        * @param vcId
        * @param peerCode
        * @param callback The method to be invoked when the server response is received
        */
        void getPeerCurrency(const char* vcId, const std::string& peerCode, IServerCallback * callback = NULL);

        /**
        * Reset player's currency to zero
        *
        * Service Name - VirtualCurrency
        * Service Operation - RESET_PLAYER_VC
        *
        * @param callback The method to be invoked when the server response is received
        */
        void resetCurrency(IServerCallback * callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
