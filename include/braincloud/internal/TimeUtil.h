//
//  TimeUtil.h
//  BrainCloudLib
//


#ifndef _TIMEUTIL_H_
#define _TIMEUTIL_H_

#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud {

    class TimeUtil
    {
    public:
        static int64_t getCurrentTimeMillis();
        // monotonic; use for heartbeats/timeouts so midnight, DST and clock changes can't skew them
        static int64_t getMonotonicTimeMillis();
        static void sleepMillis(int millis);
    };

}

#endif
