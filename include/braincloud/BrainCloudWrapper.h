// Copyright 2026 bitHeads, Inc. All Rights Reserved.
#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include <map>
#include <string>
#include <list>
#include <algorithm>

#include "braincloud/BrainCloudClient.h"

#include "json/json.h"

namespace BrainCloud {

    /**
     * The BrainCloudWrapper provides some convenience functionality to developers when they are
     * getting started with the authentication system.
     *
     * By using the wrapper authentication methods, the anonymous and profile ids will be automatically
     * persisted upon successful authentication. When authenticating, any stored anonymous/profile ids will
     * be sent to the server. This strategy is useful when using anonymous authentication.
     */
    class BrainCloudWrapper : public BrainCloud::IServerCallback
    {
    public:

        BrainCloud::BrainCloudClient* client;

        /* Service getter methods */
        BrainCloudPlayerStatistics * getPlayerStatisticsService() { return client->getPlayerStatisticsService(); }
        BrainCloudGlobalStatistics * getGlobalStatisticsService() { return client->getGlobalStatisticsService(); }
        BrainCloudIdentity * getIdentityService() { return client->getIdentityService(); }
        BrainCloudVirtualCurrency * getVirtualCurrencyService() { return client->getVirtualCurrencyService(); }
        BrainCloudAppStore * getAppStoreService() { return client->getAppStoreService(); }
        BrainCloudEntity * getEntityService() { return client->getEntityService(); }
        BrainCloudGlobalEntity * getGlobalEntityService() { return client->getGlobalEntityService(); }
        BrainCloudMatchmaking * getMatchmakingService() { return client->getMatchmakingService(); }
        BrainCloudOneWayMatch * getOneWayMatchService() { return client->getOneWayMatchService(); }
        BrainCloudPlaybackStream * getPlaybackStreamService() { return client->getPlaybackStreamService(); }
        BrainCloudAsyncMatch * getAsyncMatchService() { return client->getAsyncMatchService(); }
        BrainCloudScript * getScriptService() { return client->getScriptService(); }
        BrainCloudPlayerState * getPlayerStateService() { return client->getPlayerStateService(); }
        BrainCloudFriend * getFriendService() { return client->getFriendService(); }
        BrainCloudEvent * getEventService() { return client->getEventService(); }
        BrainCloudSocialLeaderboard * getSocialLeaderboardService() { return client->getSocialLeaderboardService(); }
        BrainCloudSocialLeaderboard * getLeaderboardService() { return client->getLeaderboardService(); }
        BrainCloudGamification * getGamificationService() { return client->getGamificationService(); }
        BrainCloudTime * getTimeService() { return client->getTimeService(); }
        BrainCloudTournament * getTournamentService() { return client->getTournamentService(); }
        BrainCloudCustomEntity * getCustomEntityService() { return client->getCustomEntityService(); }
        BrainCloudItemCatalog* getItemCatalogService() { return client->getItemCatalogService(); }
        BrainCloudUserItems* getUserItemsService() { return client->getUserItemsService(); }
        BrainCloudPushNotification * getPushNotificationService() { return client->getPushNotificationService(); }
        BrainCloudPlayerStatisticsEvent * getPlayerStatisticsEventService() { return client->getPlayerStatisticsEventService(); }
        BrainCloudGlobalApp * getGlobalAppService() { return client->getGlobalAppService(); }
        BrainCloudS3Handling * getS3HandlingService() { return client->getS3HandlingService(); }
        BrainCloudRedemptionCode * getRedemptionCodeService() { return client->getRedemptionCodeService(); }
        BrainCloudDataStream * getDataStreamService() { return client->getDataStreamService(); }
        BrainCloudProfanity * getProfanityService() { return client->getProfanityService(); }
        BrainCloudFile * getFileService() { return client->getFileService(); }
        BrainCloudGlobalFile* getGlobalFileService() { return client->getGlobalFileService(); }
        BrainCloudGroup * getGroupService() { return client->getGroupService(); }
        BrainCloudMail * getMailService() { return client->getMailService(); }
        BrainCloudChat * getChatService() { return client->getChatService(); }
        BrainCloudMessaging * getMessagingService() { return client->getMessagingService(); }
        BrainCloudLobby * getLobbyService() { return client->getLobbyService(); }
        BrainCloudPresence * getPresenceService() { return client->getPresenceService(); }
		BrainCloudRelay * getRelayService() { return client->getRelayService(); }
		BrainCloudRTT * getRTTService() { return client->getRTTService(); }
        BrainCloudBlockchain * getBlockchainService() { return client->getBlockchainService(); }
        BrainCloudCampaign * getCampaignService() { return client->getCampaignService(); }

