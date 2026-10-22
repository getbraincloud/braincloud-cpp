#pragma once

#include "braincloud/BrainCloudTypes.h"

#include "braincloud/IEventCallback.h"
#include "braincloud/IFileUploadCallback.h"
#include "braincloud/IGlobalErrorCallback.h"
#include "braincloud/INetworkErrorCallback.h"
#include "braincloud/IRewardCallback.h"
#include "braincloud/IAutoReconnectCallback.h"
#include "braincloud/IServerCallback.h"

#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"

#include <map>

#define NO_PACKET_EXPECTED -1
#define DEFAULT_AUTHENTICATION_TIMEOUT_MILLIS 15000

namespace BrainCloud
{
	class IFileUploader;
	class BrainCloudClient;

	struct BrainCloudCallbackEvent
	{
		IServerCallback* callback;
		ServiceName m_service;
		ServiceOperation m_operation;
		bool m_error;
		unsigned short m_retryCount;
		int m_statusCode;
		int m_reasonCode;
		std::string m_data;

		BrainCloudCallbackEvent()
			: m_service(ServiceName::None)
			, m_operation(ServiceOperation::None)
			, m_error(false)
			, m_retryCount(0)
			, m_statusCode(0)
			, m_reasonCode(0)
		{
		}
	};

	enum UploadStatus
	{
		NONE = 0,
		UPLOADING = 1,
		CANCELLING = 2,
		FAILED = 3,
		COMPLETE = 4,
	};

	// although named as an interface, this is actually an abstract class
	class IBrainCloudComms
	{
	public:
        static IBrainCloudComms* create(BrainCloudClient* client);

		virtual ~IBrainCloudComms();

		// pure virtual methods
		virtual void initialize(const char * serverURL, const char * appId, const char * secretKey) = 0;
		void initializeWithApps(const char * serverURL, const char * defaultAppId, const std::map<std::string, std::string>& secretMap);
		virtual void addToQueue(ServerCall *) = 0;

		virtual void sendHeartbeat() = 0;
		virtual void resetCommunication() = 0;
		virtual void shutdown() = 0;
		virtual void runCallbacks() = 0;

		virtual void registerEventCallback(IEventCallback *eventCallback) = 0;
		virtual void deregisterEventCallback() = 0;

		virtual void registerFileUploadCallback(IFileUploadCallback *fileUploadCallback) = 0;
		virtual void deregisterFileUploadCallback() = 0;

		virtual void registerAutoReconnectCallback(std::shared_ptr<IAutoReconnectCallback> autoReconnectCallback) = 0;
		virtual void deregisterAutoReconnectCallback() = 0;

		virtual void registerRewardCallback(IRewardCallback *rewardCallback) = 0;
		virtual void deregisterRewardCallback() = 0;

		virtual void registerGlobalErrorCallback(IGlobalErrorCallback *globalErrorCallback) = 0;
		virtual void deregisterGlobalErrorCallback() = 0;

		virtual void registerNetworkErrorCallback(INetworkErrorCallback * networkErrorCallback) = 0;
		virtual void deregisterNetworkErrorCallback() = 0;

		virtual void cancelUpload(const char * fileUploadId) = 0;
		virtual double getUploadProgress(const char * fileUploadId) = 0;
		virtual int64_t getUploadTotalBytesToTransfer(const char * fileUploadId) = 0;
		virtual int64_t getUploadBytesTransferred(const char * fileUploadId) = 0;

		virtual void enableNetworkErrorMessageCaching(bool enabled) = 0;
		virtual void retryCachedMessages() = 0;
		virtual void flushCachedMessages(bool sendApiErrorCallbacks) = 0;

		// implemented methods
		void enableLogging(bool shouldEnable);
		bool isInitialized();
		bool isAuthenticated();
		bool isKillswitchEngaged();
		void setAuthenticated();

		//compression
		bool compressRequests = true;

		void setHeartbeatInterval(int milliseconds);
		void setImmediateRetryOnError(bool value);

