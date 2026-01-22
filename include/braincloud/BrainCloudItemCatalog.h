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
	class AuthenticationType;

	class BrainCloudItemCatalog
	{
	public:
		BrainCloudItemCatalog(BrainCloudClient *in_client);

		/**
		 * Reads an existing item definition from the server, with language fields
		 * limited to the current or default language.
		 *
		 * Service Name - ItemCatalog
		 * Service Operation - GET_CATALOG_ITEM_DEFINITION
		 *
		 * @param in_defId The identifier of the catalog item definition to retrieve
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getCatalogItemDefinition(const std::string &in_defId, IServerCallback *in_callback = NULL);

		/**
		 * Retrieve a page of catalog items from the server, with language fields
		 * limited to the text for the current or default language.
		 *
		 * Service Name - ItemCatalog
		 * Service Operation - GET_CATALOG_ITEMS_PAGE
		 *
		 * @param in_context The pagination context returned from a previous catalog page request
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getCatalogItemsPage(const std::string &in_context, IServerCallback *in_callback = NULL);

		/**
		 * Gets a page of catalog items from the server based on the encoded
		 * context and specified page offset, with language fields limited to the
		 * text for the current or default language.
		 *
		 * Service Name - ItemCatalog
		 * Service Operation - GET_CATALOG_ITEMS_PAGE_OFFSET
		 *
		 * @param in_context The pagination context returned from a previous catalog page request
		 * @param in_pageOffset The page offset relative to the current context
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getCatalogItemsPageOffset(const std::string &in_context, int32_t in_pageOffset, IServerCallback *in_callback = NULL);

	private:
		BrainCloudClient *m_client;
	};
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
