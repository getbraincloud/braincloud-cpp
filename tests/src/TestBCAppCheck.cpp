// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "gtest/gtest.h"
#include <thread>
#include "braincloud/reason_codes.h"
#include "braincloud/BrainCloudClient.h"
#include "braincloud/AuthenticationIds.h"
#include "braincloud/internal/DefaultBrainCloudComms.h"

namespace
{
    using namespace BrainCloud;

    // Capture the actual ServerCall at the queue boundary. No network or test account is needed.
    class AppCheckCaptureComms : public DefaultBrainCloudComms
    {
    public:
        explicit AppCheckCaptureComms(BrainCloudClient* client) : DefaultBrainCloudComms(client)
        {
            _appId = "test-app";
        }

        void addToQueue(ServerCall* call) override
        {
            payload = *call->getPayload();
            delete call;
        }

        Json::Value payload;
    };

    class AppCheckClient : public BrainCloudClient
    {
    public:
        AppCheckClient()
        {
            delete _brainCloudComms;
            _brainCloudComms = new AppCheckCaptureComms(this);
            _releasePlatform = "IOS";
            _appVersion = "1.2.3";
            _countryCode = "CA";
            _languageCode = "en";
            _timezoneOffset = 0.0;
            getAuthenticationService()->initialize("profile", "anonymous");
        }

        const Json::Value& payload() const
        {
            return static_cast<AppCheckCaptureComms*>(_brainCloudComms)->payload;
        }
    };

    class TestBCAppCheck : public testing::Test
    {
    protected:
        AppCheckClient client;

        std::string serializedPayload()
        {
            Json::FastWriter writer;
            return writer.write(client.payload());
        }

        // Legacy authenticate ServerCall bytes for fixed inputs, including field order and newline.
        std::string legacyPayload()
        {
            return std::string("{\"data\":{\"anonymousId\":\"anonymous\",\"authenticationToken\":\"password\","
                "\"authenticationType\":\"Universal\",\"clientLib\":\"cpp\",\"clientLibVersion\":\"") +
                client.getBrainCloudClientVersion() +
                "\",\"compressResponses\":true,\"countryCode\":\"CA\",\"externalId\":\"user\","
                "\"forceCreate\":true,\"gameId\":\"test-app\",\"gameVersion\":\"1.2.3\","
                "\"languageCode\":\"en\",\"profileId\":\"profile\",\"releasePlatform\":\"IOS\","
                "\"timeZoneOffset\":0.0},\"operation\":\"AUTHENTICATE\",\"service\":\"authenticationV2\"}\n";
        }
    };

    TEST_F(TestBCAppCheck, OmittedTokenPreservesLegacyBytes)
    {
        client.getAuthenticationService()->authenticateUniversal("user", "password", true);
        EXPECT_FALSE(client.payload()["data"].isMember("appCheckToken"));
        EXPECT_EQ(legacyPayload(), serializedPayload());
    }

    TEST_F(TestBCAppCheck, SuppliedTokenIsCopiedAndExistingFieldsAreUnchanged)
    {
        std::string token = "opaque.token.with-\"quotes\"\\and\nnewline";
        const std::string original = token;
        client.getAuthenticationService()->setAppCheckToken(token);
        token = "changed-by-caller";
        client.getAuthenticationService()->authenticateUniversal("user", "password", true);
        EXPECT_EQ(original, client.payload()["data"]["appCheckToken"].asString());

        Json::Value withoutToken = client.payload();
        withoutToken["data"].removeMember("appCheckToken");
        Json::FastWriter writer;
        EXPECT_EQ(legacyPayload(), writer.write(withoutToken));
    }

    TEST_F(TestBCAppCheck, ClearingTokenRestoresLegacyBytes)
    {
        auto auth = client.getAuthenticationService();
        auth->setAppCheckToken("token");
        auth->authenticateUniversal("user", "password", true);
        auth->setAppCheckToken("");
        auth->authenticateUniversal("user", "password", true);
        EXPECT_EQ(legacyPayload(), serializedPayload());
    }

    TEST_F(TestBCAppCheck, RetryUsesLatestTokenAndClearingIsRespected)
    {
        auto auth = client.getAuthenticationService();
        auth->setAppCheckToken("first");
        auth->authenticateUniversal("user", "password", true);
        auth->setAppCheckToken("refreshed");
        EXPECT_EQ("first", client.payload()["data"]["appCheckToken"].asString());
        auth->retryPreviousAuthenticate(NULL);
        EXPECT_EQ("refreshed", client.payload()["data"]["appCheckToken"].asString());
        auth->setAppCheckToken("");
        auth->retryPreviousAuthenticate(NULL);
        EXPECT_EQ(legacyPayload(), serializedPayload());
    }

    TEST_F(TestBCAppCheck, TokenPersistsAcrossAuthenticationMethods)
    {
        auto auth = client.getAuthenticationService();
        auth->setAppCheckToken("token");
        auth->authenticateAnonymous(true);
        EXPECT_EQ("token", client.payload()["data"]["appCheckToken"].asString());
        AuthenticationIds ids = { "user", "password", "provider" };
        auth->authenticateAdvanced(AuthenticationType::External, ids, false, "{\"custom\":42}");
        EXPECT_EQ("token", client.payload()["data"]["appCheckToken"].asString());
        EXPECT_EQ("provider", client.payload()["data"]["externalAuthName"].asString());
        EXPECT_EQ(42, client.payload()["data"]["extraJson"]["custom"].asInt());
    }