		const std::string& getAppId() const;
		void clearSessionId();
		const std::string& getSessionId() const;
		void setSessionId(const char *);
		void setServerUrl(const char *);

		const std::vector<int> & getPacketTimeouts();
		void setPacketTimeouts(const std::vector<int> & packetTimeouts);
		void setPacketTimeoutsToDefault();
		void setAuthenticationPacketTimeout(int timeoutSecs);
		int getAuthenticationPacketTimeout();
		void setOldStyleStatusMessageErrorCallback(bool enabled);
		void setErrorCallbackOn202Status(bool isError);

		int getUploadLowTransferRateTimeout();
		void setUploadLowTransferRateTimeout(int timeoutSecs);
		int getUploadLowTransferRateThreshold();
		void setUploadLowTransferRateThreshold(int bytesPerSec);

		void insertEndOfMessageBundleMarker();

		void setAutoReconnectEnabled(bool enabled) { _autoReconnectEnabled = enabled; }
		bool getAutoReconnectEnabled() { return _autoReconnectEnabled; }

		static void createJsonErrorResponse(int statusCode,
                                            int reasonCode,
                                            const std::string & statusMessage,
                                            std::string & out_jsonErrorResponse);

	protected:
        IBrainCloudComms(BrainCloudClient* client);

		BrainCloudClient* _client;

		int64_t _packetId;
		int64_t _expectedPacketId; // the next packetId we're expecting or -1 if nothing is in transit
		int _retryCount;
		int64_t _packetSendTimeMillis;
		bool _isInitialized;
		bool _isAuthenticated;
		int _heartbeatInterval;

		// server time = monotonic ms + offset, synced from server_time so device clock changes can't skew it
		int64_t _serverTimeOffsetMs;
		bool _isServerTimeSynced;
		int64_t getServerTimeMs() const;
		void syncServerTime(const Json::Value& data);
		int _maxBundleSize;
		bool _immediateRetryOnError;
		bool _loggingEnabled;
		int _authenticationTimeoutMillis;
		bool _oldStyleStatusMessageErrorCallback;
		bool _cacheMessagesOnNetworkError;
		bool _errorCallbackOn202;
		int _killSwitchThreshold;

		//compression
		int _clientSideCompressionThreshold = 51200;

		// This flag is set when _cacheMessagesOnNetworkError is true
		// and a network error occurs. It is reset when a call is made
		// to either retryCachedMessages or flushCachedMessages
		bool _blockingQueue;

		IEventCallback *_eventCallback;
		IFileUploadCallback *_fileUploadCallback;
		IGlobalErrorCallback * _globalErrorCallback;
		std::shared_ptr<IAutoReconnectCallback> _autoReconnectCallback;
		IRewardCallback *_rewardCallback;
		INetworkErrorCallback *_networkErrorCallback;

		std::string _appId;
		std::string _sessionId;
		std::string _serverUrl;
		std::string _uploadUrl;
		std::string _secretKey;
		std::map<std::string, std::string> _secretMap;
		std::vector<int> _packetTimeouts;

		typedef std::map<std::string, IFileUploader *>::iterator tFileUploadsIterator;
		std::map<std::string, IFileUploader *> _fileUploads;
		int _uploadLowTransferRateTimeoutSecs;
		int _uploadLowTransferRateThresholdBytesPerSec;
		int _uploadOverallTimeoutSecs;

		int32_t _statusCodeCache;
		int32_t _reasonCodeCache;
		std::string _statusMessageCache;

		bool _killSwitchEngaged = false;
		int32_t _killSwitchErrorCount;
		std::string _killSwitchService;
		std::string _killSwitchOperation;

		bool _autoReconnectEnabled = false;

		void setCredentials(const Json::Value& jsonAuthenticationResponse);
		void filterIncomingMessages(const ServerCall* servercall, const Json::Value& response);

		virtual void startFileUpload(const Json::Value & jsonPrepareUploadResponse) = 0;
		void runCallbacksFileUpload();
	};
};
