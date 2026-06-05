// Copyright 2026 bitHeads, Inc. All Rights Reserved.
#include "braincloud/Platform.h"

namespace BrainCloud
{
    const Platform & Platform::Unknown           = Platform("UNKNOWN");
    const Platform & Platform::Amazon            = Platform("AMAZON");
    const Platform & Platform::AppleTVOS         = Platform("APPLE_TV_OS");
    const Platform & Platform::BlackBerry        = Platform("BB");
    const Platform & Platform::Facebook          = Platform("FB");
    const Platform & Platform::GooglePlayAndroid = Platform("ANG");
    const Platform & Platform::iOS               = Platform("IOS");
    const Platform & Platform::Linux             = Platform("LINUX");
    const Platform & Platform::Mac               = Platform("MAC");
    const Platform & Platform::Nintendo          = Platform("NINTENDO");
    const Platform & Platform::Oculus            = Platform("OCULUS");
    const Platform & Platform::PS3               = Platform("PS3");
    const Platform & Platform::PS4               = Platform("PS4");
    const Platform & Platform::PSVita            = Platform("PS_VITA");
    const Platform & Platform::Roku              = Platform("ROKU");
    const Platform & Platform::Tizen             = Platform("TIZEN");
    const Platform & Platform::VisionOS          = Platform("VISION_OS");
    const Platform & Platform::WatchOS           = Platform("WATCH_OS");
    const Platform & Platform::Web               = Platform("WEB");
    const Platform & Platform::Wii               = Platform("WII");
    const Platform & Platform::Windows           = Platform("WINDOWS");
    const Platform & Platform::WindowsPhone      = Platform("WINP");
    const Platform & Platform::Xbox360           = Platform("XBOX_360");
    const Platform & Platform::XboxOne           = Platform("XBOX_ONE");

    Platform::Platform(const std::string & platform)
    {
        m_value = platform;
    }

    const std::string & Platform::toString() const
    {
        return m_value;
    }

    const Platform & Platform::fromString(const std::string &platform)
    {
        // this is a bit slow but not used often
        if (platform == Amazon)
        {
            return Platform::Amazon;
        }
        if (platform == AppleTVOS)
        {
            return Platform::AppleTVOS;
        }
        if (platform == BlackBerry)
        {
            return Platform::BlackBerry;
        }
        if (platform == Facebook)
        {
            return Platform::Facebook;
        }
        if (platform == Oculus)
        {
            return Platform::Oculus;
        }
        if (platform == GooglePlayAndroid)
        {
            return Platform::GooglePlayAndroid;
        }
        if (platform == iOS)
        {
            return Platform::iOS;
        }
        if (platform == Linux)
        {
            return Platform::Linux;
        }
        if (platform == Mac)
        {
            return Platform::Mac;
        }
        if (platform == Nintendo)
        {
            return Platform::Nintendo;
        }
        if (platform == Web)
        {
            return Platform::Web;
        }
        if (platform == Windows)
        {
            return Platform::Windows;
        }
        if (platform == WindowsPhone)
        {
            return Platform::WindowsPhone;
        }
        if (platform == Xbox360)
        {
            return Platform::Xbox360;
        }
        if (platform == PS3)
        {
            return Platform::PS3;
        }
        if (platform == XboxOne)
        {
            return Platform::XboxOne;
        }
        if (platform == PS4)
        {
            return Platform::PS4;
        }
        if (platform == Wii)
        {
            return Platform::Wii;
        }
        if (platform == PSVita)
        {
            return Platform::PSVita;
        }
        if (platform == Tizen)
        {
            return Platform::Tizen;
        }
        if (platform == Roku)
        {
            return Platform::Roku;
        }
        if (platform == VisionOS)
        {
            return Platform::VisionOS;
        }
        if (platform == WatchOS)
        {
            return Platform::WatchOS;
        }

        return Platform::Unknown;
    }
}
