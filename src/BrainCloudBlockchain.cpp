// Copyright 2026 bitHeads, Inc. All Rights Reserved.

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
    BrainCloudBlockchain::BrainCloudBlockchain(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudBlockchain::GetBlockchainItems(const std::string &integrationID, const std::string &contextJson,
                                                  IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::BlockChainIntegrationId.getValue()] = integrationID;
        message[OperationParam::BlockChainContext.getValue()] = JsonUtil::jsonStringToValue(contextJson);

        ServerCall *sc = new ServerCall(ServiceName::Blockchain, ServiceOperation::GetBlockchainItems, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudBlockchain::GetUniqs(const std::string &integrationID, const std::string &contextJson, IServerCallback *callback)
    {

        Json::Value message;
        message[OperationParam::BlockChainIntegrationId.getValue()] = integrationID;
        message[OperationParam::BlockChainContext.getValue()] = JsonUtil::jsonStringToValue(contextJson);

        ServerCall *sc = new ServerCall(ServiceName::Blockchain, ServiceOperation::GetUniqs, message, callback);
        m_client->sendRequest(sc);
    }
};
