// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudGlobalApp.h"

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
    BrainCloudGlobalApp::BrainCloudGlobalApp(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudGlobalApp::readProperties(IServerCallback *callback)
    {
        Json::Value message = Json::nullValue;
        ServerCall *sc = new ServerCall(ServiceName::GlobalApp, ServiceOperation::ReadProperties, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGlobalApp::readSelectedProperties(const std::vector<std::string> &propertyNames, IServerCallback *callback)
    {
        Json::Value message = Json::nullValue;

        message[OperationParam::GlobalAppPropertyNames.getValue()] = JsonUtil::stringVectorToJson(propertyNames);

        ServerCall *sc = new ServerCall(ServiceName::GlobalApp, ServiceOperation::ReadSelectedProperties, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGlobalApp::readPropertiesInCategories(const std::vector<std::string> &categories, IServerCallback *callback)
    {
        Json::Value message = Json::nullValue;

        message[OperationParam::GlobalAppCategories.getValue()] = JsonUtil::stringVectorToJson(categories);

        ServerCall *sc = new ServerCall(ServiceName::GlobalApp, ServiceOperation::ReadPropertiesInCategories, message, callback);
        m_client->sendRequest(sc);
    }
}