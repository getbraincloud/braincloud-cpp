//
//  IRelaySystemCallback.h
//  GameClientLib
//
//  Created by David St-Louis on 2020-02-12.
//

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>

namespace BrainCloud
{
    class IRelaySystemCallback
    {
    public:
        virtual ~IRelaySystemCallback() { }

        /**
         * System message from the Relay Service
         *
         * @param jsonResponse - System message in json string.
         */
        virtual void relaySystemCallback(const std::string& jsonResponse) = 0;
    };
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
