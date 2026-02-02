// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudFriend.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/JsonUtil.h"
#include "braincloud/FriendPlatform.h"
#include "braincloud/AuthenticationType.h"

namespace BrainCloud
{
	BrainCloudFriend::BrainCloudFriend(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudFriend::findUserByExactUniversalId(const char *searchText, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceSearchText.getValue()] = searchText;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::FindUserByExactUniversalId, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::findUsersByNameStartingWith(const char *searchText, int32_t maxResults, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceSearchText.getValue()] = searchText;
		message[OperationParam::FriendServiceMaxResults.getValue()] = maxResults;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::FindUsersByUniversalIdStartingWith, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::findUsersByUniversalIdStartingWith(const char *searchText, int32_t maxResults, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceSearchText.getValue()] = searchText;
		message[OperationParam::FriendServiceMaxResults.getValue()] = maxResults;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::FindUsersByNameStartingWith, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::getProfileInfoForCredential(const char *externalId, AuthenticationType authenticationType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceExternalId.getValue()] = externalId;
		message[OperationParam::FriendServiceAuthenticationType.getValue()] = authenticationType.toString();

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::GetProfileInfoForCredential, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::getProfileInfoForCredentialIfExists(const char *externalId, AuthenticationType authenticationType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceExternalId.getValue()] = externalId;
		message[OperationParam::FriendServiceAuthenticationType.getValue()] = authenticationType.toString();

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::GetProfileInfoForCredentialIfExists, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::getProfileInfoForExternalAuthId(const char *externalId, const char *externalAuthType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceExternalId.getValue()] = externalId;
		message[OperationParam::ExternalAuthType.getValue()] = externalAuthType;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::GetProfileInfoForExternalAuthId, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::getProfileInfoForExternalAuthIdIfExists(const char *externalId, const char *externalAuthType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceExternalId.getValue()] = externalId;
		message[OperationParam::ExternalAuthType.getValue()] = externalAuthType;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::GetProfileInfoForExternalAuthIdIfExists, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::getExternalIdForProfileId(const char *profileId, const char *authenticationType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceProfileId.getValue()] = profileId;
		message[OperationParam::FriendServiceAuthenticationType.getValue()] = authenticationType;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::GetExternalIdForProfileId, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::readFriendEntity(const char *entityId, const char *friendId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceEntityId.getValue()] = entityId;
		message[OperationParam::FriendServiceFriendId.getValue()] = friendId;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::ReadFriendEntity, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::readFriendsEntities(const char *entityType, IServerCallback *callback)
	{
		Json::Value message;
		if (StringUtil::IsOptionalParameterValid(entityType))
		{
			message[OperationParam::FriendServiceEntityType.getValue()] = entityType;
		}

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::ReadFriendsEntities, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::readFriendUserState(const char *friendId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceReadPlayerStateFriendId.getValue()] = friendId;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::ReadFriendsPlayerState, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::getSummaryDataForProfileId(const char *profileId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceProfileId.getValue()] = profileId;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::GetSummaryDataForProfileId, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::findUsersByExactName(const char *searchText, int32_t maxResults, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceSearchText.getValue()] = searchText;
		message[OperationParam::FriendServiceMaxResults.getValue()] = maxResults;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::FindUsersByExactName, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::findUsersBySubstrName(const char *searchText, int32_t maxResults, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceSearchText.getValue()] = searchText;
		message[OperationParam::FriendServiceMaxResults.getValue()] = maxResults;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::FindUsersBySubstrName, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::listFriends(const FriendPlatform &friendPlatform, bool includeSummaryData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::FriendServiceFriendPlatform.getValue()] = friendPlatform.toString();
		message[OperationParam::FriendServiceIncludeSummaryData.getValue()] = includeSummaryData;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::ListFriends, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::addFriends(const std::vector<std::string> &profileIds, IServerCallback *callback)
	{
		Json::Value message;
		Json::Value profileArray = JsonUtil::stringVectorToJson(profileIds);
		message[OperationParam::FriendServiceProfileIds.getValue()] = profileArray;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::AddFriends, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::addFriendsFromPlatform(const FriendPlatform &friendPlatform, const std::string &mode, const std::vector<std::string> &externalIds, IServerCallback *callback)
	{
		Json::Value message;

		message[OperationParam::FriendServiceFriendPlatform.getValue()] = friendPlatform.toString();
		message[OperationParam::FriendServiceMode.getValue()] = mode;
		message[OperationParam::FriendServiceExternalIds.getValue()] = JsonUtil::stringVectorToJson(externalIds);

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::AddFriendsFromPlatform, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::removeFriends(const std::vector<std::string> &profileIds, IServerCallback *callback)
	{
		Json::Value message;
		Json::Value profileArray = JsonUtil::stringVectorToJson(profileIds);
		message[OperationParam::FriendServiceProfileIds.getValue()] = profileArray;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::RemoveFriends, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudFriend::getUsersOnlineStatus(const std::vector<std::string> &profileIds, IServerCallback *callback)
	{
		Json::Value message;
		Json::Value profileArray = JsonUtil::stringVectorToJson(profileIds);
		message[OperationParam::FriendServiceProfileIds.getValue()] = profileArray;

		ServerCall *sc = new ServerCall(ServiceName::Friend, ServiceOperation::GetUsersOnlineStatus, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}
}
