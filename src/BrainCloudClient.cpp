// BrainCloudClient.cpp
// BrainCloudLib
// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "braincloud/BrainCloudClient.h"

#include "braincloud/internal/Device.h"
#include "braincloud/internal/JsonUtil.h"
#include "braincloud/internal/URLRequestMethod.h"

namespace BrainCloud
{
    // This call is deprecated in a pure virtual class. We create an empty declaration here so we can clean up the warnings everywhere
    void IServerCallback::serverWarning( ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, int numRetries, const std::string & statusMessage)
    {
    }

    // Define all static member variables.
    bool BrainCloudClient::EnableSingletonMode = false;
    const char * BrainCloudClient::SingletonUseErrorMessage =
            "Singleton usage is disabled. If called by mistake, use your own variable that holds an instance of the bcWrapper/bcClient.";

    BrainCloudClient * BrainCloudClient::_instance = NULL;
    std::string BrainCloudClient::s_brainCloudClientVersion = "6.0.0";
    const char* BC_SERVER_URL = "https://api.braincloudservers.com/dispatcherv2"; 

    /**
     * Constructor
     */
    BrainCloudClient::BrainCloudClient() :
        _brainCloudComms(IBrainCloudComms::create(this)),
        _rttComms(new RTTComms(this)),
        _relayComms(new RelayComms(this)),
        _asyncMatchService(new BrainCloudAsyncMatch(this)),
        _authenticationService(new BrainCloudAuthentication(this)),
        _blockchainService(new BrainCloudBlockchain(this)),
        _campaignService(new BrainCloudCampaign(this)),
        _chatService(new BrainCloudChat(this)),
        _dataStreamService(new BrainCloudDataStream(this)),
        _entityService(new BrainCloudEntity(this)),
        _eventService(new BrainCloudEvent(this)),
        _fileService(new BrainCloudFile(this)),
        _globalFileService(new BrainCloudGlobalFile(this)),
        _friendService(new BrainCloudFriend(this)),
        _gamificationService(new BrainCloudGamification(this)),
        _globalAppService(new BrainCloudGlobalApp(this)),
        _globalEntityService(new BrainCloudGlobalEntity(this)),
        _globalStatisticsService(new BrainCloudGlobalStatistics(this)),
        _groupService(new BrainCloudGroup(this)),
        _groupFileService(new BrainCloudGroupFile(this)),
        _identityService(new BrainCloudIdentity(this)),
        _lobbyService(new BrainCloudLobby(this)),
        _mailService(new BrainCloudMail(this)),
        _matchmakingService(new BrainCloudMatchmaking(this)),
        _messagingService(new BrainCloudMessaging(this)),
        _oneWayMatchService(new BrainCloudOneWayMatch(this)),
        _playbackStreamService(new BrainCloudPlaybackStream(this)),
        _playerStateService(new BrainCloudPlayerState(this)),
        _playerStatisticsService(new BrainCloudPlayerStatistics(this)),
        _playerStatisticsEventService(new BrainCloudPlayerStatisticsEvent(this)),
        _presenceService(new BrainCloudPresence(this)),
        _virtualCurrencyService(new BrainCloudVirtualCurrency(this)),
        _appStoreService(new BrainCloudAppStore(this)),
        _profanityService(new BrainCloudProfanity(this)),
        _pushNotificationService(new BrainCloudPushNotification(this)),
        _redemptionCodeService(new BrainCloudRedemptionCode(this)),
        _s3HandlingService(new BrainCloudS3Handling(this)),
        _scriptService(new BrainCloudScript(this)),
        _socialLeaderboardService(new BrainCloudSocialLeaderboard(this)),
        _timeService(new BrainCloudTime(this)),
        _tournamentService(new BrainCloudTournament(this)),
        _customEntityService(new BrainCloudCustomEntity(this)),
        _itemCatalogService(new BrainCloudItemCatalog(this)),
        _userItemsService(new BrainCloudUserItems(this)),
        _releasePlatform(""),
        _appVersion(""),
        _timezoneOffset(0.0)
    {
        //needed this here otherwise out of scope compiler error
        _rttService = new BrainCloudRTT(_rttComms, this);
        _relayService = new BrainCloudRelay(_relayComms, this);
    }

    void BrainCloudClient::enableCompression(bool isEnabled)
    {
        enableCompressedRequests(isEnabled);
        enableCompressedResponses(isEnabled);
    }

    void BrainCloudClient::enableCompressedRequests(bool isEnabled)
    {
        _brainCloudComms->compressRequests = isEnabled;
    }

