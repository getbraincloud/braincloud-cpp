// Copyright 2016 bitHeads, Inc. All Rights Reserved.

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
	BrainCloudIdentity::BrainCloudIdentity(BrainCloudClient* in_client) : m_client(in_client) { }

	void BrainCloudIdentity::attachFacebookIdentity(const char * in_facebookId, const char * in_authenticationToken, IServerCallback * in_callback)
	{
		attachIdentity(in_facebookId, in_authenticationToken, AuthenticationType::Facebook, in_callback);
	}

	void BrainCloudIdentity::mergeFacebookIdentity(const char * in_facebookId, const char * in_authenticationToken, IServerCallback * in_callback)
	{
		mergeIdentity(in_facebookId, in_authenticationToken, AuthenticationType::Facebook, in_callback);
	}

	void BrainCloudIdentity::detachFacebookIdentity(const char * in_facebookId, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_facebookId, AuthenticationType::Facebook, in_continueAnon, in_callback);
	}

		void BrainCloudIdentity::attachOculusIdentity(const char * in_oculusId, const char * in_oculusNonce, IServerCallback * in_callback)
	{
		attachIdentity(in_oculusId, in_oculusNonce, AuthenticationType::Oculus, in_callback);
	}

	void BrainCloudIdentity::mergeOculusIdentity(const char * in_oculusId, const char * in_oculusNonce, IServerCallback * in_callback)
	{
		mergeIdentity(in_oculusId, in_oculusNonce, AuthenticationType::Oculus, in_callback);
	}

	void BrainCloudIdentity::detachOculusIdentity(const char * in_oculusId, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_oculusId, AuthenticationType::Oculus, in_continueAnon, in_callback);
	}

    void BrainCloudIdentity::attachAdvancedIdentity(AuthenticationType in_authenticationType, const AuthenticationIds &in_ids, const std::string &in_extraJson, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::IdentityServiceExternalId.getValue()] = in_ids.externalId;
        message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
        message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = in_ids.authenticationToken;
        if (StringUtil::IsOptionalParameterValid(in_ids.authenticationSubType))
        {
            message[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = in_ids.authenticationSubType;
        }

        if (StringUtil::IsOptionalParameterValid(in_extraJson))
        {
            message[OperationParam::AuthenticateServiceAuthenticateExtraJson.getValue()] = JsonUtil::jsonStringToValue(in_extraJson);
        }

        ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::Attach, message, in_callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudIdentity::mergeAdvancedIdentity(AuthenticationType in_authenticationType, const AuthenticationIds &in_ids, const std::string &in_extraJson, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::IdentityServiceExternalId.getValue()] = in_ids.externalId;
        message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
        message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = in_ids.authenticationToken;
        if (StringUtil::IsOptionalParameterValid(in_ids.authenticationSubType))
        {
            message[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = in_ids.authenticationSubType;
        }

        if (StringUtil::IsOptionalParameterValid(in_extraJson))
        {
            message[OperationParam::AuthenticateServiceAuthenticateExtraJson.getValue()] = JsonUtil::jsonStringToValue(in_extraJson);
        }

        ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::Merge, message, in_callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudIdentity::detachAdvancedIdentity(AuthenticationType in_authenticationType, const std::string &in_externalId, bool in_continueAnon, const std::string &in_extraJson, IServerCallback * in_callback)
    {
        Json::Value message;
        message[OperationParam::IdentityServiceExternalId.getValue()] = in_externalId;
        message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
        message[OperationParam::IdentityServiceConfirmAnonymous.getValue()] = in_continueAnon;

        if (StringUtil::IsOptionalParameterValid(in_extraJson))
        {
            message[OperationParam::AuthenticateServiceAuthenticateExtraJson.getValue()] = JsonUtil::jsonStringToValue(in_extraJson);
        }

        ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::Detach, message, in_callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudIdentity::attachAppleIdentity(const char * in_appleId, const char * in_authenticationToken, IServerCallback * in_callback)
	{
		attachIdentity(in_appleId, in_authenticationToken, AuthenticationType::Apple, in_callback);
	}

	void BrainCloudIdentity::mergeAppleIdentity(const char * in_appleId, const char * in_authenticationToken, IServerCallback * in_callback)
	{
		mergeIdentity(in_appleId, in_authenticationToken, AuthenticationType::Apple, in_callback);
	}

	void BrainCloudIdentity::detachAppleIdentity(const char * in_appleId, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_appleId, AuthenticationType::Apple, in_continueAnon, in_callback);
	}

    void BrainCloudIdentity::attachUltraIdentity(const std::string &in_ultraUsername, const std::string &in_ultraIdToken, IServerCallback * in_callback)
    {
        attachIdentity(in_ultraUsername.c_str(), in_ultraIdToken.c_str(), AuthenticationType::Ultra, in_callback);
    }

    void BrainCloudIdentity::mergeUltraIdentity(const std::string &in_ultraUsername, const std::string &in_ultraIdToken, IServerCallback * in_callback)
    {
        mergeIdentity(in_ultraUsername.c_str(), in_ultraIdToken.c_str(), AuthenticationType::Ultra, in_callback);
    }

    void BrainCloudIdentity::detachUltraIdentity(const std::string &in_ultraUsername, bool in_continueAnon, IServerCallback * in_callback)
    {
        detachIdentity(in_ultraUsername.c_str(), AuthenticationType::Ultra, in_continueAnon, in_callback);
    }

	void BrainCloudIdentity::attachEmailIdentity(const char * in_email, const char * in_password, IServerCallback * in_callback)
	{
		attachIdentity(in_email, in_password, AuthenticationType::Email, in_callback);
	}

	void BrainCloudIdentity::mergeEmailIdentity(const char * in_email, const char * in_password, IServerCallback * in_callback)
	{
		mergeIdentity(in_email, in_password, AuthenticationType::Email, in_callback);
	}

	void BrainCloudIdentity::detachEmailIdentity(const char * in_email, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_email, AuthenticationType::Email, in_continueAnon, in_callback);
	}

	void BrainCloudIdentity::attachUniversalIdentity(const char * in_userId, const char * in_password, IServerCallback * in_callback)
	{
		attachIdentity(in_userId, in_password, AuthenticationType::Universal, in_callback);
	}

	void BrainCloudIdentity::mergeUniversalIdentity(const char * in_userId, const char * in_password, IServerCallback * in_callback)
	{
		mergeIdentity(in_userId, in_password, AuthenticationType::Universal, in_callback);
	}

	void BrainCloudIdentity::detachUniversalIdentity(const char * in_userId, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_userId, AuthenticationType::Universal, in_continueAnon, in_callback);
	}

	void BrainCloudIdentity::attachGameCenterIdentity(const char * in_gameCenterId, IServerCallback * in_callback)
	{
		attachIdentity(in_gameCenterId, "", AuthenticationType::GameCenter, in_callback);
	}

	void BrainCloudIdentity::mergeGameCenterIdentity(const char * in_gameCenterId, IServerCallback * in_callback)
	{
		mergeIdentity(in_gameCenterId, "", AuthenticationType::GameCenter, in_callback);
	}

	void BrainCloudIdentity::detachGameCenterIdentity(const char * in_gameCenterId, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_gameCenterId, AuthenticationType::GameCenter, in_continueAnon, in_callback);
	}

	void BrainCloudIdentity::attachSteamIdentity(const char * in_steamId, const char * in_sessionTicket, IServerCallback * in_callback)
	{
		attachIdentity(in_steamId, in_sessionTicket, AuthenticationType::Steam, in_callback);
	}

	void BrainCloudIdentity::mergeSteamIdentity(const char * in_steamId, const char * in_sessionTicket, IServerCallback * in_callback)
	{
		mergeIdentity(in_steamId, in_sessionTicket, AuthenticationType::Steam, in_callback);
	}

	void BrainCloudIdentity::detachSteamIdentity(const char * in_steamId, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_steamId, AuthenticationType::Steam, in_continueAnon, in_callback);
	}

	void BrainCloudIdentity::attachGoogleIdentity(const char * in_googleUserId, const char * in_serverAuthCode, IServerCallback * in_callback)
	{
		attachIdentity(in_googleUserId, in_serverAuthCode, AuthenticationType::Google, in_callback);
	}

	void BrainCloudIdentity::mergeGoogleIdentity(const char * in_googleUserId, const char * in_serverAuthCode, IServerCallback * in_callback)
	{
		mergeIdentity(in_googleUserId, in_serverAuthCode, AuthenticationType::Google, in_callback);
	}

	void BrainCloudIdentity::detachGoogleIdentity(const char * in_googleUserId, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_googleUserId, AuthenticationType::Google, in_continueAnon, in_callback);
	}

	void BrainCloudIdentity::attachGoogleOpenIdIdentity(const char * in_googleOpenId, const char * in_authenticationToken, IServerCallback * in_callback)
	{
		attachIdentity(in_googleOpenId, in_authenticationToken, AuthenticationType::GoogleOpenId, in_callback);
	}

	void BrainCloudIdentity::mergeGoogleOpenIdIdentity(const char * in_googleOpenId, const char * in_authenticationToken, IServerCallback * in_callback)
	{
		mergeIdentity(in_googleOpenId, in_authenticationToken, AuthenticationType::GoogleOpenId, in_callback);
	}

	void BrainCloudIdentity::detachGoogleOpenIdIdentity(const char * in_googleOpenId, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_googleOpenId, AuthenticationType::GoogleOpenId, in_continueAnon, in_callback);
	}

	void BrainCloudIdentity::attachTwitterIdentity(const char * in_twitterId, const char * in_authenticationToken, const char * in_secret, IServerCallback * in_callback)
	{
		std::string buffer(in_authenticationToken);
		buffer += ":";
		buffer += in_secret;
		attachIdentity(in_twitterId, buffer.c_str(), AuthenticationType::Twitter, in_callback);
	}

	void BrainCloudIdentity::mergeTwitterIdentity(const char * in_twitterId, const char * in_authenticationToken, const char * in_secret, IServerCallback * in_callback)
	{
		std::string buffer(in_authenticationToken);
		buffer += ":";
		buffer += in_secret;
		mergeIdentity(in_twitterId, buffer.c_str(), AuthenticationType::Twitter, in_callback);
	}

	void BrainCloudIdentity::detachTwitterIdentity(const char * in_twitterId, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_twitterId, AuthenticationType::Twitter, in_continueAnon, in_callback);
	}

	void BrainCloudIdentity::attachParseIdentity(const char * in_parseId, const char * in_authenticationToken, IServerCallback * in_callback)
	{
		attachIdentity(in_parseId, in_authenticationToken, AuthenticationType::Parse, in_callback);
	}

	void BrainCloudIdentity::mergeParseIdentity(const char * in_parseId, const char * in_authenticationToken, IServerCallback * in_callback)
	{
		mergeIdentity(in_parseId, in_authenticationToken, AuthenticationType::Parse, in_callback);
	}

	void BrainCloudIdentity::detachParseIdentity(const char * in_parseId, bool in_continueAnon, IServerCallback * in_callback)
	{
		detachIdentity(in_parseId, AuthenticationType::Parse, in_continueAnon, in_callback);
	}

	void BrainCloudIdentity::switchToChildProfile(const char * in_childProfileId, const char * in_childAppId, bool in_forceCreate, IServerCallback * in_callback)
	{
		switchToChildProfile(in_childProfileId, in_childAppId, in_forceCreate, false, in_callback);
	}

	void BrainCloudIdentity::switchToSingletonChildProfile(const char * in_childAppId, bool in_forceCreate, IServerCallback * in_callback)
	{
		switchToChildProfile(NULL, in_childAppId, in_forceCreate, true, in_callback);
	}

	void BrainCloudIdentity::switchToParentProfile(const char * in_parentLevelName, IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::AuthenticationServiceAuthenticationLevelName.getValue()] = in_parentLevelName;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::SwitchToParentProfile, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::getChildProfiles(bool in_includeSummaryData, IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::PlayerStateServiceIncludeSummaryData.getValue()] = in_includeSummaryData;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetChildProfiles, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::getIdentities(IServerCallback * in_callback)
	{
		Json::Value message = Json::nullValue;
		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetIdentities, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::getIdentityStatus(AuthenticationType in_authenticationType, const char *in_externalAuthName, IServerCallback *in_callback)
	{
		Json::Value message;

		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
		if (StringUtil::IsOptionalParameterValid(in_externalAuthName))
			message[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = in_externalAuthName;

		ServerCall *sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetIdentityStatus, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::getExpiredIdentities(IServerCallback * in_callback)
	{
		Json::Value message = Json::nullValue;
		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetExpiredIdentities, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::refreshIdentity(const char * in_externalId, const char * in_authenticationToken, AuthenticationType in_authenticationType, IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = in_externalId;
		message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = in_authenticationToken;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::RefreshIdentity, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::changeEmailIdentity(const char * in_oldEmailAddress, const char * in_password, const char * in_newEmailAddress, bool in_updateContactEmail,  IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::OldEmailAddress.getValue()] = in_oldEmailAddress;
		message[OperationParam::FieldAuthenticationToken.getValue()] = in_password;
		message[OperationParam::NewEmailAddress.getValue()] = in_newEmailAddress;
		message[OperationParam::UpdateContactEmail.getValue()] = in_updateContactEmail;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::ChangeEmailIdentity, message, in_callback);
		m_client->sendRequest(sc);
	}


	void BrainCloudIdentity::attachParentWithIdentity(const char * in_externalId, const char * in_authenticationToken, AuthenticationType in_authenticationType,
		const char * in_externalAuthName, bool in_forceCreate, IServerCallback * in_callback)
	{
		Json::Value data;
		data[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = in_authenticationToken;
		data[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
		data[OperationParam::IdentityServiceExternalId.getValue()] = in_externalId;
		data[OperationParam::AuthenticateServiceAuthenticateForceCreate.getValue()] = in_forceCreate;

		if (StringUtil::IsOptionalParameterValid(in_externalAuthName))
			data[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = in_externalAuthName;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::AttachParentWithIdentity, data, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::detachParent(IServerCallback * in_callback)
	{
		Json::Value data = Json::nullValue;
		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::DetachParent, data, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::attachPeerProfile(const char * in_peer, const char * in_externalId, const char * in_authenticationToken, AuthenticationType in_authenticationType,
		const char * in_externalAuthName, bool in_forceCreate, IServerCallback * in_callback)
	{
		Json::Value data;
		data[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = in_authenticationToken;
		data[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
		data[OperationParam::IdentityServiceExternalId.getValue()] = in_externalId;
		data[OperationParam::AuthenticateServiceAuthenticateForceCreate.getValue()] = in_forceCreate;
		data[OperationParam::Peer.getValue()] = in_peer;

		if (StringUtil::IsOptionalParameterValid(in_externalAuthName))
			data[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = in_externalAuthName;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::AttachPeerProfile, data, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::detachPeer(const char * in_peer, IServerCallback * in_callback)
	{
		Json::Value data;
		data[OperationParam::Peer.getValue()] = in_peer;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::DetachPeer, data, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::getPeerProfiles(IServerCallback * in_callback)
	{
		Json::Value data = Json::nullValue;
		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::GetPeerProfiles, data, in_callback);
		m_client->sendRequest(sc);
	}

	// Private methods

	void BrainCloudIdentity::switchToChildProfile(const char * in_childProfileId, const char * in_childAppId, bool in_forceCreate, bool in_forceSingleton, IServerCallback * in_callback)
	{
		BrainCloudClient * brainCloudClientRef = m_client;
		Json::Value message;

		if (StringUtil::IsOptionalParameterValid(in_childProfileId)) {
			message[OperationParam::AuthenticateServiceAuthenticateProfileId.getValue()] = in_childProfileId;
		}

		message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = in_childAppId;
		message[OperationParam::AuthenticateServiceAuthenticateForceCreate.getValue()] = in_forceCreate;
		message[OperationParam::IdentityServiceForceSingleton.getValue()] = in_forceSingleton;

		message[OperationParam::AuthenticateServiceAuthenticateReleasePlatform.getValue()] = brainCloudClientRef->getReleasePlatform().c_str();
		message[OperationParam::AuthenticateServiceAuthenticateCountryCode.getValue()] = brainCloudClientRef->getCountryCode().c_str();
		message[OperationParam::AuthenticateServiceAuthenticateLanguageCode.getValue()] = brainCloudClientRef->getLanguageCode().c_str();
		message[OperationParam::AuthenticateServiceAuthenticateTimeZoneOffset.getValue()] = brainCloudClientRef->getTimezoneOffset();

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::SwitchToChildProfile, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::attachIdentity(const char * in_facebookId, const char * in_authenticationToken, AuthenticationType in_authenticationType, IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = in_facebookId;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
		message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = in_authenticationToken;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::Attach, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::mergeIdentity(const char * in_facebookId, const char * in_authenticationToken, AuthenticationType in_authenticationType, IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = in_facebookId;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
		message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = in_authenticationToken;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::Merge, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::detachIdentity(const char * in_facebookId, AuthenticationType in_authenticationType, bool in_continueAnon, IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = in_facebookId;
		message[OperationParam::IdentityServiceAuthenticationType.getValue()] = in_authenticationType.toString();
		message[OperationParam::IdentityServiceConfirmAnonymous.getValue()] = in_continueAnon;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::Detach, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::attachNonLoginUniversalId(const char * in_externalId, IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = in_externalId;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::AttachNonLoginUniversalId, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::updateUniversalIdLogin(const char * in_externalId, IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceExternalId.getValue()] = in_externalId;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::UpdateUniversalIdLogin, message, in_callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudIdentity::attachBlockchainIdentity(const char* in_blockchainConfig, const char* in_publicKey, IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceBlockchainConfig.getValue()] = in_blockchainConfig;
		message[OperationParam::IdentityServicePublicKey.getValue()] = in_publicKey;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::AttachBlockchainIdentity, message, in_callback);
		m_client->sendRequest(sc);
	}
	
	void BrainCloudIdentity::detachBlockchainIdentity(const char* in_blockchainConfig, IServerCallback * in_callback)
	{
		Json::Value message;
		message[OperationParam::IdentityServiceBlockchainConfig.getValue()] = in_blockchainConfig;

		ServerCall * sc = new ServerCall(ServiceName::Identity, ServiceOperation::DetachBlockchainIdentity, message, in_callback);
		m_client->sendRequest(sc);
	}

}
