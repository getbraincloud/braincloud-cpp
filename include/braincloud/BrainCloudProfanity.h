// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>

namespace BrainCloud
{
    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudProfanity
    {
    public:
        BrainCloudProfanity(BrainCloudClient* client);

        /**
         * Checks supplied text for profanity.
         *
         * Service Name - profanity
         * Service Operation - PROFANITY_CHECK
         *
         * @param text The text to check
         * @param languages Optional comma delimited list of two character language codes
         * @param flagEmail Optional processing of email addresses
         * @param flagPhone Optional processing of phone numbers
         * @param flagUrls Optional processing of urls
         * @param callback The method to be invoked when the server response is received
         *
         * Significant error codes:
         *
         * 40421 - WebPurify not configured
         * 40422 - General exception occurred
         * 40423 - WebPurify returned an error (Http status != 200)
         * 40424 - WebPurify not enabled
         */
        void profanityCheck(const char * text,
            const char * languages,
            bool flagEmail,
            bool flagPhone,
            bool flagUrls,
            IServerCallback * callback = NULL);


        /**
         * Replaces the characters of profanity text with a passed character(s).
         *
         * Service Name - profanity
         * Service Operation - PROFANITY_REPLACE_TEXT
         *
         * @param text The text to check
         * @param replaceSymbol The text to replace individual characters of profanity text with
         * @param languages Optional comma delimited list of two character language codes
         * @param flagEmail Optional processing of email addresses
         * @param flagPhone Optional processing of phone numbers
         * @param flagUrls Optional processing of urls
         * @param callback The method to be invoked when the server response is received
         *
         * Significant error codes:
         *
         * 40421 - WebPurify not configured
         * 40422 - General exception occurred
         * 40423 - WebPurify returned an error (Http status != 200)
         * 40424 - WebPurify not enabled
         */
        void profanityReplaceText(const char * text,
            const char * replaceSymbol,
            const char * languages,
            bool flagEmail,
            bool flagPhone,
            bool flagUrls,
            IServerCallback * callback = NULL);


        /**
         * Checks supplied text for profanity and returns a list of bad wors.
         *
         * Service Name - profanity
         * Service Operation - PROFANITY_IDENTIFY_BAD_WORDS
         *
         * @param text The text to check
         * @param languages Optional comma delimited list of two character language codes
         * @param flagEmail Optional processing of email addresses
         * @param flagPhone Optional processing of phone numbers
         * @param flagUrls Optional processing of urls
         * @param callback The method to be invoked when the server response is received
         *
         * Significant error codes:
         *
         * 40421 - WebPurify not configured
         * 40422 - General exception occurred
         * 40423 - WebPurify returned an error (Http status != 200)
         * 40424 - WebPurify not enabled
         */
        void profanityIdentifyBadWords(const char * text,
            const char * languages,
            bool flagEmail,
            bool flagPhone,
            bool flagUrls,
            IServerCallback * callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
