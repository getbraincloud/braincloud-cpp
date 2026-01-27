// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudVirtualCurrency.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"

namespace BrainCloud
{
    BrainCloudVirtualCurrency::BrainCloudVirtualCurrency(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudVirtualCurrency::awardCurrency(const std::string &currencyType, int32_t amount, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::VirtualCurrencyServiceVCId.getValue()] = currencyType;
        message[OperationParam::VirtualCurrencyServiceAmount.getValue()] = (Json::Int)amount;

        ServerCall *sc = new ServerCall(ServiceName::VirtualCurrency, ServiceOperation::AwardVC, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudVirtualCurrency::consumeCurrency(const std::string &currencyType, int32_t amount, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::VirtualCurrencyServiceVCId.getValue()] = currencyType;
        message[OperationParam::VirtualCurrencyServiceAmount.getValue()] = (Json::Int)amount;
        ServerCall *sc = new ServerCall(ServiceName::VirtualCurrency, ServiceOperation::ConsumeVC, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudVirtualCurrency::getCurrency(const char *vcId, IServerCallback *callback)
    {
        Json::Value message = Json::nullValue;
        if (StringUtil::IsOptionalParameterValid(vcId))
        {
            message[OperationParam::VirtualCurrencyServiceVCId.getValue()] = vcId;
        }

        ServerCall *sc = new ServerCall(ServiceName::VirtualCurrency, ServiceOperation::VirtualCurrencyGetCurrency, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudVirtualCurrency::getParentCurrency(const char *vcId, const std::string &levelName, IServerCallback *callback)
    {
        Json::Value message;
        if (StringUtil::IsOptionalParameterValid(vcId))
        {
            message[OperationParam::VirtualCurrencyServiceVCId.getValue()] = vcId;
        }
        message[OperationParam::VirtualCurrencyServiceLevelName.getValue()] = levelName;

        ServerCall *sc = new ServerCall(ServiceName::VirtualCurrency, ServiceOperation::VirtualCurrencyGetParentCurrency, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudVirtualCurrency::getPeerCurrency(const char *vcId, const std::string &peerCode, IServerCallback *callback)
    {
        Json::Value message;
        if (StringUtil::IsOptionalParameterValid(vcId))
        {
            message[OperationParam::VirtualCurrencyServiceVCId.getValue()] = vcId;
        }
        message[OperationParam::VirtualCurrencyServicePeerCode.getValue()] = peerCode;

        ServerCall *sc = new ServerCall(ServiceName::VirtualCurrency, ServiceOperation::VirtualCurrencyGetPeerCurrency, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudVirtualCurrency::resetCurrency(IServerCallback *callback)
    {
        Json::Value message;
        ServerCall *sc = new ServerCall(ServiceName::VirtualCurrency, ServiceOperation::VirtualCurrencyResetCurrency, message, callback);
        m_client->sendRequest(sc);
    }
}
