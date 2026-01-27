// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudEntity.h"

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
	BrainCloudEntity::BrainCloudEntity(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudEntity::createEntity(const char *entityType, const std::string &jsonEntityData, const std::string &jsonEntityAcl, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::EntityServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonEntityData);
		message[OperationParam::EntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(jsonEntityAcl);

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::Create, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::getEntity(const char *entityId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceEntityId.getValue()] = entityId;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::Read, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::getSingleton(const char *entityType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceEntityType.getValue()] = entityType;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::ReadSingleton, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::getEntitiesByType(const char *entityType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceEntityType.getValue()] = entityType;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::ReadByType, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::updateEntity(const char *entityId, const char *entityType, const std::string &jsonEntityData, const std::string &jsonEntityAcl, int64_t version, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::EntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::EntityServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonEntityData);
		message[OperationParam::EntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(jsonEntityAcl);
		message[OperationParam::EntityServiceVersion.getValue()] = (Json::Int64)version;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::Update, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::updateSingleton(const char *entityType, const std::string &jsonEntityData, const std::string &jsonEntityAcl, int64_t version, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::EntityServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonEntityData);
		message[OperationParam::EntityServiceAcl.getValue()] = JsonUtil::jsonStringToValue(jsonEntityAcl);
		message[OperationParam::EntityServiceVersion.getValue()] = (Json::Int64)version;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::UpdateSingleton, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::deleteEntity(const char *entityId, int64_t version, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::EntityServiceVersion.getValue()] = (Json::Int64)version;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::Delete, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::deleteSingleton(const char *entityType, int64_t version, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::EntityServiceVersion.getValue()] = (Json::Int64)version;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::DeleteSingleton, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::getSharedEntityForProfileId(const char *profileId, const char *entityId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceTargetPlayerId.getValue()] = profileId;
		message[OperationParam::EntityServiceEntityId.getValue()] = entityId;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::ReadSharedEntity, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::getSharedEntitiesForProfileId(const char *profileId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceTargetPlayerId.getValue()] = profileId;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::ReadShared, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::getSharedEntitiesListForProfileId(const char *profileId, std::string whereJson, std::string orderByJson, int32_t maxReturn, IServerCallback *callback)
	{
		Json::Value message;

		message[OperationParam::EntityServiceTargetPlayerId.getValue()] = profileId;
		if (StringUtil::IsOptionalParameterValid(whereJson))
		{
			message[OperationParam::GlobalEntityServiceWhere.getValue()] = JsonUtil::jsonStringToValue(whereJson);
		}
		if (StringUtil::IsOptionalParameterValid(orderByJson))
		{
			message[OperationParam::GlobalEntityServiceOrderBy.getValue()] = JsonUtil::jsonStringToValue(orderByJson);
		}
		message[OperationParam::GlobalEntityServiceMaxReturn.getValue()] = (Json::Int64)maxReturn;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::ReadSharedEntitesList, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::updateSharedEntity(
		const char *entityId,
		const char *targetProfileId,
		const char *entityType,
		const std::string &jsonEntityData,
		int64_t version,
		IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::EntityServiceTargetPlayerId.getValue()] = targetProfileId;
		message[OperationParam::EntityServiceEntityType.getValue()] = entityType;
		message[OperationParam::EntityServiceVersion.getValue()] = (Json::Int64)version;
		message[OperationParam::EntityServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonEntityData);

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::UpdateShared, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::getList(const std::string &whereJson, const std::string &orderByJson, int64_t maxReturn, IServerCallback *callback)
	{
		Json::Value message;

		if (StringUtil::IsOptionalParameterValid(whereJson))
		{
			message[OperationParam::GlobalEntityServiceWhere.getValue()] = JsonUtil::jsonStringToValue(whereJson);
		}

		if (StringUtil::IsOptionalParameterValid(orderByJson))
		{
			message[OperationParam::GlobalEntityServiceOrderBy.getValue()] = JsonUtil::jsonStringToValue(orderByJson);
		}

		message[OperationParam::GlobalEntityServiceMaxReturn.getValue()] = (Json::Int64)maxReturn;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::GetList, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::getListCount(const std::string &whereJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GlobalEntityServiceWhere.getValue()] = JsonUtil::jsonStringToValue(whereJson);

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::GetListCount, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::getPage(const char *context, IServerCallback *callback)
	{
		Json::Value message;
		Json::Reader reader;

		Json::Value parsedContext;
		reader.parse(context, parsedContext);
		message[OperationParam::GlobalEntityServiceContext.getValue()] = parsedContext;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::GetPage, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::getPageOffset(const char *context, int pageOffset, IServerCallback *callback)
	{
		Json::Value message;

		message[OperationParam::GlobalEntityServiceContext.getValue()] = context;
		message[OperationParam::GlobalEntityServicePageOffset.getValue()] = pageOffset;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::GetPageOffset, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::incrementUserEntityData(const char *entityId, std::string jsonData, IServerCallback *callback)
	{
		Json::Value message;
		Json::Reader reader;

		message[OperationParam::EntityServiceEntityId.getValue()] = entityId;

		Json::Value parsedData;
		reader.parse(jsonData, parsedData);
		message[OperationParam::EntityServiceData.getValue()] = parsedData;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::IncrementUserEntityData, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudEntity::incrementSharedUserEntityData(const char *entityId, const char *targetProfileId, std::string jsonData, IServerCallback *callback)
	{
		Json::Value message;
		Json::Reader reader;

		message[OperationParam::EntityServiceEntityId.getValue()] = entityId;
		message[OperationParam::EntityServiceTargetPlayerId.getValue()] = targetProfileId;

		Json::Value parsedData;
		reader.parse(jsonData, parsedData);
		message[OperationParam::EntityServiceData.getValue()] = parsedData;

		ServerCall *sc = new ServerCall(ServiceName::Entity, ServiceOperation::IncrementSharedUserEntityData, message, callback);
		m_client->sendRequest(sc);
	}
}