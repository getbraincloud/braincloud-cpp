//
//  IRelayCallback.h
//  GameClientLib
//
//  Created by David St-Louis on 2020-02-12.
//

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <cinttypes>

namespace BrainCloud
{
    class IRelayCallback
    {
    public:
        virtual ~IRelayCallback() { }

        /**
         * Relay message from the Relay Service
         *
         * @param netId - Net Id of the member this message is from.
         * @param bytes - Data received
         * @param size - Size of data in bytes
         */
        virtual void relayCallback(int netId, const uint8_t* bytes, int size) = 0;
    };
};
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
