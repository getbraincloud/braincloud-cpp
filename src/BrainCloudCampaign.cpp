// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudCampaign.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "braincloud/internal/JsonUtil.h"
#include "json/json.h"

namespace BrainCloud
{
    BrainCloudCampaign::BrainCloudCampaign(BrainCloudClient* client) : m_client(client) { }

    void BrainCloudCampaign::getMyCampaigns(const std::string& optionsJson, IServerCallback* callback)
    {
        Json::Value message;
        message[OperationParam::UserItemsServiceOptionsJson.getValue()] = JsonUtil::jsonStringToValue(optionsJson);

        ServerCall* sc = new ServerCall(ServiceName::Campaign, ServiceOperation::GetMyCampaigns, message, callback);
        m_client->sendRequest(sc);
    }
}
