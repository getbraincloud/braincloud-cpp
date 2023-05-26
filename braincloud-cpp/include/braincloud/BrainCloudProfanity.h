// Copyright 2016 bitHeads, Inc. All Rights Reserved.

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
        BrainCloudProfanity(BrainCloudClient* in_client);

        /**
         * Checks supplied text for profanity.
         *
         * Service Name - Profanity
         * Service Operation - ProfanityCheck
         *
         * @param in_text The text to check
         * @param in_languages Optional comma delimited list of two character language codes
         * @param in_flagEmail Optional processing of email addresses
         * @param in_flagPhone Optional processing of phone numbers
         * @param in_flagUrls Optional processing of urls
         * @param in_callback The method to be invoked when the server response is received
         *
         * Significant error codes:
         *
         * 40421 - WebPurify not configured
         * 40422 - General exception occurred
         * 40423 - WebPurify returned an error (Http status != 200)
         * 40424 - WebPurify not enabled
         */
        void profanityCheck(const char * in_text,
            const char * in_languages,
            bool in_flagEmail,
            bool in_flagPhone,
            bool in_flagUrls,
            IServerCallback * in_callback = NULL);


        /**
         * Replaces the characters of profanity text with a passed character(s).
         *
         * Service Name - Profanity
         * Service Operation - ProfanityReplaceText
         *
         * @param in_text The text to check
         * @param in_replaceSymbol The text to replace individual characters of profanity text with
         * @param in_languages Optional comma delimited list of two character language codes
         * @param in_flagEmail Optional processing of email addresses
         * @param in_flagPhone Optional processing of phone numbers
         * @param in_flagUrls Optional processing of urls
         * @param in_callback The method to be invoked when the server response is received
         *
         * Significant error codes:
         *
         * 40421 - WebPurify not configured
         * 40422 - General exception occurred
         * 40423 - WebPurify returned an error (Http status != 200)
         * 40424 - WebPurify not enabled
         */
        void profanityReplaceText(const char * in_text,
            const char * in_replaceSymbol,
            const char * in_languages,
            bool in_flagEmail,
            bool in_flagPhone,
            bool in_flagUrls,
            IServerCallback * in_callback = NULL);


        /**
         * Checks supplied text for profanity and returns a list of bad wors.
         *
         * Service Name - Profanity
         * Service Operation - ProfanityIdentifyBadWords
         *
         * @param in_text The text to check
         * @param in_languages Optional comma delimited list of two character language codes
         * @param in_flagEmail Optional processing of email addresses
         * @param in_flagPhone Optional processing of phone numbers
         * @param in_flagUrls Optional processing of urls
         * @param in_callback The method to be invoked when the server response is received
         *
         * Significant error codes:
         *
         * 40421 - WebPurify not configured
         * 40422 - General exception occurred
         * 40423 - WebPurify returned an error (Http status != 200)
         * 40424 - WebPurify not enabled
         */
        void profanityIdentifyBadWords(const char * in_text,
            const char * in_languages,
            bool in_flagEmail,
            bool in_flagPhone,
            bool in_flagUrls,
            IServerCallback * in_callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
