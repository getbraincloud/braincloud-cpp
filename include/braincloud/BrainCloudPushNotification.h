// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>
#include <vector>
#include "braincloud/BrainCloudTypes.h"

#include "braincloud/Platform.h"

namespace BrainCloud
{
    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudPushNotification
    {
    public:
        BrainCloudPushNotification(BrainCloudClient* client);

        /**
         * Deregisters all device tokens currently registered to the user.
         *
         * Service Name - pushNotification
         * Service Operation - DEREGISTER_ALL
         *
         * @param callback The method to be invoked when the server response is received
         */
        void deregisterAllPushNotificationDeviceTokens(IServerCallback * callback = NULL);

        /**
         * Deregisters the given device token from the server to disable this device
         * from receiving push notifications.
         *
         * @param device The device platform being deregistered.
         * @param token The platform-dependent device token needed for push notifications.
         * @param callback The method to be invoked when the server response is received
         */
        void deregisterPushNotificationDeviceToken(const Platform & platform, const char * token, IServerCallback * callback = NULL);

        /**
         * Registers the given device token with the server to enable this device
         * to receive push notifications.
         *
         * @param platform The device platform
         * @param deviceToken The platform-dependent device token needed for push notifications.
         *  On IOS, this is obtained using the application:didRegisterForRemoteNotificationsWithDeviceToken callback
         * @param callback The method to be invoked when the server response is received
         */
        void registerPushNotificationDeviceToken(const Platform & platform, const char * deviceToken, IServerCallback * callback = NULL);
		
        /**
         * Sends a simple push notification based on the passed in message.
         * NOTE: It is possible to send a push notification to oneself.
         *
         * @param toProfileId The braincloud profileId of the user to receive the notification
         * @param message Text of the push notification
         * @param callback The method to be invoked when the server response is received
         */
        void sendSimplePushNotification(const char * toProfileId, const char * message, IServerCallback * callback = NULL);

        /**
         * Sends a notification to a user based on a brainCloud portal configured notification template.
         * NOTE: It is possible to send a push notification to oneself.
         *
         * @param toProfileId The braincloud profileId of the user to receive the notification
         * @param notificationTemplateId Id of the notification template
         * @param callback The method to be invoked when the server response is received
         */
        void sendRichPushNotification(const char * toProfileId, int32_t notificationTemplateId, IServerCallback * callback = NULL);

        /**
         * Sends a notification to a user based on a brainCloud portal configured notification template.
         * Includes JSON defining the substitution params to use with the template.
         * See the Portal documentation for more info.
         * NOTE: It is possible to send a push notification to oneself.
         *
         * @param toProfileId The braincloud profileId of the user to receive the notification
         * @param notificationTemplateId Id of the notification template
         * @param substitutionJson JSON defining the substitution params to use with the template
         * @param callback The method to be invoked when the server response is received
         */
        void sendRichPushNotificationWithParams(const char * toProfileId, int32_t notificationTemplateId, const char * substitutionJson, IServerCallback * callback = NULL);

        /**
        * Sends a notification to a "group" of user based on a brainCloud portal configured notification template.
        * Includes JSON defining the substitution params to use with the template.
        * See the Portal documentation for more info.
        *
        * @param groupId Target group
        * @param notificationTemplateId Template to use
        * @param substitutionsJson Map of substitution positions to strings
        * @param callback The method to be invoked when the server response is received
        */
        void sendTemplatedPushNotificationToGroup(const char * groupId, int32_t notificationTemplateId, std::string substitutionsJson, IServerCallback * callback = NULL);

        /**
        * Sends a notification to a "group" of user consisting of alert content and custom data.
        * See the Portal documentation for more info.
        *
        * @param groupId Target group
        * @param alertContentJson Body and title of alert
        * @param customDataJson Optional custom data
        * @param callback The method to be invoked when the server response is received
        */
        void sendNormalizedPushNotificationToGroup(const char * groupId, std::string alertContentJson, std::string customDataJson, IServerCallback * callback = NULL);

		/**
     	* Schedules a normalized push notification to a user
     	*
     	* @param profileId The profileId of the user to receive the notification
     	* @param fcmContent Valid Fcm data content
     	* @param iosContent Valid ios data content
     	* @param facebookContent Facebook template string
     	* @param startTimeUTC Start time of sending the push notification in milliseconds, use UTC time in milliseconds since epoch
     	* @param callback The method to be invoked when the server response is received
     	*/
		void scheduleRawPushNotificationUTC(const char * profileId, std::string fcmContent, std::string iosContent, std::string facebookContent, int64_t startTimeUTC, IServerCallback * callback = NULL);

		/**
     	* Schedules a normalized push notification to a user
     	*
     	* @param profileId The profileId of the user to receive the notification
     	* @param fcmContent Valid Fcm data content
     	* @param iosContent Valid ios data content
     	* @param facebookContent Facebook template string
     	* @param minutesFromNow Minutes from now to send the push notification
		* @param callback The method to be invoked when the server response is received
     	*/
		void scheduleRawPushNotificationMinutes(const char * profileId, std::string fcmContent, std::string iosContent, std::string facebookContent, int32_t minutesFromNow, IServerCallback * callback = NULL);

		/**
     	* Sends a raw push notification to a target user.
     	*
     	* @param toProfileId The profileId of the user to receive the notification
 		* @param fcmContent Valid Fcm data content
 		* @param iosContent Valid ios data content
 		* @param facebookContent Facebook template string
 		* @param callback The method to be invoked when the server response is received
     	*/
		void sendRawPushNotification(const char * profileId, std::string fcmContent, std::string iosContent, std::string facebookContent, IServerCallback * callback = NULL);

		/**
     	* Sends a raw push notification to a target list of users.
     	*
     	* @param profileIds Collection of profile IDs to send the notification to
     	* @param fcmContent Valid Fcm data content
     	* @param iosContent Valid ios data content
     	* @param facebookContent Facebook template string
     	* @param callback The method to be invoked when the server response is received
     	*/
		void sendRawPushNotificationBatch(std::vector<std::string> profileIds, std::string fcmContent, std::string iosContent, std::string facebookContent, IServerCallback * callback = NULL);

		/**
     	* Sends a raw push notification to a target group.
     	*
     	* @param groupId Target group
     	* @param fcmContent Valid Fcm data content
     	* @param iosContent Valid ios data content
     	* @param facebookContent Facebook template stringn
     	* @param callback The method to be invoked when the server response is received
     	*/
		void sendRawPushNotificationToGroup(const char * groupId, std::string fcmContent, std::string iosContent, std::string facebookContent, IServerCallback * callback = NULL);

		/**
		* Schedules a normalized push notification to a user
	    *
		* @param toProfileId The profileId of the user to receive the notification
		* @param alertContentJson Body and title of alert
		* @param customDataJson Optional custom data
		* @param startTimeUTC Start time of sending the push notification in milliseconds, use UTC time in milliseconds since epoch
		* @param callback The method to be invoked when the server response is received
		*/
		void scheduleNormalizedPushNotificationUTC(const char * toProfileId, std::string alertContentJson, std::string customDataJson, int64_t startTimeUTC, IServerCallback * callback = NULL);

		/**
		* Schedules a normalized push notification to a user
		*
		* @param toProfileId The profileId of the user to receive the notification
		* @param alertContentJson Body and title of alert
		* @param customDataJson Optional custom data
		* @param minutesFromNow Minutes from now to send the push notification
		* @param callback The method to be invoked when the server response is received
		*/
		void scheduleNormalizedPushNotificationMinutes(const char * toProfileId, std::string alertContentJson, std::string customDataJson, int32_t minutesFromNow, IServerCallback * callback = NULL);

		/**
		* Schedules a rich push notification to a user
		*
		* @param toProfileId The profileId of the user to receive the notification
		* @param notificationTemplateId Body and title of alert
		* @param substitutionsJson Map of substitution positions to strings
		* @param startTimeUTC Start time of sending the push notification in milliseconds, use UTC time in milliseconds since epoch
		* @param callback The method to be invoked when the server response is received
		*/
		void scheduleRichPushNotificationUTC(const char * toProfileId, int32_t notificationTemplateId, std::string substitutionsJson, int64_t startTimeUTC, IServerCallback * callback = NULL);

		/**
		* Schedules a rich push notification to a user
		*
		* @param toProfileId The profileId of the user to receive the notification
		* @param notificationTemplateId Body and title of alert
		* @param substitutionsJson Map of substitution positions to strings
		* @param minutesFromNow Minutes from now to send the push notification
		* @param callback The method to be invoked when the server response is received
		*/
		void scheduleRichPushNotificationMinutes(const char * toProfileId, int32_t notificationTemplateId, std::string substitutionsJson, int32_t minutesFromNow, IServerCallback * callback = NULL);

		/**
		* Sends a notification to a user consisting of alert content and custom data.
		*
		* @param toProfileId The profileId of the user to receive the notification
		* @param alertContent Body and title of alert
		* @param customData Optional custom data
		* @param callback The method to be invoked when the server response is received
		*/
		void sendNormalizedPushNotification(const char * toProfileId, std::string alertContentJson, std::string customDataJson, IServerCallback * callback = NULL);

		/**
		* Sends a notification to multiple users consisting of alert content and custom data.
		*
		* @param profileIds Collection of profile IDs to send the notification to
		* @param alertContent Body and title of alert
		* @param customData Optional custom data
		* @param callback The method to be invoked when the server response is received
		*/
		void sendNormalizedPushNotificationBatch(std::vector<std::string> profileIds, std::string alertContentJson, std::string customDataJson, IServerCallback * callback = NULL);

    private:
        BrainCloudClient * m_client;
        void sendRichPushNotification(const char * toProifleId, int32_t notificationTemplateId, const char * substitutionJson, IServerCallback * callback = NULL);
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
