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
        BrainCloudVirtualCurrency(BrainCloudClient *in_client);

        /**
         * @warning Method is recommended to be used in Cloud Code only for security
         * If you need to use it client side, enable 'Allow Currency Calls from Client' on the brainCloud dashboard
         * @param in_currencyType The currency type to award
         * @param in_amount The amount to award
         * @param in_callback The method to be invoked when the server response is received
         */
        void awardCurrency(const std::string &in_currencyType, int32_t in_amount, IServerCallback *in_callback = NULL);

        /**
         * @warning Method is recommended to be used in Cloud Code only for security
         * If you need to use it client side, enable 'Allow Currency Calls from Client' on the brainCloud dashboard
         * @param in_currencyType The currency type to consume
         * @param in_amount The amount to consume
         * @param in_callback The method to be invoked when the server response is received
         */
        void consumeCurrency(const std::string &in_currencyType, int32_t in_amount, IServerCallback *in_callback = NULL);

        /**
         * Retrieve the user's currency account. Optional parameter: `in_vcId` (if retrieving a specific currency).
         *
         * Service Name - VirtualCurrency
         * Service Operation - GetCurrency
         *
         * @param in_vcId Optional currency id to retrieve (pass NULL to get all currencies)
         * @param in_callback The method to be invoked when the server response is received
         */
        void getCurrency(const char *in_vcId, IServerCallback *in_callback = NULL);

        /**
         * Retrieve the parent user's currency account. Optional parameter: `in_vcId` (if retrieving a specific currency).
         *
         * Service Name - VirtualCurrency
         * Service Operation - GetParentCurrency
         *
         * @param in_vcId Optional currency id to retrieve (pass NULL to get all currencies)
         * @param in_levelName The parent level name
         * @param in_callback The method to be invoked when the server response is received
         */
        void getParentCurrency(const char *in_vcId, const std::string &in_levelName, IServerCallback *in_callback = NULL);

        /**
         * Retrieve the peer user's currency account. Optional parameter: `in_vcId` (if retrieving a specific currency).
         *
         * Service Name - VirtualCurrency
         * Service Operation - GetPeerCurrency
         *
         * @param in_vcId Optional currency id to retrieve (pass NULL to get all currencies)
         * @param in_peerCode The peer code identifying the other user
         * @param in_callback The method to be invoked when the server response is received
         */
        void getPeerCurrency(const char *in_vcId, const std::string &in_peerCode, IServerCallback *in_callback = NULL);

        /**
         * Reset player's currency to zero
         *
         * Service Name - VirtualCurrency
         * Service Operation - ResetCurrency
         *
         * @param in_callback The method to be invoked when the server response is received
         */
        void resetCurrency(IServerCallback *in_callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
