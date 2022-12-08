//
//  IRTTConnectCallback.h
//  GameClientLib
//
//  Created by David St-Louis on 2018-08-09.
//

#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <string>

namespace BrainCloud {
    
    class IRTTConnectCallback
    {
    public:
        virtual ~IRTTConnectCallback( )  { }
        
        /**
         * Method called when connection to RTT was successful
         */
        virtual void rttConnectSuccess() = 0;
        
        /**
         * Method called when connection to RTT failed
         *
         * @param errorMessage - Text message describing the error.
         */
        virtual void rttConnectFailure(const std::string& errorMessage) = 0;
    };
    
};

#pragma clang diagnostic pop
