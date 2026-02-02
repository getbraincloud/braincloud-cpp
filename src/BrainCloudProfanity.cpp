// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudProfanity.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"

namespace BrainCloud
{
    BrainCloudProfanity::BrainCloudProfanity(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudProfanity::profanityCheck(const char *text,
                                             const char *languages,
                                             bool flagEmail,
                                             bool flagPhone,
                                             bool flagUrls,
                                             IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::ProfanityText.getValue()] = text;
        if (languages != NULL)
        {
            message[OperationParam::ProfanityLanguages.getValue()] = languages;
        }
        message[OperationParam::ProfanityFlagEmail.getValue()] = flagEmail;
        message[OperationParam::ProfanityFlagPhone.getValue()] = flagPhone;
        message[OperationParam::ProfanityFlagUrls.getValue()] = flagUrls;

        ServerCall *sc = new ServerCall(ServiceName::Profanity, ServiceOperation::ProfanityCheck, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudProfanity::profanityReplaceText(const char *text,
                                                   const char *replaceSymbol,
                                                   const char *languages,
                                                   bool flagEmail,
                                                   bool flagPhone,
                                                   bool flagUrls,
                                                   IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::ProfanityText.getValue()] = text;
        message[OperationParam::ProfanityReplaceSymbol.getValue()] = replaceSymbol;
        if (languages != NULL)
        {
            message[OperationParam::ProfanityLanguages.getValue()] = languages;
        }
        message[OperationParam::ProfanityFlagEmail.getValue()] = flagEmail;
        message[OperationParam::ProfanityFlagPhone.getValue()] = flagPhone;
        message[OperationParam::ProfanityFlagUrls.getValue()] = flagUrls;

        ServerCall *sc = new ServerCall(ServiceName::Profanity, ServiceOperation::ProfanityReplaceText, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudProfanity::profanityIdentifyBadWords(const char *text,
                                                        const char *languages,
                                                        bool flagEmail,
                                                        bool flagPhone,
                                                        bool flagUrls,
                                                        IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::ProfanityText.getValue()] = text;
        if (languages != NULL)
        {
            message[OperationParam::ProfanityLanguages.getValue()] = languages;
        }
        message[OperationParam::ProfanityFlagEmail.getValue()] = flagEmail;
        message[OperationParam::ProfanityFlagPhone.getValue()] = flagPhone;
        message[OperationParam::ProfanityFlagUrls.getValue()] = flagUrls;

        ServerCall *sc = new ServerCall(ServiceName::Profanity, ServiceOperation::ProfanityIdenitfyBadWords, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }
};