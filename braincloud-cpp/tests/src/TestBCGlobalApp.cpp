#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestResult.h"
#include "TestBCGlobalApp.h"

using namespace BrainCloud;


TEST_F(TestBCGlobalApp, ReadProperties)
{
    TestResult tr;
    
    m_bc->getGlobalAppService()->readProperties(&tr);
    tr.run(m_bc);
}

TEST_F(TestBCGlobalApp, ReadSelectedProperties)
{
    TestResult tr;
    
    m_bc->getGlobalAppService()->readSelectedProperties({ "prop1", "prop2", "prop3" }, &tr);
    tr.run(m_bc);
}

TEST_F(TestBCGlobalApp, ReadPropertiesInCategories)
{
    TestResult tr;
    
    m_bc->getGlobalAppService()->readPropertiesInCategories({ "test" }, &tr);
    tr.run(m_bc);
}