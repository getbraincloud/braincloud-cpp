//
//  INetworkErrorCallback.h
//  GameClientLib
//
//  Created by brainCloud Team on 2015-10-29.
//
//

#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

namespace BrainCloud {
    
    class ServiceName;
    class ServiceOperation;
    class OperationParam;
    
    class INetworkErrorCallback
    {
    public:
        
        /**
         * The networkError method is invoked whenever a network error is encountered
         * communicating to the brainCloud server.
         *
         * Note this method is *not* invoked when FlushCachedMessages(true) is called.
         */
        virtual void networkError() = 0;
    };
    
};
#pragma clang diagnostic pop
