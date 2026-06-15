// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudAuthentication.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "braincloud/AuthenticationIds.h"
#include "json/json.h"
#include "braincloud/internal/JsonUtil.h"
#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/GUID.h"

namespace BrainCloud {
    BrainCloudAuthentication::BrainCloudAuthentication(BrainCloudClient* client) :
        m_client(client),
        _anonymousId(""),
        _profileId("") ,
		_clientLib("cpp")
    {
    }

	void BrainCloudAuthentication::setClientLib(const char * lib)
	{
		if (_clientLib == "obj-c" || _clientLib == "cpp")
			_clientLib = lib;
	}

    void BrainCloudAuthentication::initialize(const char * profileId, const char * anonymousId)
    {
        _profileId = profileId;
        _anonymousId = anonymousId;
    }

	std::string BrainCloudAuthentication::generateAnonymousId()
	{
		return BrainCloud::GUID::generateGUID();
	}

    void BrainCloudAuthentication::clearSavedProfileId()
    {
        _profileId = "";
    }

    void BrainCloudAuthentication::authenticateAdvanced(AuthenticationType authenticationType, 
                                                        const AuthenticationIds &ids, 
                                                        bool forceCreate, 
                                                        const std::string &extraJson, 
                                                        IServerCallback * callback)
    {
        authenticate(ids.externalId.c_str(), ids.authenticationToken.c_str(), authenticationType, ids.authenticationSubType.c_str(), forceCreate, extraJson, callback);
    }

