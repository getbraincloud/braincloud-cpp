//
//  IRTTConnectCallback.h
//  GameClientLib
//
//  Created by David St-Louis on 2018-08-09.
//

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>

namespace BrainCloud {
    
    class IRTTCallback
    {
    public:
        virtual ~IRTTCallback( )  { }
        
        /**
         * Method called on RTT events
         *
         * @param eventJson - Data for the event.
         */
        virtual void rttCallback(const std::string& jsonData) = 0;
    };
    
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

