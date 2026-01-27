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
		BrainCloudUserItems(BrainCloudClient *client);

		/**
		 * Awards item(s) to a user without collecting the purchase amount.
		 * If includeDef is true, response includes associated itemDef
		 * with language fields limited to the current or default language.
		 *
		 * Service Name - userItems
		 * Service Operation - AWARD_USER_ITEM
		 *
		 * @param defId       The unique id of the item definition to award.
		 * @param quantity    The quantity of the item to award.
		 * @param includeDef  If true, include associated item definition in the response.
		 * @param callback The method to be invoked when the server response is received
		 */
		void awardUserItem(const std::string &defId, int32_t quantity, bool includeDef, IServerCallback *callback = nullptr);

		/**
		 * Awards item(s) to a user with additional options.
		 *
		 * Service Name - userItems
		 * Service Operation - AWARD_USER_ITEM
		 *
		 * @param defId       The unique id of the item definition to award.
		 * @param quantity    The quantity of the item to award.
		 * @param includeDef  If true, include associated item definition in the response.
		 * @param optionsJson JSON string specifying additional options (e.g., blockIfExceedItemMaxStackable).
		 * @param callback The method to be invoked when the server response is received
		 */
		void awardUserItemWithOptions(const std::string &defId, int32_t quantity, bool includeDef, const std::string &optionsJson, IServerCallback *callback = nullptr);

		/**
		 * Drops a quantity of a specified user item without recovering the purchase cost.
		 * If any quantity remains, it may include the associated itemDef.
		 *
		 * Service Name - userItems
		 * Service Operation - DROP_USER_ITEM
		 *
		 * @param defId       The unique id of the item definition to drop.
		 * @param quantity    The quantity of the item to drop.
		 * @param includeDef  If true, include associated item definition in the response.
		 * @param callback The method to be invoked when the server response is received
		 */
		void dropUserItem(const std::string &defId, int32_t quantity, bool includeDef, IServerCallback *callback = nullptr);

		/**
		 * Retrieves a page of the user's inventory.
		 *
		 * Service Name - userItems
		 * Service Operation - GET_USER_INVENTORY_PAGE
		 *
		 * @param context     Context string used to filter inventory.
		 * @param includeDef  If true, include associated item definitions in the response.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getUserItemsPage(const std::string &context, bool includeDef, IServerCallback *callback = nullptr);

		/**
		 * Retrieves a page of the user's inventory with an offset.
		 *
		 * Service Name - userItems
		 * Service Operation - GET_USER_INVENTORY_PAGE_OFFSET
		 *
		 * @param context     Context string used to filter inventory.
		 * @param pageOffset  Page offset to retrieve.
		 * @param includeDef  If true, include associated item definitions in the response.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getUserItemsPageOffset(const std::string &context, int32_t pageOffset, bool includeDef, IServerCallback *callback = nullptr);

		/**
		 * Retrieves a specific user item.
		 *
		 * Service Name - userItems
		 * Service Operation - GET_USER_ITEM
		 *
		 * @param itemId      ID of the user item to retrieve.
		 * @param includeDef  If true, include associated item definition in the response.
		 * @param callback The method to be invoked when the server response is received
		 */
		void getUserItem(const std::string &itemId, bool includeDef, IServerCallback *callback = nullptr);

		/**
		 * Gifts an item to another user.
		 *
		 * Service Name - userItems
		 * Service Operation - GIVE_USER_ITEM_TO
		 *
		 * @param profileId   Profile ID of the recipient.
		 * @param itemId      ID of the item to gift.
		 * @param version     Version of the item being gifted.
		 * @param quantity    Quantity of the item to gift.
		 * @param immediate   If true, the gift is delivered immediately.
		 * @param callback The method to be invoked when the server response is received
		 */
		void giveUserItemTo(const std::string &profileId, const std::string &itemId, int32_t version, int32_t quantity, bool immediate, IServerCallback *callback = nullptr);

		/**
		 * Purchases a user item from a store.
		 *
		 * Service Name - userItems
		 * Service Operation - PURCHASE_USER_ITEM
		 *
		 * @param defId       The unique id of the item definition to purchase.
		 * @param quantity    Quantity of the item to purchase.
		 * @param shopId      Store ID for the purchase.
		 * @param includeDef  If true, include associated item definition in the response.
		 * @param callback The method to be invoked when the server response is received
		 */
		void purchaseUserItem(const std::string &defId, int32_t quantity, const std::string &shopId, bool includeDef, IServerCallback *callback = nullptr);

		/**
		 * Purchases a user item with options.
		 *
		 * Service Name - userItems
		 * Service Operation - PURCHASE_USER_ITEM
		 *
		 * @param defId       The unique id of the item definition to purchase.
		 * @param quantity    Quantity of the item to purchase.
		 * @param shopId      Store ID for the purchase.
		 * @param includeDef  If true, include associated item definition in the response.
		 * @param optionsJson JSON string specifying additional options.
		 * @param callback The method to be invoked when the server response is received
		 */
		void purchaseUserItemsWithOptions(const std::string &defId, int32_t quantity, const std::string &shopId, bool includeDef, const std::string &optionsJson, IServerCallback *callback = nullptr);

		/**
		 * Retrieves and transfers a gift item from another user.
		 *
		 * Service Name - userItems
		 * Service Operation - RECEIVE_USER_ITEM_FROM
		 *
		 * @param profileId   Profile ID of the sender.
		 * @param itemId      ID of the item being received.
		 * @param callback The method to be invoked when the server response is received
		 */
		void receiveUserItemFrom(const std::string &profileId, const std::string &itemId, IServerCallback *callback = nullptr);

		/**
		 * Sells a user item back to the store.
		 *
		 * Service Name - userItems
		 * Service Operation - SELL_USER_ITEM
		 *
		 * @param itemId      ID of the user item to sell.
		 * @param version     Version of the item being sold.
		 * @param quantity    Quantity of the item to sell.
		 * @param shopId      Store ID for the sale.
		 * @param includeDef  If true, include associated item definition in the response.
		 * @param callback The method to be invoked when the server response is received
		 */
		void sellUserItem(const std::string &itemId, int32_t version, int32_t quantity, const std::string &shopId, bool includeDef, IServerCallback *callback = nullptr);

		/**
		 * Updates the data of a specific user item.
		 *
		 * Service Name - userItems
		 * Service Operation - UPDATE_USER_ITEM_DATA
		 *
		 * @param itemId      ID of the user item to update.
		 * @param version     Version of the item being updated.
		 * @param newItemData JSON string with updated item data.
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateUserItemData(const std::string &itemId, int32_t version, const std::string &newItemData, IServerCallback *callback = nullptr);

		/**
		 * Uses a user item, potentially consuming it.
		 *
		 * Service Name - userItems
		 * Service Operation - USE_USER_ITEM
		 *
		 * @param itemId      ID of the user item to use.
		 * @param version     Version of the user item (pass -1 for any version).
		 * @param newItemData Optional JSON string to update item fields.
		 * @param includeDef  If true, include associated item definition in the response.
		 * @param callback The method to be invoked when the server response is received
		 */
		void useUserItem(const std::string &itemId, int32_t version, const std::string &newItemData, bool includeDef, IServerCallback *callback = nullptr);

		/**
		 * Publishes a user item to the blockchain.
		 *
		 * Service Name - userItems
		 * Service Operation - PUBLISH_USER_ITEM_TO_BLOCKCHAIN
		 *
		 * @param itemId      ID of the user item to publish.
		 * @param version     Version of the item to publish.
		 * @param callback The method to be invoked when the server response is received
		 */
		void publishUserItemToBlockchain(const std::string &itemId, int32_t version, IServerCallback *callback = nullptr);

		/**
		 * Refreshes blockchain user items.
		 *
		 * Service Name - userItems
		 * Service Operation - REFRESH_BLOCKCHAUSER_ITEMS
		 *
		 * @param callback Callback invoked when the server response is received.
		 */
		void refreshBlockchainUserItems(IServerCallback *callback = nullptr);

		/**
		 * Removes a user item from the blockchain.
		 *
		 * Service Name - userItems
		 * Service Operation - REMOVE_USER_ITEM_FROM_BLOCKCHAIN
		 *
		 * @param itemId   ID of the user item to remove.
		 * @param version  Version of the user item to remove.
		 * @param callback Callback invoked when the server response is received.
		 */
		void removeUserItemFromBlockchain(const std::string &itemId, int32_t version, IServerCallback *callback = nullptr);

		/**
		 * Returns a list of promotional details for a specified item.
		 *
		 * Service Name - userItems
		 * Service Operation - GET_ITEM_PROMOTION_DETAILS
		 *
		 * @param defId                    Item definition ID.
		 * @param shopId                   Store ID.
		 * @param includeDef               Include associated item definition if true.
		 * @param includePromotionDetails  Include promotion details if true.
		 * @param callback                 Callback invoked when the server response is received.
		 */
		void getItemPromotionDetails(const std::string &defId, const std::string &shopId, bool includeDef, bool includePromotionDetails, IServerCallback *callback = nullptr);

		/**
		 * Returns a list of items on promotion available to the current user.
		 *
		 * Service Name - userItems
		 * Service Operation - GET_ITEMS_ON_PROMOTION
		 *
		 * @param shopId                  Store ID.
		 * @param includeDef              Include associated item definition if true.
		 * @param includePromotionDetails Include promotion details if true.
		 * @param optionsJson             JSON string specifying additional options (e.g., category).
		 * @param callback                Callback invoked when the server response is received.
		 */
		void getItemsOnPromotion(const std::string &shopId, bool includeDef, bool includePromotionDetails, const std::string &optionsJson, IServerCallback *callback = nullptr);

		/**
		 * Opens a quantity of a bundle user item.
		 * Creates applicable items and awards any currencies.
		 *
		 * Service Name - userItems
		 * Service Operation - OPEN_BUNDLE
		 *
		 * @param itemId      ID of the bundle item to open.
		 * @param version     Version of the bundle item (pass -1 for any version).
		 * @param quantity    Quantity of the item to open.
		 * @param includeDef  Include associated item definitions if true.
		 * @param optionsJson JSON string specifying additional options.
		 * @param callback The method to be invoked when the server response is received
		 */
		void openBundle(const std::string &itemId, int32_t version, int32_t quantity, bool includeDef, const std::string &optionsJson, IServerCallback *callback = nullptr);

	private:
		BrainCloudClient *m_client;
	};
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
