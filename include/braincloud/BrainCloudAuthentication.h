// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include <string>
#include <map>
#include "json/json.h"
#include "braincloud/BrainCloudTypes.h"
#include "braincloud/AuthenticationType.h"

namespace BrainCloud
{
	class BrainCloudClient;
	class IServerCallback;
	struct AuthenticationIds;

	class BrainCloudAuthentication
	{
	public:
		static const char *AUTH_FACEBOOK;

		BrainCloudAuthentication(BrainCloudClient *client);

		// internal use only
		void setClientLib(const char *lib);

		/**
		 * Initialize - initializes the identity service with a saved
		 * anonymous installation id and most recently used profile id
		 *
		 * @param anonymousId  The anonymous installation id that was generated for this device
		 * @param profileId The id of the profile id that was most recently used by the app (on this device)
		 */
		void initialize(const char *profileId, const char *anonymousId);

		/**
		 * Used to create the anonymous installation id for the brainCloud profile.
		 * @returns A unique Anonymous ID
		 */
		std::string generateAnonymousId();

		/**
		 * Used to clear the saved profile id - to use in cases when the user is
		 * attempting to switch to a different game profile.
		 */
		void clearSavedProfileId();

		/**
		 * Authenticate a user anonymously with brainCloud - used for apps that don't want to bother
		 * the user to login, or for users who are sensitive to their privacy
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param forceCreate  Should a new profile be created if it does not exist?
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void authenticateAnonymous(bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user with brainCloud using their Facebook Credentials
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param fbUserId The facebook id of the user
		 * @param fbAuthToken The validated token from the Facebook SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void authenticateFacebook(const char *fbUserId, const char *fbAuthToken, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user with brainCloud using their Oculus Credentials
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param oculusUserId The oculus id of the user
		 * @param oculusNonce Oculus token from the Oculus SDK
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void authenticateOculus(const char *oculusUserId, const char *oculusNonce, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user using their Game Center id
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param gameCenterId The player's game center id  (use the playerID property from the local GKPlayer object)
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateGameCenter(const char *gameCenterId, bool forceCreate, IServerCallback *callback = NULL);

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
		 * @param email  The e-mail address of the user
		 * @param password  The password of the user
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void authenticateEmailPassword(const char *email, const char *password, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user using a userid and password (without any validation on the userid).
		 * Similar to AuthenticateEmailPassword - except that that method has additional features to
		 * allow for e-mail validation, password resets, etc.
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param email  The e-mail address of the user
		 * @param password  The password of the user
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateUniversal(const char *userId, const char *password, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Get server version.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getServerVersion(IServerCallback *callback = NULL);

		/**
		 * Authenticate the user using a steam userid and session ticket (without any validation on the userid).
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param userId  String representation of 64 bit steam id
		 * @param sessionticket  The session ticket of the user (hex encoded)
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateSteam(const char *userId, const char *sessionticket, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user using a google userid(email address) and google authentication token.
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param appleUserId  String of the apple accounts user Id OR email
		 * @param identityToken  The authentication token confirming users identity
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateApple(const char *appleUserId, const char *identityToken, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user using a google userid(email address) and google authentication token.
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param googleUserId  String representation of google+ userid (email)
		 * @param serverAuthCode  The authentication token derived via the google apis.
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateGoogle(const char *googleUserId, const char *serverAuthCode, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user using a google userid(email address) and google authentication token.
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param googleUserAccountEmail String representation of google+ userid (email)
		 * @param IdToken  The authentication token derived via the google apis.
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateGoogleOpenId(const char *googleUserAccountEmail, const char *IdToken, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user using a Twitter userid, authentication token, and secret from Twitter.
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param userId  String representation of Twitter userid
		 * @param token  The authentication token derived via the Twitter apis.
		 * @param secret  The secret given when attempting to link with Twitter
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateTwitter(const char *userId, const char *token, const char *secret, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user using a Parse userid and authentication token
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param userId String representation of Parse userid
		 * @param token The authentication token
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateParse(const char *userId, const char *token, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user using a handoffId and authentication token
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param handoffId braincloud handoff id generated from cloud script
		 * @param securityToken The authentication token
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateHandoff(const char *handoffId, const char *securityToken, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user using a handoffCode
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param handoffCode the code we generate in cloudcode
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateSettopHandoff(const char *handoffCode, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user via cloud code (which in turn validates the supplied credentials against an external system).
		 * This allows the developer to extend brainCloud authentication to support other backend authentication systems.
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param userId The user id
		 * @param token The user token (password etc)
		 * @param externalAuthName The name of the cloud script to call for external authentication
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateExternal(const char *userId, const char *token, const char *externalAuthName, bool forceCreate, IServerCallback *callback = NULL);

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
		 */
		void authenticateAdvanced(AuthenticationType authenticationType, const AuthenticationIds &ids, bool forceCreate, const std::string &extraJson, IServerCallback *callback = NULL);

