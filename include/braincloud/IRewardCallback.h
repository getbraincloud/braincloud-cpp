//
//  IRewardCallback.h
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


namespace BrainCloud {
    
    class ServiceName;
    class ServiceOperation;
    class OperationParam;
    
    class IRewardCallback
    {
    public:
        
        /**
         * The rewardCallback method is invoked whenever a brainCloud server API call returns rewards data.
         *
         * @param serviceName The service that returned rewards
         * @param serviceOperation The operation that returned rewards
         * @param jsonData The rewards JSON data. The format is as follows:
         * {
         *   "status": 200,
         *   "apiRewards": [
         *     {
         *       "service": "authenticationV2",
         *       "operation": "AUTHENTICATE",
         *       "rewards": {
         *         "rewardDetails": {
         *           // the reward depending on type (see docs)
         *         }
         *       }
         *     }
         *   ]
         * }
         */
        virtual void rewardCallback(std::string const & jsonData) = 0;
        
    };
    
};
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