    TEST_F(TestBCAppCheck, TokenIsNotSentWithOtherOperationsOrSharedBetweenClients)
    {
        client.getAuthenticationService()->setAppCheckToken("token");
        client.getAuthenticationService()->resetEmailPassword("user@example.com");
        EXPECT_FALSE(client.payload()["data"].isMember("appCheckToken"));
        client.getAuthenticationService()->getServerVersion();
        EXPECT_FALSE(client.payload()["data"].isMember("appCheckToken"));

        AppCheckClient other;
        other.getAuthenticationService()->authenticateAnonymous(true);
        EXPECT_FALSE(other.payload()["data"].isMember("appCheckToken"));
    }

    TEST_F(TestBCAppCheck, ProviderRefreshesEachAuthenticationAndCanBeRemoved)
    {
        auto auth = client.getAuthenticationService();
        auth->setAppCheckToken("manual");
        int calls = 0;
        auth->setAppCheckTokenProvider([&](BrainCloudAuthentication::AppCheckTokenCompletion done) {
            done(++calls == 1 ? "first" : "fresh", "");
        });
        auth->authenticateAnonymous(false);
        EXPECT_TRUE(client.payload().isNull());
        client.runCallbacks(eBrainCloudUpdateType::REST);
        EXPECT_EQ("first", client.payload()["data"]["appCheckToken"].asString());
        auth->retryPreviousAuthenticate(nullptr);
        client.runCallbacks(eBrainCloudUpdateType::REST);
        EXPECT_EQ("fresh", client.payload()["data"]["appCheckToken"].asString());
        EXPECT_EQ(2, calls);
        auth->setAppCheckTokenProvider(nullptr);
        auth->authenticateAnonymous(false);
        EXPECT_EQ("manual", client.payload()["data"]["appCheckToken"].asString());
    }

    TEST_F(TestBCAppCheck, DelayedWorkerCompletionIsMarshalledAndOnlyUsedOnce)
    {
        auto auth = client.getAuthenticationService();
        BrainCloudAuthentication::AppCheckTokenCompletion done;
        auth->setAppCheckTokenProvider([&](BrainCloudAuthentication::AppCheckTokenCompletion completion) { done = completion; });
        auth->authenticateUniversal("user", "password", true);
        client.runCallbacks(eBrainCloudUpdateType::REST);
        EXPECT_TRUE(client.payload().isNull());
        std::thread worker([&] { done("fresh", ""); done("duplicate", ""); });
        worker.join();
        EXPECT_TRUE(client.payload().isNull());
        client.runCallbacks(eBrainCloudUpdateType::REST);
        EXPECT_EQ("fresh", client.payload()["data"]["appCheckToken"].asString());
        EXPECT_EQ("user", client.payload()["data"]["externalId"].asString());
    }

    class ProviderErrorCallback : public IServerCallback
    {
    public:
        int errors = 0;
        void serverCallback(ServiceName, ServiceOperation, const std::string&) override { ADD_FAILURE(); }
        void serverError(ServiceName, ServiceOperation, int status, int reason, const std::string&) override
        {
            ++errors;
            EXPECT_EQ(400, status);
            EXPECT_EQ(CLIENT_APP_CHECK_TOKEN_ERROR, reason);
        }
    };

    TEST_F(TestBCAppCheck, ProviderFailureAndEmptyTokenNeverFallBackToStoredToken)
    {
        auto auth = client.getAuthenticationService();
        ProviderErrorCallback callback;
        auth->setAppCheckToken("stale");
        auth->setAppCheckTokenProvider([](BrainCloudAuthentication::AppCheckTokenCompletion done) { done("ignored", "failed"); });
        auth->authenticateAnonymous(false, &callback);
        client.runCallbacks(eBrainCloudUpdateType::REST);
        auth->setAppCheckTokenProvider([](BrainCloudAuthentication::AppCheckTokenCompletion done) { done("", ""); });
        auth->authenticateAnonymous(false, &callback);
        client.runCallbacks(eBrainCloudUpdateType::REST);
        EXPECT_EQ(2, callback.errors);
        EXPECT_TRUE(client.payload().isNull());
    }

    TEST_F(TestBCAppCheck, ResetAndDestructionIgnoreLateCompletions)
    {
        BrainCloudAuthentication::AppCheckTokenCompletion done;
        auto auth = client.getAuthenticationService();
        auth->setAppCheckTokenProvider([&](BrainCloudAuthentication::AppCheckTokenCompletion completion) { done = completion; });
        auth->authenticateAnonymous(false);
        client.resetCommunication();
        done("late", "");
        client.runCallbacks(eBrainCloudUpdateType::REST);
        EXPECT_TRUE(client.payload().isNull());
        {
            AppCheckClient temporary;
            temporary.getAuthenticationService()->setAppCheckTokenProvider([&](BrainCloudAuthentication::AppCheckTokenCompletion completion) { done = completion; });
            temporary.getAuthenticationService()->authenticateAnonymous(false);
        }
        done("after destruction", "");
    }

}
