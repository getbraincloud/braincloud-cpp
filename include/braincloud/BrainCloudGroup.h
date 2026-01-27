// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include <vector>
#include <string>
#include "BrainCloudTypes.h"

namespace BrainCloud
{
	namespace eGroupMember
	{
		enum Role
		{
			UNKNOWN = 0,
			MEMBER,
			ADMIN,
			OWNER,
			OTHER
		};
	}

	namespace eAutoJoinStrategy
	{
		enum Strategy
		{
			JoinFirstGroup = 0,
			JoinRandomGroup
		};
	}

	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudGroup
	{
	public:
		BrainCloudGroup(BrainCloudClient *client);

		/**
		 * Accept an outstanding invitation to join the group.
		 *
		 * Service Name - group
		 * Service Operation - ACCEPT_GROUP_INVITATION
		 *
		 * @param groupId ID of the group.
		 * @param callback The method to be invoked when the server response is received
		 */
		void acceptGroupInvitation(const char *groupId, IServerCallback *callback = NULL);

		/**
		 * Add a member to the group.
		 *
		 * Service Name - group
		 * Service Operation - ADD_GROUP_MEMBER
		 *
		 * @param groupId ID of the group.
		 * @param profileId Profile ID of the member being added.
		 * @param role Role of the member being added.
		 * @param jsonAttributes Attributes of the member being added.
		 * @param callback The method to be invoked when the server response is received
		 */
		void addGroupMember(const char *groupId, const char *profileId, eGroupMember::Role role, const std::string &jsonAttributes, IServerCallback *callback = NULL);

		/**
		 * Approve an outstanding request to join the group.
		 *
		 * Service Name - group
		 * Service Operation - APPROVE_GROUP_JOREQUEST
		 *
		 * @param groupId ID of the group.
		 * @param profileId Profile ID of the invitation being deleted.
		 * @param role Role of the member being invited.
		 * @param jsonAttributes Attributes of the member being invited.
		 * @param callback The method to be invoked when the server response is received
		 */
		void approveGroupJoinRequest(const char *groupId, const char *profileId, eGroupMember::Role role, const std::string &jsonAttributes, IServerCallback *callback = NULL);

		/**
		 * Automatically join an open group that matches the search criteria and has space available.
		 *
		 * Service Name - group
		 * Service Operation - AUTO_JOGROUP
		 *
		 * @param groupType Name of the associated group type.
		 * @param autoJoinStrategy Selection strategy to employ when there are multiple matches
		 * @param dataQueryJson Query parameters (optional)
		 * @param callback The method to be invoked when the server response is received
		 */
		void autoJoinGroup(const char *groupType, eAutoJoinStrategy::Strategy autoJoinStrategy, std::string dataQueryJson, IServerCallback *callback = NULL);

		/**
		 * Find and join an open group in the pool of groups in multiple group types provided as input arguments.		*
		 * Service Name - group
		 * Service Operation - AUTO_JOGROUP_MULTI
		 *
		 * @param groupTypes Name of the associated group type.
		 * @param autoJoinStrategy Selection strategy to employ when there are multiple matches
		 * @param where Query parameters (optional)
		 * @param callback The method to be invoked when the server response is received
		 */
		void autoJoinGroupMulti(const std::vector<std::string> &groupTypes, eAutoJoinStrategy::Strategy autoJoinStrategy, std::string where, IServerCallback *callback = NULL);

		/**
		 * Cancel an outstanding invitation to the group.
		 *
		 * Service Name - group
		 * Service Operation - CANCEL_GROUP_INVITATION
		 *
		 * @param groupId ID of the group.
		 * @param profileId Profile ID of the invitation being deleted.
		 * @param callback The method to be invoked when the server response is received
		 */
		void cancelGroupInvitation(const char *groupId, const char *profileId, IServerCallback *callback = NULL);

		/**
		 * Create a group.
		 *
		 * Service Name - group
		 * Service Operation - CREATE_GROUP
		 *
		 * @param name Name of the group.
		 * @param groupType Name of the type of group.
		 * @param isOpenGroup true if group is open; false if closed.
		 * @param acl The group's access control list. A null ACL implies default.
		 * @param jsonOwnerAttributes Attributes for the group owner (current user).
		 * @param jsonDefaultMemberAttributes Default attributes for group members.
		 * @param jsonData Custom application data.
		 * @param callback The method to be invoked when the server response is received
		 */
		void createGroup(
			const char *name,
			const char *groupType,
			bool isOpenGroup,
			const std::string &acl,
			const std::string &jsonData,
			const std::string &jsonOwnerAttributes,
			const std::string &jsonDefaultMemberAttributes,
			IServerCallback *callback = NULL);

		/**
		 * Create a group with Summary Data.
		 *
		 * Service Name - group
		 * Service Operation - CREATE_GROUP
		 *
		 * @param name Name of the group.
		 * @param groupType Name of the type of group.
		 * @param isOpenGroup true if group is open; false if closed.
		 * @param acl The group's access control list. A null ACL implies default.
		 * @param jsonOwnerAttributes Attributes for the group owner (current user).
		 * @param jsonDefaultMemberAttributes Default attributes for group members.
		 * @param jsonSummaryData the summary.
		 * @param jsonData Custom application data.
		 * @param callback The method to be invoked when the server response is received
		 */
		void createGroupWithSummaryData(
			const char *name,
			const char *groupType,
			bool isOpenGroup,
			const std::string &acl,
			const std::string &jsonData,
			const std::string &jsonOwnerAttributes,
			const std::string &jsonDefaultMemberAttributes,
			const std::string &jsonSummaryData,
			IServerCallback *callback = NULL);

		/**
		 * Create a group entity.
		 *
		 * Service Name - group
		 * Service Operation - CREATE_GROUP_ENTITY
		 *
		 * @param groupId ID of the group.
		 * @param isOwnedByGroupMember true if entity is owned by a member; false if owned by the entire group.
		 * @param entityType Type of the group entity.
		 * @param acl Access control list for the group entity.
		 * @param jsonData Custom application data.
		 * @param callback The method to be invoked when the server response is received
		 */
		void createGroupEntity(
			const char *groupId,
			const char *entityType,
			bool isOwnedByGroupMember,
			const std::string &acl,
			const std::string &jsonData,
			IServerCallback *callback = NULL);

		/**
		 * Delete a group.
		 *
		 * Service Name - group
		 * Service Operation - DELETE_GROUP
		 *
		 * @param groupId ID of the group.
		 * @param version Current version of the group
		 * @param callback The method to be invoked when the server response is received
		 */
		void deleteGroup(const char *groupId, int32_t version, IServerCallback *callback = NULL);

		/**
		 * Delete a group entity.
		 *
		 * Service Name - group
		 * Service Operation - DELETE_GROUP_ENTITY
		 *
		 * @param groupId ID of the group.
		 * @param entityId ID of the entity.
		 * @param version The current version of the group entity (for concurrency checking).
		 * @param callback The method to be invoked when the server response is received
		 */
		void deleteGroupEntity(const char *groupId, const char *entityId, int32_t version, IServerCallback *callback = NULL);

		/**
		 * Delete an outstanding request to join the group.
		 *
		 * Service Name - group
		 * Service Operation - DELETE_GROUP_JOREQUEST
		 *
		 * @param groupId ID of the group.
		 * @param callback The method to be invoked when the server response is received
		 */
		void deleteGroupJoinRequest(const char *groupId, IServerCallback *callback = NULL);

		/**
		 * Read information on groups to which the current user belongs.
		 *
		 * Service Name - group
		 * Service Operation - GET_MY_GROUPS
		 * @param callback The method to be invoked when the server response is received
		 */
		void getMyGroups(IServerCallback *callback = NULL);

		/**
		 * Increment elements for the group's data field.
		 *
		 * Service Name - group
		 * Service Operation - INCREMENT_GROUP_DATA
		 *
		 * @param groupId ID of the group.
		 * @param jsonData Partial data map with incremental values.
		 * @param callback The method to be invoked when the server response is received
		 */
		void incrementGroupData(const char *groupId, const std::string &jsonData, IServerCallback *callback = NULL);

		/**
		 * Increment elements for the group entity's data field.
		 *
		 * Service Name - group
		 * Service Operation - INCREMENT_GROUP_ENTITY_DATA
		 *
		 * @param groupId ID of the group.
		 * @param entityId ID of the entity.
		 * @param jsonData Partial data map with incremental values.
		 * @param callback The method to be invoked when the server response is received
		 */
		void incrementGroupEntityData(const char *groupId, const char *entityId, const std::string &jsonData, IServerCallback *callback = NULL);

		/**
		 * Invite a member to the group.
		 *
		 * Service Name - group
		 * Service Operation - INVITE_GROUP_MEMBER
		 *
		 * @param groupId ID of the group.
		 * @param profileId Profile ID of the member being invited.
		 * @param role Role of the member being invited.
		 * @param jsonAttributes Attributes of the member being invited.
		 * @param callback The method to be invoked when the server response is received
		 */
		void inviteGroupMember(const char *groupId, const char *profileId, eGroupMember::Role role, const std::string &jsonAttributes, IServerCallback *callback = NULL);

		/**
		 * Join an open group or request to join a closed group.
		 *
		 * Service Name - group
		 * Service Operation - JOGROUP
		 *
		 * @param groupId ID of the group.
		 * @param callback The method to be invoked when the server response is received
		 */
		void joinGroup(const char *groupId, IServerCallback *callback = NULL);

		/**
		 * Leave a group in which the user is a member.
		 *
		 * Service Name - group
		 * Service Operation - LEAVE_GROUP
		 *
		 * @param groupId ID of the group.
		 * @param callback The method to be invoked when the server response is received
		 */
		void leaveGroup(const char *groupId, IServerCallback *callback = NULL);

		/**
		 * Read a page of group information.
		 *
		 * Service Name - group
		 * Service Operation - LIST_GROUPS_PAGE
		 *
		 * @param context Query context.
		 * @param callback The method to be invoked when the server response is received
		 */
		void listGroupsPage(const std::string &jsonContext, IServerCallback *callback = NULL);

		/**
		 * Read a page of group information.
		 *
		 * Service Name - group
		 * Service Operation - LIST_GROUPS_PAGE_BY_OFFSET
		 *
		 * @param encodedContext Encoded reference query context.
		 * @param offset Number of pages by which to offset the query.
		 * @param callback The method to be invoked when the server response is received
		 */
		void listGroupsPageByOffset(const char *encodedContext, int32_t pageOffset, IServerCallback *callback = NULL);

		/**
		 * Read information on groups to which the specified user belongs.  Access is subject to restrictions.
		 *
		 * Service Name - group
		 * Service Operation - LIST_GROUPS_WITH_MEMBER
		 *
		 * @param profileId
		 * @param callback The method to be invoked when the server response is received
		 */
		void listGroupsWithMember(const char *profileId, IServerCallback *callback = NULL);

		/**
		 * Read the specified group.
		 *
		 * Service Name - group
		 * Service Operation - READ_GROUP
		 *
		 * @param groupId ID of the group.
		 * @param callback The method to be invoked when the server response is received
		 */
		void readGroup(const char *groupId, IServerCallback *callback = NULL);

		/**
		 * Read the specified group's data.
		 *
		 * Service Name - group
		 * Service Operation - READ_GROUP_DATA
		 *
		 * @param groupId ID of the group.
		 * @param callback The method to be invoked when the server response is received
		 */
		void readGroupData(const char *groupId, IServerCallback *callback = NULL);

		/**
		 * Read a page of group entity information.
		 *
		 * Service Name - group
		 * Service Operation - READ_GROUP_ENTITIES_PAGE
		 *
		 * @param context Query context.
		 * @param callback The method to be invoked when the server response is received
		 */
		void readGroupEntitiesPage(const std::string &jsonContext, IServerCallback *callback = NULL);

		/**
		 * Read a page of group entity information.
		 *
		 * Service Name - group
		 * Service Operation - READ_GROUP_ENTITIES_PAGE_BY_OFFSET
		 *
		 * @param encodedContext Encoded reference query context.
		 * @param offset Number of pages by which to offset the query.
		 * @param callback The method to be invoked when the server response is received
		 */
		void readGroupEntitiesPageByOffset(const char *encodedContext, int32_t pageOffset, IServerCallback *callback = NULL);

		/**
		 * Read the specified group entity.
		 *
		 * Service Name - group
		 * Service Operation - READ_GROUP_ENTITY
		 *
		 * @param groupId ID of the group.
		 * @param entityId ID of the entity.
		 * @param callback The method to be invoked when the server response is received
		 */
		void readGroupEntity(const char *groupId, const char *entityId, IServerCallback *callback = NULL);

		/**
		 * Read the members of the group.
		 *
		 * Service Name - group
		 * Service Operation - READ_MEMBERS_OF_GROUP
		 *
		 * @param groupId ID of the group.
		 * @param callback The method to be invoked when the server response is received
		 */
		void readGroupMembers(const char *groupId, IServerCallback *callback = NULL);

		/**
		 * Reject an outstanding invitation to join the group.
		 *
		 * Service Name - group
		 * Service Operation - REJECT_GROUP_INVITATION
		 *
		 * @param groupId ID of the group.
		 * @param callback The method to be invoked when the server response is received
		 */
		void rejectGroupInvitation(const char *groupId, IServerCallback *callback = NULL);

		/**
		 * Reject an outstanding request to join the group.
		 *
		 * Service Name - group
		 * Service Operation - REJECT_GROUP_JOREQUEST
		 *
		 * @param groupId ID of the group.
		 * @param profileId Profile ID of the invitation being deleted.
		 * @param callback The method to be invoked when the server response is received
		 */
		void rejectGroupJoinRequest(const char *groupId, const char *profileId, IServerCallback *callback = NULL);

		/**
		 * Remove a member from the group.
		 *
		 * Service Name - group
		 * Service Operation - REMOVE_GROUP_MEMBER
		 *
		 * @param groupId ID of the group.
		 * @param profileId Profile ID of the member being deleted.
		 * @param callback The method to be invoked when the server response is received
		 */
		void removeGroupMember(const char *groupId, const char *profileId, IServerCallback *callback = NULL);

		/**
		 * Updates a group's data.
		 *
		 * Service Name - group
		 * Service Operation - UPDATE_GROUP_DATA
		 *
		 * @param groupId ID of the group.
		 * @param version Version to verify.
		 * @param jsonData Data to apply.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateGroupData(const char *groupId, int32_t version, const std::string &jsonData, IServerCallback *callback = NULL);

		/**
		 * Update the acl settings for a group entity, enforcing ownership.
		 *
		 * Service Name - Group
		 * Service Operation - UPDATE_GROUP_ENTITY_ACL
		 *
		 * @param groupId The id of the group
		 * @param entityId The id of the entity to update
		 * @param acl Access control list for the group entity
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateGroupEntityAcl(const char *groupId, const char *entityId, const std::string &acl, IServerCallback *callback = NULL);

		/**
		 * Update a group entity.
		 *
		 * Service Name - group
		 * Service Operation - UPDATE_GROUP_ENTITY_DATA
		 *
		 * @param groupId ID of the group.
		 * @param entityId ID of the entity.
		 * @param version The current version of the group entity (for concurrency checking).
		 * @param jsonData Custom application data.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateGroupEntityData(const char *groupId, const char *entityId, int32_t version, const std::string &jsonData, IServerCallback *callback = NULL);

		/**
		 * Update a member of the group.
		 *
		 * Service Name - group
		 * Service Operation - UPDATE_GROUP_MEMBER
		 *
		 * @param groupId ID of the group.
		 * @param profileId Profile ID of the member being updated.
		 * @param role Role of the member being updated (optional).
		 * @param jsonAttributes Attributes of the member being updated (optional).
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateGroupMember(const char *groupId, const char *profileId, eGroupMember::Role role, const std::string &jsonAttributes, IServerCallback *callback = NULL);

		/**
		 * Updates a group's name.
		 *
		 * Service Name - group
		 * Service Operation - UPDATE_GROUP_NAME
		 *
		 * @param groupId ID of the group.
		 * @param name Name to apply.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateGroupName(const char *groupId, const char *name, IServerCallback *callback = NULL);

		/**
		 * Set whether a group is open true or false
		 *
		 * Service Name - group
		 * Service Operation - SET_GROUP_OPEN
		 *
		 * @param groupId ID of the group.
		 * @param isOpenGroup whether its open or not
		 * @param callback The method to be invoked when the server response is received
		 */
		void setGroupOpen(const char *groupId, bool isOpenGroup, IServerCallback *callback = nullptr);

		/**
		 * Set a group's access conditions.
		 *
		 * Service Name - Group
		 * Service Operation - UPDATE_GROUP_ACL
		 *
		 * @param groupId ID of the group
		 * @param acl The group's access control list. A null ACL implies default
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateGroupAcl(const char *groupId, const std::string &acl, IServerCallback *callback = NULL);

		/**
		 * Update a group's summary data
		 *
		 * Service Name - group
		 * Service Operation - UPDATE_GROUP_SUMMARY_DATA
		 *
		 * @param groupId ID of the group.
		 * @param version the version of the group
		 * @param jsonSummaryData custom application data
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateGroupSummaryData(const char *groupId, int version, const std::string &jsonSummaryData, IServerCallback *callback = nullptr);

		/**
		 * Gets a list of up to maxReturn randomly selected groups from the server based on the where condition.
		 *
		 * Service Name - group
		 * Service Operation - GET_RANDOM_GROUPS_MATCHING
		 *
		 * @param jsonWhere where to search
		 * @param maxReturn # of groups to search
		 * @param callback The method to be invoked when the server response is received
		 */
		void getRandomGroupsMatching(std::string jsonWhere, int maxReturn, IServerCallback *callback = nullptr);

	private:
		BrainCloudClient *m_client;

		std::string roleToString(eGroupMember::Role role);
		std::string autoJoinStrategyToString(eAutoJoinStrategy::Strategy strategy);
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
