#include "braincloud/internal/SecureRandom.h"

#if defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
#elif defined(__APPLE__)
#include <Security/SecRandom.h>
#elif defined(__ANDROID__)
#include <stdlib.h>
#else
#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <sys/syscall.h>
#endif

namespace BrainCloud
{
    bool secureRandomBytes(uint8_t* out, size_t length)
    {
        if (length == 0)
            return true;
#if defined(_WIN32)
        return BCRYPT_SUCCESS(BCryptGenRandom(NULL, out, static_cast<ULONG>(length), BCRYPT_USE_SYSTEM_PREFERRED_RNG));
#elif defined(__APPLE__)
        return SecRandomCopyBytes(kSecRandomDefault, length, out) == errSecSuccess;
#elif defined(__ANDROID__)
        arc4random_buf(out, length);
        return true;
#else
        // The getrandom syscall directly: older glibc has no wrapper for it.
#if defined(SYS_getrandom)
        size_t filled = 0;
        while (filled < length)
        {
            long n = syscall(SYS_getrandom, out + filled, length - filled, 0);
            if (n < 0)
            {
                if (errno == EINTR)
                    continue;
                break;
            }
            filled += static_cast<size_t>(n);
        }
        if (filled == length)
            return true;
#endif
        FILE* urandom = fopen("/dev/urandom", "rb");
        if (urandom == NULL)
            return false;
        const bool ok = fread(out, 1, length, urandom) == length;
        fclose(urandom);
        return ok;
#endif
    }
}
