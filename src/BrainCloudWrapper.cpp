// Copyright 2026 bitHeads, Inc. All Rights Reserved.
#include "braincloud/BrainCloudWrapper.h"
#include "braincloud/internal/SaveDataHelper.h"
#include "braincloud/BrainCloudClient.h"
#include "braincloud/AuthenticationIds.h"
#include "braincloud/reason_codes.h"
#include "braincloud/http_codes.h"
#if defined(__ANDROID__)
#include "braincloud/internal/android/AndroidGlobals.h"
#endif
#define PROFILE_ID_KEY "profileId"
#define ANONYMOUS_ID_KEY "anonymousId"
#define AUTHENTICATION_TYPE_KEY "authenticationType"

namespace BrainCloud
{

    BrainCloudWrapper *BrainCloudWrapper::m_instance = NULL;

    std::string BrainCloudWrapper::AUTHENTICATION_ANONYMOUS = "anonymous";

    BrainCloudWrapper::BrainCloudWrapper(const char *wrapperName)
        : client(NULL), m_authenticateCallback(NULL), m_lastUrl(""), m_lastSecretKey(""), m_lastGameId(""), m_lastGameVersion(""), m_wrapperName(wrapperName), m_alwaysAllowProfileSwitch(true)
    {
    }

    BrainCloudWrapper::~BrainCloudWrapper()
    {
        if (this != m_instance)
        {
            if (client != NULL)
            {
                delete client;
                client = NULL;
            }
        }
    }

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#else
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
    BrainCloudWrapper *BrainCloudWrapper::getInstance()
    {
        if (BrainCloudClient::EnableSingletonMode == false)
        {
            throw std::invalid_argument(BrainCloudClient::SingletonUseErrorMessage);
        }

        if (m_instance == NULL)
        {
            m_instance = new BrainCloudWrapper();

            m_instance->client = BrainCloudClient::getInstance();
        }

        return m_instance;
    }
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#else
#pragma warning(pop)
#endif

    void BrainCloudWrapper::initialize(const char *url, const char *secretKey, const char *appId, const char *version, const char *companyName, const char *appName)
    {
        if (client == NULL)
        {
            client = new BrainCloudClient();
        }

        // save the app info in case we need to reauthenticate
        m_lastUrl = url;
        m_lastSecretKey = secretKey;
        m_lastGameId = appId;
        m_lastGameVersion = version;
        m_secretMap.clear();

        // initialize the client with our app info
        client->initialize(url, secretKey, appId, version);

        // inialize the save data helper with our company and app name
        // if this is not called the profile ids will not be saved
        SaveDataHelper::getInstance()->initialize(companyName, appName);

        // initialize brainCloud identity
        initializeIdentity();
    }

    void BrainCloudWrapper::initializeWithApps(const char *url, const char *defaultAppId, const std::map<std::string, std::string> &secretMap, const char *version, const char *companyName, const char *appName)
    {
        if (client == NULL)
        {
            client = new BrainCloudClient();
        }

        // Find the default secret key that matches the default app id
        std::string defaultSecretKey;
        std::map<std::string, std::string>::const_iterator it = secretMap.find(defaultAppId);
        if (it != secretMap.end())
        {
            defaultSecretKey = it->second;
        }

        // save the app info in case we need to reauthenticate
        m_lastUrl = url;
        m_lastSecretKey = defaultSecretKey;
        m_lastGameId = defaultAppId;
        m_lastGameVersion = version;
        m_secretMap = secretMap;

        // initialize the client with our app info
        client->initializeWithApps(url, defaultAppId, secretMap, version);

        // inialize the save data helper with our company and app name
        // if this is not called the profile ids will not be saved
        SaveDataHelper::getInstance()->initialize(companyName, appName);

        // initialize brainCloud identity
        initializeIdentity();
    }

    void BrainCloudWrapper::initializeIdentity(bool isAnonymousAuth)
    {
        // check if we already have saved IDs
        std::string profileId = getStoredProfileId();
        std::string anonymousId = getStoredAnonymousId();

        // create an anonymous ID if necessary
        if ((!anonymousId.empty() && profileId.empty()) || anonymousId.empty())
        {
            anonymousId = client->getAuthenticationService()->generateAnonymousId();
            profileId = "";
            setStoredAnonymousId(anonymousId.c_str());
            setStoredProfileId(profileId.c_str());
        }

        std::string profileIdToAuthenticateWith = profileId;
        if (!isAnonymousAuth && m_alwaysAllowProfileSwitch)
        {
            profileIdToAuthenticateWith = "";
        }
        setStoredAuthenticationType(isAnonymousAuth ? AUTHENTICATION_ANONYMOUS.c_str() : "");

        // send our IDs to brainCloud
        client->initializeIdentity(profileIdToAuthenticateWith.c_str(), anonymousId.c_str());
    }

