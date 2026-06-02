// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include "braincloud/BrainCloudTypes.h"

#include <string>
#include <vector>

namespace BrainCloud
{
	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudMessaging
	{
	public:
		BrainCloudMessaging(BrainCloudClient* client);

		/**
		 * Deletes specified user messages on the server.
		 *
		 * Service Name - Messaging
		 * Service Operation - DELETE_MESSAGES
		 *
		 * @param msgIds Arrays of message ids to delete.
		 * @param callback The method to be invoked when the server response is received
		 */
		void deleteMessages(const std::string& msgbox, const std::vector<std::string>& msgIds, IServerCallback* callback = NULL);

		/**
		 * Retrieve user's message boxes, including 'inbox', 'sent', etc.
		 *
		 * Service Name - Messaging
		 * Service Operation - GET_MESSAGE_BOXES
		 *
		 * @param callback The method to be invoked when the server response is received
		 */	
		void getMessageboxes(IServerCallback* callback = NULL);

		/**
		 * Retrieve user's message boxes, including 'inbox', 'sent', etc.
		 *
		 * Service Name - Messaging
		 * Service Operation - GET_MESSAGE_COUNTS
		 *
		 * @param callback The method to be invoked when the server response is received
		 */	
		void getMessageCounts(IServerCallback* callback = NULL);

		/**
		 * Retrieves list of specified messages.
		 *
		 * Service Name - Messaging
		 * Service Operation - GET_MESSAGES
		 *
		 * @param msgIds Arrays of message ids to get.
		 * @param markAsRead mark messages that are read
		 * @param callback The method to be invoked when the server response is received
		 */
		void getMessages(const std::string& msgbox, const std::vector<std::string>& msgIds, bool markAsRead, IServerCallback* callback = NULL);

		/**
		 * Retrieves a page of messages.
		 *
		 * Service Name - Messaging
		 * Service Operation - GET_MESSAGES_PAGE
		 *
		 * @param context
		 * @param callback The method to be invoked when the server response is received
		 */
		void getMessagesPage(const std::string& context, IServerCallback* callback = NULL);

		/**
		 * Gets the page of messages from the server based on the encoded context and specified page offset.
		 *
		 * Service Name - Messaging
		 * Service Operation - GET_MESSAGES_PAGE_OFFSET
		 *
		 * @param context
		 * @param pageOffset
		 * @param callback The method to be invoked when the server response is received
		 */
		void getMessagesPageOffset(const std::string& context, int pageOffset, IServerCallback* callback = NULL);

		/**
		 * Sends a message with specified 'subject' and 'text' to list of users.
		 *
		 * Service Name - Messaging
		 * Service Operation - SEND_MESSAGE
		 *
		 * @param toProfileIds
		 * @param contentJson the message you are sending
		 * @param callback The method to be invoked when the server response is received
		 */
		void sendMessage(const std::vector<std::string>& toProfileIds, std::string contentJson, IServerCallback* callback = NULL);

		/**
		 * Sends a simple message to specified list of users.
		 *
		 * Service Name - Messaging
		 * Service Operation - SEND_MESSAGE_SIMPLE
		 *
		 * @param toProfileIds
		 * @param messageText
		 * @param callback The method to be invoked when the server response is received
		 */
		void sendMessageSimple(const std::vector<std::string>& toProfileIds, const std::string& messageText, IServerCallback* callback = NULL);

		/**
		 * Marks list of user messages as read on the server.
		 *
		 * Service Name - Messaging
		 * Service Operation - MARK_MESSAGES_READ
		 *
		 * @param msgbox
		 * @param msgIds
		 * @param callback The method to be invoked when the server response is received
		 */
		void markMessagesRead(const std::string& msgbox, const std::vector<std::string>& msgIds, IServerCallback* callback = NULL);

	private:
		BrainCloudClient* m_client;
	};
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
