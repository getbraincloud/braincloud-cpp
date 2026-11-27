#pragma once

#include <cstddef>
#include <cstdint>

namespace BrainCloud
{
    /**
     * Signs request payloads for one app.
     */
    class IAppProfile
    {
    public:
        /** The number of characters in a signature, not counting the terminating null. */
        static const size_t SignatureLength = 32;

        virtual ~IAppProfile() {}

        /**
         * Signs a request payload.
         *
         * @param payload The request payload
         * @param length The length of the payload in bytes
         * @param signatureOut Receives SignatureLength uppercase hex characters and a terminating null
         * @return True if the payload was signed
         */
        virtual bool sign(const uint8_t* payload, size_t length, char* signatureOut) const = 0;
    };
}
