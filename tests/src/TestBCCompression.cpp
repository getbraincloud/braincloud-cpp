#include "TestBCCompression.h"


TEST_F(TestBCCompression, CompressedAuthUniversal)
{
    TestResult tr;

    m_bc->enableCompression(true);

    m_bc->getAuthenticationService()->authenticateUniversal(GetUser(UserA)->m_id, GetUser(UserA)->m_password, true, &tr);
    tr.run(m_bc);

    Logout();
}
