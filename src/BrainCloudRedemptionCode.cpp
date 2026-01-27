// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudRedemptionCode.h"

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
    BrainCloudRedemptionCode::BrainCloudRedemptionCode(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudRedemptionCode::redeemCode(const char *scanCode, const char *codeType, const char *jsonCustomRedemptionInfo, IServerCallback *callback)
    {
        Json::Value message;

        message[OperationParam::RedemptionCodeServiceScanCode.getValue()] = scanCode;
        message[OperationParam::RedemptionCodeServiceCodeType.getValue()] = codeType;

        if (StringUtil::IsOptionalParameterValid(jsonCustomRedemptionInfo))
        {
            Json::Reader reader;
            Json::Value parsedInfo;
            reader.parse(jsonCustomRedemptionInfo, parsedInfo);
            message[OperationParam::RedemptionCodeServiceCustomRedemptionInfo.getValue()] = parsedInfo;
        }

        ServerCall *sc = new ServerCall(ServiceName::RedemptionCode, ServiceOperation::RedeemCode, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudRedemptionCode::getRedeemedCodes(const char *codeType, IServerCallback *callback)
    {
        Json::Value message;

        if (StringUtil::IsOptionalParameterValid(codeType))
        {
            message[OperationParam::RedemptionCodeServiceCodeType.getValue()] = codeType;
        }

        ServerCall *sc = new ServerCall(ServiceName::RedemptionCode, ServiceOperation::GetRedeemedCodes, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }
}