    void BrainCloudClient::enableCompressedResponses(bool isEnabled)
    {
        _authenticationService->compressResponses = isEnabled;
    }

    BrainCloudClient::~BrainCloudClient()
    {
        delete _relayService;
        delete _rttService;
        delete _tournamentService;
        delete _customEntityService;
        delete _itemCatalogService;
        delete _timeService;
        delete _socialLeaderboardService;
        delete _scriptService;
        delete _s3HandlingService;
        delete _redemptionCodeService;
        delete _pushNotificationService;
        delete _profanityService;
        delete _appStoreService;
        delete _virtualCurrencyService;
        delete _presenceService;
        delete _playerStatisticsEventService;
        delete _playerStatisticsService;
        delete _playerStateService;
        delete _playbackStreamService;
        delete _oneWayMatchService;
        delete _messagingService;
        delete _matchmakingService;
        delete _mailService;
        delete _lobbyService;
        delete _identityService;
        delete _groupService;
        delete _globalStatisticsService;
        delete _globalEntityService;
        delete _globalAppService;
        delete _gamificationService;
        delete _friendService;
        delete _fileService;
        delete _globalFileService;
        delete _eventService;
        delete _entityService;
        delete _dataStreamService;
        delete _chatService;
        delete _authenticationService;
        delete _campaignService;
        delete _asyncMatchService;
        delete _relayComms;
        delete _rttComms;
        delete _brainCloudComms;
    }

    ////////////////////////////////////////////////////
    // Public Methods
    ////////////////////////////////////////////////////

    const char * BrainCloudClient::getSessionId() const {
        return(_brainCloudComms->getSessionId().c_str());
    }

    const char * BrainCloudClient::getRttConnectionId() const{
        return(_rttComms->getConnectionId().c_str());
    }

    void BrainCloudClient::initializeComms(const char * serverURL, const char * appId, const std::map<std::string, std::string>& secretMap)
    {
        if (_brainCloudComms)
        {
            // automatically upgrade any older clients using "dispatcher" url
            // to "dispatcherv2" endpoint. Comms supports this now and otherwise
            // the change is transparent to the client.
            const char * urlToUse = serverURL;
            std::string url = serverURL;
            if (url.find("dispatcherv2") == std::string::npos)
            {
                size_t index = url.find("dispatcher");
                if (index != std::string::npos)
                {
                    url = url.substr(0, index);
                    url += "dispatcherv2";
                    urlToUse = url.c_str();
                }
            }
            _brainCloudComms->initializeWithApps(urlToUse, appId, secretMap);
        }

        if (_rttComms)
        {
            _rttComms->initialize();
        }

        if (_relayComms)
        {
            _relayComms->initialize();
        }
    }

    void BrainCloudClient::initialize(const char * serverURL, const char * secretKey, const char * appId, const char * appVersion)
    {
        resetCommunication();
        std::string error = "";
        if (serverURL == NULL || strlen(serverURL) <= 0)
            error = "serverURL was null or empty";
        else if (secretKey == NULL || strlen(secretKey) <= 0)
            error = "secretKey was null or empty";
        else if (appId == NULL || strlen(appId) <= 0)
            error = "appId was null or empty";
        else if (appVersion == NULL || strlen(appVersion) <= 0)
            error = "appVersion was null or empty";

        if (error.length() > 0)
        {
            std::cout << "ERROR | Failed to initialize brainCloud - " << error;
            return;
        }

        std::map<std::string, std::string> secretMap;
        secretMap[appId] = secretKey;

        initializeComms(serverURL, appId, secretMap);
        setupOSLocaleData();

        _releasePlatform = Device::getPlatformName();
        _appVersion = appVersion;
    }

    void BrainCloudClient::initialize(const char * secretKey, const char * appId, const char * appVersion)
    {
        initialize(BC_SERVER_URL, secretKey, appId, appVersion);
    }

    void BrainCloudClient::initializeWithApps(const char * serverURL, const char * defaultAppId, const std::map<std::string, std::string>& secretMap, const char * appVersion)
    {
        resetCommunication();
        std::string error = "";
        if (serverURL == NULL || strlen(serverURL) <= 0)
            error = "serverURL was null or empty";
        else if (defaultAppId == NULL || strlen(defaultAppId) <= 0)
            error = "appId was null or empty";
        else if (appVersion == NULL || strlen(appVersion) <= 0)
            error = "appVersion was null or empty";
        else if (secretMap.find(defaultAppId) == secretMap.end())
            error = "not secretKey match for appid";

        if (error.length() > 0)
        {
            std::cout << "ERROR | Failed to initialize brainCloud - " << error;
            return;
        }

        initializeComms(serverURL, defaultAppId, secretMap);
        setupOSLocaleData();

        _releasePlatform = Device::getPlatformName();
        _appVersion = appVersion;
    }

