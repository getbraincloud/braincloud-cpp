#ifndef _TESTUSER_H_
#define _TESTUSER_H_

#include <stdlib.h>
#include "braincloud/BrainCloudWrapper.h"

using namespace BrainCloud;

class TestUser
{
public:
    char m_id[256];
    char m_password[256];
    char m_profileId[256];
    char m_email[256];

    BrainCloudWrapper *m_bcWrapper;
    
    TestUser()
    {
        m_id[0] = '\0';
        m_password[0] = '\0';
        m_profileId[0] = '\0';
        m_email[0] = '\0';
    }
    TestUser(std::string prefix, int suffix, BrainCloudWrapper *wrapper);
    
private:
    void Authenticate();
};

#endif

