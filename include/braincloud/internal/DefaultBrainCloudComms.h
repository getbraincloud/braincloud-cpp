//
//  DefaultBrainCloudComms.h
//  BrainCloudLib
//

#ifndef _BRAINCLOUDMANAGER_H_
#define _BRAINCLOUDMANAGER_H_

#include <string>
#include <vector>
#include <list>
#include <queue>

#if defined(WIN32) && defined(USE_CURL)
#include <WinSock2.h>
#include <Windows.h>
#endif
#include "braincloud/internal/Mutex.h"

#include "json/json.h"
#include "braincloud/internal/IBrainCloudComms.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"

#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"

#include "braincloud/internal/URLLoader.h"
#include "braincloud/internal/URLRequest.h"
#include "braincloud/internal/URLResponse.h"

#define RETRY_TIME_NOT_RETRYING 0

namespace BrainCloud
{
    class BrainCloudClient;

    class DefaultBrainCloudComms : public IBrainCloudComms
    {
    public:
        virtual ~DefaultBrainCloudComms();

        // from IBrainCloudComms
        virtual void initialize(const char * serverURL, const char * appId, const char * secretKey);

        virtual void addToQueue( ServerCall * );

        virtual void enableNetworkErrorMessageCaching(bool enabled);
        virtual void retryCachedMessages();
        virtual void flushCachedMessages(bool sendApiErrorCallbacks);

        virtual void sendHeartbeat();
        virtual void resetCommunication();
        virtual void shutdown();
        virtual void runCallbacks();
        virtual void registerEventCallback(IEventCallback *eventCallback);
        virtual void deregisterEventCallback();
        virtual void registerFileUploadCallback(IFileUploadCallback *fileUploadCallback);
        virtual void deregisterFileUploadCallback();
        virtual void registerAutoReconnectCallback(std::shared_ptr<IAutoReconnectCallback> autoReconnectCallback);
        virtual void deregisterAutoReconnectCallback();
        virtual void registerRewardCallback(IRewardCallback *rewardCallback);
        virtual void deregisterRewardCallback();
        virtual void registerGlobalErrorCallback(IGlobalErrorCallback *globalErrorCallback);
        virtual void deregisterGlobalErrorCallback();
        virtual void registerNetworkErrorCallback(INetworkErrorCallback * networkErrorCallback);
        virtual void deregisterNetworkErrorCallback();

        virtual void cancelUpload(const char * fileUploadId);
        virtual double getUploadProgress(const char * fileUploadId);
        virtual int64_t getUploadTotalBytesToTransfer(const char * fileUploadId);
        virtual int64_t getUploadBytesTransferred(const char * fileUploadId);

        // returns true if packet requires a retry
        bool handleResult( URLResponse const & response, URLRequest const request);

    protected:
        friend class IBrainCloudComms;

        DefaultBrainCloudComms(BrainCloudClient* client);

        virtual void startFileUpload(const Json::Value & jsonPrepareUploadResponse);

    private:
        URLLoader * _loader;
        URLRequest  * _request;

        std::vector<ServerCall*> _queue;
        std::queue<BrainCloudCallbackEvent *>  _apiCallbackQueue;
        std::list<std::string> _eventCallbackQueue;
        std::list<std::string> _rewardCallbackQueue;
        std::list<URLResponse> _responses;
        std::vector<ServerCall*> _inProgress;

        long receivedPacketIdChecker = 0;

        RecursiveMutex _loaderMutex;
        RecursiveMutex _queueMutex;
        RecursiveMutex _mutex;

        int64_t _retryTimeMillis;

        void handleResponseBundle(Json::Value & root);
        void handleError(URLResponse const& response, URLRequest const& request);
        void triggerCommsError(int statusCode, int responseCode, const std::string & error, const std::string & severity);
        void processEvents( Json::Value *, bool = true );
        bool shouldRetryPacket();
        long getRetryTimeoutMillis(int retry);
        int getMaxSendAttempts();

        void resend();
        void createAndSendBundle();
        void startHttpRequest();

        void ProcessSwitchResponse(Json::Value data);

        void resetErrorCache();
        void fakeErrorResponse(int32_t statusCode, int32_t reasonCode, const std::string & statusMessage);

		void updateKillSwitch(const std::string & service, const std::string & operation, int32_t statusCode);
		void resetKillSwitch();
    };

    struct CachedCall
    {
        ServiceName service;
        ServiceOperation operation;
        Json::Value payload;
        IServerCallback* callback;
    };

    class AutoReconnectAuthCallback final : public IServerCallback
    {
    public:
        AutoReconnectAuthCallback(DefaultBrainCloudComms* commsRef, std::shared_ptr<IAutoReconnectCallback> callback, std::vector<CachedCall> lastPacket);

        void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const& jsonData) override;
        void serverError(ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string& jsonError) override;

    protected:
        DefaultBrainCloudComms* _commsRef;
        std::shared_ptr<IAutoReconnectCallback> _callback;
        std::vector<CachedCall> _lastPacket;

    };
};

#endif
