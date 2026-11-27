#pragma once

#if defined(BC_NATIVE_CONFIG)

#include "braincloud/IAppProfile.h"

#include <map>
#include <memory>
#include <string>

namespace braincloud
{
    class NativeConfig;
}

namespace BrainCloud
{
    // Request signer for an app in braincloud.cfg.
    class NativeConfigProfile : public IAppProfile
    {
    public:
        NativeConfigProfile(const std::shared_ptr<braincloud::NativeConfig>& config, const std::string& appId);

        virtual bool sign(const uint8_t* payload, size_t length, char* signatureOut) const;

    private:
        std::shared_ptr<braincloud::NativeConfig> _config;
        std::string _appId;
    };

    // What a braincloud.cfg holds, ready for BrainCloudClient::initializeWithApps.
    struct NativeAppConfig
    {
        std::string serverUrl;
        std::string appVersion;
        std::string appId;
        std::map<std::string, std::shared_ptr<IAppProfile> > profiles;
    };

    // braincloud.cfg from the working directory, beside the executable or a Mac bundle's Resources;
    // on Android, from the APK assets (needs BrainCloud::appEnv/appContext set).
    bool loadDefaultNativeAppConfig(NativeAppConfig& out);
}

#endif
