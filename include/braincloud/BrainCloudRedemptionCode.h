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

    class BrainCloudRedemptionCode
    {
    public:
        BrainCloudRedemptionCode(BrainCloudClient *client);

        /**
         * Redeem a code.
         *
         * Service Name - RedemptionCode
         * Service Operation - REDEEM_CODE
         *
         * @param scanCode The code to redeem
         * @param codeType The type of code
         * @param jsonCustomRedemptionInfo Optional - A JSON string containing custom redemption data
         * @param callback The method to be invoked when the server response is received
         */
        void redeemCode(const char *scanCode, const char *codeType, const char *jsonCustomRedemptionInfo, IServerCallback *callback = NULL);

        /**
         * Retrieve the codes already redeemed by player.
         *
         * Service Name - RedemptionCode
         * Service Operation - GET_REDEEMED_CODES
         *
         * @param codeType Optional - The type of codes to retrieve. Returns all codes if left unspecified.
         * @param callback The method to be invoked when the server response is received
         */
        void getRedeemedCodes(const char *codeType, IServerCallback *callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
