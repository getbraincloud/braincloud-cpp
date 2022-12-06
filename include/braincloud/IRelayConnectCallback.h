//
//  IRelayConnectCallback.h
//  GameClientLib
//
//  Created by David St-Louis on 2020-02-12.
//

#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <string>

namespace BrainCloud {

    class IRelayConnectCallback
    {
    public:
        virtual ~IRelayConnectCallback() { }

        /**
         * Method called when connection to Relay server was successful
         *
         * @param jsonData - System message with connect success packet
         */
        virtual void relayConnectSuccess(const std::string& jsonResponse) = 0;

        /**
         * Method called when connection to relay failed or got disconnected
         *
         * @param errorMessage - Text message describing the error.
         */
        virtual void relayConnectFailure(const std::string& errorMessage) = 0;
    };

};

#pragma clang diagnostic pop
