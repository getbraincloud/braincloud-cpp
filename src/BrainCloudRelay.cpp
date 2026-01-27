// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudRelay.h"
#include "braincloud/BrainCloudClient.h"

#include "braincloud/internal/RelayComms.h"

namespace BrainCloud
{
    BrainCloudRelay::BrainCloudRelay(RelayComms *commsLayer, BrainCloudClient *client)
        : m_commsLayer(commsLayer), m_client(client)
    {
    }

    void BrainCloudRelay::connect(eRelayConnectionType connectionType, const std::string &host, int port, const std::string &passcode, const std::string &lobbyId, IRelayConnectCallback *callback)
    {
        m_commsLayer->connect(connectionType, host, port, passcode, lobbyId, callback);
    }

    void BrainCloudRelay::disconnect()
    {
        m_commsLayer->disconnect();
    }

    void BrainCloudRelay::endMatch(const std::string &jsonPayload)
    {
        m_commsLayer->endMatch(JsonUtil::jsonStringToValue(jsonPayload));
    }

    bool BrainCloudRelay::isConnected() const
    {
        return m_commsLayer->isConnected();
    }

    int BrainCloudRelay::getPing() const
    {
        return m_commsLayer->getPing();
    }

    void BrainCloudRelay::setPingInterval(int intervalSeconds)
    {
        m_commsLayer->setPingInterval(intervalSeconds);
    }

    const std::string &BrainCloudRelay::getOwnerProfileId() const
    {
        return m_commsLayer->getOwnerProfileId();
    }

    const std::string &BrainCloudRelay::getOwnerCxId() const
    {
        return m_commsLayer->getOwnerCxId();
    }

    const std::string &BrainCloudRelay::getProfileIdForNetId(int netId) const
    {
        return m_commsLayer->getProfileIdForNetId(netId);
    }

    int BrainCloudRelay::getNetIdForProfileId(const std::string &profileId) const
    {
        return m_commsLayer->getNetIdForProfileId(profileId);
    }

    const std::string &BrainCloudRelay::getCxIdForNetId(int netId) const
    {
        return m_commsLayer->getCxIdForNetId(netId);
    }

    int BrainCloudRelay::getNetIdForCxId(const std::string &cxId) const
    {
        return m_commsLayer->getNetIdForCxId(cxId);
    }

    void BrainCloudRelay::registerRelayCallback(IRelayCallback *callback)
    {
        m_commsLayer->registerRelayCallback(callback);
    }

    void BrainCloudRelay::deregisterRelayCallback()
    {
        m_commsLayer->deregisterRelayCallback();
    }

    void BrainCloudRelay::registerSystemCallback(IRelaySystemCallback *callback)
    {
        m_commsLayer->registerSystemCallback(callback);
    }

    void BrainCloudRelay::deregisterSystemCallback()
    {
        m_commsLayer->deregisterSystemCallback();
    }

    void BrainCloudRelay::send(const uint8_t *data, int size, uint64_t toNetId, bool reliable, bool ordered, eRelayChannel channel)
    {
        if (toNetId == TO_ALL_PLAYERS)
        {
            sendToAll(data, size, reliable, ordered, channel);
        }
        else
        {
            uint64_t playerMask = (uint64_t)1 << (uint64_t)toNetId;
            m_commsLayer->send(data, size, playerMask, reliable, ordered, channel);
        }
    }

    void BrainCloudRelay::sendToPlayers(const uint8_t *data, int size, uint64_t playerMask, bool reliable, bool ordered, eRelayChannel channel)
    {
        m_commsLayer->send(data, size, playerMask, reliable, ordered, channel);
    }

    void BrainCloudRelay::sendToAll(const uint8_t *data, int size, bool reliable, bool ordered, eRelayChannel channel)
    {
        const auto &myProfileId = m_client->getAuthenticationService()->getProfileId();
        auto myNetId = m_commsLayer->getNetIdForProfileId(myProfileId);

        uint64_t myBit = (uint64_t)1 << (uint64_t)myNetId;
        uint64_t myInvertedBits = ~myBit;
        uint64_t playerMask = TO_ALL_PLAYERS & myInvertedBits;
        m_commsLayer->send(data, size, playerMask, reliable, ordered, channel);
    }
};