        /**
         * Method returns a singleton instance of the BrainCloudWrapper.
         * @return A singleton instance of the BrainCloudWrapper.
         *
		 * @deprecated Use of the *singleton* has been deprecated. We recommend that you create your own *variable* to hold an instance of the brainCloudWrapper. Explanation here: http://getbraincloud.com/apidocs/wrappers-clients-and-inconvenient-singletons/
		 */
        DEPRECATED static BrainCloudWrapper* getInstance();

        /**
         * Instantiate a copy of the brainCloud wrapper. Don't use getInstance if creating your own copy.
         *
         * @param wrapperName
         */
        BrainCloudWrapper(const char* wrapperName = "");

        ~BrainCloudWrapper();

        /**
         * Method initializes the BrainCloudClient.
         *
         * @param serverURL The url to the brainCloud server
         * @param secretKey The secret key for your app
         * @param appId The app id
         * @param version The app version
         * @param companyName The company name used in the keychain for storing anonymous and profile ids.
         *                    You are free to pick anything you want.
         * @param appName The app name used in the keychain for storing anonymous and profile ids.
         *                You are free to pick anything you want.
         */
        void initialize(const char* serverUrl, const char* secretKey, const char* appId, const char* version, const char* companyName, const char* appName);

        /**
         * Method initializes the BrainCloudClient with multiple app/secret.
         * Used when needed to switch between child and parent apps.
         *
         * @param serverURL The url to the brainCloud server
         *                  Currently this should be:  https://api.braincloudservers.com/dispatcherv2
         * @param defaultAppId The default app id that we start with
         * @param secretMap A map of <appId, secretKey>
         * @param version The app version
         * @param companyName The company name used in the keychain for storing anonymous and profile ids.
         *                    You are free to pick anything you want.
         * @param appName The app name used in the keychain for storing anonymous and profile ids.
         *                You are free to pick anything you want.
         */
        void initializeWithApps(const char* serverURL, const char* defaultAppId, const std::map<std::string, std::string>& secretMap, const char* version, const char* companyName, const char* appName);

