#include <vector>
#include "TestResult.h"
#include "TestUser.h"

using namespace std;

TestUser::TestUser(std::string prefix, int suffix, BrainCloudClient* client) :
	m_client(client)
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

	m_client->getAuthenticationService()->authenticateUniversal(m_id, m_password, true, &tr);
	tr.run(m_client);
	std::strcpy(m_profileId, m_client->getAuthenticationService()->getProfileId().c_str());

	m_client->getPlayerStateService()->updateUserName(m_id, &tr);
	tr.run(m_client);

	m_client->getPlayerStateService()->updateContactEmail("braincloudunittest@gmail.com", &tr);
	tr.run(m_client);

	m_client->getPlayerStateService()->logout(&tr);
	tr.run(m_client);

	m_client->getAuthenticationService()->clearSavedProfileId();
}

