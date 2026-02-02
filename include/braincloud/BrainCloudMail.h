// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include <string>
#include <vector>

#include "braincloud/IServerCallback.h"

namespace BrainCloud
{
	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudMail
	{
	public:
		BrainCloudMail(BrainCloudClient *client);

		/**
		 * Sends a simple text email to the specified player
		 *
		 * Service Name - mail
		 * Service Operation - SEND_BASIC_EMAIL
		 *
		 * @param profileId The user to send the email to
		 * @param subject The email subject
		 * @param body The email body
		 * @param callback The method to be invoked when the server response is received
		 */
		void sendBasicEmail(const char *profileId, const char *subject, const char *body, IServerCallback *callback = NULL);

		/**
		 * Sends an advanced email to the specified player
		 *
		 * Service Name - mail
		 * Service Operation - SEND_ADVANCED_EMAIL
		 *
		 * @param profileId The user to send the email to
		 * @param jsonServiceParams Parameters to send to the email service. See the documentation for
		 *	a full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param callback The method to be invoked when the server response is received
		 */
		void sendAdvancedEmail(const char *profileId, const std::string &jsonServiceParams, IServerCallback *callback = NULL);

		/**
		 * Sends an advanced email to the specified email address
		 *
		 * Service Name - mail
		 * Service Operation - SEND_ADVANCED_EMAIL_BY_ADDRESS
		 *
		 * @param emailAddress The address to send the email to
		 * @param jsonServiceParams Parameters to send to the email service. See the documentation for
		 *	a full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param callback The method to be invoked when the server response is received
		 */
		void sendAdvancedEmailByAddress(const char *emailAddress, const std::string &jsonServiceParams, IServerCallback *callback = NULL);

		/**
		 * Sends an advanced email to the specified email addresses.
		 *
		 * Service Name - mail
		 * Service Operation - SEND_ADVANCED_EMAIL_BY_ADDRESSES
		 *
		 * @param emailAddress The list of addresses to send the email to
		 * @param serviceParams Set of parameters dependant on the mail service configured
		 * @param callback The method to be invoked when the server response is received
		 */
		void sendAdvancedEmailByAddresses(const std::vector<std::string> &emailAddresses, const std::string &serviceParams, IServerCallback *callback = NULL);

	private:
		BrainCloudClient *m_client;
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
