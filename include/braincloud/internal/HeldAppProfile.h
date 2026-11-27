#pragma once

#include "braincloud/IAppProfile.h"

#include <string>
#include <vector>

namespace BrainCloud
{
    // Request signer for an app.
    class HeldAppProfile : public IAppProfile
    {
    public:
        HeldAppProfile(const char* value, size_t length);
        explicit HeldAppProfile(const char* value);
        explicit HeldAppProfile(const std::string& value);
        virtual ~HeldAppProfile();

        virtual bool sign(const uint8_t* payload, size_t length, char* signatureOut) const;

        bool isValid() const { return _valid; }

    private:
        HeldAppProfile(const HeldAppProfile&);
        HeldAppProfile& operator=(const HeldAppProfile&);

        std::vector<uint8_t> _a;
        std::vector<uint8_t> _b;
        bool _valid;
    };

    // MD5(payload + value) as uppercase hex, hashing the two one after the other.
    void computeSignature(const uint8_t* payload, size_t length, const uint8_t* value, size_t valueLength, char* signatureOut);
}
