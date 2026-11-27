#include "braincloud/internal/HeldAppProfile.h"
#include "braincloud/internal/SecureMemory.h"
#include "braincloud/internal/SecureRandom.h"

#if defined(BC_UWP)
#include "braincloud/internal/win/MD5.h"
#include <algorithm>
#include <cctype>
#else
#include "braincloud/internal/md5x.h"
#endif

#include <cstring>

namespace BrainCloud
{
    HeldAppProfile::HeldAppProfile(const char* value, size_t length)
        : _a(value != NULL ? length : 0)
        , _b(value != NULL ? length : 0)
        , _valid(false)
    {
        if (_b.empty() || !secureRandomBytes(&_a[0], _a.size()))
            return;
        for (size_t i = 0; i < _b.size(); ++i)
            _b[i] = static_cast<uint8_t>(value[i]) ^ _a[i];
        _valid = true;
    }

    HeldAppProfile::HeldAppProfile(const char* value)
        : HeldAppProfile(value, value != NULL ? strlen(value) : 0)
    {
    }

    HeldAppProfile::HeldAppProfile(const std::string& value)
        : HeldAppProfile(value.data(), value.size())
    {
    }

    HeldAppProfile::~HeldAppProfile()
    {
        secureWipe(_a);
        secureWipe(_b);
    }

    bool HeldAppProfile::sign(const uint8_t* payload, size_t length, char* signatureOut) const
    {
        if (!_valid)
            return false;
        std::vector<uint8_t> v(_b.size());
        for (size_t i = 0; i < _b.size(); ++i)
            v[i] = _b[i] ^ _a[i];
        computeSignature(payload, length, &v[0], v.size(), signatureOut);
        secureWipe(v);
        return true;
    }

    void computeSignature(const uint8_t* payload, size_t length, const uint8_t* value, size_t valueLength, char* signatureOut)
    {
#if defined(BC_UWP)
        MD5 md5;
        md5.update(payload, static_cast<MD5::size_type>(length));
        md5.update(value, static_cast<MD5::size_type>(valueLength));
        std::string hex = md5.finalize().hexdigest();
        std::transform(hex.begin(), hex.end(), hex.begin(), ::toupper);
        memcpy(signatureOut, hex.c_str(), IAppProfile::SignatureLength);
#else
        md5_state_t state;
        md5_byte_t digest[16];
        md5_init(&state);
        md5_append(&state, payload, static_cast<int>(length));
        md5_append(&state, value, static_cast<int>(valueLength));
        md5_finish(&state, digest);
        secureWipe(&state, sizeof(state));

        static const char* const hex = "0123456789ABCDEF";
        for (int i = 0; i < 16; ++i)
        {
            signatureOut[i * 2] = hex[digest[i] >> 4];
            signatureOut[i * 2 + 1] = hex[digest[i] & 0x0F];
        }
#endif
        signatureOut[IAppProfile::SignatureLength] = '\0';
    }
}