    void BrainCloudWrapper::reauthenticate()
    {
        // send our saved app info to brainCloud
        // company and app name can be NULL since they are already set
        initialize(m_lastUrl.c_str(), m_lastSecretKey.c_str(), m_lastGameId.c_str(), m_lastGameVersion.c_str(), NULL, NULL);

        std::string authType = getStoredAuthenticationType();
        if (authType == AUTHENTICATION_ANONYMOUS)
        {
            authenticateAnonymous();
        }
    }

    // authenticate the player with an anonymous id
    void BrainCloudWrapper::authenticateAnonymous(IServerCallback *callback, bool forceCreate)
    {
        m_authenticateCallback = callback;

        initializeIdentity(true);

        client->getAuthenticationService()->authenticateAnonymous(forceCreate, this);
    }

    void BrainCloudWrapper::authenticateEmailPassword(const char *email, const char *password, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateEmailPassword(email, password, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateExternal(const char *userid, const char *token, const char *externalAuthName, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateExternal(userid, token, externalAuthName, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateFacebook(const char *fbUserId, const char *fbAuthToken, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateFacebook(fbUserId, fbAuthToken, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateOculus(const char *oculusUserId, const char *oculusNonce, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateOculus(oculusUserId, oculusNonce, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateGameCenter(const char *gameCenterId, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateGameCenter(gameCenterId, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateApple(const char *appleUserId, const char *identityToken, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateApple(appleUserId, identityToken, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateGoogle(const char *googleUserId, const char *serverAuthCode, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateGoogle(googleUserId, serverAuthCode, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateGoogleOpenId(const char *googleUserAccountEmail, const char *IdToken, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateGoogleOpenId(googleUserAccountEmail, IdToken, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateSteam(const char *userid, const char *sessionticket, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateSteam(userid, sessionticket, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateTwitter(const char *userid, const char *token, const char *secret, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateTwitter(userid, token, secret, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateUniversal(const char *userid, const char *password, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateUniversal(userid, password, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateUltra(const std::string &ultraUsername, const std::string &ultraIdToken, bool forceCreate, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateUltra(ultraUsername, ultraIdToken, forceCreate, this);
    }

    void BrainCloudWrapper::authenticateAdvanced(AuthenticationType authenticationType, const AuthenticationIds &ids, bool forceCreate, const std::string &extraJson, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateAdvanced(authenticationType, ids, forceCreate, extraJson, this);
    }

    void BrainCloudWrapper::authenticateHandoff(const char *handoffId, const char *securityToken, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateHandoff(handoffId, securityToken, callback);
    }

    void BrainCloudWrapper::authenticateSettopHandoff(const char *handoffCode, IServerCallback *callback)
    {
        m_authenticateCallback = callback;

        initializeIdentity();

        client->getAuthenticationService()->authenticateSettopHandoff(handoffCode, callback);
    }

    /**
     *	Callbacks used for the smart switches
     */
    class IdentityCallback : public IServerCallback
    {
    public:
        BrainCloudWrapper *wrapper;
        IServerCallback *authenticateCallback;

        IdentityCallback(BrainCloudWrapper *wrapper, IServerCallback *authenticateCallback)
        {
            wrapper = wrapper;
            authenticateCallback = authenticateCallback;
        }

        void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
        {
            Json::Value response;
            Json::Reader reader;

            reader.parse(jsonData, response);

            Json::Value identitiesObject = response["data"]["identities"];
            std::vector<std::string> listOfIdentities = identitiesObject.getMemberNames();

            if (listOfIdentities.size())
            {
                wrapper->client->getPlayerStateService()->logout();
            }
            else
            {
                wrapper->client->getPlayerStateService()->deleteUser();
            }

            wrapper->client->insertEndOfMessageBundleMarker();
            authenticateCallback->serverCallback(serviceName, serviceOperation, jsonData);
            delete this;
        }

        void serverError(ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string &jsonError)
        {
            authenticateCallback->serverError(serviceName, serviceOperation, statusCode, reasonCode, jsonError);
            delete this;
        }
    };

    class SmartSwitchCallback : public IServerCallback
    {
    public:
        SmartSwitchCallback(BrainCloudWrapper *wrapper, IServerCallback *callback)
        {
            wrapper = wrapper;
            callback = callback;
        }

        BrainCloudWrapper *wrapper;
        IServerCallback *callback;

        void serverError(ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string &jsonError)
        {
            callback->serverError(serviceName, serviceOperation, statusCode, reasonCode, jsonError);
            delete this;
        }
    };

    void BrainCloudWrapper::smartSwitchAuthenticateEmailPassword(const char *email, const char *password, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *email, const char *password, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                email = email;
                password = password;
                forceCreate = forceCreate;
            }

            const char *email;
            const char *password;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateEmailPassword(email, password, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, email, password, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateExternal(const char *userid, const char *token, const char *externalAuthName, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *userid, const char *token, const char *externalAuthName, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                userid = userid;
                token = token;
                externalAuthName = externalAuthName;
                forceCreate = forceCreate;
            }

            const char *userid;
            const char *token;
            const char *externalAuthName;
            const char *fbAuthToken;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateExternal(userid, token, externalAuthName, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, userid, token, externalAuthName, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateFacebook(const char *fbUserId, const char *fbAuthToken, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *fbUserId, const char *fbAuthToken, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                fbUserId = fbUserId;
                fbAuthToken = fbAuthToken;
                forceCreate = forceCreate;
            }

            const char *fbUserId;
            const char *fbAuthToken;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateFacebook(fbUserId, fbAuthToken, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, fbUserId, fbAuthToken, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateOculus(const char *oculusUserId, const char *oculusNonce, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *oculusUserId, const char *oculusNonce, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                oculusUserId = oculusUserId;
                oculusNonce = oculusNonce;
                forceCreate = forceCreate;
            }

            const char *oculusUserId;
            const char *oculusNonce;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateOculus(oculusUserId, oculusNonce, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, oculusUserId, oculusNonce, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateGameCenter(const char *gameCenterId, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *gameCenterId, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                gameCenterId = gameCenterId;
                forceCreate = forceCreate;
            }

            const char *gameCenterId;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateGameCenter(gameCenterId, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, gameCenterId, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateGoogle(const char *userid, const char *token, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *userid, const char *token, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                userid = userid;
                token = token;
                forceCreate = forceCreate;
            }

            const char *userid;
            const char *token;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateGoogle(userid, token, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, userid, token, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateGoogleOpenId(const char *userid, const char *token, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *userid, const char *token, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                userid = userid;
                token = token;
                forceCreate = forceCreate;
            }

            const char *userid;
            const char *token;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateGoogle(userid, token, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, userid, token, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateApple(const char *userid, const char *token, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *userid, const char *token, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                userid = userid;
                token = token;
                forceCreate = forceCreate;
            }

            const char *userid;
            const char *token;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateApple(userid, token, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, userid, token, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateSteam(const char *userid, const char *sessionticket, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *userid, const char *sessionticket, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                userid = userid;
                sessionticket = sessionticket;
                forceCreate = forceCreate;
            }

            const char *userid;
            const char *sessionticket;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateSteam(userid, sessionticket, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, userid, sessionticket, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateTwitter(const char *userid, const char *token, const char *secret, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *userid, const char *token, const char *secret, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                userid = userid;
                token = token;
                secret = secret;
                forceCreate = forceCreate;
            }

            const char *userid;
            const char *token;
            const char *secret;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateTwitter(userid, token, secret, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, userid, token, secret, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateUniversal(const char *userid, const char *password, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const char *userid, const char *password, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                userid = userid;
                password = password;
                forceCreate = forceCreate;
            }

            const char *userid;
            const char *password;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateUniversal(userid, password, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, userid, password, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateUltra(const std::string &ultraUsername, const std::string &ultraIdToken, bool forceCreate, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, const std::string &ultraUsername, const std::string &ultraIdToken, bool forceCreate, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                ultraUsername = ultraUsername;
                ultraIdToken = ultraIdToken;
                forceCreate = forceCreate;
            }

            std::string ultraUsername;
            std::string ultraIdToken;
            bool forceCreate;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateUltra(ultraUsername, ultraIdToken, forceCreate, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, ultraUsername, ultraIdToken, forceCreate, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::smartSwitchAuthenticateAdvanced(AuthenticationType authenticationType, const AuthenticationIds &ids, bool forceCreate, const std::string &extraJson, IServerCallback *callback)
    {
        class SmartSwitchAuthenticateCallback : public SmartSwitchCallback
        {
        public:
            SmartSwitchAuthenticateCallback(BrainCloudWrapper *wrapper, AuthenticationType authenticationType, const AuthenticationIds &ids, bool forceCreate, const std::string &extraJson, IServerCallback *callback) : SmartSwitchCallback(wrapper, callback)
            {
                authenticationType = authenticationType;
                ids = ids;
                forceCreate = forceCreate;
                extraJson = extraJson;
            }

            AuthenticationType authenticationType = AuthenticationType::Unknown;
            AuthenticationIds ids;
            bool forceCreate;
            std::string extraJson;

            void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData)
            {
                wrapper->clearIds();
                wrapper->client->getAuthenticationService()->authenticateAdvanced(authenticationType, ids, forceCreate, extraJson, callback);
                delete this;
            }
        };

        SmartSwitchAuthenticateCallback *smartCallback = new SmartSwitchAuthenticateCallback(this, authenticationType, ids, forceCreate, extraJson, callback);
        getIdentitiesCallback(smartCallback);
    }

    void BrainCloudWrapper::getIdentitiesCallback(IServerCallback *success)
    {
        IdentityCallback *identityCallback = new IdentityCallback(this, success);
        identityCallback->authenticateCallback = success;

        if (client->isAuthenticated())
        {
            client->getIdentityService()->getIdentities(identityCallback);
        }
        else
        {
            success->serverCallback(BrainCloud::ServiceName::AuthenticateV2, BrainCloud::ServiceOperation::Authenticate, "");
        }
    }

    void BrainCloudWrapper::logout(bool forgetUser, IServerCallback *callback)
    {
        if (forgetUser)
        {
            resetStoredProfileId();
        }
        client->getPlayerStateService()->logout(callback);
    }

    void BrainCloudWrapper::resetEmailPassword(const char *externalId, IServerCallback *callback)
    {
        client->getAuthenticationService()->resetEmailPassword(externalId, callback);
    }

    void BrainCloudWrapper::resetEmailPasswordAdvanced(const char *emailAddress, std::string serviceParams, IServerCallback *callback)
    {
        client->getAuthenticationService()->resetEmailPasswordAdvanced(emailAddress, serviceParams, callback);
    }

    void BrainCloudWrapper::resetEmailPasswordWithExpiry(const char *externalId, int tokenTtlInMinutes, IServerCallback *callback)
    {
        client->getAuthenticationService()->resetEmailPasswordWithExpiry(externalId, tokenTtlInMinutes, callback);
    }

    void BrainCloudWrapper::resetEmailPasswordAdvancedWithExpiry(const char *emailAddress, std::string serviceParams, int tokenTtlInMinutes, IServerCallback *callback)
    {
        client->getAuthenticationService()->resetEmailPasswordAdvancedWithExpiry(emailAddress, serviceParams, tokenTtlInMinutes, callback);
    }

    void BrainCloudWrapper::resetUniversalIdPassword(const char *externalId, IServerCallback *callback)
    {
        client->getAuthenticationService()->resetUniversalIdPassword(externalId, callback);
    }

    void BrainCloudWrapper::resetUniversalIdPasswordAdvanced(const char *externalId, std::string serviceParams, IServerCallback *callback)
    {
        client->getAuthenticationService()->resetUniversalIdPasswordAdvanced(externalId, serviceParams, callback);
    }

    void BrainCloudWrapper::resetUniversalIdPasswordWithExpiry(const char *externalId, int tokenTtlInMinutes, IServerCallback *callback)
    {
        client->getAuthenticationService()->resetUniversalIdPasswordWithExpiry(externalId, tokenTtlInMinutes, callback);
    }

    void BrainCloudWrapper::resetUniversalIdPasswordAdvancedWithExpiry(const char *externalId, std::string serviceParams, int tokenTtlInMinutes, IServerCallback *callback)
    {
        client->getAuthenticationService()->resetUniversalIdPasswordAdvancedWithExpiry(externalId, serviceParams, tokenTtlInMinutes, callback);
    }

    void BrainCloudWrapper::reconnect(IServerCallback *callback)
    {
        authenticateAnonymous(callback, false);
    }

    bool BrainCloudWrapper::canReconnect()
    {
        return getStoredProfileId() != "" && getStoredAnonymousId() != "";
    }

    void BrainCloudWrapper::clearIds()
    {
        resetStoredProfileId();
        resetStoredAnonymousId();
    }

    void BrainCloudWrapper::runCallbacks()
    {
        client->runCallbacks();
    }

    std::string BrainCloudWrapper::getStoredProfileId()
    {
        return SaveDataHelper::getInstance()->readData(PROFILE_ID_KEY);
    }

    void BrainCloudWrapper::setStoredProfileId(const char *profileId)
    {
        SaveDataHelper::getInstance()->saveData(PROFILE_ID_KEY, profileId);
    }

    std::string BrainCloudWrapper::getStoredAnonymousId()
    {
        return SaveDataHelper::getInstance()->readData(ANONYMOUS_ID_KEY);
    }

    void BrainCloudWrapper::setStoredAnonymousId(const char *anonymousId)
    {
        SaveDataHelper::getInstance()->saveData(ANONYMOUS_ID_KEY, anonymousId);
    }

    void BrainCloudWrapper::resetStoredAnonymousId()
    {
        SaveDataHelper::getInstance()->deleteData(ANONYMOUS_ID_KEY);
        client->getAuthenticationService()->setAnonymousId("");
    }

    void BrainCloudWrapper::resetStoredProfileId()
    {
        SaveDataHelper::getInstance()->deleteData(PROFILE_ID_KEY);
        client->getAuthenticationService()->setProfileId("");
    }

    std::string BrainCloudWrapper::getStoredAuthenticationType()
    {
        return SaveDataHelper::getInstance()->readData(AUTHENTICATION_TYPE_KEY);
    }

    void BrainCloudWrapper::setStoredAuthenticationType(const char *authenticationType)
    {
        SaveDataHelper::getInstance()->saveData(AUTHENTICATION_TYPE_KEY, authenticationType);
    }

    void BrainCloudWrapper::resetStoredAuthenticationType()
    {
        SaveDataHelper::getInstance()->deleteData(AUTHENTICATION_TYPE_KEY);
    }

    void BrainCloudWrapper::setAlwaysAllowProfileSwitch(bool alwaysAllow)
    {
        m_alwaysAllowProfileSwitch = alwaysAllow;
    }

    bool BrainCloudWrapper::getAlwaysAllowProfileSwitch()
    {
        return m_alwaysAllowProfileSwitch;
    }

    void BrainCloudWrapper::serverCallback(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, std::string const &jsonData)
    {
        if (serviceName == BrainCloud::ServiceName::AuthenticateV2 && serviceOperation == BrainCloud::ServiceOperation::Authenticate)
        {
            Json::Reader reader;
            Json::Value data;
            reader.parse(jsonData, data);

            std::string profileId = data["data"]["profileId"].asString();

            if (profileId != "")
            {
                setStoredProfileId(profileId.c_str());
            }
        }

        if (m_authenticateCallback != NULL)
        {
            m_authenticateCallback->serverCallback(serviceName, serviceOperation, jsonData);
        }
    }

    void BrainCloudWrapper::serverError(BrainCloud::ServiceName serviceName,
                                        BrainCloud::ServiceOperation serviceOperation,
                                        int statusCode, int reasonCode, const std::string &message)
    {
        if (statusCode == HTTP_ACCEPTED && reasonCode == MANUAL_REDIRECT) // This should only happen on auth calls
        {
            // Manual redirection
            Json::Reader reader;
            Json::Value data;
            reader.parse(message, data);

            m_lastUrl = data["redirect_url"].asString();
            auto newGameId = data["redirect_appid"].asString();

            // re-initialize the client with our app info
            if (m_secretMap.empty())
            {
                if (newGameId != "")
                    m_lastGameId = newGameId;
                client->initialize(m_lastUrl.c_str(), m_lastSecretKey.c_str(), m_lastGameId.c_str(), m_lastGameVersion.c_str());
            }
            else
            {
                // For initialize with apps, we ignore the app id
                client->initializeWithApps(m_lastUrl.c_str(), m_lastGameId.c_str(), m_secretMap, m_lastGameVersion.c_str());
            }

            // Retry the previous authenticate, that is cached inside the authentication service.
            initializeIdentity(true);
            client->getAuthenticationService()->retryPreviousAuthenticate(this);

            return;
        }

        if (m_authenticateCallback != NULL)
        {
            m_authenticateCallback->serverError(serviceName, serviceOperation, statusCode, reasonCode, message);
        }
    }
}
