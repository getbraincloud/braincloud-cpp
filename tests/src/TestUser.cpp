#include <vector>
#include "TestResult.h"
#include "TestUser.h"

using namespace std;

TestUser::TestUser(std::string prefix, int suffix, BrainCloudWrapper* wrapper) :
	m_bcWrapper(wrapper)
{
    std::string id = prefix + "_CPP_" + std::to_string(suffix);
    std::string password = id;
	std::string email= id + "@bctestuser.com" ;
    std::strcpy(m_id, id.c_str());
    std::strcpy(m_email, email.c_str());
    std::strcpy(m_password, password.c_str());

	Authenticate();
}

void TestUser::Authenticate()
{
	TestResult tr;

	m_bcWrapper->authenticateUniversal(m_id, m_password, true, &tr);
	tr.run(m_bcWrapper->client);
	std::strcpy(m_profileId, m_bcWrapper->client->getAuthenticationService()->getProfileId().c_str());

	m_bcWrapper->client->getPlayerStateService()->updateUserName(m_id, &tr);
	tr.run(m_bcWrapper->client);

    m_bcWrapper->client->getPlayerStateService()->updateContactEmail("braincloudunittest@gmail.com", &tr);
	tr.run(m_bcWrapper->client);

	m_bcWrapper->logout(true, &tr);
	tr.run(m_bcWrapper->client);
}

