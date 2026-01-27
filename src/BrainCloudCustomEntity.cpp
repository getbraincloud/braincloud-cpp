// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudCustomEntity.h"

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
	BrainCloudCustomEntity::BrainCloudCustomEntity(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudCustomEntity::createEntity(const char *entityType, const std::string &jsonEntityData, const std::string &jsonEntityAcl, int64_t timeToLive, bool isOwned, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonEntityData);
		message[OperationParam::CustomEntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(jsonEntityAcl);
		message[OperationParam::CustomEntityServiceTimeToLive.getValue()] = (Json::Int64)timeToLive;
		message[OperationParam::CustomEntityServiceIsOwned.getValue()] = isOwned;

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::CreateEntity, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::deleteEntity(const char *entityType, const char *entityId, const std::string &jsonEntityData, int32_t version, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::CustomEntityServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonEntityData);
		message[OperationParam::CustomEntityServiceVersion.getValue()] = version;

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::DeleteEntity, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::getCount(const char *entityType, const std::string &whereJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceData.getValue()] = JsonUtil::jsonStringToValue(whereJson);

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::GetCount, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::getRandomEntitiesMatching(const char *entityType, const std::string &whereJson, const int32_t maxReturn, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceData.getValue()] = JsonUtil::jsonStringToValue(whereJson);
		message[OperationParam::CustomEntityServiceMaxReturn.getValue()] = maxReturn;

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::GetRandomEntitiesMatching, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::getEntityPage(const char *entityType, const char *context, IServerCallback *callback)
	{
		Json::Value message;
		Json::Reader reader;

		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		Json::Value parsedContext;
		reader.parse(context, parsedContext);
		message[OperationParam::CustomEntityServiceContext.getValue()] = parsedContext;

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::GetCustomEntityPage, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::getEntityPageOffset(const char *entityType, const std::string &context, int32_t pageOffset, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceContext.getValue()] = context;
		message[OperationParam::CustomEntityServicePageOffset.getValue()] = pageOffset;

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::GetEntityPageOffset, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::readEntity(const char *entityType, const char *entityId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceEntityId.getValue()] = entityId;

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::ReadEntity, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::updateEntity(const char *entityType, const char *entityId, int32_t version, const std::string &jsonEntityData, const std::string &jsonEntityAcl, int64_t timeToLive, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::CustomEntityServiceVersion.getValue()] = version;
		message[OperationParam::CustomEntityServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonEntityData);
		message[OperationParam::CustomEntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(jsonEntityAcl);
		message[OperationParam::CustomEntityServiceTimeToLive.getValue()] = (Json::Int64)timeToLive;

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::UpdateEntity, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::updateEntityFields(const char *entityType, const char *entityId, int32_t version, const std::string &fieldsJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::CustomEntityServiceVersion.getValue()] = version;
		message[OperationParam::CustomEntityFieldsJson.getValue()] = JsonUtil::jsonStringToValue(fieldsJson);

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::UpdateEntityFields, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::updateEntityFieldsSharded(const char *entityType, const char *entityId, int32_t version, const std::string &fieldsJson, const std::string &shardKeyJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::CustomEntityServiceVersion.getValue()] = version;
		message[OperationParam::CustomEntityFieldsJson.getValue()] = JsonUtil::jsonStringToValue(fieldsJson);
		message[OperationParam::CustomEntityShardKeyJson.getValue()] = JsonUtil::jsonStringToValue(shardKeyJson);

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::UpdateEntityFieldsSharded, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::deleteEntities(const char *entityType, const char *deleteCriteria, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceDeleteCriteria.getValue()] = JsonUtil::jsonStringToValue(deleteCriteria);

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::DeleteEntities, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::deleteSingleton(const char *entityType, int32_t version, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceVersion.getValue()] = version;

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::DeleteSingleton, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::updateSingleton(const char *entityType, int32_t version, const std::string &dataJson, const std::string &acl, int64_t timeToLive, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceVersion.getValue()] = version;
		message[OperationParam::CustomEntityServiceData.getValue()] = JsonUtil::jsonStringToValue(dataJson);
		message[OperationParam::CustomEntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(acl);
		message[OperationParam::CustomEntityServiceTimeToLive.getValue()] = (Json::Int64)timeToLive;

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::UpdateSingleton, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::updateSingletonFields(const char *entityType, int32_t version, const std::string &fieldsJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceVersion.getValue()] = version;
		message[OperationParam::CustomEntityFieldsJson.getValue()] = JsonUtil::jsonStringToValue(fieldsJson);

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::UpdateSingletonFields, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::readSingleton(const char *entityType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::ReadSingleton, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::incrementData(const char *entityType, const char *entityId, const std::string &fieldsJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::CustomEntityFieldsJson.getValue()] = JsonUtil::jsonStringToValue(fieldsJson);

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::IncrementData, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudCustomEntity::incrementSingletonData(const char *entityType, const std::string &fieldsJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::CustomEntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::CustomEntityFieldsJson.getValue()] = JsonUtil::jsonStringToValue(fieldsJson);

		ServerCall *sc = new ServerCall(ServiceName::CustomEntity, ServiceOperation::IncrementSingletonData, message, callback);
		m_client->sendRequest(sc);
	}

}