        /**
         * Authenticate a user anonymously with brainCloud - used for apps that don't want to bother
         * the user to login, or for users who are sensitive to their privacy
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param callback The method to be invoked when the server response is received.
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void authenticateAnonymous(IServerCallback* callback = NULL, bool forceCreate = true);

        /**
         * Authenticate the user with a custom Email and Password.  Note that the client app
         * is responsible for collecting (and storing) the e-mail and potentially password
         * (for convenience) in the client data.  For the greatest security,
         * force the user to re-enter their * password at each login.
         * (Or at least give them that option).
         *
         * Note that the password sent from the client to the server is protected via SSL.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param email The e-mail address of the user
         * @param password The password of the user
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         */
        void authenticateEmailPassword(const char* email, const char* password, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Authenticate the user via cloud code (which in turn validates the supplied credentials against an external system).
         * This allows the developer to extend brainCloud authentication to support other backend authentication systems.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param userid The user id
         * @param token The user token (password etc)
         * @param externalAuthName The name of the cloud script to call for external authentication
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         */
        void authenticateExternal(const char* userid, const char* token, const char* externalAuthName, bool forceCreate, IServerCallback* callback = NULL);
        		
        /**
		 * Authenticate the user using a handoffId and authentication token
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param handoffId braincloud handoff id generated from cloud script
		 * @param securityToken The authentication token
		 * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         * 
		 */
		void authenticateHandoff(const char* handoffId, const char* securityToken, IServerCallback* callback = NULL);

		/**
		 * Authenticate the user using a handoffCode 
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param handoffCode the code we generate in cloudcode
		 * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         * 
		 */
		void authenticateSettopHandoff(const char * handoffCode, IServerCallback * callback = NULL);

        /**
         * Authenticate the user with brainCloud using their Facebook Credentials
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param fbUserId The facebook id of the user
         * @param fbAuthToken The validated token from the Facebook SDK
         *                    (that will be further validated when sent to the bC service)
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void authenticateFacebook(const char* fbUserId, const char* fbAuthToken, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Authenticate the user with brainCloud using their FacebookLimited Credentials
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param fbLimitedUserId The facebookLimited id of the user
         * @param fbAuthToken The validated token from the Facebook SDK
         *                    (that will be further validated when sent to the bC service)
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void authenticateFacebookLimited(const char* fbLimitedUserId, const char* fbAuthToken, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Authenticate the user with brainCloud using their Oculus Credentials
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param oculusUserId The oculus id of the user
         * @param oculusNonce oculus token from the Oculus SDK
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void authenticateOculus(const char* oculusUserId, const char* oculusNonce, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Authenticate the user using their Game Center Id (legacy support only, not recommended).
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param gameCenterId The user's Game Center Id which can be the playerId, gamePlayerId, or teamPlayerId from the localPlayer object.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received.
         *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         * @deprecated This version of the method requires a compatibility flag on brainCloud to be enabled and is only included for legacy support.
         *             Use the overloaded method which includes proper Game Center account verification.
         *             We will be removing this method in a future client release of our brainCloud libraries.
         *
         */
        DEPRECATED void authenticateGameCenter(const char* gameCenterId, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Authenticate the user using their Game Center Id and identity verification signature.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param gameCenterId The user's Game Center Id which can be the playerId, gamePlayerId, or teamPlayerId from the localPlayer object.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param timestamp The timestamp value returned as part of the identity verification signature fetch from Game Center.
		 * 		            Required for modern Game Center verification.
		 * @param publicKeyUrl The publicKeyUrl value returned as part of the identity verification signature fetch from Game Center.
		 *                     Required for modern Game Center verification.
		 * @param signature The raw signature bytes returned as part of the identity verification signature fetch from Game Center.
		 *                  Required for modern Game Center verification.
		 * @param signatureLength The length of the returned identity verification signature.
		 *                        Required for modern Game Center verification.
		 * @param salt The raw salt bytes returned as part of the identity verification signature fetch from Game Center.
		 *             Required for modern Game Center verification.
		 * @param saltLength The length of the returned identity verification salt.
		 *                   Required for modern Game Center verification.
		 * @param teamPlayerId Optional for Game Center verification; only required when gameCenterId is set to a value other than teamPlayerId (e.g. playerId),
		 *                     so that brainCloud can still associate the user with their team-scoped identity.
         * @param callback The method to be invoked when the server response is received.
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void authenticateGameCenter(const char* gameCenterId, bool forceCreate, uint64_t timestamp, const std::string& publicKeyUrl, const uint8_t* signature, size_t signatureLength, const uint8_t* salt, size_t saltLength, const std::string& teamPlayerId = "", IServerCallback* callback = NULL);

		/**
		 * Authenticate the user using a google userid(email address) and google authentication token.
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param appleUserId String of the apple accounts user Id OR email
		 * @param identityToken The authentication token confirming users identity
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         * 
		 */
		void authenticateApple(const char * appleUserId, const char * identityToken, bool forceCreate, IServerCallback * callback = NULL);

        /**
         * Authenticate the user using a google userid(email address) and google authentication token.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param googleUserId String representation of google+ userid (email)
         * @param serverAuthCode The authentication token derived via the google apis.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void authenticateGoogle(const char* googleUserId, const char* serverAuthCode, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Authenticate the user using a google openId
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param googleUserAccountEmail String representation of google+ userid (email)
         * @param IdToken The authentication token derived via the google apis.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void authenticateGoogleOpenId(const char* googleUserAccountEmail, const char* IdToken, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Authenticate the user using a steam userid and session ticket (without any validation on the userid).
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param userid String representation of 64 bit steam id
         * @param sessionticket The session ticket of the user (hex encoded)
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void authenticateSteam(const char* userid, const char* sessionticket, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Authenticate the user using a Twitter userid, authentication token, and secret from Twitter.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param userid String representation of Twitter userid
         * @param token The authentication token derived via the Twitter apis.
         * @param secret The secret given when attempting to link with Twitter
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void authenticateTwitter(const char* userid, const char* token, const char* secret, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Authenticate the user using a userid and password (without any validation on the userid).
         * Similar to AuthenticateEmailPassword - except that that method has additional features to
         * allow for e-mail validation, password resets, etc.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param email The e-mail address of the user
         * @param password The password of the user
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void authenticateUniversal(const char* userid, const char* password, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Authenticate the user for Ultra.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param ultraUsername It's what the user uses to log into the Ultra endpoint initially
         * @param ultraIdToken The "id_token" taken from Ultra's JWT.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         */
        void authenticateUltra(const std::string &ultraUsername, const std::string &ultraIdToken, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * A generic Authenticate method that translates to the same as calling a specific one, except it takes an extraJson
         * that will be passed along to pre- or post- hooks.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param authenticationType Universal, Email, Facebook, etc
         * @param ids Auth IDs structure
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param extraJson Additional to piggyback along with the call, to be picked up by pre- or post- hooks. Leave empty string for no extraJson.
         * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         */
        void authenticateAdvanced(AuthenticationType authenticationType, const AuthenticationIds &ids, bool forceCreate, const std::string &extraJson, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user with a custom Email and Password.  Note that the client app
         * is responsible for collecting (and storing) the e-mail and potentially password
         * (for convenience) in the client data.  For the greatest security,
         * force the user to re-enter their * password at each login.
         * (Or at least give them that option).
         *
         * Note that the password sent from the client to the server is protected via SSL.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param email The e-mail address of the user
         * @param password The password of the user
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void smartSwitchAuthenticateEmailPassword(const char* email, const char* password, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user via cloud code (which in turn validates the supplied credentials against an external system).
         * This allows the developer to extend brainCloud authentication to support other backend authentication systems.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param userid The user id
         * @param token The user token (password etc)
         * @param externalAuthName The name of the cloud script to call for external authentication
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         */
        void smartSwitchAuthenticateExternal(const char* userid, const char* token, const char* externalAuthName, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user with brainCloud using their Facebook Credentials
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param fbUserId The facebook id of the user
         * @param fbAuthToken The validated token from the Facebook SDK
         *                    (that will be further validated when sent to the bC service)
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         */
        void smartSwitchAuthenticateFacebook(const char* fbUserId, const char* fbAuthToken, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user with brainCloud using their FacebookLimited Credentials
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param fbLimitedUserId The facebookLimited id of the user
         * @param fbAuthToken The validated token from the Facebook SDK
         *                    (that will be further validated when sent to the bC service)
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void smartSwitchAuthenticateFacebookLimited(const char* fbLimitedUserId, const char* fbAuthToken, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user with brainCloud using their Oculus Credentials
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param oculusUserId The Oculus ID of the user
         * @param oculusNonce Oculus nonce from the Oculus SDK
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         */
        void smartSwitchAuthenticateOculus(const char* oculusUserId, const char* oculusNonce, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user using their Game Center Id (legacy support only, not recommended).
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param gameCenterId The user's Game Center Id which can be the playerId, gamePlayerId, or teamPlayerId from the localPlayer object.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received.
         *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         * @deprecated This version of the method requires a compatibility flag on brainCloud to be enabled and is only included for legacy support.
         *             Use the overloaded method which includes proper Game Center account verification.
         *             We will be removing this method in a future client release of our brainCloud libraries.
         *
         */
        DEPRECATED void smartSwitchAuthenticateGameCenter(const char* gameCenterId, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user using their Game Center Id and identity verification signature.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param gameCenterId The user's Game Center Id which can be the playerId, gamePlayerId, or teamPlayerId from the localPlayer object.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param timestamp The timestamp value returned as part of the identity verification signature fetch from Game Center.
		 * 		            Required for modern Game Center verification.
		 * @param publicKeyUrl The publicKeyUrl value returned as part of the identity verification signature fetch from Game Center.
		 *                     Required for modern Game Center verification.
		 * @param signature The raw signature bytes returned as part of the identity verification signature fetch from Game Center.
		 *                  Required for modern Game Center verification.
		 * @param signatureLength The length of the returned identity verification signature.
		 *                        Required for modern Game Center verification.
		 * @param salt The raw salt bytes returned as part of the identity verification signature fetch from Game Center.
		 *             Required for modern Game Center verification.
		 * @param saltLength The length of the returned identity verification salt.
		 *                   Required for modern Game Center verification.
		 * @param teamPlayerId Optional for Game Center verification; only required when gameCenterId is set to a value other than teamPlayerId (e.g. playerId),
		 *                     so that brainCloud can still associate the user with their team-scoped identity.
         * @param callback The method to be invoked when the server response is received.
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void smartSwitchAuthenticateGameCenter(const char* gameCenterId, bool forceCreate, uint64_t timestamp, const std::string& publicKeyUrl, const uint8_t* signature, size_t signatureLength, const uint8_t* salt, size_t saltLength, const std::string& teamPlayerId = "", IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user using a google userid(email address) and google authentication token.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param userid String representation of google+ userid (email)
         * @param token The authentication token derived via the google apis.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void smartSwitchAuthenticateGoogle(const char* userid, const char* token, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user using a google userid(email address) and google authentication token.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param userid String representation of google+ userid (email)
         * @param token The authentication token derived via the google apis.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void smartSwitchAuthenticateGoogleOpenId(const char* userid, const char* token, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user using a google userid(email address) and google authentication token.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param userid String representation of apple+ userid (email)
         * @param token The authentication token derived via the apple apis.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void smartSwitchAuthenticateApple(const char* userid, const char* token, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user using a steam userid and session ticket (without any validation on the userid).
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param userid String representation of 64 bit steam id
         * @param sessionticket The session ticket of the user (hex encoded)
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void smartSwitchAuthenticateSteam(const char* userid, const char* sessionticket, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user using a Twitter userid, authentication token, and secret from Twitter.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param userid String representation of Twitter userid
         * @param token The authentication token derived via the Twitter apis.
         * @param secret The secret given when attempting to link with Twitter
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         *
         */
        void smartSwitchAuthenticateTwitter(const char* userid, const char* token, const char* secret, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * Authenticate the user using a userid and password (without any validation on the userid).
         * Similar to AuthenticateEmailPassword - except that that method has additional features to
         * allow for e-mail validation, password resets, etc.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param email The e-mail address of the user
         * @param password The password of the user
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         */
        void smartSwitchAuthenticateUniversal(const char* userid, const char* password, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         * 
         * Authenticate the user for Ultra.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param ultraUsername It's what the user uses to log into the Ultra endpoint initially
         * @param ultraIdToken The "id_token" taken from Ultra's JWT.
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         */
        void smartSwitchAuthenticateUltra(const std::string &ultraUsername, const std::string &ultraIdToken, bool forceCreate, IServerCallback* callback = NULL);

        /**
         * Smart Switch Authenticate will logout of the current profile, and switch to the new authentication type.
	     * In event the current session was previously an anonymous account, the smart switch will delete that profile.
	     * Use this function to keep a clean designflow from anonymous to signed profiles
         *
         * A generic Authenticate method that translates to the same as calling a specific one, except it takes an extraJson
         * that will be passed along to pre- or post- hooks.
         *
         * Service Name - authenticationV2
         * Service Operation - AUTHENTICATE
         *
         * @param authenticationType Universal, Email, Facebook, etc
         * @param ids Auth IDs structure
         * @param forceCreate Should a new profile be created for this user if the account does not exist?
         * @param extraJson Additional to piggyback along with the call, to be picked up by pre- or post- hooks. Leave empty string for no extraJson.
         * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         */
        void smartSwitchAuthenticateAdvanced(AuthenticationType authenticationType, const AuthenticationIds &ids, bool forceCreate, const std::string &extraJson, IServerCallback* callback = NULL);

	    /**
		 * Reset Email password - Sends a password reset email to the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD
		 *
		 * @param externalId The email address to send the reset email to.
		 * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
		 *
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
		void resetEmailPassword(const char* externalId, IServerCallback* callback = NULL);

		/**
		 * Reset Email password with service parameters - Sends a password reset email to 
		 * the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD_ADVANCED
		 *
		 * @param appId the applicationId
		 * @param emailAddress The email address to send the reset email to.
		 * @param serviceParams Parameters to send to the email service. See documentation for
		 *                      full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
		void resetEmailPasswordAdvanced(const char* emailAddress, std::string serviceParams, IServerCallback* callback = NULL);

        /**
		 * Reset Email password - Sends a password reset email to the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD
		 *
		 * @param externalId The email address to send the reset email to.
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
		void resetEmailPasswordWithExpiry(const char* externalId, int tokenTtlInMinutes , IServerCallback* callback = NULL);

		/**
		 * Reset Email password with service parameters - Sends a password reset email to 
		 * the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD_ADVANCED
		 *
		 * @param appId the applicationId
		 * @param emailAddress The email address to send the reset email to.
		 * @param serviceParams Parameters to send to the email service. See documentation for
		 *                      full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param callback The method to be invoked when the server response is received
		 *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
		 * Note The follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
         * 
		 */
		void resetEmailPasswordAdvancedWithExpiry(const char* emailAddress, std::string serviceParams, int tokenTtlInMinutes , IServerCallback* callback = NULL);
        
	    /**
		 * Reset Email password - Sends a password reset email to the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD
		 *
		 * @param externalId The email address to send the reset email to.
		 * @param callback The method to be invoked when the server response is received
		 *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
		void resetUniversalIdPassword(const char* externalId, IServerCallback* callback = NULL);

		/**
		 * Reset Email password with service parameters - Sends a password reset email to 
		 * the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD_ADVANCED
		 *
		 * @param appId the applicationId
		 * @param emailAddress The email address to send the reset email to.
		 * @param serviceParams Parameters to send to the email service. See documentation for
		 *                      full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param callback The method to be invoked when the server response is received
		 *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
		void resetUniversalIdPasswordAdvanced(const char* emailAddress, std::string serviceParams, IServerCallback* callback = NULL);

        /**
		 * Reset Email password - Sends a password reset email to the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD
		 *
		 * @param externalId The email address to send the reset email to.
		 * @param callback The method to be invoked when the server response is received
		 *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
		void resetUniversalIdPasswordWithExpiry(const char* externalId, int tokenTtlInMinutes , IServerCallback* callback = NULL);

		/**
		 * Reset Email password with service parameters - Sends a password reset email to 
		 * the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD_ADVANCED
		 *
		 * @param emailAddress The email address to send the reset email to.
		 * @param serviceParams Parameters to send to the email service. See documentation for
		 *                      full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param callback The method to be invoked when the server response is received
		 *
         * @returns Performs the success callback on success, failure callback on failure.
         * 
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
		void resetUniversalIdPasswordAdvancedWithExpiry(const char* emailAddress, std::string serviceParams, int tokenTtlInMinutes , IServerCallback* callback = NULL);

        /**
         * Returns true IF both Profile ID and Anonymous ID are stored - meaning reconnect possible
         * 
         * @return true if reconnect possible
         * 
         */
        bool canReconnect();

        /**
		 * Re-authenticates the user with brainCloud
		 *
		 * @param callback The method to be invoked when the server response is received
         * 
         * @returns Performs the success callback on success, failure callback on failure.
		 *
		 */
		void reconnect(IServerCallback* callback = NULL);

        /**
         * Run callbacks, to be called once per frame from your main thread
         */
        void runCallbacks();

        /**
         * Returns a singleton instance of the BrainCloudClient.
         * @return A singleton instance of the BrainCloudClient.
         *
		 * @deprecated Use of the *singleton* has been deprecated. We recommend that you create your own *variable* to hold an instance of the brainCloudWrapper. Explanation here: http://getbraincloud.com/apidocs/wrappers-clients-and-inconvenient-singletons/
		 */
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#else
#pragma warning( push )
#pragma warning (disable : 4996)
#endif
        DEPRECATED static BrainCloud::BrainCloudClient* getBC() { return getInstance()->getBCClient(); }
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#else
#pragma warning( pop )
#endif

        /**
         * @return A singleton instance of the BrainCloudClient.
         * 
         */
        BrainCloud::BrainCloudClient* getBCClient() {
            return client;
        }

        /**
         * Clears Profile Id and Anonymous Id and deletes data entry on device
         * 
         * @note Use Logout
         * 
         * If this is called when AnonymousAuthentication is used, the portal user cannot be reconnected or recovered!
         * 
         */
        void clearIds();
        
        /**
         * @return The stored profile id
         * 
         */
        std::string getStoredProfileId();

        /**
         * Sets the stored profile id
         * 
         * @param profileId The profile id to set
         * 
         */
        void setStoredProfileId(const char* profileId);

        /**
         * Resets the profile id to empty string
         */
        void resetStoredProfileId();

        /**
         * @return The stored anonymous id
         * 
         */
        std::string getStoredAnonymousId();

        /**
         * Sets the stored anonymous id
         * 
         * @param anonymousId The anonymous id to set
         * 
         */
        void setStoredAnonymousId(const char* anonymousId);

        /**
         * Resets the anonymous id to empty string
         */
        void resetStoredAnonymousId();

        /**
         * For non-anonymous authentication methods, a profile id will be passed in
         * when this value is set to false. This will generate an error on the server
         * if the profile id passed in does not match the profile associated with the
         * authentication credentials. By default, this value is true.
         *
         * @param alwaysAllow Controls whether the profile id is passed in with
         *                    non-anonymous authentications.
         */
        void setAlwaysAllowProfileSwitch(bool alwaysAllow);

        /**
         * @return Whether to always allow profile switches
         * 
         */
        bool getAlwaysAllowProfileSwitch();

        /**
         * Logs user out of playerState and optionally clears the profile id (eg. shared computer)
         * 
         * @param forgetUser true if user profile should be deleted from device on logout, false to allow reconnect
         * @param callback
         *  
         * @returns Performs the success callback on success, failure callback on failure.
         * 
         * @note If forgetUser is true for an AuthenticateAnonymous THEN the user data will be in-accessible and non-recoverable
         * 
         */
        void logout(bool forgetUser, IServerCallback* callback);

        virtual void serverCallback(BrainCloud::ServiceName serviceName, BrainCloud::ServiceOperation serviceOperation, std::string const & jsonData);
        virtual void serverError(BrainCloud::ServiceName serviceName,
                                 BrainCloud::ServiceOperation serviceOperation,
                                 int statusCode, int reasonCode, const std::string & message);

    protected:
        static BrainCloudWrapper* m_instance;
        static std::string AUTHENTICATION_ANONYMOUS;

        IServerCallback* m_authenticateCallback;

        std::string m_lastUrl;
        std::string m_lastSecretKey;
        std::string m_lastGameId;
        std::string m_lastGameVersion;
        std::string m_wrapperName;
        std::map<std::string, std::string> m_secretMap;

        bool m_alwaysAllowProfileSwitch;

        void initializeIdentity(bool isAnonymousAuth = false);
        void reauthenticate();

		void getIdentitiesCallback(IServerCallback* success);

        // these methods are not really used
        std::string getStoredAuthenticationType();
        void setStoredAuthenticationType(const char* authenticationType);
        void resetStoredAuthenticationType();
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
