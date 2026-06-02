// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>

#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud {

	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudEntity
	{
	public:
		BrainCloudEntity(BrainCloudClient* client);

		/**
		 * Method creates a new entity on the server.
		 *
		 * Service Name - Entity
		 * Service Operation - CREATE
		 *
		 * @param entityType The entity type as defined by the user
		 * @param jsonEntityData The entity's data as a json string
		 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
		 * permissions which make the entity readable/writeable by only the user.
		 * @param callback The method to be invoked when the server response is received
		 */
		void createEntity(const char * entityType, const std::string& jsonEntityData, const std::string& jsonEntityAcl, IServerCallback * callback = NULL);

		/**
		 * Method updates a new entity on the server. This operation results in the entity
		 * data being completely replaced by the passed in JSON string.
		 *
		 * Service Name - Entity
		 * Service Operation - UPDATE
		 *
		 * @param entityId The id of the entity to update
		 * @param entityType The entity type as defined by the user
		 * @param jsonEntityData The entity's data as a json string.
		 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
		 * permissions which make the entity readable/writeable by only the user.
		 * @param version Current version of the entity. If the version of the
		 * entity on the server does not match the version passed in, the
		 * server operation will fail. Use -1 to skip version checking.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateEntity(
			const char * entityId,
			const char * entityType,
			const std::string& jsonEntityData,
			const std::string& jsonEntityAcl,
			int64_t version,
			IServerCallback * callback = NULL);

		/**
		* Method updates a new singleton entity on the server. This operation results in the entity
		* data being completely replaced by the passed in JSON string. If the entity doesn't exists it is created
		*
		* Service Name - Entity
		* Service Operation - UPDATE_SINGLETON
		*
		* @param entityType The entity type as defined by the user
		* @param jsonEntityData  The entity's data as a json string.
		* permissions which make the entity readable/writeable by only the user.
		* @param jsonEntityAcl The entity's access control list as json. A null acl implies default
		* permissions which make the entity readable/writeable by only the user.
		* @param version Current version of the entity. If the version of the
		* entity on the server does not match the version passed in, the
		* server operation will fail. Use -1 to skip version checking.
		* @param callback The method to be invoked when the server response is received
		*/
		void updateSingleton(const char * entityType, const std::string& jsonEntityData, const std::string& jsonEntityAcl, int64_t version, IServerCallback * callback = NULL);

		/**
		 * Method deletes the given entity on the server.
		 *
		 * Service Name - Entity
		 * Service Operation - DELETE
		 *
		 * @param entityId The id of the entity to update
		 * @param version Current version of the entity. If the version of the
		 * entity on the server does not match the version passed in, the
		 * server operation will fail. Use -1 to skip version checking.
		 * @param callback The method to be invoked when the server response is received
		 */
		void deleteEntity(const char * entityId, int64_t version, IServerCallback * callback = NULL);

		/**
		* Method deletes the given singleton entity on the server.
		*
		* Service Name - Entity
		* Service Operation - DELETE_SINGLETON
		*
		* @param entityType The type of the entity to delete
		* @param version Current version of the entity. If the version of the
		* entity on the server does not match the version passed in, the
		* server operation will fail. Use -1 to skip version checking.
		* @param callback The method to be invoked when the server response is received
		*/
		void deleteSingleton(const char * entityType, int64_t version, IServerCallback * callback = NULL);

		/** Method to get a specific entity.
		 *
		 * Service Name - Entity
		 * Service Operation - READ
		 *
		 * @param entityId The entity id
		 * @param callback The method to be invoked when the server response is received
		 */
		void getEntity(const char * entityId, IServerCallback * callback = NULL);

		/** Method retreives a singleton entity on the server. If the entity doesn't exist, null is returned.
		 *
		 * Service Name - Entity
		 * Service Operation - READ_SINGLETON
		 *
		 * @param entityType The entity type as defined by the user
		 * @param callback The method to be invoked when the server response is received
		 */
		void getSingleton(const char * entityType, IServerCallback * callback = NULL);

		/** Method returns all user entities that match the given type.
		 * Service Name - Entity
		 * Service Operation - READ_BY_TYPE
		 *
		 * @param entityType The entity type to search for
		 * @param callback The method to be invoked when the server response is received
		 */
		void getEntitiesByType(const char * entityType, IServerCallback * callback = NULL);

		/**
		* Method returns a shared entity for the given user and entity ID.
		* An entity is shared if its ACL allows for the currently logged
		* in user to read the data.
		*
		* Service Name - Entity
		* Service Operation - READ_SHARED_ENTITY
		*
		* @param profileId The the profile ID of the user who owns the entity
		* @param entityId The ID of the entity that will be retrieved
		* @param callback The method to be invoked when the server response is received
		*/
		void getSharedEntityForProfileId(const char * profileId, const char * entityId, IServerCallback * callback = NULL);

		/**
		 * Method returns all shared entities for the given profile id.
		 * An entity is shared if its ACL allows for the currently logged
		 * in user to read the data.
		 *
		 * Service Name - Entity
		 * Service Operation - READ_SHARED
		 *
		 * @param profileId The profile id to retrieve shared entities for
		 * @param callback The method to be invoked when the server response is received
		 */
		void getSharedEntitiesForProfileId(const char * profileId, IServerCallback * callback = NULL);

		/**
		* Method gets list of shared entities for the specified user based on type and/or where clause
		*
		* Service Name - Entity
		* Service Operation - READ_SHARED_ENTITIES_LIST
		*
		* @param profileId The profile ID to retrieve shared entities for
		* @param whereJson Mongo style query
		* @param orderByJson Sort order
		* @param maxReturn The maximum number of entities to return
		* @param callback The method to be invoked when the server response is received
		*/
		void getSharedEntitiesListForProfileId(const char * profileId, std::string whereJson, std::string orderByJson, int32_t maxReturn, IServerCallback * callback = NULL);

		/**
		 * Method updates a shared entity owned by another user. This operation results in the entity
		 * data being completely replaced by the passed in JSON string.
		 *
		 * Service Name - Entity
		 * Service Operation - UPDATE_SHARED
		 *
		 * @param entityId The id of the entity to update
		 * @param targetProfileId The id of the user who owns the shared entity
		 * @param entityType The entity type as defined by the user
		 * @param jsonEntityData The entity's data as a json string.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateSharedEntity(
			const char * entityId,
			const char * targetProfileId,
			const char * entityType,
			const std::string& jsonEntityData,
			int64_t version,
			IServerCallback * callback = NULL);

		/**
		* Method gets list of entities from the server base on type and/or where clause
		*
		* Service Name - Entity
		* Service Operation - GET_LIST
		*
		* @param whereJson Mongo style query string
		* @param orderByJson Sort order
		* @param maxReturn The maximum number of entities to return
		* @param callback The callback object
		*/
		void getList(const std::string& whereJson, const std::string& orderByJson, int64_t maxReturn, IServerCallback * callback = NULL);

		/**
		* Method gets a count of entities based on the where clause
		*
		* Service Name - Entity
		* Service Operation - GET_LIST_COUNT
		*
		* @param whereJson Mongo style query string
		* @param callback The callback object
		*/
		void getListCount(const std::string& whereJson, IServerCallback * callback = NULL);

		/**
		 * Method uses a paging system to iterate through user entities
		 * After retrieving a page of entities with this method,
		 * use GetPageOffset() to retrieve previous or next pages.
		 *
		 * Service Name - Entity
		 * Service Operation - GET_PAGE
		 *
		 * @param context The json context for the page request.
		 *                   See the portal appendix documentation for format.
		 * @param callback The callback object
		 */
		void getPage(const char * context, IServerCallback * callback = NULL);

		/**
		 * Method to retrieve previous or next pages after having called the GetPage method.
		 *
		 * Service Name - Entity
		 * Service Operation - GET_PAGE_BY_OFFSET
		 *
		 * @param context The context string returned from the server from a
		 *      previous call to GetPage or GetPageOffset
		 * @param pageOffset The positive or negative page offset to fetch. Uses the last page
		 *      retrieved using the context string to determine a starting point.
		 * @param callback The callback object
		 */
		void getPageOffset(const char * context, int pageOffset, IServerCallback * callback = NULL);

		/**
		* Partial increment of entity data field items. Partial set of items incremented as specified.
		*
		* Service Name - entity
		* Service Operation - INCREMENT_USER_ENTITY_DATA
		*
		* @param entityId The id of the entity to update
		* @param jsonData The entity's data object
		* @param callback The callback object
		*/
		void incrementUserEntityData(const char * entityId, std::string jsonData, IServerCallback * callback = NULL);

		/**
		* Partial increment of entity data field items. Partial set of items incremented as specified.
		*
		* Service Name - entity
		* Service Operation - INCREMENT_SHARED_USER_ENTITY_DATA
		*
		* @param entityId The id of the entity to update
		* @param targetProfileId Profile ID of the entity owner
		* @param jsonData The entity's data object
		* @param callback The callback object
		*/
		void incrementSharedUserEntityData(const char * entityId, const char * targetProfileId, std::string jsonData, IServerCallback * callback = NULL);

	private:
		BrainCloudClient * m_client;
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
