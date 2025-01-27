// Copyright 2016 bitHeads, Inc. All Rights Reserved.

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
		BrainCloudMail(BrainCloudClient* in_client);

		/**
		 * Sends a simple text email to the specified player
		 *
		 * Service Name - mail
		 * Service Operation - SEND_BASIC_EMAIL
		 *
		 * @param in_profileId The user to send the email to
		 * @param in_subject The email subject
		 * @param in_body The email body
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void sendBasicEmail(const char * in_profileId, const char * in_subject, const char * in_body, IServerCallback * in_callback = NULL);

		/**
		 * Sends an advanced email to the specified player
		 *
		 * Service Name - mail
		 * Service Operation - SEND_ADVANCED_EMAIL
		 *
		 * @param in_profileId The user to send the email to
		 * @param in_jsonServiceParams Parameters to send to the email service. See the documentation for
		 *	a full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void sendAdvancedEmail(const char * in_profileId, const std::string & in_jsonServiceParams, IServerCallback * in_callback = NULL);

		/**
		 * Sends an advanced email to the specified email address
		 *
		 * Service Name - mail
		 * Service Operation - SEND_ADVANCED_EMAIL_BY_ADDRESS
		 *
		 * @param in_emailAddress The address to send the email to
		 * @param in_jsonServiceParams Parameters to send to the email service. See the documentation for
		 *	a full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void sendAdvancedEmailByAddress(const char * in_emailAddress, const std::string & in_jsonServiceParams, IServerCallback * in_callback = NULL);

		/**
		 * Sends an advanced email to the specified email addresses.
		 *
		 * Service Name - Mail
		 * Service Operation - SEND_ADVANCED_EMAIL_BY_ADDRESSES
		 *
		 * @param in_emailAddress The list of addresses to send the email to
		 * @param in_serviceParams Set of parameters dependant on the mail service configured
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void sendAdvancedEmailByAddresses(const std::vector<std::string> &in_emailAddresses, const std::string &in_serviceParams, IServerCallback *in_callback = NULL);

	private:
		BrainCloudClient * m_client;
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
