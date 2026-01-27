// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudIdentity.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "braincloud/AuthenticationType.h"
#include "braincloud/AuthenticationIds.h"
#include "json/json.h"

#include "braincloud/internal/JsonUtil.h"
#include "braincloud/internal/StringUtil.h"

namespace BrainCloud
{
	BrainCloudIdentity::BrainCloudIdentity(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudIdentity::attachFacebookIdentity(const char *facebookId, const char *authenticationToken, IServerCallback *callback)
	{
		attachIdentity(facebookId, authenticationToken, AuthenticationType::Facebook, callback);
	}

	void BrainCloudIdentity::mergeFacebookIdentity(const char *facebookId, const char *authenticationToken, IServerCallback *callback)
	{
		mergeIdentity(facebookId, authenticationToken, AuthenticationType::Facebook, callback);
	}

	void BrainCloudIdentity::detachFacebookIdentity(const char *facebookId, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(facebookId, AuthenticationType::Facebook, continueAnon, callback);
	}

	void BrainCloudIdentity::attachOculusIdentity(const char *oculusId, const char *oculusNonce, IServerCallback *callback)
	{
		attachIdentity(oculusId, oculusNonce, AuthenticationType::Oculus, callback);
	}

	void BrainCloudIdentity::mergeOculusIdentity(const char *oculusId, const char *oculusNonce, IServerCallback *callback)
	{
		mergeIdentity(oculusId, oculusNonce, AuthenticationType::Oculus, callback);
	}

	void BrainCloudIdentity::detachOculusIdentity(const char *oculusId, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(oculusId, AuthenticationType::Oculus, continueAnon, callback);
	}

	void BrainCloudIdentity::attachAdvancedIdentity(AuthenticationType authenticationType, const AuthenticationIds &ids, const std::string &extraJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = ids.externalId;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = authenticationType.toString();
		message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = ids.authenticationToken;
		if (StringUtil::IsOptionalParameterValid(ids.authenticationSubType))
		{
			message[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = ids.authenticationSubType;
		}

		if (StringUtil::IsOptionalParameterValid(extraJson))
		{
			message[OperationParam::AuthenticateServiceAuthenticateExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson);
		}

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::Attach, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::mergeAdvancedIdentity(AuthenticationType authenticationType, const AuthenticationIds &ids, const std::string &extraJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = ids.externalId;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = authenticationType.toString();
		message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = ids.authenticationToken;
		if (StringUtil::IsOptionalParameterValid(ids.authenticationSubType))
		{
			message[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = ids.authenticationSubType;
		}

		if (StringUtil::IsOptionalParameterValid(extraJson))
		{
			message[OperationParam::AuthenticateServiceAuthenticateExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson);
		}

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::Merge, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::detachAdvancedIdentity(AuthenticationType authenticationType, const std::string &externalId, bool continueAnon, const std::string &extraJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = externalId;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = authenticationType.toString();
		message[OperationParam::IdentityServiceConfirmAnonymous.getValue()] = continueAnon;

		if (StringUtil::IsOptionalParameterValid(extraJson))
		{
			message[OperationParam::AuthenticateServiceAuthenticateExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson);
		}

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::Detach, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::attachAppleIdentity(const char *appleId, const char *authenticationToken, IServerCallback *callback)
	{
		attachIdentity(appleId, authenticationToken, AuthenticationType::Apple, callback);
	}

	void BrainCloudIdentity::mergeAppleIdentity(const char *appleId, const char *authenticationToken, IServerCallback *callback)
	{
		mergeIdentity(appleId, authenticationToken, AuthenticationType::Apple, callback);
	}

	void BrainCloudIdentity::detachAppleIdentity(const char *appleId, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(appleId, AuthenticationType::Apple, continueAnon, callback);
	}

	void BrainCloudIdentity::attachUltraIdentity(const std::string &ultraUsername, const std::string &ultraIdToken, IServerCallback *callback)
	{
		attachIdentity(ultraUsername.c_str(), ultraIdToken.c_str(), AuthenticationType::Ultra, callback);
	}

	void BrainCloudIdentity::mergeUltraIdentity(const std::string &ultraUsername, const std::string &ultraIdToken, IServerCallback *callback)
	{
		mergeIdentity(ultraUsername.c_str(), ultraIdToken.c_str(), AuthenticationType::Ultra, callback);
	}

	void BrainCloudIdentity::detachUltraIdentity(const std::string &ultraUsername, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(ultraUsername.c_str(), AuthenticationType::Ultra, continueAnon, callback);
	}

	void BrainCloudIdentity::attachEmailIdentity(const char *email, const char *password, IServerCallback *callback)
	{
		attachIdentity(email, password, AuthenticationType::Email, callback);
	}

	void BrainCloudIdentity::mergeEmailIdentity(const char *email, const char *password, IServerCallback *callback)
	{
		mergeIdentity(email, password, AuthenticationType::Email, callback);
	}

	void BrainCloudIdentity::detachEmailIdentity(const char *email, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(email, AuthenticationType::Email, continueAnon, callback);
	}

	void BrainCloudIdentity::attachUniversalIdentity(const char *userId, const char *password, IServerCallback *callback)
	{
		attachIdentity(userId, password, AuthenticationType::Universal, callback);
	}

	void BrainCloudIdentity::mergeUniversalIdentity(const char *userId, const char *password, IServerCallback *callback)
	{
		mergeIdentity(userId, password, AuthenticationType::Universal, callback);
	}

	void BrainCloudIdentity::detachUniversalIdentity(const char *userId, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(userId, AuthenticationType::Universal, continueAnon, callback);
	}

	void BrainCloudIdentity::attachGameCenterIdentity(const char *gameCenterId, IServerCallback *callback)
	{
		attachIdentity(gameCenterId, "", AuthenticationType::GameCenter, callback);
	}

	void BrainCloudIdentity::mergeGameCenterIdentity(const char *gameCenterId, IServerCallback *callback)
	{
		mergeIdentity(gameCenterId, "", AuthenticationType::GameCenter, callback);
	}

	void BrainCloudIdentity::detachGameCenterIdentity(const char *gameCenterId, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(gameCenterId, AuthenticationType::GameCenter, continueAnon, callback);
	}

	void BrainCloudIdentity::attachSteamIdentity(const char *steamId, const char *sessionTicket, IServerCallback *callback)
	{
		attachIdentity(steamId, sessionTicket, AuthenticationType::Steam, callback);
	}

	void BrainCloudIdentity::mergeSteamIdentity(const char *steamId, const char *sessionTicket, IServerCallback *callback)
	{
		mergeIdentity(steamId, sessionTicket, AuthenticationType::Steam, callback);
	}

	void BrainCloudIdentity::detachSteamIdentity(const char *steamId, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(steamId, AuthenticationType::Steam, continueAnon, callback);
	}

	void BrainCloudIdentity::attachGoogleIdentity(const char *googleUserId, const char *serverAuthCode, IServerCallback *callback)
	{
		attachIdentity(googleUserId, serverAuthCode, AuthenticationType::Google, callback);
	}

	void BrainCloudIdentity::mergeGoogleIdentity(const char *googleUserId, const char *serverAuthCode, IServerCallback *callback)
	{
		mergeIdentity(googleUserId, serverAuthCode, AuthenticationType::Google, callback);
	}

	void BrainCloudIdentity::detachGoogleIdentity(const char *googleUserId, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(googleUserId, AuthenticationType::Google, continueAnon, callback);
	}

	void BrainCloudIdentity::attachGoogleOpenIdIdentity(const char *googleOpenId, const char *authenticationToken, IServerCallback *callback)
	{
		attachIdentity(googleOpenId, authenticationToken, AuthenticationType::GoogleOpenId, callback);
	}

	void BrainCloudIdentity::mergeGoogleOpenIdIdentity(const char *googleOpenId, const char *authenticationToken, IServerCallback *callback)
	{
		mergeIdentity(googleOpenId, authenticationToken, AuthenticationType::GoogleOpenId, callback);
	}

	void BrainCloudIdentity::detachGoogleOpenIdIdentity(const char *googleOpenId, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(googleOpenId, AuthenticationType::GoogleOpenId, continueAnon, callback);
	}

	void BrainCloudIdentity::attachTwitterIdentity(const char *twitterId, const char *authenticationToken, const char *secret, IServerCallback *callback)
	{
		std::string buffer(authenticationToken);
		buffer += ":";
		buffer += secret;
		attachIdentity(twitterId, buffer.c_str(), AuthenticationType::Twitter, callback);
	}

	void BrainCloudIdentity::mergeTwitterIdentity(const char *twitterId, const char *authenticationToken, const char *secret, IServerCallback *callback)
	{
		std::string buffer(authenticationToken);
		buffer += ":";
		buffer += secret;
		mergeIdentity(twitterId, buffer.c_str(), AuthenticationType::Twitter, callback);
	}

	void BrainCloudIdentity::detachTwitterIdentity(const char *twitterId, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(twitterId, AuthenticationType::Twitter, continueAnon, callback);
	}

	void BrainCloudIdentity::attachParseIdentity(const char *parseId, const char *authenticationToken, IServerCallback *callback)
	{
		attachIdentity(parseId, authenticationToken, AuthenticationType::Parse, callback);
	}

	void BrainCloudIdentity::mergeParseIdentity(const char *parseId, const char *authenticationToken, IServerCallback *callback)
	{
		mergeIdentity(parseId, authenticationToken, AuthenticationType::Parse, callback);
	}

	void BrainCloudIdentity::detachParseIdentity(const char *parseId, bool continueAnon, IServerCallback *callback)
	{
		detachIdentity(parseId, AuthenticationType::Parse, continueAnon, callback);
	}

	void BrainCloudIdentity::switchToChildProfile(const char *childProfileId, const char *childAppId, bool forceCreate, IServerCallback *callback)
	{
		switchToChildProfile(childProfileId, childAppId, forceCreate, false, callback);
	}

	void BrainCloudIdentity::switchToSingletonChildProfile(const char *childAppId, bool forceCreate, IServerCallback *callback)
	{
		switchToChildProfile(NULL, childAppId, forceCreate, true, callback);
	}

	void BrainCloudIdentity::switchToParentProfile(const char *parentLevelName, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::AuthenticationServiceAuthenticationLevelName.getValue()] = parentLevelName;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::SwitchToParentProfile, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::getChildProfiles(bool includeSummaryData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PlayerStateServiceIncludeSummaryData.getValue()] = includeSummaryData;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetChildProfiles, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::getIdentities(IServerCallback *callback)
	{
		Json::Value message = Json::nullValue;
		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetIdentities, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::getIdentityStatus(AuthenticationType authenticationType, const char *externalAuthName, IServerCallback *callback)
	{
		Json::Value message;

		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = authenticationType.toString();
		if (StringUtil::IsOptionalParameterValid(externalAuthName))
			message[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = externalAuthName;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetIdentityStatus, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::getExpiredIdentities(IServerCallback *callback)
	{
		Json::Value message = Json::nullValue;
		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetExpiredIdentities, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::refreshIdentity(const char *externalId, const char *authenticationToken, AuthenticationType authenticationType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = externalId;
		message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = authenticationToken;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = authenticationType.toString();

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::RefreshIdentity, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::changeEmailIdentity(const char *oldEmailAddress, const char *password, const char *newEmailAddress, bool updateContactEmail, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::OldEmailAddress.getValue()] = oldEmailAddress;
		message[OperationParam::FieldAuthenticationToken.getValue()] = password;
		message[OperationParam::NewEmailAddress.getValue()] = newEmailAddress;
		message[OperationParam::UpdateContactEmail.getValue()] = updateContactEmail;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::ChangeEmailIdentity, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::attachParentWithIdentity(const char *externalId, const char *authenticationToken, AuthenticationType authenticationType,
													  const char *externalAuthName, bool forceCreate, IServerCallback *callback)
	{
		Json::Value data;
		data[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = authenticationToken;
		data[OperationParam::IdentityServiceAuthenticationType.getValue()] = authenticationType.toString();
		data[OperationParam::IdentityServiceExternalId.getValue()] = externalId;
		data[OperationParam::AuthenticateServiceAuthenticateForceCreate.getValue()] = forceCreate;

		if (StringUtil::IsOptionalParameterValid(externalAuthName))
			data[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = externalAuthName;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::AttachParentWithIdentity, data, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::detachParent(IServerCallback *callback)
	{
		Json::Value data = Json::nullValue;
		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::DetachParent, data, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::attachPeerProfile(const char *peer, const char *externalId, const char *authenticationToken, AuthenticationType authenticationType,
											   const char *externalAuthName, bool forceCreate, IServerCallback *callback)
	{
		Json::Value data;
		data[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = authenticationToken;
		data[OperationParam::IdentityServiceAuthenticationType.getValue()] = authenticationType.toString();
		data[OperationParam::IdentityServiceExternalId.getValue()] = externalId;
		data[OperationParam::AuthenticateServiceAuthenticateForceCreate.getValue()] = forceCreate;
		data[OperationParam::Peer.getValue()] = peer;

		if (StringUtil::IsOptionalParameterValid(externalAuthName))
			data[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = externalAuthName;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::AttachPeerProfile, data, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::detachPeer(const char *peer, IServerCallback *callback)
	{
		Json::Value data;
		data[OperationParam::Peer.getValue()] = peer;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::DetachPeer, data, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::getPeerProfiles(IServerCallback *callback)
	{
		Json::Value data = Json::nullValue;
		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetPeerProfiles, data, callback);
		m_client->sendRequest(sc);
	}

	// Private methods

	void BrainCloudIdentity::switchToChildProfile(const char *childProfileId, const char *childAppId, bool forceCreate, bool forceSingleton, IServerCallback *callback)
	{
		BrainCloudClient *brainCloudClientRef = m_client;
		Json::Value message;

		if (StringUtil::IsOptionalParameterValid(childProfileId))
		{
			message[OperationParam::AuthenticateServiceAuthenticateProfileId.getValue()] = childProfileId;
		}

		message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = childAppId;
		message[OperationParam::AuthenticateServiceAuthenticateForceCreate.getValue()] = forceCreate;
		message[OperationParam::IdentityServiceForceSingleton.getValue()] = forceSingleton;

		message[OperationParam::AuthenticateServiceAuthenticateReleasePlatform.getValue()] = brainCloudClientRef->getReleasePlatform().c_str();
		message[OperationParam::AuthenticateServiceAuthenticateCountryCode.getValue()] = brainCloudClientRef->getCountryCode().c_str();
		message[OperationParam::AuthenticateServiceAuthenticateLanguageCode.getValue()] = brainCloudClientRef->getLanguageCode().c_str();
		message[OperationParam::AuthenticateServiceAuthenticateTimeZoneOffset.getValue()] = brainCloudClientRef->getTimezoneOffset();

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::SwitchToChildProfile, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::attachIdentity(const char *facebookId, const char *authenticationToken, AuthenticationType authenticationType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = facebookId;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = authenticationType.toString();
		message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = authenticationToken;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::Attach, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::mergeIdentity(const char *facebookId, const char *authenticationToken, AuthenticationType authenticationType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = facebookId;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = authenticationType.toString();
		message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = authenticationToken;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::Merge, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::detachIdentity(const char *facebookId, AuthenticationType authenticationType, bool continueAnon, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = facebookId;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = authenticationType.toString();
		message[OperationParam::IdentityServiceConfirmAnonymous.getValue()] = continueAnon;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::Detach, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::attachNonLoginUniversalId(const char *externalId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = externalId;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::AttachNonLoginUniversalId, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::updateUniversalIdLogin(const char *externalId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = externalId;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::UpdateUniversalIdLogin, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::attachBlockchainIdentity(const char *blockchainConfig, const char *publicKey, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceBlockchainConfig.getValue()] = blockchainConfig;
		message[OperationParam::IdentityServicePublicKey.getValue()] = publicKey;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::AttachBlockchainIdentity, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::detachBlockchainIdentity(const char *blockchainConfig, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceBlockchainConfig.getValue()] = blockchainConfig;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::DetachBlockchainIdentity, message, callback);
		m_client->sendRequest(sc);
	}

}
