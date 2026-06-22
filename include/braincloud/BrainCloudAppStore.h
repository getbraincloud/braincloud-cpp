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
        BrainCloudAppStore(BrainCloudClient* client);

        /**
        * Verifies that purchase was properly made at the store.
        *
        * Service Name - appStore
        * Service Operation - VERIFY_PURCHASE
        *
        * @param storeId The store platform. Valid stores are:
        * - itunes
        * - facebook
        * - appworld
        * - steam
        * - windows
        * - windowsPhone
        * - googlePlay
        * @param receiptData the specific store data required
        * @param callback The method to be invoked when the server response is received
        */
        void verifyPurchase(const std::string& storeId, const std::string& jsonReceiptData, IServerCallback* callback = NULL);

        /**
        * Returns the eligible promotions for the player.
        *
        * Service Name - appStore
        * Service Operation - ELIGIBLE_PROMOTIONS
        *
        * @param callback The method to be invoked when the server response is received
        */
        void getEligiblePromotions(IServerCallback* callback = NULL);

        /**
        * Method gets the active sales inventory for the passed-in
        * currency type.
        *
        * Service Name - appStore
        * Service Operation - GET_INVENTORY
        *
        * @param platform The store platform. Valid stores are:
        * - itunes
        * - facebook
        * - appworld
        * - steam
        * - windows
        * - windowsPhone
        * - googlePlay
        * @param userCurrency The currency type to retrieve the sales inventory for.
        * @param callback The method to be invoked when the server response is received
        */
        void getSalesInventory(const std::string& storeId, const std::string& userCurrency, IServerCallback* callback = NULL);

        /**
        * Method gets the active sales inventory for the passed-in
        * currency type.
        *
        * Service Name - appStore
        * Service Operation - GET_INVENTORY
        *
        * @param storeId The store platform. Valid stores are:
        * - itunes
        * - facebook
        * - appworld
        * - steam
        * - windows
        * - windowsPhone
        * - googlePlay
        * @param userCurrency The currency type to retrieve the sales inventory for.
        * @param category The product category
        * @param callback The method to be invoked when the server response is received
        */
        void getSalesInventoryByCategory(const std::string& storeId, const std::string& userCurrency, const std::string& category, IServerCallback* callback = NULL);

        /**
        * Start A Two Staged Purchase Transaction
        *
        * Service Name - appStore
        * Service Operation - START_PURCHASE
        *
        * @param storeId The store platform. Valid stores are:
        * - itunes
        * - facebook
        * - appworld
        * - steam
        * - windows
        * - windowsPhone
        * - googlePlay
        * @param purchaseData specific data for purchasing 2 staged purchases
        * @param callback The method to be invoked when the server response is received
        */
        void startPurchase(const std::string& storeId, const std::string& jsonPurchaseData, IServerCallback* callback = NULL);

        /**
        * Finalize A Two Staged Purchase Transaction
        *
        * Service Name - appStore
        * Service Operation - FINALIZE_PURCHASE
        *
        * @param storeId The store platform. Valid stores are:
        * - itunes
        * - facebook
        * - appworld
        * - steam
        * - windows
        * - windowsPhone
        * - googlePlay
        * @param transactionId the transactionId returned from start Purchase
        * @param transactionData specific data for purchasing 2 staged purchases
        * @param callback The method to be invoked when the server response is received
        */
        void finalizePurchase(const std::string& storeId, const std::string& transactionId, const std::string& jsonTransactionData, IServerCallback* callback = NULL);

        /**
        * Returns up-to-date eligible 'promotions' for the user and a 'promotionsRefreshed' flag indicating whether the user's promotion info required refreshing.
        *
        * Service Name - appStore
        * Service Operation - REFRESH_PROMOTIONS
        */
        void refreshPromotions(IServerCallback* callback = NULL);
        
        /*
        * Before making a purchase with the IAP store, you will need to store the purchase
        * payload context on brainCloud so that the purchase can be verified for the proper IAP product.
        * This payload will be used during the VerifyPurchase method to ensure the
        * user properly paid for the correct product before awarding them the IAP product.
        *
        * Service Name - appStore
        * Service Operation - CACHE_PURCHASE_PAYLOAD_CONTEXT
        *
        * @param storeId The store platform. Valid stores are:
        * - itunes
        * - facebook
        * - appworld
        * - steam
        * - windows
        * - windowsPhone
        * - googlePlay
        * @param transactionId the transactionId returned from start Purchase
        * @param transactionData specific data for purchasing 2 staged purchases
        * @param callback The method to be invoked when the server response is received
        */
        void cachePurchasePayloadContext(const std::string& storeId, const std::string& iapId, const std::string& payload, IServerCallback* callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