    void BrainCloudAuthentication::authenticateAnonymous(bool forceCreate, IServerCallback * callback)
    {
        authenticate(_anonymousId.c_str(), "", AuthenticationType::Anonymous, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::authenticateFacebook(const char * fbUserId, const char * fbAuthToken, bool forceCreate, IServerCallback * callback)
    {
        authenticate(fbUserId, fbAuthToken, AuthenticationType::Facebook, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::authenticateFacebookLimited(const char * fbLimitedUserId, const char * fbAuthToken, bool forceCreate, IServerCallback * callback)
    {
        authenticate(fbLimitedUserId, fbAuthToken, AuthenticationType::FacebookLimited, NULL, forceCreate, "", callback);
    }

       void BrainCloudAuthentication::authenticateOculus(const char * oculusUserId, const char * oculusNonce, bool forceCreate, IServerCallback * callback)
    {
        authenticate(oculusUserId, oculusNonce, AuthenticationType::Oculus, NULL, forceCreate, "", callback);
    }

    std::string BrainCloudAuthentication::createGameCenterAuthenticationToken(uint64_t timestamp,
                                                                              const std::string& publicKeyUrl,
                                                                              const uint8_t* signature, size_t signatureLength,
                                                                              const uint8_t* salt, size_t saltLength,
                                                                              const std::string& teamPlayerId)
    {
        if (salt == NULL || saltLength == 0 ||
            signature == NULL || signatureLength == 0 ||
            publicKeyUrl.empty() ||
            timestamp == 0)
        {
            return "";
        }

        std::string teamPlayerIdJson = teamPlayerId.empty() ? "null" : "\"" + teamPlayerId + "\"";

        std::string json = "{\"playerId\":" + teamPlayerIdJson +
                           ",\"timestamp\":" + std::to_string(timestamp) +
                           ",\"publicKeyUrl\":\"" + publicKeyUrl + "\"" +
                           ",\"signature\":\"" + StringUtil::Base64Encode(signature, signatureLength) + "\"" +
                           ",\"salt\":\"" + StringUtil::Base64Encode(salt, saltLength) + "\"}";

        return StringUtil::Base64Encode(json);
    }

    void BrainCloudAuthentication::authenticateGameCenter(const char* gameCenterId, bool forceCreate,
                                                          uint64_t timestamp,
                                                          const std::string& publicKeyUrl,
                                                          const uint8_t* signature, size_t signatureLength,
                                                          const uint8_t* salt, size_t saltLength,
                                                          const std::string& teamPlayerId,
                                                          IServerCallback* callback)
    {
        std::string authenticationToken = createGameCenterAuthenticationToken(timestamp, publicKeyUrl, signature, signatureLength, salt, saltLength, teamPlayerId);

        authenticate(gameCenterId, authenticationToken.c_str(), AuthenticationType::GameCenter, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::authenticateEmailPassword(const char * email, const char * password, bool forceCreate, IServerCallback * callback)
    {
        authenticate(email, password, AuthenticationType::Email, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::authenticateUltra(const std::string &ultraUsername, const std::string &ultraIdToken, bool forceCreate, IServerCallback * callback)
    {
        authenticate(ultraUsername.c_str(), ultraIdToken.c_str(), AuthenticationType::Ultra, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::authenticateUniversal(const char * userid, const char * password, bool forceCreate, IServerCallback * callback)
    {
        authenticate(userid, password, AuthenticationType::Universal, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::getServerVersion(IServerCallback *callback)
    {
        Json::Value message;

        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = m_client->getAppId().c_str();

        ServerCall *sc = new ServerCall(ServiceName::AuthenticateV2, ServiceOperation::GetServerVersion, message, callback);

        m_client->sendRequest(sc);
    }

    void BrainCloudAuthentication::authenticateSteam(const char * userid, const char * sessionticket, bool forceCreate, IServerCallback * callback)
    {
        authenticate(userid, sessionticket, AuthenticationType::Steam, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::authenticateApple(const char * appleUserId, const char * identityToken, bool forceCreate, IServerCallback * callback)
    {
        authenticate(appleUserId, identityToken, AuthenticationType::Apple, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::authenticateGoogle(const char * googleUserId, const char * serverAuthToken, bool forceCreate, IServerCallback * callback)
    {
        authenticate(googleUserId, serverAuthToken, AuthenticationType::Google, NULL, forceCreate, "", callback);
    }
    
    void BrainCloudAuthentication::authenticateGoogleOpenId(const char * googleUserAccountEmail, const char * IdToken, bool forceCreate, IServerCallback * callback)
    {
        authenticate(googleUserAccountEmail, IdToken, AuthenticationType::GoogleOpenId, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::authenticateTwitter(const char * userid, const char * token, const char * secret, bool forceCreate, IServerCallback * callback)
    {
        std::string buffer(token);
        buffer += ":";
        buffer += secret;
        authenticate(userid, buffer.c_str(), AuthenticationType::Twitter, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::authenticateParse(const char * userid, const char * token, bool forceCreate, IServerCallback * callback)
    {
        authenticate(userid, token, AuthenticationType::Parse, NULL, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::authenticateHandoff(const char * handoffId, const char * securityToken, IServerCallback * callback)
    {
        authenticate(handoffId, securityToken, AuthenticationType::Handoff, NULL, false, "", callback);
    }

	void BrainCloudAuthentication::authenticateSettopHandoff(const char * handoffCode, IServerCallback * callback)
    {
        authenticate(handoffCode, "", AuthenticationType::SettopHandoff, NULL, false, "", callback);
    }

    void BrainCloudAuthentication::authenticateExternal(
        const char * userid,
        const char * token,
        const char * externalAuthName,
        bool forceCreate,
        IServerCallback * callback)
    {
        authenticate(userid, token, AuthenticationType::External, externalAuthName, forceCreate, "", callback);
    }

    void BrainCloudAuthentication::resetEmailPassword(const char * externalId, IServerCallback * callback)
    {
        Json::Value message;
        message[OperationParam::AuthenticateServiceAuthenticateExternalId.getValue()] = externalId;
        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = m_client->getAppId().c_str();

        ServerCall * sc = new ServerCall(ServiceName::AuthenticateV2, ServiceOperation::ResetEmailPassword, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudAuthentication::resetEmailPasswordAdvanced(const char * emailAddress, std::string serviceParams, IServerCallback * callback)
    {
        Json::Value message;
        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = m_client->getAppId().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateEmailAddress.getValue()] = emailAddress;
        message[OperationParam::AuthenticateServiceAuthenticateServiceParams.getValue()] = JsonUtil::jsonStringToValue(serviceParams);

        ServerCall * sc = new ServerCall(ServiceName::AuthenticateV2, ServiceOperation::ResetEmailPasswordAdvanced, message, callback);
        m_client->sendRequest(sc);
    }

        void BrainCloudAuthentication::resetEmailPasswordWithExpiry(const char * externalId, int tokenTtlInMinutes, IServerCallback * callback)
    {
        Json::Value message;
        message[OperationParam::AuthenticateServiceAuthenticateExternalId.getValue()] = externalId;
        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = m_client->getAppId().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateTokenTtlInMinutes.getValue()] = tokenTtlInMinutes;

        ServerCall * sc = new ServerCall(ServiceName::AuthenticateV2, ServiceOperation::ResetEmailPasswordWithExpiry, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudAuthentication::resetEmailPasswordAdvancedWithExpiry(const char * emailAddress, std::string serviceParams, int tokenTtlInMinutes, IServerCallback * callback)
    {
        Json::Value message;
        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = m_client->getAppId().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateEmailAddress.getValue()] = emailAddress;
        message[OperationParam::AuthenticateServiceAuthenticateServiceParams.getValue()] = JsonUtil::jsonStringToValue(serviceParams);;
        message[OperationParam::AuthenticateServiceAuthenticateTokenTtlInMinutes.getValue()] = tokenTtlInMinutes;

        ServerCall * sc = new ServerCall(ServiceName::AuthenticateV2, ServiceOperation::ResetEmailPasswordAdvancedWithExpiry, message, callback);
        m_client->sendRequest(sc);
    }

	void BrainCloudAuthentication::resetUniversalIdPassword(const char * universalId, IServerCallback * callback)
    {
        Json::Value message;
        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = m_client->getAppId().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateUniversalId.getValue()] = universalId;

        ServerCall * sc = new ServerCall(ServiceName::AuthenticateV2, ServiceOperation::ResetUniversalIdPassword, message, callback);
        m_client->sendRequest(sc);
    }

	void BrainCloudAuthentication::resetUniversalIdPasswordAdvanced(const char * universalId, std::string serviceParams, IServerCallback * callback)
    {
        Json::Value message;
        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = m_client->getAppId().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateUniversalId.getValue()] = universalId;
        message[OperationParam::AuthenticateServiceAuthenticateServiceParams.getValue()] = JsonUtil::jsonStringToValue(serviceParams);;

        ServerCall * sc = new ServerCall(ServiceName::AuthenticateV2, ServiceOperation::ResetUniversalIdPasswordAdvanced, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudAuthentication::resetUniversalIdPasswordWithExpiry(const char * universalId, int tokenTtlInMinutes, IServerCallback * callback)
    {
        Json::Value message;
        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = m_client->getAppId().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateUniversalId.getValue()] = universalId;
        message[OperationParam::AuthenticateServiceAuthenticateTokenTtlInMinutes.getValue()] = tokenTtlInMinutes;

        ServerCall * sc = new ServerCall(ServiceName::AuthenticateV2, ServiceOperation::ResetUniversalIdPasswordWithExpiry, message, callback);
        m_client->sendRequest(sc);
    }

	void BrainCloudAuthentication::resetUniversalIdPasswordAdvancedWithExpiry(const char * universalId, std::string serviceParams, int tokenTtlInMinutes, IServerCallback * callback)
    {
        Json::Value message;
        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = m_client->getAppId().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateUniversalId.getValue()] = universalId;
        message[OperationParam::AuthenticateServiceAuthenticateServiceParams.getValue()] = JsonUtil::jsonStringToValue(serviceParams);;
        message[OperationParam::AuthenticateServiceAuthenticateTokenTtlInMinutes.getValue()] = tokenTtlInMinutes;

        ServerCall * sc = new ServerCall(ServiceName::AuthenticateV2, ServiceOperation::ResetUniversalIdPasswordAdvancedWithExpiry, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudAuthentication::retryPreviousAuthenticate(IServerCallback * callback)
    {
        authenticate(m_previousAuthParams.externalId.c_str(),
                     m_previousAuthParams.authenticationToken.c_str(),
                     m_previousAuthParams.authenticationType,
                     m_previousAuthParams.externalAuthName.c_str(),
                     m_previousAuthParams.forceCreate,
                     m_previousAuthParams.extraJson.c_str(),
                     callback);
    }

    void BrainCloudAuthentication::authenticate(
        const char * externalId,
        const char * authenticationToken,
        AuthenticationType authenticationType,
        const char * externalAuthName,
        bool forceCreate, 
        const std::string &extraJson,
        IServerCallback * callback)
    {
        m_previousAuthParams.externalId = externalId ? externalId : "";
        m_previousAuthParams.authenticationToken = authenticationToken ? authenticationToken : "";
        m_previousAuthParams.authenticationType = authenticationType;
        m_previousAuthParams.externalAuthName = externalAuthName ? externalAuthName : "";
        m_previousAuthParams.forceCreate = forceCreate;
        m_previousAuthParams.extraJson = extraJson;

        Json::Value message;
        message[OperationParam::AuthenticateServiceAuthenticateExternalId.getValue()] = externalId;
        message[OperationParam::AuthenticateServiceAuthenticateAuthenticationToken.getValue()] = authenticationToken;
        message[OperationParam::AuthenticateServiceAuthenticateAuthenticationType.getValue()] = authenticationType.toString();
        message[OperationParam::AuthenticateServiceAuthenticateForceCreate.getValue()] = forceCreate;
        message[OperationParam::AuthenticateServiceAuthenticateCompressResponses.getValue()] = compressResponses;

        message[OperationParam::AuthenticateServiceAuthenticateProfileId.getValue()] = _profileId;
        message[OperationParam::AuthenticateServiceAuthenticateAnonymousId.getValue()] = _anonymousId;
        message[OperationParam::AuthenticateServiceAuthenticateGameId.getValue()] = m_client->getAppId().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateReleasePlatform.getValue()] = m_client->getReleasePlatform().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateGameVersion.getValue()] = m_client->getAppVersion().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateBrainCloudVersion.getValue()] = m_client->getBrainCloudClientVersion().c_str();
        if (StringUtil::IsOptionalParameterValid(externalAuthName))
        {
            message[OperationParam::AuthenticateServiceAuthenticateExternalAuthName.getValue()] = externalAuthName;
        }

        message[OperationParam::AuthenticateServiceAuthenticateCountryCode.getValue()] = m_client->getCountryCode().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateLanguageCode.getValue()] = m_client->getLanguageCode().c_str();
        message[OperationParam::AuthenticateServiceAuthenticateTimeZoneOffset.getValue()] = m_client->getTimezoneOffset();
        message["clientLib"] = _clientLib;

        if (StringUtil::IsOptionalParameterValid(extraJson))
        {
            message[OperationParam::AuthenticateServiceAuthenticateExtraJson.getValue()] = JsonUtil::jsonStringToValue(extraJson);
        }

        ServerCall * sc = new ServerCall(ServiceName::AuthenticateV2, ServiceOperation::Authenticate, message, callback);
        m_client->sendRequest(sc);
    }
}
