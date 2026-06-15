// Copyright 2026 bitHeads, Inc. All Rights Reserved.
#include "braincloud/AuthenticationType.h"

namespace BrainCloud
{
    const AuthenticationType & AuthenticationType::Anonymous = AuthenticationType("Anonymous");
    const AuthenticationType & AuthenticationType::Universal = AuthenticationType("Universal");
    const AuthenticationType & AuthenticationType::Email = AuthenticationType("Email");
    const AuthenticationType & AuthenticationType::Facebook = AuthenticationType("Facebook");
    const AuthenticationType & AuthenticationType::FacebookLimited = AuthenticationType("FacebookLimited");
    const AuthenticationType & AuthenticationType::Oculus = AuthenticationType("Oculus");
    const AuthenticationType & AuthenticationType::GameCenter = AuthenticationType("GameCenter");
    const AuthenticationType & AuthenticationType::Steam = AuthenticationType("Steam");
    const AuthenticationType & AuthenticationType::Apple = AuthenticationType("Apple");
    const AuthenticationType & AuthenticationType::Google = AuthenticationType("Google");
    const AuthenticationType & AuthenticationType::GoogleOpenId = AuthenticationType("GoogleOpenId");
    const AuthenticationType & AuthenticationType::Twitter = AuthenticationType("Twitter");
    const AuthenticationType & AuthenticationType::Parse = AuthenticationType("Parse");
    const AuthenticationType & AuthenticationType::Handoff = AuthenticationType("Handoff");
    const AuthenticationType & AuthenticationType::SettopHandoff = AuthenticationType("SettopHandoff");
    const AuthenticationType & AuthenticationType::External = AuthenticationType("External");
    const AuthenticationType & AuthenticationType::Ultra = AuthenticationType("Ultra");
    const AuthenticationType & AuthenticationType::Unknown = AuthenticationType("Unknown");

    AuthenticationType::AuthenticationType(const std::string & AuthenticationType)
    {
        m_value = AuthenticationType;
    }

    const std::string & AuthenticationType::toString() const
    {
        return m_value;
    }

    const AuthenticationType & AuthenticationType::fromString(const std::string &AuthenticationType)
    {
        // this is a bit slow but not used often
        if (AuthenticationType == Anonymous)
        {
            return AuthenticationType::Anonymous;
        }
        if (AuthenticationType == Universal)
        {
            return AuthenticationType::Universal;
        }
        if (AuthenticationType == Email)
        {
            return AuthenticationType::Email;
        }
        if (AuthenticationType == Facebook)
        {
            return AuthenticationType::Facebook;
        }
        if (AuthenticationType == FacebookLimited)
        {
            return AuthenticationType::FacebookLimited;
        }
        if (AuthenticationType == GameCenter)
        {
            return AuthenticationType::GameCenter;
        }
        if (AuthenticationType == Steam)
        {
            return AuthenticationType::Steam;
        }
        if (AuthenticationType == Apple)
        {
            return AuthenticationType::Apple;
        }
        if (AuthenticationType == Google)
        {
            return AuthenticationType::Google;
        }
        if (AuthenticationType == GoogleOpenId)
        {
            return AuthenticationType::GoogleOpenId;
        }
        if (AuthenticationType == Twitter)
        {
            return AuthenticationType::Twitter;
        }
        if (AuthenticationType == Parse)
        {
            return AuthenticationType::Parse;
        }
        if (AuthenticationType == Handoff)
        {
            return AuthenticationType::Handoff;
        }
        if (AuthenticationType == Ultra)
        {
            return AuthenticationType::Ultra;
        }
        if (AuthenticationType == External)
        {
            return AuthenticationType::External;
        }

        return AuthenticationType::Unknown;
    }
}
