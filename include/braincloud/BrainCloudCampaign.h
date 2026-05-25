// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include <string>

namespace BrainCloud {

    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudCampaign
    {
    public:
        BrainCloudCampaign(BrainCloudClient* client);

        /**
         * Returns the list of campaigns the current player is participating in,
         * providing campaign, campaign scenario, and participation details.
         *
         * Service Name - Campaign
         * Service Operation - GET_MY_CAMPAIGNS
         *
         * @param optionsJson Optional parameters as a JSON string (reserved for future use).
         * @param callback The method to be invoked when the server response is received.
         */
        void getMyCampaigns(const std::string& optionsJson, IServerCallback* callback = NULL);

    private:
        BrainCloudClient* m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
