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

    class BrainCloudAppStore
    {
    public:
        BrainCloudAppStore(BrainCloudClient *in_client);

        /**
         * Verifies that purchase was properly made at the store.
         *
         * Service Name - AppStore
         * Service Operation - VerifyPurchase
         *
         * @param in_storeId The store platform. Valid stores are:
         * - itunes
         * - facebook
         * - appworld
         * - steam
         * - windows
         * - windowsPhone
         * - googlePlay
         * @param in_jsonReceiptData The specific store data required
         * @param in_callback The method to be invoked when the server response is received
         */
        void verifyPurchase(const std::string &in_storeId, const std::string &in_jsonReceiptData, IServerCallback *in_callback = NULL);

        /**
         * Returns the eligible promotions for the player.
         *
         * Service Name - AppStore
         * Service Operation - EligiblePromotions
         *
         * @param in_callback The method to be invoked when the server response is received
         */
        void getEligiblePromotions(IServerCallback *in_callback = NULL);

        /**
         * Method gets the active sales inventory for the passed-in
         * currency type.
         *
         * Service Name - AppStore
         * Service Operation - GetInventory
         *
         * @param in_storeId The store platform. Valid stores are:
         * - itunes
         * - facebook
         * - appworld
         * - steam
         * - windows
         * - windowsPhone
         * - googlePlay
         * @param in_userCurrency The currency type to retrieve the sales inventory for.
         * @param in_callback The method to be invoked when the server response is received
         */
        void getSalesInventory(const std::string &in_storeId, const std::string &in_userCurrency, IServerCallback *in_callback = NULL);

        /**
         * Method gets the active sales inventory for the passed-in
         * currency type.
         *
         * Service Name - AppStore
         * Service Operation - GetInventory
         *
         * @param in_storeId The store platform. Valid stores are:
         * - itunes
         * - facebook
         * - appworld
         * - steam
         * - windows
         * - windowsPhone
         * - googlePlay
         * @param in_userCurrency The currency type to retrieve the sales inventory for.
         * @param in_category The product category
         * @param in_callback The method to be invoked when the server response is received
         */
        void getSalesInventoryByCategory(const std::string &in_storeId, const std::string &in_userCurrency, const std::string &in_category, IServerCallback *in_callback = NULL);

        /**
         * Start A Two Staged Purchase Transaction
         *
         * Service Name - AppStore
         * Service Operation - StartPurchase
         *
         * @param in_storeId The store platform. Valid stores are:
         * - itunes
         * - facebook
         * - appworld
         * - steam
         * - windows
         * - windowsPhone
         * - googlePlay
         * @param in_jsonPurchaseData Specific data for starting a two-stage purchase
         * @param in_callback The method to be invoked when the server response is received
         */
        void startPurchase(const std::string &in_storeId, const std::string &in_jsonPurchaseData, IServerCallback *in_callback = NULL);

        /**
         * Finalize A Two Staged Purchase Transaction
         *
         * Service Name - AppStore
         * Service Operation - FinalizePurchase
         *
         * @param in_storeId The store platform. Valid stores are:
         * - itunes
         * - facebook
         * - appworld
         * - steam
         * - windows
         * - windowsPhone
         * - googlePlay
         * @param in_transactionId The transaction id returned from startPurchase
         * @param in_jsonTransactionData Specific transaction data for finalizing purchase
         * @param in_callback The method to be invoked when the server response is received
         */
        void finalizePurchase(const std::string &in_storeId, const std::string &in_transactionId, const std::string &in_jsonTransactionData, IServerCallback *in_callback = NULL);

        /**
         * Returns up-to-date eligible 'promotions' for the user and a 'promotionsRefreshed' flag indicating whether the user's promotion info required refreshing.
         *
         * Service Name - AppStore
         * Service Operation - RefreshPromotions
         * @param in_callback The method to be invoked when the server response is received
         */
        void refreshPromotions(IServerCallback *in_callback = NULL);

        /*
         * Before making a purchase with the IAP store, you will need to store the purchase
         * payload context on brainCloud so that the purchase can be verified for the proper IAP product.
         * This payload will be used during the VerifyPurchase method to ensure the
         * user properly paid for the correct product before awarding them the IAP product.
         *
         * Service Name - AppStore
         * Service Operation - CachePurchasePayloadContext
         *
         * @param in_storeId The store platform. Valid stores are:
         * - itunes
         * - facebook
         * - appworld
         * - steam
         * - windows
         * - windowsPhone
         * - googlePlay
         * @param in_iapId The IAP product id as configured on brainCloud
         * @param in_payload The payload retrieved for the IAP product
         * @param in_callback The method to be invoked when the server response is received
         */
        void cachePurchasePayloadContext(const std::string &in_storeId, const std::string &in_iapId, const std::string &in_payload, IServerCallback *in_callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
