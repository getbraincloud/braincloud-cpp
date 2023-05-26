// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudBlockchain.h"

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
BrainCloudBlockchain::BrainCloudBlockchain(BrainCloudClient* in_client) : m_client(in_client) { }

void BrainCloudBlockchain::GetBlockchainItems(const std::string& in_integrationID, const std::string& in_contextJson,
    IServerCallback* in_callback)
{
    Json::Value message;
    message[OperationParam::BlockChainIntegrationId.getValue()] = in_integrationID;
    message[OperationParam::BlockChainContext.getValue()] = JsonUtil::jsonStringToValue(in_contextJson);
    
    ServerCall *sc = new ServerCall(ServiceName::Blockchain, ServiceOperation::GetBlockchainItems, message, in_callback);
    m_client->sendRequest(sc);
}

void BrainCloudBlockchain::GetUniqs(const std::string& in_integrationID, const std::string& in_contextJson, IServerCallback* in_callback)
{
    
    Json::Value message;
    message[OperationParam::BlockChainIntegrationId.getValue()] = in_integrationID;
    message[OperationParam::BlockChainContext.getValue()] = JsonUtil::jsonStringToValue(in_contextJson);
    
    ServerCall *sc = new ServerCall(ServiceName::Blockchain, ServiceOperation::GetUniqs, message, in_callback);
    m_client->sendRequest(sc);
}};
