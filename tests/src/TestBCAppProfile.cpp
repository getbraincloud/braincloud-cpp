#include "gtest/gtest.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/internal/HeldAppProfile.h"
#if !defined(BC_UWP)
#include "braincloud/internal/md5x.h"
#endif

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace BrainCloud;

namespace
{
    const std::string kValue = "01234567-89ab-cdef-0123-456789abcdef";

#if !defined(BC_UWP)
    // How comms signed before profiles: join payload and value, hash, uppercase.
    std::string legacySignature(const std::string& payload, const std::string& value)
    {
        std::string data = payload + value;
        md5_state_t state;
        md5_byte_t digest[16];
        md5_init(&state);
        md5_append(&state, (const md5_byte_t*)data.c_str(), (int)data.length());
        md5_finish(&state, digest);
        char buf[33];
        for (int i = 0; i < 16; ++i)
            snprintf(&buf[i * 2], 8, "%02x", digest[i]);
        std::string sig(buf, 32);
        std::transform(sig.begin(), sig.end(), sig.begin(), ::toupper);
        return sig;
    }
#endif

    std::string signWith(const IAppProfile& profile, const std::string& payload)
    {
        char signature[IAppProfile::SignatureLength + 1] = {};
        if (!profile.sign(reinterpret_cast<const uint8_t*>(payload.data()), payload.size(), signature))
            return std::string();
        return signature;
    }
}

#if !defined(BC_UWP)
// Payload sizes either side of MD5's 55/56/64-byte block boundaries.
TEST(TestBCAppProfile, SignatureMatchesLegacy)
{
    HeldAppProfile profile(kValue);
    for (size_t length = 0; length <= 130; ++length)
    {
        const std::string payload(length, 'p');
        EXPECT_EQ(legacySignature(payload, kValue), signWith(profile, payload)) << "payload length " << length;
    }
    for (size_t length = 1; length <= 70; ++length)
    {
        const std::string value(length, 'v');
        EXPECT_EQ(legacySignature("{\"packetId\":1}", value), signWith(HeldAppProfile(value), "{\"packetId\":1}")) << "value length " << length;
    }
}
#endif

TEST(TestBCAppProfile, EmptyValueDoesNotSign)
{
    HeldAppProfile profile("");
    EXPECT_FALSE(profile.isValid());
    EXPECT_EQ(std::string(), signWith(profile, "payload"));
}

TEST(TestBCAppProfile, ClientSignsWithTheRightApp)
{
    BrainCloudClient client;
    std::map<std::string, std::string> apps;
    apps["10001"] = "parent-value";
    apps["10002"] = "child-value";
    client.initializeWithApps("https://api.example.com/dispatcherv2", "10001", apps, "1.0.0");

    IBrainCloudComms* comms = client.getBrainCloudComms();
    const std::string payload = "{\"packetId\":0}";
    EXPECT_EQ(signWith(HeldAppProfile("parent-value"), payload), comms->signPayload(payload));

    // Redirected to another server: the held profiles carry over, nothing is passed in again.
    client.reinitialize("https://other.example.com/dispatcherv2", "10002");
    EXPECT_EQ("10002", client.getAppId());
    EXPECT_EQ(signWith(HeldAppProfile("child-value"), payload), comms->signPayload(payload));

    // An app id with no profile of its own signs with the current one, as a single-app redirect did.
    client.reinitialize("https://other.example.com/dispatcherv2", "20001");
    EXPECT_EQ(signWith(HeldAppProfile("child-value"), payload), comms->signPayload(payload));
}

TEST(TestBCAppProfile, ProfileInitialize)
{
    BrainCloudClient client;
    std::shared_ptr<IAppProfile> profile = std::make_shared<HeldAppProfile>("profile-value");
    client.initialize("https://api.example.com/dispatcherv2", profile, "10001", "1.0.0");
    EXPECT_EQ("10001", client.getAppId());
    EXPECT_EQ(signWith(HeldAppProfile("profile-value"), "x"), client.getBrainCloudComms()->signPayload("x"));
}

TEST(TestBCAppProfile, SingleAppInitialize)
{
    BrainCloudClient client;
    client.initialize("https://api.example.com/dispatcherv2", "single-value", "10001", "1.0.0");
    EXPECT_EQ(signWith(HeldAppProfile("single-value"), "x"), client.getBrainCloudComms()->signPayload("x"));
}
