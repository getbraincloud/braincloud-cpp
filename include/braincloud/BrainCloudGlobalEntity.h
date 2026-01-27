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
		BrainCloudGlobalEntity(BrainCloudClient *client);

		/**
		 * Method creates a new entity on the server.
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - Create
		 *
		 * @param entityType The entity type as defined by the user
		 * @param timeToLive Sets expiry time for entity in milliseconds if > 0
		 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
		 * @param jsonEntityData  The entity's data as a json string
		 * @param callback The method to be invoked when the server response is received
		 */
		void createEntity(const std::string &entityType, int64_t timeToLive, const std::string &jsonEntityAcl,
						  const std::string &jsonEntityData, IServerCallback *callback = NULL);

		/**
		 * Method creates a new entity on the server with an indexed id.
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - CreateWithIndexedId
		 *
		 * @param entityType The entity type as defined by the user
		 * @param indexedId A secondary ID that will be indexed
		 * @param timeToLive Sets expiry time for entity in milliseconds if > 0
		 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
		 * @param jsonEntityData  The entity's data as a json string
		 * @param callback The method to be invoked when the server response is received
		 */
		void createEntityWithIndexedId(const std::string &entityType, const std::string &indexedId, int64_t timeToLive,
									   const std::string &jsonEntityAcl, const std::string &jsonEntityData, IServerCallback *callback = NULL);

		/**
		 * Method updates an existing entity on the server.
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - Update
		 *
		 * @param entityId The entity ID
		 * @param version The version of the entity to update
		 * @param jsonEntityData  The entity's data as a json string
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateEntity(const std::string &entityId, int64_t version, const std::string &jsonEntityData, IServerCallback *callback = NULL);

		/**
		 * Method updates an existing entity's Acl on the server.
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - UpdateAcl
		 *
		 * @param entityId The entity ID
		 * @param version The version of the entity to update
		 * @param jsonEntityAcl The entity's access control list as json.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateEntityAcl(const std::string &entityId, int64_t version, const std::string &jsonEntityAcl, IServerCallback *callback = NULL);

		/**
		 * Method updates an existing entity's time to live on the server.
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - UpdateTimeToLive
		 *
		 * @param entityId The entity ID
		 * @param version The version of the entity to update
		 * @param timeToLive Sets expiry time for entity in milliseconds if > 0
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateEntityTimeToLive(const std::string &entityId, int64_t version, int64_t timeToLive, IServerCallback *callback = NULL);

		/**
		 * Method deletes an existing entity on the server.
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - Delete
		 *
		 * @param entityId The entity ID
		 * @param version The version of the entity to delete
		 * @param callback The method to be invoked when the server response is received
		 */
		void deleteEntity(const std::string &entityId, int64_t version, IServerCallback *callback = NULL);

		/**
		 * Method reads an existing entity from the server.
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - Read
		 *
		 * @param entityId The entity ID
		 * @param callback The method to be invoked when the server response is received
		 */
		void readEntity(const std::string &entityId, IServerCallback *callback = NULL);

		/**
		 * Method gets list of entities from the server base on type and/or where clause
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - GetList
		 *
		 * @param where Mongo style query string
		 * @param orderBy Sort order
		 * @param maxReturn The maximum number of entities to return
		 * @param callback The method to be invoked when the server response is received
		 */
		void getList(const std::string &where, const std::string &orderBy, int64_t maxReturn, IServerCallback *callback = NULL);

		/**
		 * Method gets list of entities from the server base on indexed id
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - GetListByIndexedId
		 *
		 * @param entityIndexedId The entity indexed Id
		 * @param maxReturn The maximum number of entities to return
		 * @param callback The method to be invoked when the server response is received
		 */
		void getListByIndexedId(const std::string &entityIndexedId, int64_t maxReturn, IServerCallback *callback = NULL);

		/**
		 * Method gets a count of entities based on the where clause
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - GetListCount
		 *
		 * @param where Mongo style query string
		 * @param callback The method to be invoked when the server response is received
		 */
		void getListCount(const std::string &where, IServerCallback *callback = NULL);

		/**
		 * Method uses a paging system to iterate through Global Entities
		 * After retrieving a page of Global Entities with this method,
		 * use GetPageOffset() to retrieve previous or next pages.
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - GetPage
		 *
		 * @param context The json context for the page request.
		 *                   See the portal appendix documentation for format.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getPage(const char *context, IServerCallback *callback = NULL);

		/**
		 * Method to retrieve previous or next pages after having called the GetPage method.
		 *
		 * Service Name - GlobalEntity
		 * Service Operation - GetPageOffset
		 *
		 * @param context The context string returned from the server from a
		 *      previous call to GetPage or GetPageOffset
		 * @param pageOffset The positive or negative page offset to fetch. Uses the last page
		 *      retrieved using the context string to determine a starting point.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getPageOffset(const char *context, int pageOffset, IServerCallback *callback = NULL);

		/**
		 * Partial increment of global entity data field items. Partial set of items incremented as specified.
		 *
		 * Service Name - globalEntity
		 * Service Operation - INCREMENT_GLOBAL_ENTITY_DATA
		 *
		 * @param entityId The id of the entity to update
		 * @param jsonData The entity's data object
		 * @param callback The method to be invoked when the server response is received
		 */
		void incrementGlobalEntityData(const char *entityId, std::string jsonData, IServerCallback *callback = NULL);

		/**
		 * Gets a list of up to randomCount randomly selected entities from the server based on the where condition and specified maximum return count.
		 *
		 * Service Name - globalEntity
		 * Service Operation - GET_RANDOM_ENTITIES_MATCHING
		 *
		 * @param where Mongo style query string
		 * @param maxReturn The maximum number of entities to return
		 * @param callback The method to be invoked when the server response is received
		 */
		void getRandomEntitiesMatching(const std::string &where, int64_t maxReturn, IServerCallback *callback);

		/**
		 * Method updates an existing entity's Indexed Id
		 *
		 * Service Name - globalEntity
		 * Service Operation - UPDATE_ENTITY_OWNER_AND_ACL
		 *
		 * @param entityId The entity ID
		 * @param version The version of the entity to update
		 * @param entityIndexedId the id index of the entity
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateEntityIndexedId(const char *entityId, int64_t version, const char *entityIndexedId, IServerCallback *callback = NULL);

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
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateEntityOwnerAndAcl(const char *entityId, int64_t version, const char *ownerId, const std::string &jsonEntityAcl, IServerCallback *callback = NULL);

		/**
		 * Method clears the owner id of an existing entity and sets the ACL on the server.
		 *
		 * Service Name - globalEntity
		 * Service Operation - MAKE_SYSTEM_ENTITY
		 *
		 * @param entityId The entity ID
		 * @param version The version of the entity to update
		 * @param jsonEntityAcl The entity's access control list as JSON.
		 * @param callback The method to be invoked when the server response is received
		 */
		void makeSystemEntity(const char *entityId, int64_t version, const std::string &jsonEntityAcl, IServerCallback *callback = NULL);

	private:
		BrainCloudClient *m_client;
	};
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
