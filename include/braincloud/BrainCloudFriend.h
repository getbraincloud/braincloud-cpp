// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>
#include <vector>
#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud
{
	class IServerCallback;
	class BrainCloudClient;
	class FriendPlatform;
	class AuthenticationType;

	class BrainCloudFriend
	{
	public:
		BrainCloudFriend(BrainCloudClient* client);

		/**
		 * Retrieves profile information of the specified universal Id.
		 *
		 * @param searchText Universal ID text on which to search.
		 */
		void findUserByExactUniversalId(const char * searchText, IServerCallback * callback = NULL);

		/**
		 * Retrieves profile information for the users whos names start with search text.
		 *
		 * @param searchText Name text on which to search.
		 * @param maxResults Maximum number of results to return.
		 */
		void findUsersByNameStartingWith(const char * searchText, int32_t maxResults, IServerCallback * callback = NULL);

		/**
		 * Retrieves profile information for the users whos UniversalId start with search text.
		 *
		 * @param searchText Universal ID text on which to search.
		 * @param maxResults Maximum number of results to return.
		 */
		void findUsersByUniversalIdStartingWith(const char * searchText, int32_t maxResults, IServerCallback * callback = NULL);


		/**
		* Retrieves profile information for the specified user.
		*
		* Service Name - friend
		* Service Operation - GET_PROFILE_INFO_FOR_CREDENTIAL
		*
		* @param externalId The users's external ID
		* @param authenticationType The authentication type of the user ID
		* @param callback Method to be invoked when the server response is received.
		*/
		void getProfileInfoForCredential(const char * externalId, AuthenticationType authenticationType, IServerCallback * callback = NULL);

		/**
		* Retrieves profile information for the specified user.
		Silently fails, if profile does not exist, just returns null and success, instead of an error.
		*
		* Service Name - friend
		* Service Operation - GET_PROFILE_INFO_FOR_CREDENTIAL_IF_EXISTS
		*
		* @param externalId The users's external ID
		* @param authenticationType The authentication type of the user ID
		* @param callback Method to be invoked when the server response is received.
		*/
		void getProfileInfoForCredentialIfExists(const char *externalId, AuthenticationType authenticationType, IServerCallback *callback = NULL);

		/**
		* Retrieves profile information for the specified external auth user.
		*
		* Service Name - friend
		* Service Operation - GET_PROFILE_INFO_FOR_EXTERNAL_AUTH_ID
		*
		* @param externalId External ID of the friend to find
		* @param externalAuthType The external authentication type used for this friend's external ID
		* @param callback Method to be invoked when the server response is received.
		*/
		void getProfileInfoForExternalAuthId(const char * externalId, const char * externalAuthType, IServerCallback * callback = NULL);

		/**
		 * Retrieves profile information for the specified user. Silently fails, if profile does not exist, just returns null and success, instead of an error.
		 *
		 * Service Name - Friend
		 * Service Operation - GET_PROFILE_INFO_FOR_EXTERNAL_AUTH_ID_IF_EXISTS
		 *
		 * @param externalId External ID of the friend to find
		 * @param externalAuthType The external authentication type used for this friend's external ID
		 * @param callback Method to be invoked when the server response is received.
		 */
		void getProfileInfoForExternalAuthIdIfExists(const char *externalId, const char *externalAuthType, IServerCallback *callback = NULL);

		/**
		 * Retrieves the external ID for the specified user profile ID on the specified social platform.
		 *
		 * @param profileId Profile (user) ID.
		 * @param authenticationType Associated authentication type.
		 */
		void getExternalIdForProfileId(const char * profileId, const char * authenticationType, IServerCallback * callback = NULL);

		/**
		* Returns a particular entity of a particular friend.
		*
		* Service Name - Friend
		* Service Operation - READ_FRIEND_ENTITY
		*
		* @param entityId Id of entity to retrieve.
		* @param friendId Profile Id of friend who owns entity.
		* @param callback Method to be invoked when the server response is received.
		*/
		void readFriendEntity(const char* entityId, const char*  friendId, IServerCallback * callback = NULL);

		/**
		* Returns entities of all friends optionally based on type.
		*
		* Service Name - Friend
		* Service Operation - READ_FRIENDS_ENTITIES
		*
		* @param entityType Types of entities to retrieve.
		* @param callback Method to be invoked when the server response is received.
		*/
		void readFriendsEntities(const char* entityType, IServerCallback * callback = NULL);
		
		/**
		 * Read a friend's user state.
         * If you are not friend with this user, you will get an error
         * with NOT_FRIENDS reason code.
		 *
		 * Service Name - Friend
		 * Service Operation - READ_FRIEND_PLAYER_STATE
		 *
		 * @param friendId Target friend
		 * @param callback Method to be invoked when the server response is received.
		 */
		void readFriendUserState(const char * friendId, IServerCallback * callback = NULL);

		/**
		* Returns user state of a particular user.
		*
		* Service Name - Friend
		* Service Operation - GET_SUMMARY_DATA_FOR_PROFILE_ID
		*
		* @param profileId Profile Id of user to retrieve user state for.
		* @param callback Method to be invoked when the server response is received.
		*/
		void getSummaryDataForProfileId(const char * profileId, IServerCallback * callback = NULL);

		/**
		* Finds a list of users matching the search text by performing an exact match search
		*
		* Service Name - friend
		* Service Operation - FIND_USERS_BY_EXACT_NAME
		*
		* @param searchText The string to search for.
		* @param maxResults  Maximum number of results to return.
		* @param callback Method to be invoked when the server response is received.
		*/
		void findUsersByExactName(const char * searchText, int32_t maxResults, IServerCallback * callback = NULL);

		/**
		* Finds a list of users matching the search text by performing a substring
		* search of all user names.
		*
		* Service Name - friend
		* Service Operation - FIND_USERS_BY_SUBSTR_NAME
		*
		* @param searchText The substring to search for. Minimum length of 3 characters.
		* @param maxResults  Maximum number of results to return. If there are more the message
		* @param callback Method to be invoked when the server response is received.
		*/
		void findUsersBySubstrName(const char * searchText, int32_t maxResults, IServerCallback * callback = NULL);

		/**
		 * Retrieves a list of user and friend platform information for all friends of the current user.
		 *
		 * Service Name - Friend
		 * Service Operation - LIST_FRIENDS
		 *
		 * @param friendPlatform Friend platform to query.
		 * @param includeSummaryData  True if including summary data; false otherwise.
		 * @param callback Method to be invoked when the server response is received.
		 */
		void listFriends(const FriendPlatform& friendPlatform, bool includeSummaryData, IServerCallback * callback = NULL);

		/**
		 * Links the current user and the specified users as brainCloud friends.
		 *
		 * Service Name - Friend
		 * Service Operation - ADD_FRIENDS
		 *
		 * @param profileIds Collection of profile IDs.
		 * @param callback Method to be invoked when the server response is received.
		 */
		void addFriends(const std::vector<std::string>& profileIds, IServerCallback * callback = NULL);

		/**
		 * Links the profiles for the specified externalIds for the given friend platform as internal friends.
		 *
		 * Service Name - Friend
		 * Service Operation - ADD_FRIENDS_FROM_PLATFORM
		 *
		 * @param friendPlatform Platform to add from (i.e: FriendPlatform::Facebook)
		 * @param mode ADD or SYNC
		 * @param externalIds Collection of external IDs from the friend platform.
		 * @param callback Method to be invoked when the server response is received.
		 */
		void addFriendsFromPlatform(const FriendPlatform &friendPlatform, 
									const std::string &mode, 
									const std::vector<std::string> &externalIds, 
									IServerCallback *callback = NULL);

		/**
		* Unlinks the current user and the specified users as brainCloud friends.
		*
		* Service Name - Friend
		* Service Operation - REMOVE_FRIENDS
		*
		* @param profileIds Collection of profile IDs.
		* @param callback Method to be invoked when the server response is received.
		*/
		void removeFriends(const std::vector<std::string>& profileIds, IServerCallback * callback = NULL);

		/**
		* Get users online status
		*
		* Service Name - Friend
		* Service Operation - GET_USERS_ONLINE_STATUS
		*
		* @param profileIds Collection of profile IDs.
		* @param callback Method to be invoked when the server response is received.
		*/
		void getUsersOnlineStatus(const std::vector<std::string>& profileIds, IServerCallback * callback = NULL);

	private:
		BrainCloudClient * m_client;
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
