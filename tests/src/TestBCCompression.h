#ifndef _TESTBCCOMPRESSION_H_
#define _TESTBCCOMPRESSION_H_

#include "TestFixtureBase.h"

using namespace BrainCloud;

class TestBCCompression : public TestFixtureBase
{
protected:
    virtual bool ShouldSkipAuthenticate() {
        return true;
    }

};

#endif
