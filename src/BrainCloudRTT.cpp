// Copyright 2026 bitHeads, Inc. All Rights Reserved.
#include "braincloud/BrainCloudRTT.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "json/json.h"

namespace BrainCloud
{
    BrainCloudRTT::BrainCloudRTT(RTTComms *comms, BrainCloudClient *client)
        : m_client(client), m_commsLayer(comms)
    {
    }

    void BrainCloudRTT::requestClientConnection(IServerCallback *callback)
    {
        Json::Value nullMsg = Json::nullValue;
        ServerCall *sc = new ServerCall(ServiceName::RTTRegistration, ServiceOperation::RequestClientConnection, nullMsg, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    // RTT stuff
    void BrainCloudRTT::enableRTT(IRTTConnectCallback *callback, bool useWebSocket)
    {
        m_commsLayer->enableRTT(callback, useWebSocket);
    }

    void BrainCloudRTT::disableRTT()
    {
        m_commsLayer->disableRTT();
    }

    bool BrainCloudRTT::getRTTEnabled()
    {
        return m_commsLayer->isRTTEnabled();
    }

    BrainCloudRTT::RTTConnectionStatus BrainCloudRTT::getConnectionStatus()
    {
        return m_commsLayer->getConnectionStatus();
    }

    void BrainCloudRTT::registerRTTEventCallback(IRTTCallback *callback)
    {
        m_commsLayer->registerRTTCallback(ServiceName::Event, callback);
    }

    void BrainCloudRTT::deregisterRTTEventCallback()
    {
        m_commsLayer->deregisterRTTCallback(ServiceName::Event);
    }

    void BrainCloudRTT::registerRTTChatCallback(IRTTCallback *callback)
    {
        m_commsLayer->registerRTTCallback(ServiceName::Chat, callback);
    }

    void BrainCloudRTT::deregisterRTTChatCallback()
    {
        m_commsLayer->deregisterRTTCallback(ServiceName::Chat);
    }

    void BrainCloudRTT::registerRTTMessagingCallback(IRTTCallback *callback)
    {
        m_commsLayer->registerRTTCallback(ServiceName::Messaging, callback);
    }

    void BrainCloudRTT::deregisterRTTMessagingCallback()
    {
        m_commsLayer->deregisterRTTCallback(ServiceName::Messaging);
    }

    void BrainCloudRTT::registerRTTLobbyCallback(IRTTCallback *callback)
    {
        m_commsLayer->registerRTTCallback(ServiceName::Lobby, callback);
    }

    void BrainCloudRTT::deregisterRTTLobbyCallback()
    {
        m_commsLayer->deregisterRTTCallback(ServiceName::Lobby);
    }

    void BrainCloudRTT::registerRTTPresenceCallback(IRTTCallback *callback)
    {
        m_commsLayer->registerRTTCallback(ServiceName::Presence, callback);
    }

    void BrainCloudRTT::deregisterRTTPresenceCallback()
    {
        m_commsLayer->deregisterRTTCallback(ServiceName::Presence);
    }

    void BrainCloudRTT::registerRTTBlockchainRefresh(IRTTCallback *callback)
    {
        m_commsLayer->registerRTTCallback(ServiceName::UserItems, callback);
    }

    void BrainCloudRTT::deregisterRTTBlockchainRefresh()
    {
        m_commsLayer->deregisterRTTCallback(ServiceName::UserItems);
    }

    void BrainCloudRTT::deregisterAllRTTCallbacks()
    {
        m_commsLayer->deregisterAllRTTCallbacks();
    }

    const std::string &BrainCloudRTT::getRTTConnectionId() const
    {
        return m_commsLayer->getConnectionId();
    }

}
