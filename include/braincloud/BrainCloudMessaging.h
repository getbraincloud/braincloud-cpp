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
		BrainCloudMessaging(BrainCloudClient *in_client);

		/**
		 * Deletes specified user messages on the server.
		 *
		 * Service Name - Messaging
		 * Service Operation - DeleteMessages
		 *
		 * @param in_msgbox The message box to delete from.
		 * @param in_msgIds Arrays of message ids to delete.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void deleteMessages(const std::string &in_msgbox, const std::vector<std::string> &in_msgIds, IServerCallback *in_callback = NULL);

		/**
		 * Retrieve user's message boxes, including 'inbox', 'sent', etc.
		 *
		 * Service Name - Messaging
		 * Service Operation - GetMessageboxes
		 *
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getMessageboxes(IServerCallback *in_callback = NULL);

		/**
		 * Retrieve user's message boxes, including 'inbox', 'sent', etc.
		 *
		 * Service Name - Messaging
		 * Service Operation - GetMessageCounts
		 *
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getMessageCounts(IServerCallback *in_callback = NULL);

		/**
		 * Retrieves list of specified messages.
		 *
		 * Service Name - Messaging
		 * Service Operation - GetMessages
		 *
		 * @param in_msgbox The message box to get messages from.
		 * @param in_msgIds Arrays of message ids to get.
		 * @param markAsRead mark messages that are read
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getMessages(const std::string &in_msgbox, const std::vector<std::string> &in_msgIds, bool markAsRead, IServerCallback *in_callback = NULL);

		/**
		 * Retrieves a page of messages.
		 *
		 * Service Name - Messaging
		 * Service Operation - GetMessagesPage
		 *
		 * @param in_context The context for the page of messages.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getMessagesPage(const std::string &in_context, IServerCallback *in_callback = NULL);

		/**
		 * Gets the page of messages from the server based on the encoded context and specified page offset.
		 *
		 * Service Name - Messaging
		 * Service Operation - GetMessagesPageOffset
		 *
		 * @param in_context The context for the page of messages.
		 * @param pageOffset The page offset.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getMessagesPageOffset(const std::string &in_context, int in_pageOffset, IServerCallback *in_callback = NULL);

		/**
		 * Sends a message with specified 'subject' and 'text' to list of users.
		 *
		 * Service Name - Messaging
		 * Service Operation - SendMessage
		 *
		 * @param toProfileIds The list of profile ids to send the message to.
		 * @param contentJson The message you are sending
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void sendMessage(const std::vector<std::string> &in_toProfileIds, std::string in_contentJson, IServerCallback *in_callback = NULL);

		/**
		 * Sends a simple message to specified list of users.
		 *
		 * Service Name - Messaging
		 * Service Operation - SendMessageSimple
		 *
		 * @param toProfileIds The list of profile ids to send the message to.
		 * @param messageText The message text you are sending
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void sendMessageSimple(const std::vector<std::string> &in_toProfileIds, const std::string &in_messageText, IServerCallback *in_callback = NULL);

		/**
		 * Marks list of user messages as read on the server.
		 *
		 * Service Name - Messaging
		 * Service Operation - MarkMessagesRead
		 *
		 * @param msgbox The message box to mark as read.
		 * @param msgIds Arrays of message ids to mark as read.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void markMessagesRead(const std::string &in_msgbox, const std::vector<std::string> &in_msgIds, IServerCallback *in_callback = NULL);

	private:
		BrainCloudClient *m_client;
	};
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
