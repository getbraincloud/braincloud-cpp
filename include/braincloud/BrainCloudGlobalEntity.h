// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>
#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud
{
	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudGlobalEntity
	{
	public:
		BrainCloudGlobalEntity(BrainCloudClient* client);

		/**
		* Method creates a new entity on the server.
		*
		* Service Name - globalEntity
		* Service Operation - CREATE
		*
		* @param entityType The entity type as defined by the user
		* @param timeToLive Sets expiry time for entity in milliseconds if > 0
		* @param jsonEntityAcl The entity's access control list as json. A null acl implies default
		* @param jsonEntityData  The entity's data as a json string
		* @param callback The callback object
		*/
		void createEntity(const std::string& entityType, int64_t timeToLive, const std::string& jsonEntityAcl,
			const std::string& jsonEntityData, IServerCallback * callback = NULL);

		/**
		* Method creates a new entity on the server with an indexed id.
		*
		* Service Name - globalEntity
		* Service Operation - CREATE_WITH_INDEXED_ID
		*
		* @param entityType The entity type as defined by the user
		* @param indexedId A secondary ID that will be indexed
		* @param timeToLive Sets expiry time for entity in milliseconds if > 0
		* @param jsonEntityAcl The entity's access control list as json. A null acl implies default
		* @param jsonEntityData  The entity's data as a json string
		* @param callback The callback object
		*/
		void createEntityWithIndexedId(const std::string& entityType, const std::string& indexedId, int64_t timeToLive,
			const std::string& jsonEntityAcl, const std::string& jsonEntityData, IServerCallback * callback = NULL);

		/**
		* Method updates an existing entity on the server.
		*
		* Service Name - globalEntity
		* Service Operation - UPDATE
		*
		* @param entityId The entity ID
		* @param version The version of the entity to update
		* @param jsonEntityData  The entity's data as a json string
		* @param callback The callback object
		*/
		void updateEntity(const std::string& entityId, int64_t version, const std::string& jsonEntityData, IServerCallback * callback = NULL);

		/**
		* Method updates an existing entity's Acl on the server.
		*
		* Service Name - globalEntity
		* Service Operation - UPDATE_ACL
		*
		* @param entityId The entity ID
		* @param version The version of the entity to update
		* @param jsonEntityAcl The entity's access control list as json.
		* @param callback The callback object
		*/
		void updateEntityAcl(const std::string& entityId, int64_t version, const std::string& jsonEntityAcl, IServerCallback * callback = NULL);

		/**
		* Method updates an existing entity's time to live on the server.
		*
		* Service Name - globalEntity
		* Service Operation - UPDATE_TIME_TO_LIVE
		*
		* @param entityId The entity ID
		* @param version The version of the entity to update
		* @param timeToLive Sets expiry time for entity in milliseconds if > 0
		* @param callback The callback object
		*/
		void updateEntityTimeToLive(const std::string& entityId, int64_t version, int64_t timeToLive, IServerCallback * callback = NULL);

		/**
		* Method deletes an existing entity on the server.
		*
		* Service Name - globalEntity
		* Service Operation - DELETE
		*
		* @param entityId The entity ID
		* @param version The version of the entity to delete
		* @param callback The callback object
		*/
		void deleteEntity(const std::string& entityId, int64_t version, IServerCallback * callback = NULL);

		/**
		* Method reads an existing entity from the server.
		*
		* Service Name - globalEntity
		* Service Operation - READ
		*
		* @param entityId The entity ID
		* @param callback The callback object
		*/
		void readEntity(const std::string& entityId, IServerCallback * callback = NULL);

		/**
		* Method gets list of entities from the server base on type and/or where clause
		*
		* Service Name - globalEntity
		* Service Operation - GET_LIST
		*
		* @param where Mongo style query string
		* @param orderBy Sort order
		* @param maxReturn The maximum number of entities to return
		* @param callback The callback object
		*/
		void getList(const std::string& where, const std::string& orderBy, int64_t maxReturn, IServerCallback * callback = NULL);

		/**
		* Method gets list of entities from the server base on indexed id
		*
		* Service Name - globalEntity
		* Service Operation - GET_LIST_BY_INDEXED_ID
		*
		* @param entityIndexedId The entity indexed Id
		* @param maxReturn The maximum number of entities to return
		* @param callback The callback object
		*/
		void getListByIndexedId(const std::string& entityIndexedId, int64_t maxReturn, IServerCallback * callback = NULL);

		/**
		* Method gets a count of entities based on the where clause
		*
		* Service Name - globalEntity
		* Service Operation - GET_LIST_COUNT
		*
		* @param where Mongo style query string
		* @param callback The callback object
		*/
		void getListCount(const std::string& where, IServerCallback * callback = NULL);

		/**
		 * Method uses a paging system to iterate through Global Entities
		 * After retrieving a page of Global Entities with this method,
		 * use GetPageOffset() to retrieve previous or next pages.
		 *
		 * Service Name - globalEntity
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
		 * Service Name - globalEntity
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
		* Partial increment of global entity data field items. Partial set of items incremented as specified.
		*
		* Service Name - globalEntity
		* Service Operation - INCREMENT_GLOBAL_ENTITY_DATA
		*
		* @param entityId The id of the entity to update
		* @param jsonData The entity's data object
		* @param callback The callback object
		*/
		void incrementGlobalEntityData(const char * entityId, std::string jsonData, IServerCallback * callback = NULL);

		/**
    	* Gets a list of up to randomCount randomly selected entities from the server based on the where condition and specified maximum return count.
    	*
    	* Service Name - globalEntity
    	* Service Operation - GET_RANDOM_ENTITIES_MATCHING
    	*
    	* @param where Mongo style query string
    	* @param maxReturn The maximum number of entities to return
    	* @param callback The callback object
    	*/
		void getRandomEntitiesMatching(const std::string& where, int64_t maxReturn, IServerCallback * callback);

		/**
		* Method updates an existing entity's Indexed Id
		*
		* Service Name - globalEntity
		* Service Operation - UPDATE_ENTITY_OWNER_AND_ACL
		*
		* @param entityId The entity ID
		* @param version The version of the entity to update
		* @param entityIndexedId the id index of the entity
		* @param callback The callback object
		*/
		void updateEntityIndexedId(const char * entityId, int64_t version, const char * entityIndexedId, IServerCallback * callback = NULL);

		/**
		* Method updates an existing entity's Owner and ACL on the server.
		*
		* Service Name - globalEntity
		* Service Operation - UPDATE_ENTITY_OWNER_AND_ACL
		*
		* @param entityId The entity ID
		* @param version The version of the entity to update
		* @param ownerId The owner ID
		* @param jsonEntityAcl The entity's access control list as JSON.
		* @param callback The callback object
		*/
		void updateEntityOwnerAndAcl(const char * entityId, int64_t version, const char * ownerId, const std::string& jsonEntityAcl, IServerCallback * callback = NULL);

		/**
		* Method clears the owner id of an existing entity and sets the ACL on the server.
		*
		* Service Name - globalEntity
		* Service Operation - MAKE_SYSTEM_ENTITY
		*
		* @param entityId The entity ID
		* @param version The version of the entity to update
		* @param jsonEntityAcl The entity's access control list as JSON.
		* @param callback The callback object
		*/
		void makeSystemEntity(const char * entityId, int64_t version, const std::string& jsonEntityAcl, IServerCallback * callback = NULL);

	private:
		BrainCloudClient * m_client;
	};
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
