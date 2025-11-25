#ifndef _LINUXPINGER_H_
#define _LINUXPINGER_H_

#include "braincloud/internal/IPinger.h"
#include <string>

namespace BrainCloud
{
    class LinuxPinger final : public IPinger
    {
    public:
        int ping(const std::string& url) override;

    protected:
        friend class IPinger;
        LinuxPinger() = default;
    };
}

#endif
