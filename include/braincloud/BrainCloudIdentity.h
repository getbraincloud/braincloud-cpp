// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include <string>

namespace BrainCloud
{
	class IServerCallback;
	class BrainCloudClient;
	class AuthenticationType;
	struct AuthenticationIds;

	class BrainCloudIdentity
	{
	public:
		BrainCloudIdentity(BrainCloudClient *client);

		/**
		 * Updates univeral id of the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - UpdateUniversalIdLogin
		 *
		 * @param externalId the id that's been connected with
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateUniversalIdLogin(const char *externalId, IServerCallback *callback = NULL);

		/**
		 * Attaches a univeral id to the current profile with no login capability.
		 *
		 * Service Name - identity
		 * Service Operation - AttachNonLoginUniversalId
		 *
		 * @param externalId the id that's been connected with
		 * @param callback The method to be invoked when the server response is received
		 */
		void attachNonLoginUniversalId(const char *externalId, IServerCallback *callback = NULL);

		/**
		 * Attach the user's Facebook credentials to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param facebookId The facebook id of the user
		 * @param authenticationToken The validated token from the Facebook SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the Facebook identity you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and call AuthenticateFacebook().
		 */
		void attachFacebookIdentity(const char *facebookId, const char *authenticationToken, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided Facebook credentials with the
		 * current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param facebookId The facebook id of the user
		 * @param authenticationToken The validated token from the Facebook SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeFacebookIdentity(const char *facebookId, const char *authenticationToken, IServerCallback *callback = NULL);

		/**
		 * Detach the Facebook identity from this profile.
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param facebookId The facebook id of the user
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachFacebookIdentity(const char *facebookId, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach the user's Oculus credentials to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param oculusId The oculus id of the user
		 * @param oculusNonce The validated token from the Oculus SDK
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the Oculus identity you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and call AuthenticateOculus().
		 */
		void attachOculusIdentity(const char *oculusId, const char *oculusNonce, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided Oculus credentials with the
		 * current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param oculusId The oculus id of the user
		 * @param oculusNonce The validated token from the Oculus SDK
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeOculusIdentity(const char *oculusId, const char *oculusNonce, IServerCallback *callback = NULL);

		/**
		 * Detach the Oculus identity from this profile.
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param oculusId The oculus id of the user
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachOculusIdentity(const char *oculusId, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach the user's credentials to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param authenticationType Universal, Email, Facebook, etc
		 * @param ids Auth IDs structure
		 * @param extraJson Additional to piggyback along with the call, to be picked up by pre- or post- hooks. Leave empty string for no extraJson.
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the identity you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and call AuthenticateAdvanced().
		 */
		void attachAdvancedIdentity(AuthenticationType authenticationType, const AuthenticationIds &ids, const std::string &extraJson, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided credentials with the
		 * current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param authenticationType Universal, Email, Facebook, etc
		 * @param ids Auth IDs structure
		 * @param extraJson Additional to piggyback along with the call, to be picked up by pre- or post- hooks. Leave empty string for no extraJson.
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeAdvancedIdentity(AuthenticationType authenticationType, const AuthenticationIds &ids, const std::string &extraJson, IServerCallback *callback = NULL);

		/**
		 * Detach the identity from this profile.
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param authenticationType Universal, Email, Facebook, etc
		 * @param externalId User ID
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param extraJson Additional to piggyback along with the call, to be picked up by pre- or post- hooks. Leave empty string for no extraJson.
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachAdvancedIdentity(AuthenticationType authenticationType, const std::string &externalId, bool continueAnon, const std::string &extraJson, IServerCallback *callback = NULL);

		/**
		 * Attach the user's Apple credentials to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param appleId The appleid of the user
		 * @param authenticationToken The validated token from the Apple SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the Apple identity you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and call AuthenticateApple().
		 */
		void attachAppleIdentity(const char *appleId, const char *authenticationToken, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided Apple credentials with the
		 * current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param appleId The apple id of the user
		 * @param authenticationToken The validated token from the Apple SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeAppleIdentity(const char *appleId, const char *authenticationToken, IServerCallback *callback = NULL);

		/**
		 * Detach the Apple identity from this profile.
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param appleId The apple id of the user
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachAppleIdentity(const char *appleId, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach the user's Ultra credentials to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param ultraUsername it's what the user uses to log into the Ultra endpoint initially
		 * @param ultraIdToken The "id_token" taken from Ultra's JWT.
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the Ultra identity you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and call AuthenticateApple().
		 */
		void attachUltraIdentity(const std::string &ultraUsername, const std::string &ultraIdToken, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided Ultra credentials with the
		 * current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param ultraUsername it's what the user uses to log into the Ultra endpoint initially
		 * @param ultraIdToken The "id_token" taken from Ultra's JWT.
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeUltraIdentity(const std::string &ultraUsername, const std::string &ultraIdToken, IServerCallback *callback = NULL);

		/**
		 * Detach the Ultra identity from this profile.
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param ultraUsername it's what the user uses to log into the Ultra endpoint initially
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachUltraIdentity(const std::string &ultraUsername, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach a Game Center identity to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param gameCenterId The player's game center id  (use the playerID property from the local GKPlayer object)
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the Game Center identity you provided
		 * already points to a different profile.  You will likely want to offer the player the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and call this method again.
		 *
		 */
		void attachGameCenterIdentity(const char *gameCenterId, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the specified Game Center identity with the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param gameCenterId The player's game center id  (use the playerID property from the local GKPlayer object)
		 * @param callback The method to be invoked when the server response is received
		 */
		void mergeGameCenterIdentity(const char *gameCenterId, IServerCallback *callback = NULL);

		/**
		 * Detach the Game Center identity from the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param gameCenterId The player's game center id  (use the playerID property from the local GKPlayer object)
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachGameCenterIdentity(const char *gameCenterId, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach a Email and Password identity to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param email The user's e-mail address
		 * @param password The user's password
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the email address you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and then call AuthenticateEmailPassword().
		 */
		void attachEmailIdentity(const char *email, const char *password, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided e=mail with the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param email The user's e-mail address
		 * @param password The user's password
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeEmailIdentity(const char *email, const char *password, IServerCallback *callback = NULL);

		/**
		 * Detach the e-mail identity from the current profile
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param email The user's e-mail address
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachEmailIdentity(const char *email, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach a Universal (userid + password) identity to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param userId The user's userid
		 * @param password The user's password
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the email address you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and then call AuthenticateEmailPassword().
		 */
		void attachUniversalIdentity(const char *userId, const char *password, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided userId with the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param userId The user's userid
		 * @param password The user's password
		 * @param callback The method to be invoked when the server response is received
		 */
		void mergeUniversalIdentity(const char *userId, const char *password, IServerCallback *callback = NULL);

		/**
		 * Detach the universal identity from the current profile
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param userId The user's userid
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachUniversalIdentity(const char *userId, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach a Steam (userid + steamsessionticket) identity to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param steamId String representation of 64 bit steam id
		 * @param sessionTicket The user's session ticket (hex encoded)
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the email address you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and then call AuthenticateSteam().
		 */
		void attachSteamIdentity(const char *steamId, const char *sessionTicket, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided steam userid with the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param steamId String representation of 64 bit steam id
		 * @param sessionTicket The user's session ticket (hex encoded)
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeSteamIdentity(const char *steamId, const char *sessionTicket, IServerCallback *callback = NULL);

		/**
		 * Detach the steam identity from the current profile
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param steamId String representation of 64 bit steam id
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachSteamIdentity(const char *steamId, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach the user's Google credentials to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param googleId The Google id of the user
		 * @param authenticationToken The validated token from the Google SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the Google identity you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and call AuthenticateGoogle().
		 */
		void attachGoogleIdentity(const char *googleId, const char *authenticationToken, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided Google credentials with the
		 * current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param googleId The Google id of the user
		 * @param authenticationToken The validated token from the Google SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeGoogleIdentity(const char *googleId, const char *authenticationToken, IServerCallback *callback = NULL);

		/**
		 *  Detach the Google identity from this profile.
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param googleId The Google id of the user
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachGoogleIdentity(const char *googleId, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach the user's Google credentials to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param googleId The Google id of the user
		 * @param authenticationToken The validated token from the Google SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the Google identity you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and call AuthenticateGoogle().
		 */
		void attachGoogleOpenIdIdentity(const char *googleId, const char *authenticationToken, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided Google credentials with the
		 * current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param googleId The Google id of the user
		 * @param authenticationToken The validated token from the Google SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeGoogleOpenIdIdentity(const char *googleId, const char *authenticationToken, IServerCallback *callback = NULL);

		/**
		 *  Detach the Google identity from this profile.
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param googleId The Google id of the user
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachGoogleOpenIdIdentity(const char *googleId, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach the user's Twitter credentials to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param twitterId The Twitter id of the user
		 * @param authenticationToken The authentication token derived from the twitter APIs
		 * @param secret The secret given when attempting to link with Twitter
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the Twitter identity you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and call AuthenticateTwitter().
		 */
		void attachTwitterIdentity(const char *twitterId, const char *authenticationToken, const char *secret, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided Twitter credentials with the
		 * current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param twitterId The Twitter id of the user
		 * @param authenticationToken The authentication token derived from the twitter APIs
		 * @param secret The secret given when attempting to link with Twitter
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeTwitterIdentity(const char *twitterId, const char *authenticationToken, const char *secret, IServerCallback *callback = NULL);

		/**
		 * Detach the Twitter identity from this profile.
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param twitterId The Twitter id of the user
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachTwitterIdentity(const char *twitterId, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Attach the user's Parse credentials to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Attach
		 *
		 * @param parseId The Parse id of the user
		 * @param authenticationToken The validated token from Parse
		 *   (that will be further validated when sent to the bC service)
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Errors to watch for:  SWITCHING_PROFILES - this means that the Google identity you provided
		 * already points to a different profile.  You will likely want to offer the user the
		 * choice to *SWITCH* to that profile, or *MERGE* the profiles.
		 *
		 * To switch profiles, call ClearSavedProfileID() and call AuthenticateParse().
		 */
		void attachParseIdentity(const char *parseId, const char *authenticationToken, IServerCallback *callback = NULL);

		/**
		 * Merge the profile associated with the provided Parse credentials with the
		 * current profile.
		 *
		 * Service Name - identity
		 * Service Operation - Merge
		 *
		 * @param parseId The Parse id of the user
		 * @param authenticationToken The validated token from Parse
		 *   (that will be further validated when sent to the bC service)
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void mergeParseIdentity(const char *parseId, const char *authenticationToken, IServerCallback *callback = NULL);

		/**
		 *  Detach the Google identity from this profile.
		 *
		 * Service Name - identity
		 * Service Operation - Detach
		 *
		 * @param parseId The Parse id of the user
		 * @param continueAnon Proceed even if the profile will revert to anonymous?
		 * @param callback The method to be invoked when the server response is received
		 *
		 * Watch for DOWNGRADING_TO_ANONYMOUS_ERROR - occurs if you set continueAnon to false, and
		 * disconnecting this identity would result in the profile being anonymous (which means that
		 * the profile wouldn't be retrievable if the user loses their device)
		 */
		void detachParseIdentity(const char *parseId, bool continueAnon, IServerCallback *callback = NULL);

		/**
		 * Switch to a Child Profile
		 *
		 * Service Name - identity
		 * Service Operation - SWITCH_TO_CHILD_PROFILE
		 *
		 * @param childProfileId The profileId of the child profile to switch to
		 * If null and forceCreate is true a new profile will be created
		 * @param childAppId The appId of the child app to switch to
		 * @param forceCreate Should a new profile be created if it does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void switchToChildProfile(const char *childProfileId, const char *childAppId, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Switches to a child profile of an app when only one profile exists
		 * If multiple profiles exist this returns an error
		 *
		 * Service Name - identity
		 * Service Operation - SWITCH_TO_CHILD_PROFILE
		 *
		 * @param childAppId The App ID of the child app to switch to
		 * @param forceCreate Should a new profile be created if it does not exist?
		 * @param callback The method to be invoked when the server response is received
		 */
		void switchToSingletonChildProfile(const char *childAppId, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Switch to a Parent Profile
		 *
		 * Service Name - identity
		 * Service Operation - SWITCH_TO_PARENT_PROFILE
		 *
		 * @param parentLevelName The level of the parent to switch to
		 * If null and forceCreate is true a new profile will be created
		 * @param callback The method to be invoked when the server response is received
		 */
		void switchToParentProfile(const char *parentLevelName, IServerCallback *callback = NULL);

		/**
		 * Returns a list of all child profiles in child Apps
		 *
		 * Service Name - identity
		 * Service Operation - GET_CHILD_PROFILES
		 *
		 * @param includeSummaryData Whether to return the summary friend data along with this call
		 * @param callback The method to be invoked when the server response is received
		 */
		void getChildProfiles(bool includeSummaryData, IServerCallback *callback = NULL);

		/**
		 * Retrieve list of identities
		 *
		 * Service Name - identity
		 * Service Operation - GET_IDENTITIES
		 *
		 * @param callback The method to be invoked when the server response is received
		 */
		void getIdentities(IServerCallback *callback = NULL);

		/**
		 *  Retrieves identity status for given identity type for this profile.
		 */
		void getIdentityStatus(AuthenticationType authenticationType, const char *externalAuthName, IServerCallback *callback);

		/**
		 * Retrieve list of expired identities
		 *
		 * Service Name - identity
		 * Service Operation - GET_EXPIRED_IDENTITIES
		 *
		 * @param callback The method to be invoked when the server response is received
		 */
		void getExpiredIdentities(IServerCallback *callback = NULL);

		/**
		 * Refreshes an identity for this user
		 *
		 * Service Name - identity
		 * Service Operation - REFRESH_IDENTITY
		 *
		 * @param externalId User ID
		 * @param authenticationToken Password or client side token
		 * @param authenticationType Type of authentication
		 * @param callback The method to be invoked when the server response is received
		 */
		void refreshIdentity(const char *externalId, const char *authenticationToken, AuthenticationType authenticationType, IServerCallback *callback = NULL);

		/**
		 * Allows email identity email address to be changed
		 *
		 * Service Name - identity
		 * Service Operation - CHANGE_EMAIL_IDENTITY
		 *
		 * @param oldEmailAddress Old email address
		 * @param password Password for identity
		 * @param newEmailAddress New email address
		 * @param updateContactEmail Whether to update contact email in profile
		 * @param callback The method to be invoked when the server response is received
		 *
		 */
		void changeEmailIdentity(const char *oldEmailAddress, const char *password, const char *newEmailAddress, bool updateContactEmail, IServerCallback *callback = NULL);

		/**
		 * Attach a new identity to a parent app
		 *
		 * Service Name - identity
		 * Service Operation - ATTACH_PARENT_WITH_IDENTITY
		 *
		 * @param externalId The users id for the new credentials
		 * @param authenticationToken The password/token
		 * @param authenticationType Type of identity
		 * @param externalAuthName Optional - if attaching an external identity
		 * @param forceCreate Should a new profile be created if it does not exist?
		 * @param successCallback The success callback
		 * @param errorCallback The failure callback.
		 * @param cbObject The user object sent to the callback
		 */
		void attachParentWithIdentity(const char *externalId, const char *authenticationToken, AuthenticationType authenticationType,
									  const char *externalAuthName, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Detaches parent from this user's profile
		 *
		 * Service Name - identity
		 * Service Operation - DETACH_PARENT
		 *
		 * @param successCallback The success callback
		 * @param errorCallback The failure callback.
		 * @param cbObject The user object sent to the callback
		 */
		void detachParent(IServerCallback *callback = NULL);

		/**
		 * Attaches a peer identity to this user's profile
		 *
		 * Service Name - identity
		 * Service Operation - ATTACH_PEER_PROFILE
		 *
		 * @param peer Name of the peer to connect to
		 * @param externalId The users id for the new credentials
		 * @param authenticationToken The password/token
		 * @param authenticationType Type of identity
		 * @param externalAuthName Optional - if attaching an external identity
		 * @param forceCreate Should a new profile be created if it does not exist?
		 * @param successCallback The success callback
		 * @param errorCallback The failure callback.
		 * @param cbObject The user object sent to the callback
		 */
		void attachPeerProfile(const char *peer, const char *externalId, const char *authenticationToken, AuthenticationType authenticationType,
							   const char *externalAuthName, bool forceCreate, IServerCallback *callback = NULL);

		/**
		 * Detaches a peer identity from this user's profile
		 *
		 * Service Name - identity
		 * Service Operation - DETACH_PEER
		 *
		 * @param peer Name of the peer to connect to
		 * @param successCallback The success callback
		 * @param errorCallback The failure callback.
		 * @param cbObject The user object sent to the callback
		 */
		void detachPeer(const char *peer, IServerCallback *callback = NULL);

		/**
		 * Returns a list of peer profiles attached to this user
		 *
		 * Service Name - identity
		 * Service Operation - GET_PEER_PROFILES
		 *
		 * @param successCallback The success callback
		 * @param errorCallback The failure callback.
		 * @param cbObject The user object sent to the callback
		 */
		void getPeerProfiles(IServerCallback *callback = NULL);

		/**
		 * Attaches the given block chain public key identity to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - ATTACH_BLOCKCHAIDENTITY
		 *
		 * @param blockchainConfig
		 * @param publicKey
		 * @param successCallback The success callback
		 * @param errorCallback The failure callback.
		 * @param cbObject The user object sent to the callback
		 */
		void attachBlockchainIdentity(const char *blockchainConfig, const char *publicKey, IServerCallback *callback = NULL);

		/**
		 * Detaches the blockchain identity to the current profile.
		 *
		 * Service Name - identity
		 * Service Operation - ATTACH_BLOCKCHAIDENTITY
		 *
		 * @param blockchainConfig
		 * @param successCallback The success callback
		 * @param errorCallback The failure callback.
		 * @param cbObject The user object sent to the callback
		 */
		void detachBlockchainIdentity(const char *blockchainConfig, IServerCallback *callback = NULL);

	private:
		BrainCloudClient *m_client;

		void switchToChildProfile(const char *childProfileId, const char *childAppId, bool forceCreate, bool forceSingleton, IServerCallback *callback = NULL);

		void attachIdentity(const char *externalId, const char *authenticationToken, AuthenticationType authenticationType, IServerCallback *callback = NULL);
		void mergeIdentity(const char *externalId, const char *authenticationToken, AuthenticationType authenticationType, IServerCallback *callback = NULL);
		void detachIdentity(const char *externalId, AuthenticationType authenticationType, bool continueAnon, IServerCallback *callback = NULL);
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
