// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudGroup.h"

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
	BrainCloudGroup::BrainCloudGroup(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudGroup::acceptGroupInvitation(const char *groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::AcceptGroupInvitation, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::addGroupMember(const char *groupId, const char *profileId, eGroupMember::Role role, const std::string &jsonAttributes, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupProfileId.getValue()] = profileId;
		if (role != eGroupMember::UNKNOWN)
			message[OperationParam::GroupRole.getValue()] = roleToString(role);
		if (StringUtil::IsOptionalParameterValid(jsonAttributes))
			message[OperationParam::GroupAttributes.getValue()] = JsonUtil::jsonStringToValue(jsonAttributes);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::AddGroupMember, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::approveGroupJoinRequest(const char *groupId, const char *profileId, eGroupMember::Role role, const std::string &jsonAttributes, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupProfileId.getValue()] = profileId;
		if (role != eGroupMember::UNKNOWN)
			message[OperationParam::GroupRole.getValue()] = roleToString(role);
		if (StringUtil::IsOptionalParameterValid(jsonAttributes))
			message[OperationParam::GroupAttributes.getValue()] = JsonUtil::jsonStringToValue(jsonAttributes);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::ApproveGroupJoinRequest, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::autoJoinGroup(const char *groupType, eAutoJoinStrategy::Strategy autoJoinStrategy, std::string dataQueryJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupType.getValue()] = groupType;
		message[OperationParam::GroupAutoJoinStrategy.getValue()] = autoJoinStrategyToString(autoJoinStrategy);

		if (StringUtil::IsOptionalParameterValid(dataQueryJson))
			message[OperationParam::GroupWhere.getValue()] = JsonUtil::jsonStringToValue(dataQueryJson);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::AutoJoinGroup, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::autoJoinGroupMulti(const std::vector<std::string> &groupTypes, eAutoJoinStrategy::Strategy autoJoinStrategy, std::string where, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupTypes.getValue()] = JsonUtil::stringVectorToJson(groupTypes);
		message[OperationParam::GroupAutoJoinStrategy.getValue()] = autoJoinStrategyToString(autoJoinStrategy);

		if (StringUtil::IsOptionalParameterValid(where))
			message[OperationParam::GroupWhere.getValue()] = JsonUtil::jsonStringToValue(where);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::AutoJoinGroupMulti, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::cancelGroupInvitation(const char *groupId, const char *profileId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupProfileId.getValue()] = profileId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::CancelGroupInvitation, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::createGroup(
		const char *name,
		const char *groupType,
		bool isOpenGroup,
		const std::string &acl,
		const std::string &jsonData,
		const std::string &jsonOwnerAttributes,
		const std::string &jsonDefaultMemberAttributes,
		IServerCallback *callback)
	{
		Json::Value message;
		if (StringUtil::IsOptionalParameterValid(name))
			message[OperationParam::GroupName.getValue()] = name;
		message[OperationParam::GroupType.getValue()] = groupType;
		message[OperationParam::GroupIsOpenGroup.getValue()] = isOpenGroup;
		if (StringUtil::IsOptionalParameterValid(acl))
			message[OperationParam::GroupAcl.getValue()] = JsonUtil::jsonStringToValue(acl);
		if (StringUtil::IsOptionalParameterValid(jsonData))
			message[OperationParam::GroupData.getValue()] = JsonUtil::jsonStringToValue(jsonData);
		if (StringUtil::IsOptionalParameterValid(jsonOwnerAttributes))
			message[OperationParam::GroupOwnerAttributes.getValue()] = JsonUtil::jsonStringToValue(jsonOwnerAttributes);
		if (StringUtil::IsOptionalParameterValid(jsonDefaultMemberAttributes))
			message[OperationParam::GroupDefaultMemberAttributes.getValue()] = JsonUtil::jsonStringToValue(jsonDefaultMemberAttributes);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::CreateGroup, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::createGroupWithSummaryData(
		const char *name,
		const char *groupType,
		bool isOpenGroup,
		const std::string &acl,
		const std::string &jsonData,
		const std::string &jsonOwnerAttributes,
		const std::string &jsonDefaultMemberAttributes,
		const std::string &jsonSummaryData,
		IServerCallback *callback)
	{
		Json::Value message;
		if (StringUtil::IsOptionalParameterValid(name))
			message[OperationParam::GroupName.getValue()] = name;
		message[OperationParam::GroupType.getValue()] = groupType;
		message[OperationParam::GroupIsOpenGroup.getValue()] = isOpenGroup;
		if (StringUtil::IsOptionalParameterValid(acl))
			message[OperationParam::GroupAcl.getValue()] = JsonUtil::jsonStringToValue(acl);
		if (StringUtil::IsOptionalParameterValid(jsonData))
			message[OperationParam::GroupData.getValue()] = JsonUtil::jsonStringToValue(jsonData);
		if (StringUtil::IsOptionalParameterValid(jsonOwnerAttributes))
			message[OperationParam::GroupOwnerAttributes.getValue()] = JsonUtil::jsonStringToValue(jsonOwnerAttributes);
		if (StringUtil::IsOptionalParameterValid(jsonDefaultMemberAttributes))
			message[OperationParam::GroupDefaultMemberAttributes.getValue()] = JsonUtil::jsonStringToValue(jsonDefaultMemberAttributes);
		if (StringUtil::IsOptionalParameterValid(jsonSummaryData))
			message[OperationParam::GroupSummaryData.getValue()] = JsonUtil::jsonStringToValue(jsonSummaryData);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::CreateGroup, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::createGroupEntity(
		const char *groupId,
		const char *entityType,
		bool isOwnedByGroupMember,
		const std::string &acl,
		const std::string &jsonData,
		IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		if (StringUtil::IsOptionalParameterValid(entityType))
			message[OperationParam::GroupEntityType.getValue()] = entityType;
		message[OperationParam::GroupIsOwnedByGroupMember.getValue()] = isOwnedByGroupMember;
		if (StringUtil::IsOptionalParameterValid(acl))
			message[OperationParam::GroupAcl.getValue()] = JsonUtil::jsonStringToValue(acl);
		if (StringUtil::IsOptionalParameterValid(jsonData))
			message[OperationParam::GroupData.getValue()] = JsonUtil::jsonStringToValue(jsonData);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::CreateGroupEntity, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::deleteGroup(const char *groupId, int32_t version, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupVersion.getValue()] = version;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::DeleteGroup, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::deleteGroupEntity(const char *groupId, const char *entityId, int32_t version, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupEntityId.getValue()] = entityId;
		message[OperationParam::GroupVersion.getValue()] = version;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::DeleteGroupEntity, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::getMyGroups(IServerCallback *callback)
	{
		Json::Value nullMsg = Json::nullValue;
		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::GetMyGroups, nullMsg, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::incrementGroupData(const char *groupId, const std::string &jsonData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupData.getValue()] = JsonUtil::jsonStringToValue(jsonData);
		;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::IncrementGroupData, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::incrementGroupEntityData(const char *groupId, const char *entityId, const std::string &jsonData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupEntityId.getValue()] = entityId;
		message[OperationParam::GroupData.getValue()] = JsonUtil::jsonStringToValue(jsonData);
		;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::IncrementGroupEntityData, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::inviteGroupMember(const char *groupId, const char *profileId, eGroupMember::Role role, const std::string &jsonAttributes, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupProfileId.getValue()] = profileId;
		if (role != eGroupMember::UNKNOWN)
			message[OperationParam::GroupRole.getValue()] = roleToString(role);
		if (StringUtil::IsOptionalParameterValid(jsonAttributes))
			message[OperationParam::GroupAttributes.getValue()] = JsonUtil::jsonStringToValue(jsonAttributes);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::InviteGroupMember, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::joinGroup(const char *groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::JoinGroup, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::leaveGroup(const char *groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::LeaveGroup, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::listGroupsPage(const std::string &jsonContext, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupContext.getValue()] = JsonUtil::jsonStringToValue(jsonContext);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::ListGroupsPage, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::listGroupsPageByOffset(const char *encodedContext, int32_t pageOffset, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupContext.getValue()] = encodedContext;
		message[OperationParam::GroupPageOffset.getValue()] = pageOffset;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::ListGroupsPageByOffset, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::listGroupsWithMember(const char *profileId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupProfileId.getValue()] = profileId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::ListGroupsWithMember, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::readGroup(const char *groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::ReadGroup, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::readGroupData(const char *groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::ReadGroupData, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::readGroupEntitiesPage(const std::string &jsonContext, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupContext.getValue()] = JsonUtil::jsonStringToValue(jsonContext);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::ReadGroupEntitiesPage, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::readGroupEntitiesPageByOffset(const char *encodedContext, int32_t pageOffset, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupContext.getValue()] = encodedContext;
		message[OperationParam::GroupPageOffset.getValue()] = pageOffset;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::ReadGroupEntitiesPageByOffset, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::readGroupEntity(const char *groupId, const char *entityId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupEntityId.getValue()] = entityId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::ReadGroupEntity, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::readGroupMembers(const char *groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::ReadGroupMembers, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::rejectGroupInvitation(const char *groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::RejectGroupInvitation, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::rejectGroupJoinRequest(const char *groupId, const char *profileId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupProfileId.getValue()] = profileId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::RejectGroupJoinRequest, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::deleteGroupJoinRequest(const char *groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::DeleteGroupJoinRequest, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::removeGroupMember(const char *groupId, const char *profileId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupProfileId.getValue()] = profileId;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::RemoveGroupMember, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::updateGroupData(const char *groupId, int32_t version, const std::string &jsonData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupVersion.getValue()] = version;
		message[OperationParam::GroupData.getValue()] = JsonUtil::jsonStringToValue(jsonData);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::UpdateGroupData, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::updateGroupEntityAcl(const char *groupId, const char *entityId, const std::string &acl, IServerCallback *callback)
	{
		Json::Value data;

		data[OperationParam::GroupId.getValue()] = groupId;
		data[OperationParam::GroupEntityId.getValue()] = entityId;
		data[OperationParam::GroupAcl.getValue()] = JsonUtil::jsonStringToValue(acl);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::UpdateGroupEntityAcl, data, callback);

		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::updateGroupEntityData(const char *groupId, const char *entityId, int32_t version, const std::string &jsonData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupEntityId.getValue()] = entityId;
		message[OperationParam::GroupVersion.getValue()] = version;
		message[OperationParam::GroupData.getValue()] = JsonUtil::jsonStringToValue(jsonData);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::UpdateGroupEntityData, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::updateGroupMember(const char *groupId, const char *profileId, eGroupMember::Role role, const std::string &jsonAttributes, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupProfileId.getValue()] = profileId;
		if (role != eGroupMember::UNKNOWN)
			message[OperationParam::GroupRole.getValue()] = roleToString(role);
		if (StringUtil::IsOptionalParameterValid(jsonAttributes))
			message[OperationParam::GroupAttributes.getValue()] = JsonUtil::jsonStringToValue(jsonAttributes);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::UpdateGroupMember, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::updateGroupName(const char *groupId, const char *name, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupName.getValue()] = name;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::UpdateGroupName, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::setGroupOpen(const char *groupId, bool isOpenGroup, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupIsOpenGroup.getValue()] = isOpenGroup;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::SetGroupOpen, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::updateGroupAcl(const char *groupId, const std::string &acl, IServerCallback *callback)
	{
		Json::Value data;

		data[OperationParam::GroupId.getValue()] = groupId;
		data[OperationParam::GroupAcl.getValue()] = JsonUtil::jsonStringToValue(acl);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::UpdateGroupAcl, data, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::updateGroupSummaryData(const char *groupId, int version, const std::string &jsonSummaryData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::GroupVersion.getValue()] = version;
		if (StringUtil::IsOptionalParameterValid(jsonSummaryData))
			message[OperationParam::GroupSummaryData.getValue()] = JsonUtil::jsonStringToValue(jsonSummaryData);

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::UpdateGroupSummaryData, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudGroup::getRandomGroupsMatching(std::string jsonWhere, int maxReturn, IServerCallback *callback)
	{
		Json::Value message;
		if (StringUtil::IsOptionalParameterValid(jsonWhere))
			message[OperationParam::GroupWhere.getValue()] = JsonUtil::jsonStringToValue(jsonWhere);
		message[OperationParam::MaxReturn.getValue()] = maxReturn;

		ServerCall *sc = new ServerCall(ServiceName::Group, ServiceOperation::GetRandomGroupsMatching, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	std::string BrainCloudGroup::roleToString(eGroupMember::Role role)
	{
		if (role == eGroupMember::MEMBER)
			return "MEMBER";
		else if (role == eGroupMember::ADMIN)
			return "ADMIN";
		else if (role == eGroupMember::OWNER)
			return "OWNER";

		return "OTHER";
	}

	std::string BrainCloudGroup::autoJoinStrategyToString(eAutoJoinStrategy::Strategy strategy)
	{
		if (strategy == eAutoJoinStrategy::JoinFirstGroup)
			return "JoinFirstGroup";
		else if (strategy == eAutoJoinStrategy::JoinRandomGroup)
			return "JoinRandomGroup";
		return "UNKNOWN";
	}
}
