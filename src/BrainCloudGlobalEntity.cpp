// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudGlobalEntity.h"

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
	BrainCloudGlobalEntity::BrainCloudGlobalEntity(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudGlobalEntity::createEntity(const std::string &entityType, int64_t timeToLive, const std::string &jsonEntityAcl,
											  const std::string &jsonEntityData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::GlobalEntityServiceTimeToLive.getValue()] = (Json::Int64)timeToLive;
		message[OperationParam::GlobalEntityServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonEntityData);
		message[OperationParam::GlobalEntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(jsonEntityAcl);

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::Create, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::createEntityWithIndexedId(const std::string &entityType, const std::string &indexedId, int64_t timeToLive,
														   const std::string &jsonEntityAcl, const std::string &jsonEntityData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::GlobalEntityServiceIndexedId.getValue()] = indexedId;
		message[OperationParam::GlobalEntityServiceTimeToLive.getValue()] = (Json::Int64)timeToLive;
		message[OperationParam::GlobalEntityServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonEntityData);
		message[OperationParam::GlobalEntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(jsonEntityAcl);

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::CreateWithIndexedId, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::updateEntity(const std::string &entityId, int64_t version, const std::string &jsonEntityData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::GlobalEntityServiceVersion.getValue()] = (Json::Int64)version;
		message[OperationParam::GlobalEntityServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonEntityData);

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::Update, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::updateEntityAcl(const std::string &entityId, int64_t version, const std::string &jsonEntityAcl, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::GlobalEntityServiceVersion.getValue()] = (Json::Int64)version;
		message[OperationParam::GlobalEntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(jsonEntityAcl);

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::UpdateAcl, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::updateEntityTimeToLive(const std::string &entityId, int64_t version, int64_t timeToLive, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::GlobalEntityServiceVersion.getValue()] = (Json::Int64)version;
		message[OperationParam::GlobalEntityServiceTimeToLive.getValue()] = (Json::Int64)timeToLive;

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::UpdateTimeToLive, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::deleteEntity(const std::string &entityId, int64_t version, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::GlobalEntityServiceVersion.getValue()] = (Json::Int64)version;

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::Delete, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::readEntity(const std::string &entityId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceEntityId.getValue()] = entityId;

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::Read, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::getList(const std::string &where, const std::string &orderBy, int64_t maxReturn, IServerCallback *callback)
	{
		Json::Value message;

		if (StringUtil::IsOptionalParameterValid(where))
		{
			message[OperationParam::GlobalEntityServiceWhere.getValue()] = JsonUtil::jsonStringToValue(where);
		}

		if (StringUtil::IsOptionalParameterValid(orderBy))
		{
			message[OperationParam::GlobalEntityServiceOrderBy.getValue()] = JsonUtil::jsonStringToValue(orderBy);
		}

		message[OperationParam::GlobalEntityServiceMaxReturn.getValue()] = (Json::Int64)maxReturn;

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::GetList, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::getListByIndexedId(const std::string &entityIndexedId, int64_t maxReturn, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceIndexedId.getValue()] = entityIndexedId;
		message[OperationParam::GlobalEntityServiceMaxReturn.getValue()] = (Json::Int64)maxReturn;

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::GetListByIndexedId, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::getListCount(const std::string &where, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceWhere.getValue()] = JsonUtil::jsonStringToValue(where);

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::GetListCount, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::getPage(const char *context, IServerCallback *callback)
	{
		Json::Value message;
		Json::Reader reader;

		Json::Value parsedContext;
		reader.parse(context, parsedContext);
		message[OperationParam::GlobalEntityServiceContext.getValue()] = parsedContext;

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::GetPage, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::getPageOffset(const char *context, int pageOffset, IServerCallback *callback)
	{
		Json::Value message;
		Json::Reader reader;

		message[OperationParam::GlobalEntityServiceContext.getValue()] = context;
		message[OperationParam::GlobalEntityServicePageOffset.getValue()] = pageOffset;

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::GetPageOffset, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::incrementGlobalEntityData(const char *entityId, std::string jsonData, IServerCallback *callback)
	{
		Json::Value message;
		Json::Reader reader;

		message[OperationParam::GlobalEntityServiceEntityId.getValue()] = entityId;

		Json::Value parsedData;
		reader.parse(jsonData, parsedData);
		message[OperationParam::GlobalEntityServiceData.getValue()] = parsedData;

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::IncrementGlobalEntityData, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::getRandomEntitiesMatching(const std::string &where, int64_t maxReturn, IServerCallback *callback)
	{
		Json::Value message;

		if (StringUtil::IsOptionalParameterValid(where))
		{
			message[OperationParam::GlobalEntityServiceWhere.getValue()] = JsonUtil::jsonStringToValue(where);
		}

		message[OperationParam::GlobalEntityServiceMaxReturn.getValue()] = (Json::Int64)maxReturn;

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::GetRandomEntitiesMatching, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::updateEntityIndexedId(const char *entityId, int64_t version, const char *entityIndexedId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::GlobalEntityServiceVersion.getValue()] = (Json::Int64)version;
		message[OperationParam::GlobalEntityServiceIndexedId.getValue()] = entityIndexedId;

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::UpdateEntityIndexedId, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::updateEntityOwnerAndAcl(const char *entityId, int64_t version, const char *ownerId, const std::string &jsonEntityAcl, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::GlobalEntityServiceVersion.getValue()] = (Json::Int64)version;
		message[OperationParam::OwnerId.getValue()] = ownerId;
		message[OperationParam::GlobalEntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(jsonEntityAcl);

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::UpdateEntityOwnerAndAcl, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudGlobalEntity::makeSystemEntity(const char *entityId, int64_t version, const std::string &jsonEntityAcl, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::GlobalEntityServiceVersion.getValue()] = (Json::Int64)version;
		message[OperationParam::GlobalEntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(jsonEntityAcl);

		ServerCall *sc = new ServerCall(ServiceName::GlobalEntity, ServiceOperation::MakeSystemEntity, message, callback);
		m_client->sendRequest(sc);
	}
}