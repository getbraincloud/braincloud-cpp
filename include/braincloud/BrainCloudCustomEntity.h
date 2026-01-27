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

	class BrainCloudCustomEntity
	{
	public:
		BrainCloudCustomEntity(BrainCloudClient *client);

		/**
		 * Creates new custom entity.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - CreateEntity
		 *
		 * @param entityType The entity type as defined by the user
		 * @param jsonEntityData The entity's data as a json string
		 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
		 * permissions which make the entity readable/writeable by only the user.
		 * @param timeToLive
		 * @param isOwned
		 * @param callback The method to be invoked when the server response is received
		 */
		void createEntity(const char *entityType, const std::string &jsonEntityData, const std::string &jsonEntityAcl, int64_t timeToLive, bool isOwned, IServerCallback *callback = NULL);

		/**
		 * Deletes the specified custom entity on the server.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - DeleteEntity
		 *
		 * @param entityType The entity type as defined by the user
		 * @param jsonEntityData The entity's data as a json string
		 * @param version Version of the custom entity being updated.
		 * @param callback The method to be invoked when the server response is received
		 */
		void deleteEntity(const char *entityType, const char *entityId, const std::string &jsonEntityData, int32_t version, IServerCallback *callback = NULL);

		/**
		 * Counts the number of custom entities meeting the specified where clause, enforcing ownership/ACL permissions
		 *
		 * Service Name - CustomEntity
		 * Service Operation - GetCount
		 *
		 * @param entityType The entity type as defined by the user
		 * @param whereJson Mongo style query string
		 * @param callback The method to be invoked when the server response is received
		 */
		void getCount(const char *entityType, const std::string &whereJson, IServerCallback *callback = NULL);

		/**
		 * Method uses a paging system to iterate through Custom Entities
		 * After retrieving a page of Custom Entities with this method,
		 * use GetEntityPageOffset() to retrieve previous or next pages.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - GetCustomEntityPage
		 *
		 * @param entityType The entity type as defined by the user
		 * @param context The json context for the page request.
		 *                   See the portal appendix documentation for format.
		 * @param callback The callback object
		 */
		void getEntityPage(const char *entityType, const char *context, IServerCallback *callback = NULL);

		/**
		 * Gets a list of up to maxReturn randomly selected custom entities from the
		 * server based on the entity type and where condition.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - GetRandomEntitiesMatching
		 *
		 * @param entityType The entity type as defined by the user
		 * @param whereJson Mongo style query string
		 * @param maxReturn Max number of returns
		 * @param callback The method to be invoked when the server response is received
		 */
		void getRandomEntitiesMatching(const char *entityType, const std::string &whereJson, const int32_t maxReturn, IServerCallback *callback = NULL);

		/**
		 * Gets the page of custom entities from the server based on the encoded context and specified page offset.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - GetEntityPageOffset
		 *
		 * @param entityType The entity type as defined by the user
		 * @param context The context string returned from the server from a previous
		 *                   call to GetPage or GetPageOffset.
		 * @param pageOffset The positive or negative page offset to fetch. Uses the
		 *                   last page retrieved using the context string to determine a
		 *                   starting point.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getEntityPageOffset(const char *entityType, const std::string &context, int32_t pageOffset, IServerCallback *callback = NULL);

		/**
		 * Reads the specified custom entity from the server.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - ReadEntity
		 *
		 * @param entityType The entity type as defined by the user
		 * @param entityId The entity id as defined by the system
		 * @param callback The method to be invoked when the server response is received
		 */
		void readEntity(const char *entityType, const char *entityId, IServerCallback *callback = NULL);

		/**
		 * Replaces the specified custom entity's data, and optionally updates the acl and expiry, on the server.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - UpdateEntity
		 *
		 * @param entityType The entity type as defined by the user
		 * @param entityId The id of custom entity being updated.
		 * @param version Version of the custom entity being updated.
		 * @param jsonEntityData The entity's data as a json string
		 * @param jsonEntityAcl The entity's access control list as json. A null acl implies default
		 * permissions which make the entity readable/writeable by only the user.
		 * @param timeToLive
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateEntity(const char *entityType, const char *entityId, int32_t version, const std::string &jsonEntityData, const std::string &jsonEntityAcl, int64_t timeToLive, IServerCallback *callback = NULL);

		/**
		 * Replaces the specified custom entity's data, and optionally updates the acl and expiry, on the server.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - UpdateEntityFields
		 *
		 * @param entityType The entity type as defined by the user
		 * @param entityId The id of custom entity being updated.
		 * @param version Version of the custom entity being updated.
		 * @param fieldsJson Specific fields, as JSON, to set within entity's custom data.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateEntityFields(const char *entityType, const char *entityId, int32_t version, const std::string &fieldsJson, IServerCallback *callback = NULL);

		/**
		 * For sharded custom collection entities. Sets the specified fields within custom entity data on the server, enforcing ownership/ACL permissions.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - UpdateEntityFieldsSharded
		 *
		 * @param entityType The entity type as defined by the user
		 * @param entityId The id of custom entity being updated.
		 * @param version Version of the custom entity being updated.
		 * @param fieldsJson Specific fields, as JSON, to set within entity's custom data.
		 * @param shardKeyJson The shard key field(s) and value(s), as JSON, applicable to the entity being updated.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateEntityFieldsSharded(const char *entityType, const char *entityId, int32_t version, const std::string &fieldsJson, const std::string &shardKeyJson, IServerCallback *callback = NULL);

		/**
		 * deletes entities based on the delete criteria.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - DeleteEntities
		 *
		 * @param entityType The entity type as defined by the user
		 * @param deleteCriteria Json string of criteria wanted for deletion
		 * @param callback The method to be invoked when the server response is received
		 */
		void deleteEntities(const char *entityType, const char *deleteCriteria, IServerCallback *callback = NULL);

		/**
		 * Deletes the specified custom entity singleton, owned by the session's user,
		 * for the specified entity type, on the server.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - DeleteSingleton
		 *
		 * @param entityType The entity type as defined by the user
		 * @param version Version of the singleton being deleted.
		 * @param callback The method to be invoked when the server response is received
		 */
		void deleteSingleton(const char *entityType, int32_t version, IServerCallback *callback = NULL);

		/**
		 * Updates the singleton owned by the user for the specified custom entity type on the server,
		 * creating the singleton if it does not exist.
		 * This operation results in the owned singleton's data being completely replaced by the passed in JSON object.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - UpdateSingleton
		 *
		 * @param entityType The entity type as defined by the user
		 * @param version Version of the singleton being updated.
		 * @param dataJson The full data for the singleton as a json string
		 * @param acl The singleton entity's Access Control List as an object.
		 * 				 A null ACL implies default permissions which make the entity readable by others.
		 * @param timeToLive The duration of time, in milliseconds, the singleton custom entity should live
		 * 				before being expired. Null indicates never expires. Value of -1 indicates no change for updates.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateSingleton(const char *entityType, int32_t version, const std::string &dataJson, const std::string &acl, int64_t timeToLive, IServerCallback *callback = NULL);

		/**
		 * Partially updates the data, of the singleton owned by the user for the specified custom entity type,
		 * with the specified fields, on the server
		 *
		 * Service Name - CustomEntity
		 * Service Operation - UpdateSingletonFields
		 *
		 * @param entityType The entity type as defined by the user
		 * @param version Version of the singleton being updated.
		 * @param fieldsJson Specific fields, as JSON, within entity's custom data to be updated.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateSingletonFields(const char *entityType, int32_t version, const std::string &fieldsJson, IServerCallback *callback = NULL);

		/**
		 * Reads the custom entity singleton owned by the session's user.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - ReadSingleton
		 *
		 * @param entityType The entity type as defined by the user
		 * @param callback The method to be invoked when the server response is received
		 */
		void readSingleton(const char *entityType, IServerCallback *callback = NULL);

		/**
		 * Increments fields on the specified custom entity owned by the user on the server.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - IncrementData
		 *
		 * @param entityType The entity type as defined by the user
		 * @param entityId The entity id as defined by the system
		 * @param fieldsJson Specific fields, as JSON, within entity's custom data, with respective increment amount.
		 * @param callback The method to be invoked when the server response is received
		 */
		void incrementData(const char *entityType, const char *entityId, const std::string &fieldsJson, IServerCallback *callback = NULL);

		/**
		 * Increments the specified fields, of the singleton owned by the user, by the specified amount within the custom entity data on the server.
		 *
		 * Service Name - CustomEntity
		 * Service Operation - IncrementSingletonData
		 *
		 * @param entityType The type of custom entity being updated.
		 * @param fieldsJson Specific fields, as JSON, within entity's custom data, with respective increment amount.
		 * @param callback The method to be invoked when the server response is received
		 */
		void incrementSingletonData(const char *entityType, const std::string &fieldsJson, IServerCallback *callback = NULL);

	private:
		BrainCloudClient *m_client;
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