		/**
		 * Authenticate the user for Ultra.
		 *
		 * Service Name - authenticationV2
		 * Service Operation - AUTHENTICATE
		 *
		 * @param ultraUsername it's what the user uses to log into the Ultra endpoint initially
		 * @param ultraIdToken The "id_token" taken from Ultra's JWT.
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void authenticateUltra(const std::string &ultraUsername, const std::string &ultraIdToken, bool forceCreate, IServerCallback *callback = NULL);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		void resetEmailPassword(const char *externalId, IServerCallback *callback = NULL);

		/**
		 * Reset Email password with service parameters - Sends a password reset email to
		 * the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD_ADVANCED
		 *
		 * @param appId the applicationId
		 * @param emailAddress The email address to send the reset email to.
		 * @param serviceParams - parameters to send to the email service. See documentation for
		 * full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
		void resetEmailPasswordAdvanced(const char *emailAddress, std::string serviceParams, IServerCallback *callback = NULL);

		/**
		 * Reset Email password - Sends a password reset email to the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD_WITH_EXPIRY
		 *
		 * @param externalId The email address to send the reset email to.
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
		void resetEmailPasswordWithExpiry(const char *externalId, int tokenTtlInMinutes, IServerCallback *callback = NULL);

		/**
		 * Reset Email password with service parameters - Sends a password reset email to
		 * the specified address
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_EMAIL_PASSWORD_ADVANCED
		 *
		 * @param appId the applicationId
		 * @param emailAddress The email address to send the reset email to.
		 * @param serviceParams - parameters to send to the email service. See documentation for
		 * full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
		void resetEmailPasswordAdvancedWithExpiry(const char *emailAddress, std::string serviceParams, int tokenTtlInMinutes, IServerCallback *callback = NULL);

		/**
		 * Resets Universal ID password
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_UNIVERSAL_ID_PASSWORD
		 *
		 * @param appId the applicationId
		 * @param universalId the universal Id in question
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void resetUniversalIdPassword(const char *universalId, IServerCallback *callback = NULL);

		/**
		 * Advanced Universal ID password reset using templates
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_UNIVERSAL_ID_PASSWORD_ADVANCED
		 *
		 * @param appId the applicationId
		 * @param universalId the universal Id in question
		 * @param serviceParams - parameters to send to the email service.
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void resetUniversalIdPasswordAdvanced(const char *universalId, std::string serviceParams, IServerCallback *callback = NULL);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/**
		 * Resets Universal ID password
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_UNIVERSAL_ID_PASSWORD_WITH_EXPIRY
		 *
		 * @param appId the applicationId
		 * @param universalId the universal Id in question
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void resetUniversalIdPasswordWithExpiry(const char *universalId, int tokenTtlInMinutes, IServerCallback *callback = NULL);

		/**
		 *  Advanced Universal ID password reset using templates
		 *
		 * Service Name - authenticationV2
		 * Service Operation - RESET_UNIVERSAL_ID_PASSWORD_ADVANCED_WITH_EXPIRY
		 *
		 * @param appId the applicationId
		 * @param universalId the universal Id in question
		 * @param serviceParams - parameters to send to the email service.
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void resetUniversalIdPasswordAdvancedWithExpiry(const char *universalId, std::string serviceParams, int tokenTtlInMinutes, IServerCallback *callback = NULL);

		const std::string &getAnonymousId() const { return _anonymousId; }
		const std::string &getProfileId() const { return _profileId; }

		bool compressResponses = true;

		void setAnonymousId(const char *anonymousId) { _anonymousId = anonymousId; }
		void setProfileId(const char *profileId) { _profileId = profileId; }

		void retryPreviousAuthenticate(IServerCallback *callback);

	protected:
		BrainCloudClient *m_client;
		std::string _anonymousId;
		std::string _profileId;
		std::string _clientLib;

		void authenticate(const char *externalId, const char *authenticationToken, AuthenticationType authenticationType, const char *externalAuthName, bool forceCreate, const std::string &extraJson, IServerCallback *callback);

	private:
		struct PreviousAuthParams
		{
			std::string externalId;
			std::string authenticationToken;
			AuthenticationType authenticationType = AuthenticationType::Unknown;
			std::string externalAuthName;
			bool forceCreate = true;
			std::string extraJson;
		} m_previousAuthParams;
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