    void BrainCloudClient::initializeWithApps(const char* defaultAppId, const std::map<std::string, std::string>& secretMap, const char* appVersion)
    {
        initializeWithApps(BC_SERVER_URL, defaultAppId, secretMap, appVersion);
    }

    void BrainCloudClient::initializeIdentity(const char * profileId, const char * anonymousId)
    {
        _authenticationService->initialize(profileId, anonymousId);
    }


    void BrainCloudClient::runCallbacks(eBrainCloudUpdateType updateType)
    {
        switch (updateType)
        {
            case eBrainCloudUpdateType::REST:
                _brainCloudComms->runCallbacks();
                break;
            case eBrainCloudUpdateType::PING:
                _lobbyService->runPingCallbacks();
                break;
            case eBrainCloudUpdateType::RTT:
                _rttComms->runCallbacks();
                break;
            case eBrainCloudUpdateType::RS:
                _relayComms->runCallbacks();
                break;
            case eBrainCloudUpdateType::ALL:
                _brainCloudComms->runCallbacks();
                _lobbyService->runPingCallbacks();
                _rttComms->runCallbacks();
                _relayComms->runCallbacks();
                break;
        }
    }

    void BrainCloudClient::registerEventCallback(IEventCallback *eventCallback)
    {
        _brainCloudComms->registerEventCallback(eventCallback);
    }

    void BrainCloudClient::deregisterEventCallback()
    {
        _brainCloudComms->deregisterEventCallback();
    }

    void BrainCloudClient::registerAutoReconnectCallback(IAutoReconnectCallback* autoReconnectCallback)
    {
        std::shared_ptr<IAutoReconnectCallback> sharedCallback(autoReconnectCallback);
        _brainCloudComms->registerAutoReconnectCallback(sharedCallback);
    }

    void BrainCloudClient::deregisterAutoReconnectCallback()
    {
        _brainCloudComms->deregisterAutoReconnectCallback();
    }

    void BrainCloudClient::registerRewardCallback(IRewardCallback *rewardCallback)
    {
        _brainCloudComms->registerRewardCallback(rewardCallback);
    }

    void BrainCloudClient::deregisterRewardCallback()
    {
        _brainCloudComms->deregisterRewardCallback();
    }

    void BrainCloudClient::registerFileUploadCallback(IFileUploadCallback * fileUploadCallback)
    {
        _brainCloudComms->registerFileUploadCallback(fileUploadCallback);
    }

    void BrainCloudClient::deregisterFileUploadCallback()
    {
        _brainCloudComms->deregisterFileUploadCallback();
    }

    void BrainCloudClient::registerGlobalErrorCallback(IGlobalErrorCallback * globalErrorCallback)
    {
        _brainCloudComms->registerGlobalErrorCallback(globalErrorCallback);
    }

    void BrainCloudClient::deregisterGlobalErrorCallback()
    {
        _brainCloudComms->deregisterGlobalErrorCallback();
    }

    void BrainCloudClient::registerNetworkErrorCallback(INetworkErrorCallback * networkErrorCallback)
    {
        _brainCloudComms->registerNetworkErrorCallback(networkErrorCallback);
    }

    void BrainCloudClient::deregisterNetworkErrorCallback()
    {
        _brainCloudComms->deregisterNetworkErrorCallback();
    }

    void BrainCloudClient::enableAutoReconnect(bool shouldEnable)
    {
        _brainCloudComms->setAutoReconnectEnabled(shouldEnable);
    }

    void BrainCloudClient::enableLogging(bool shouldEnable)
    {
        _brainCloudComms->enableLogging(shouldEnable);
        _lobbyService->enableLogging(shouldEnable);
        _rttComms->enableLogging(shouldEnable);
        _relayComms->enableLogging(shouldEnable);
    }

    /**
     * Heart beat to keep session(s) current, and retrieve any pending events...
     */
    void BrainCloudClient::heartbeat()
    {
        _brainCloudComms->sendHeartbeat();
    }

    void BrainCloudClient::sendRequest(ServerCall * serviceMessage)
    {
        _brainCloudComms->addToQueue(serviceMessage);
    }

