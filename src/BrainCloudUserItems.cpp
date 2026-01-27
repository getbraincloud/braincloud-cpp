// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudUserItems.h"

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
    BrainCloudUserItems::BrainCloudUserItems(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudUserItems::awardUserItem(const std::string &defId, int32_t quantity, bool includeDef, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceDefId.getValue()] = defId;
        message[OperationParam::UserItemsServiceQuantity.getValue()] = quantity;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::AwardUserItem, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::dropUserItem(const std::string &itemId, int32_t quantity, bool includeDef, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceItemId.getValue()] = itemId;
        message[OperationParam::UserItemsServiceQuantity.getValue()] = quantity;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::DropUserItem, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::getUserItemsPage(const std::string &context, bool includeDef, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceContext.getValue()] = JsonUtil::jsonStringToValue(context.c_str());
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::GetUserItemsPage, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::getUserItemsPageOffset(const std::string &context, int32_t pageOffset, bool includeDef, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceContext.getValue()] = context;
        message[OperationParam::UserItemsServicePageOffset.getValue()] = pageOffset;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::GetUserItemsPageOffset, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::getUserItem(const std::string &itemId, bool includeDef, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceItemId.getValue()] = itemId;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::GetUserItem, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::giveUserItemTo(const std::string &profileId, const std::string &itemId, int32_t version, int32_t quantity, bool immediate, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceProfileId.getValue()] = profileId;
        message[OperationParam::UserItemsServiceItemId.getValue()] = itemId;
        message[OperationParam::UserItemsServiceVersion.getValue()] = version;
        message[OperationParam::UserItemsServiceQuantity.getValue()] = quantity;
        message[OperationParam::UserItemsServiceImmediate.getValue()] = immediate;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::GiveUserItemTo, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::purchaseUserItem(const std::string &defId, int32_t quantity, const std::string &shopId, bool includeDef, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceDefId.getValue()] = defId;
        message[OperationParam::UserItemsServiceQuantity.getValue()] = quantity;
        message[OperationParam::UserItemsServiceShopId.getValue()] = shopId;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::PurchaseUserItem, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::receiveUserItemFrom(const std::string &profileId, const std::string &itemId, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceProfileId.getValue()] = profileId;
        message[OperationParam::UserItemsServiceItemId.getValue()] = itemId;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::ReceiveUserItemFrom, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::sellUserItem(const std::string &itemId, int32_t version, int32_t quantity, const std::string &shopId, bool includeDef, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceItemId.getValue()] = itemId;
        message[OperationParam::UserItemsServiceVersion.getValue()] = version;
        message[OperationParam::UserItemsServiceQuantity.getValue()] = quantity;
        message[OperationParam::UserItemsServiceShopId.getValue()] = shopId;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::SellUserItem, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::updateUserItemData(const std::string &itemId, int32_t version, const std::string &newItemData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceItemId.getValue()] = itemId;
        message[OperationParam::UserItemsServiceVersion.getValue()] = version;
        message[OperationParam::UserItemsServiceNewItemData.getValue()] = JsonUtil::jsonStringToValue(newItemData.c_str());

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::UpdateUserItemData, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::useUserItem(const std::string &itemId, int32_t version, const std::string &newItemData, bool includeDef, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceItemId.getValue()] = itemId;
        message[OperationParam::UserItemsServiceVersion.getValue()] = version;
        message[OperationParam::UserItemsServiceNewItemData.getValue()] = JsonUtil::jsonStringToValue(newItemData.c_str());
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::UseUserItem, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::publishUserItemToBlockchain(const std::string &itemId, int32_t version, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceItemId.getValue()] = itemId;
        message[OperationParam::UserItemsServiceVersion.getValue()] = version;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::PublishUserItemToBlockchain, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::refreshBlockchainUserItems(IServerCallback *callback)
    {
        Json::Value message;
        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::RefreshBlockchainUserItems, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::removeUserItemFromBlockchain(const std::string &itemId, int32_t version, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceItemId.getValue()] = itemId;
        message[OperationParam::UserItemsServiceVersion.getValue()] = version;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::RemoveUserItemFromBlockchain, message, callback);
        m_client->sendRequest(sc);
    }
    void BrainCloudUserItems::awardUserItemWithOptions(const std::string &defId, int32_t quantity, bool includeDef, const std::string &optionsJson, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceDefId.getValue()] = defId;
        message[OperationParam::UserItemsServiceQuantity.getValue()] = quantity;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;
        message[OperationParam::UserItemsServiceOptionsJson.getValue()] = JsonUtil::jsonStringToValue(optionsJson);

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::AwardUserItem, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::purchaseUserItemsWithOptions(const std::string &defId, int32_t quantity, const std::string &shopId, bool includeDef, const std::string &optionsJson, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceDefId.getValue()] = defId;
        message[OperationParam::UserItemsServiceQuantity.getValue()] = quantity;
        message[OperationParam::UserItemsServiceShopId.getValue()] = shopId;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;
        message[OperationParam::UserItemsServiceOptionsJson.getValue()] = JsonUtil::jsonStringToValue(optionsJson);

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::PurchaseUserItem, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::getItemPromotionDetails(const std::string &defId, const std::string &shopId, bool includeDef, bool includePromotionDetails, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceDefId.getValue()] = defId;
        message[OperationParam::UserItemsServiceShopId.getValue()] = shopId;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;
        message[OperationParam::UserItemsServiceIncludePromotionDetails.getValue()] = includePromotionDetails;

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::GetItemPromotionDetails, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::getItemsOnPromotion(const std::string &shopId, bool includeDef, bool includePromotionDef, const std::string &optionsJson, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceShopId.getValue()] = shopId;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;
        message[OperationParam::UserItemsServiceIncludePromotionDetails.getValue()] = includePromotionDef;
        if (StringUtil::IsOptionalParameterValid(optionsJson))
        {
            message[OperationParam::UserItemsServiceOptionsJson.getValue()] = JsonUtil::jsonStringToValue(optionsJson);
        }

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::GetItemsOnPromotion, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudUserItems::openBundle(const std::string &itemId, int version, int quantity, bool includeDef, const std::string &optionsJson, IServerCallback *callback)
    {
        Json::Value message;

        message[OperationParam::UserItemsServiceItemId.getValue()] = itemId;
        message[OperationParam::UserItemsServiceVersion.getValue()] = version;
        message[OperationParam::UserItemsServiceQuantity.getValue()] = quantity;
        message[OperationParam::UserItemsServiceIncludeDef.getValue()] = includeDef;

        if (StringUtil::IsOptionalParameterValid(optionsJson))
        {
            message[OperationParam::UserItemsServiceOptionsJson.getValue()] = JsonUtil::jsonStringToValue(optionsJson);
        }

        ServerCall *sc = new ServerCall(ServiceName::UserItems, ServiceOperation::OpenBundle, message, callback);

        m_client->sendRequest(sc);
    }

}
