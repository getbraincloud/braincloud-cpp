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

	class BrainCloudUserItems
	{
	public:
		BrainCloudUserItems(BrainCloudClient *in_client);

		/**
		 * Awards item(s) to a user without collecting the purchase amount.
		 * If includeDef is true, response includes associated itemDef
		 * with language fields limited to the current or default language.
		 *
		 * Service Name - userItems
		 * Service Operation - AWARD_USER_ITEM
		 *
		 * @param in_defId       The unique id of the item definition to award.
		 * @param in_quantity    The quantity of the item to award.
		 * @param in_includeDef  If true, include associated item definition in the response.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void awardUserItem(const std::string &in_defId, int32_t in_quantity, bool in_includeDef, IServerCallback *in_callback = nullptr);

		/**
		 * Awards item(s) to a user with additional options.
		 *
		 * Service Name - userItems
		 * Service Operation - AWARD_USER_ITEM
		 *
		 * @param in_defId       The unique id of the item definition to award.
		 * @param in_quantity    The quantity of the item to award.
		 * @param in_includeDef  If true, include associated item definition in the response.
		 * @param in_optionsJson JSON string specifying additional options (e.g., blockIfExceedItemMaxStackable).
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void awardUserItemWithOptions(const std::string &in_defId, int32_t in_quantity, bool in_includeDef, const std::string &in_optionsJson, IServerCallback *in_callback = nullptr);

		/**
		 * Drops a quantity of a specified user item without recovering the purchase cost.
		 * If any quantity remains, it may include the associated itemDef.
		 *
		 * Service Name - userItems
		 * Service Operation - DROP_USER_ITEM
		 *
		 * @param in_defId       The unique id of the item definition to drop.
		 * @param in_quantity    The quantity of the item to drop.
		 * @param in_includeDef  If true, include associated item definition in the response.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void dropUserItem(const std::string &in_defId, int32_t in_quantity, bool in_includeDef, IServerCallback *in_callback = nullptr);

		/**
		 * Retrieves a page of the user's inventory.
		 *
		 * Service Name - userItems
		 * Service Operation - GET_USER_INVENTORY_PAGE
		 *
		 * @param in_context     Context string used to filter inventory.
		 * @param in_includeDef  If true, include associated item definitions in the response.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getUserItemsPage(const std::string &in_context, bool in_includeDef, IServerCallback *in_callback = nullptr);

		/**
		 * Retrieves a page of the user's inventory with an offset.
		 *
		 * Service Name - userItems
		 * Service Operation - GET_USER_INVENTORY_PAGE_OFFSET
		 *
		 * @param in_context     Context string used to filter inventory.
		 * @param in_pageOffset  Page offset to retrieve.
		 * @param in_includeDef  If true, include associated item definitions in the response.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getUserItemsPageOffset(const std::string &in_context, int32_t in_pageOffset, bool in_includeDef, IServerCallback *in_callback = nullptr);

		/**
		 * Retrieves a specific user item.
		 *
		 * Service Name - userItems
		 * Service Operation - GET_USER_ITEM
		 *
		 * @param in_itemId      ID of the user item to retrieve.
		 * @param in_includeDef  If true, include associated item definition in the response.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getUserItem(const std::string &in_itemId, bool in_includeDef, IServerCallback *in_callback = nullptr);

		/**
		 * Gifts an item to another user.
		 *
		 * Service Name - userItems
		 * Service Operation - GIVE_USER_ITEM_TO
		 *
		 * @param in_profileId   Profile ID of the recipient.
		 * @param in_itemId      ID of the item to gift.
		 * @param in_version     Version of the item being gifted.
		 * @param in_quantity    Quantity of the item to gift.
		 * @param in_immediate   If true, the gift is delivered immediately.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void giveUserItemTo(const std::string &in_profileId, const std::string &in_itemId, int32_t in_version, int32_t in_quantity, bool in_immediate, IServerCallback *in_callback = nullptr);

		/**
		 * Purchases a user item from a store.
		 *
		 * Service Name - userItems
		 * Service Operation - PURCHASE_USER_ITEM
		 *
		 * @param in_defId       The unique id of the item definition to purchase.
		 * @param in_quantity    Quantity of the item to purchase.
		 * @param in_shopId      Store ID for the purchase.
		 * @param in_includeDef  If true, include associated item definition in the response.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void purchaseUserItem(const std::string &in_defId, int32_t in_quantity, const std::string &in_shopId, bool in_includeDef, IServerCallback *in_callback = nullptr);

		/**
		 * Purchases a user item with options.
		 *
		 * Service Name - userItems
		 * Service Operation - PURCHASE_USER_ITEM
		 *
		 * @param in_defId       The unique id of the item definition to purchase.
		 * @param in_quantity    Quantity of the item to purchase.
		 * @param in_shopId      Store ID for the purchase.
		 * @param in_includeDef  If true, include associated item definition in the response.
		 * @param in_optionsJson JSON string specifying additional options.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void purchaseUserItemsWithOptions(const std::string &in_defId, int32_t in_quantity, const std::string &in_shopId, bool in_includeDef, const std::string &in_optionsJson, IServerCallback *in_callback = nullptr);

		/**
		 * Retrieves and transfers a gift item from another user.
		 *
		 * Service Name - userItems
		 * Service Operation - RECEIVE_USER_ITEM_FROM
		 *
		 * @param in_profileId   Profile ID of the sender.
		 * @param in_itemId      ID of the item being received.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void receiveUserItemFrom(const std::string &in_profileId, const std::string &in_itemId, IServerCallback *in_callback = nullptr);

		/**
		 * Sells a user item back to the store.
		 *
		 * Service Name - userItems
		 * Service Operation - SELL_USER_ITEM
		 *
		 * @param in_itemId      ID of the user item to sell.
		 * @param in_version     Version of the item being sold.
		 * @param in_quantity    Quantity of the item to sell.
		 * @param in_shopId      Store ID for the sale.
		 * @param in_includeDef  If true, include associated item definition in the response.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void sellUserItem(const std::string &in_itemId, int32_t in_version, int32_t in_quantity, const std::string &in_shopId, bool in_includeDef, IServerCallback *in_callback = nullptr);

		/**
		 * Updates the data of a specific user item.
		 *
		 * Service Name - userItems
		 * Service Operation - UPDATE_USER_ITEM_DATA
		 *
		 * @param in_itemId      ID of the user item to update.
		 * @param in_version     Version of the item being updated.
		 * @param in_newItemData JSON string with updated item data.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void updateUserItemData(const std::string &in_itemId, int32_t in_version, const std::string &in_newItemData, IServerCallback *in_callback = nullptr);

		/**
		 * Uses a user item, potentially consuming it.
		 *
		 * Service Name - userItems
		 * Service Operation - USE_USER_ITEM
		 *
		 * @param in_itemId      ID of the user item to use.
		 * @param in_version     Version of the user item (pass -1 for any version).
		 * @param in_newItemData Optional JSON string to update item fields.
		 * @param in_includeDef  If true, include associated item definition in the response.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void useUserItem(const std::string &in_itemId, int32_t in_version, const std::string &in_newItemData, bool in_includeDef, IServerCallback *in_callback = nullptr);

		/**
		 * Publishes a user item to the blockchain.
		 *
		 * Service Name - userItems
		 * Service Operation - PUBLISH_USER_ITEM_TO_BLOCKCHAIN
		 *
		 * @param in_itemId      ID of the user item to publish.
		 * @param in_version     Version of the item to publish.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void publishUserItemToBlockchain(const std::string &in_itemId, int32_t in_version, IServerCallback *in_callback = nullptr);

		/**
		 * Refreshes blockchain user items.
		 *
		 * Service Name - userItems
		 * Service Operation - REFRESH_BLOCKCHAIN_USER_ITEMS
		 *
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void refreshBlockchainUserItems(IServerCallback *in_callback = nullptr);

		/**
		 * Removes a user item from the blockchain.
		 *
		 * Service Name - userItems
		 * Service Operation - REMOVE_USER_ITEM_FROM_BLOCKCHAIN
		 *
		 * @param in_itemId   ID of the user item to remove.
		 * @param in_version  Version of the user item to remove.
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void removeUserItemFromBlockchain(const std::string &in_itemId, int32_t in_version, IServerCallback *in_callback = nullptr);

		/**
		 * Returns a list of promotional details for a specified item.
		 *
		 * Service Name - userItems
		 * Service Operation - GET_ITEM_PROMOTION_DETAILS
		 *
		 * @param in_defId                    Item definition ID.
		 * @param in_shopId                   Store ID.
		 * @param in_includeDef               Include associated item definition if true.
		 * @param in_includePromotionDetails  Include promotion details if true.
		 * @param in_callback                 Callback invoked when the server response is received.
		 */
		void getItemPromotionDetails(const std::string &in_defId, const std::string &in_shopId, bool in_includeDef, bool in_includePromotionDetails, IServerCallback *in_callback = nullptr);

		/**
		 * Returns a list of items on promotion available to the current user.
		 *
		 * Service Name - userItems
		 * Service Operation - GET_ITEMS_ON_PROMOTION
		 *
		 * @param in_shopId                  Store ID.
		 * @param in_includeDef              Include associated item definition if true.
		 * @param in_includePromotionDetails Include promotion details if true.
		 * @param in_optionsJson             JSON string specifying additional options (e.g., category).
		 * @param in_callback                Callback invoked when the server response is received.
		 */
		void getItemsOnPromotion(const std::string &in_shopId, bool in_includeDef, bool in_includePromotionDetails, const std::string &in_optionsJson, IServerCallback *in_callback = nullptr);

		/**
		 * Opens a quantity of a bundle user item.
		 * Creates applicable items and awards any currencies.
		 *
		 * Service Name - userItems
		 * Service Operation - OPEN_BUNDLE
		 *
		 * @param in_itemId      ID of the bundle item to open.
		 * @param in_version     Version of the bundle item (pass -1 for any version).
		 * @param in_quantity    Quantity of the item to open.
		 * @param in_includeDef  Include associated item definitions if true.
		 * @param in_optionsJson JSON string specifying additional options.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void openBundle(const std::string &in_itemId, int32_t in_version, int32_t in_quantity, bool in_includeDef, const std::string &in_optionsJson, IServerCallback *in_callback = nullptr);

	private:
		BrainCloudClient *m_client;
	};
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
