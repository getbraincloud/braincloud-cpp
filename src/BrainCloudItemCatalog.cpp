// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudItemCatalog.h"

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
	BrainCloudItemCatalog::BrainCloudItemCatalog(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudItemCatalog::getCatalogItemDefinition(const std::string &defId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ItemCatalogServiceDefId.getValue()] = defId;

		ServerCall *sc = new ServerCall(ServiceName::ItemCatalog, ServiceOperation::GetCatalogItemDefinition, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudItemCatalog::getCatalogItemsPage(const std::string &context, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ItemCatalogServiceContext.getValue()] = JsonUtil::jsonStringToValue(context.c_str());

		ServerCall *sc = new ServerCall(ServiceName::ItemCatalog, ServiceOperation::GetCatalogItemsPage, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudItemCatalog::getCatalogItemsPageOffset(const std::string &context, int32_t pageOffset, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ItemCatalogServiceContext.getValue()] = context;
		message[OperationParam::ItemCatalogServicePageOffset.getValue()] = pageOffset;

		ServerCall *sc = new ServerCall(ServiceName::ItemCatalog, ServiceOperation::GetCatalogItemsPageOffset, message, callback);
		m_client->sendRequest(sc);
	}
}
