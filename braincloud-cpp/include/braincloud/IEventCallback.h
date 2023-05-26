//
//  IEventCallback.h
//  GameClientLib
//
//  Created by brainCloud Team on 2015-10-29.
//
//

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>

namespace BrainCloud {
    
    class IEventCallback
    {
    public:
        virtual ~IEventCallback( )  { }
        
        /**
         * Method called when an event is returned from brainCloud
         *
         * @param jsonData - returned data from the server
         */
        virtual void eventCallback(std::string const & jsonData) = 0;
    };
    
};
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