    void BrainCloudClient::resetCommunication()
    {
        _relayComms->resetCommunication();
        _rttComms->resetCommunication();
        _brainCloudComms->resetCommunication();
        _brainCloudComms->setSessionId("");
        _authenticationService->setProfileId("");
    }

    void BrainCloudClient::shutdown()
    {
        _relayComms->shutdown();
        _rttComms->shutdown();
        _brainCloudComms->shutdown();
        _brainCloudComms->setSessionId("");
        _authenticationService->setProfileId("");
    }

    bool BrainCloudClient::isAuthenticated()
    {
        return _brainCloudComms->isAuthenticated();
    }

    bool BrainCloudClient::isInitialized()
    {
        return _brainCloudComms->isInitialized() && _rttComms->isInitialized() && _relayComms->isInitialized();
    }

    bool BrainCloudClient::isKillswitchEngaged()
    {
        return _brainCloudComms->isKillswitchEngaged();
    }

    void BrainCloudClient::setImmediateRetryOnError(bool value)
    {
        _brainCloudComms->setImmediateRetryOnError(value);
    }

    /**
     * Retrieve the pointer to the singleton BrainCloudClient instance.
     */
    BrainCloudClient * BrainCloudClient::getInstance()
    {
        if(EnableSingletonMode == false) {
            throw std::invalid_argument(SingletonUseErrorMessage);
        }

        if (_instance == NULL) {
            _instance = new BrainCloudClient();
        }
        return _instance;
    }

    void BrainCloudClient::setHeartbeatInterval(int intervalInMilliseconds) {
        _brainCloudComms->setHeartbeatInterval(intervalInMilliseconds);
    }

    const std::vector<int> & BrainCloudClient::getPacketTimeouts()
    {
        return _brainCloudComms->getPacketTimeouts();
    }

    void BrainCloudClient::setPacketTimeouts(const std::vector<int> & packetTimeouts)
    {
        _brainCloudComms->setPacketTimeouts(packetTimeouts);
    }

    void BrainCloudClient::setPacketTimeoutsToDefault()
    {
        _brainCloudComms->setPacketTimeoutsToDefault();
    }

    void BrainCloudClient::setAuthenticationPacketTimeout(int timeoutSecs)
    {
        _brainCloudComms->setAuthenticationPacketTimeout(timeoutSecs);
    }

    int BrainCloudClient::getAuthenticationPacketTimeout()
    {
        return _brainCloudComms->getAuthenticationPacketTimeout();
    }

    void BrainCloudClient::setOldStyleStatusMessageErrorCallback(bool enabled)
    {
        _brainCloudComms->setOldStyleStatusMessageErrorCallback(enabled);
    }

    void BrainCloudClient::setErrorCallbackOn202Status(bool isError)
    {
        _brainCloudComms->setErrorCallbackOn202Status(isError);
    }

    int BrainCloudClient::getUploadLowTransferRateTimeout()
    {
        return _brainCloudComms->getUploadLowTransferRateTimeout();
    }

    void BrainCloudClient::setUploadLowTransferRateTimeout(int timeoutSecs)
    {
        _brainCloudComms->setUploadLowTransferRateTimeout(timeoutSecs);
    }

    int BrainCloudClient::getUploadLowTransferRateThreshold()
    {
        return _brainCloudComms->getUploadLowTransferRateThreshold();
    }

    void BrainCloudClient::setUploadLowTransferRateThreshold(int bytesPerSec)
    {
        _brainCloudComms->setUploadLowTransferRateThreshold(bytesPerSec);
    }

    void BrainCloudClient::enableNetworkErrorMessageCaching(bool enabled)
    {
        _brainCloudComms->enableNetworkErrorMessageCaching(enabled);
    }

    void BrainCloudClient::retryCachedMessages()
    {
        _brainCloudComms->retryCachedMessages();
    }

    void BrainCloudClient::flushCachedMessages(bool sendApiErrorCallbacks)
    {
        _brainCloudComms->flushCachedMessages(sendApiErrorCallbacks);
    }

    void BrainCloudClient::insertEndOfMessageBundleMarker()
    {
        _brainCloudComms->insertEndOfMessageBundleMarker();
    }

    ////////////////////////////////////////////////////
    // Private Methods
    ////////////////////////////////////////////////////
    void BrainCloudClient::setupOSLocaleData()
    {
        Device::getLocale(&_timezoneOffset, &_languageCode, &_countryCode);
    }
}  // end namespace
