// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudAppStore.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/JsonUtil.h"

namespace BrainCloud
{
    BrainCloudAppStore::BrainCloudAppStore(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudAppStore::verifyPurchase(const std::string &storeId, const std::string &jsonReceiptData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::AppStoreStoreId.getValue()] = storeId;
        message[OperationParam::AppStoreReceiptData.getValue()] = JsonUtil::jsonStringToValue(jsonReceiptData.c_str());

        ServerCall *sc = new ServerCall(ServiceName::AppStore, ServiceOperation::AppStoreVerifyPurchase, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudAppStore::getEligiblePromotions(IServerCallback *callback)
    {
        Json::Value message;

        ServerCall *sc = new ServerCall(ServiceName::AppStore, ServiceOperation::AppStoreGetEligiblePromotions, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudAppStore::getSalesInventory(const std::string &storeId, const std::string &userCurrency, IServerCallback *callback)
    {
        getSalesInventoryByCategory(storeId, userCurrency, "", callback);
    }

    void BrainCloudAppStore::getSalesInventoryByCategory(const std::string &storeId, const std::string &userCurrency, const std::string &category, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::AppStoreStoreId.getValue()] = storeId;
        if (!category.empty())
        {
            message[OperationParam::AppStoreCategory.getValue()] = category;
        }

        Json::Value priceInfoCriteria;
        if (!userCurrency.empty())
        {
            priceInfoCriteria[OperationParam::AppStoreUserCurrency.getValue()] = userCurrency;
        }
        message[OperationParam::AppStorePriceInfoCriteria.getValue()] = priceInfoCriteria;

        ServerCall *sc = new ServerCall(ServiceName::AppStore, ServiceOperation::AppStoreGetSalesInventory, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudAppStore::startPurchase(const std::string &storeId, const std::string &jsonPurchaseData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::AppStoreStoreId.getValue()] = storeId;
        message[OperationParam::AppStorePurchaseData.getValue()] = JsonUtil::jsonStringToValue(jsonPurchaseData.c_str());

        ServerCall *sc = new ServerCall(ServiceName::AppStore, ServiceOperation::AppStoreStartPurchase, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudAppStore::finalizePurchase(const std::string &storeId, const std::string &transactionId, const std::string &jsonTransactionData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::AppStoreStoreId.getValue()] = storeId;
        message[OperationParam::AppStoreTransactionId.getValue()] = transactionId;
        message[OperationParam::AppStoreTransactionData.getValue()] = JsonUtil::jsonStringToValue(jsonTransactionData.c_str());

        ServerCall *sc = new ServerCall(ServiceName::AppStore, ServiceOperation::AppStoreFinalizePurchase, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudAppStore::refreshPromotions(IServerCallback *callback)
    {
        Json::Value message;

        ServerCall *sc = new ServerCall(ServiceName::AppStore, ServiceOperation::AppStoreRefreshPromotions, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudAppStore::cachePurchasePayloadContext(const std::string &storeId, const std::string &iapId,
                                                         const std::string &payload, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::AppStoreStoreId.getValue()] = storeId;
        message[OperationParam::AppStoreStoreIAPId.getValue()] = iapId;
        message[OperationParam::AppStorePayload.getValue()] = payload;

        ServerCall *sc = new ServerCall(ServiceName::AppStore, ServiceOperation::AppStoreCachePurchaseContext, message, callback);
        m_client->sendRequest(sc);
    }
}
