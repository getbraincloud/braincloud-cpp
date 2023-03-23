//
//  ServerCall.h
//  BrainCloudLib
//


#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <map>
#include <string>
#include "json/json.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/OperationParam.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"

namespace BrainCloud {

    /**
     * This is a data structure representing a single message to send to the
     * server.
     */
    class ServerCall {

    public:
        ServerCall();
        ServerCall( ServiceName serviceName, ServiceOperation serviceOperation, const Json::Value & data, IServerCallback * callback = NULL );

        virtual ~ServerCall( );

        IServerCallback *    getCallback( ) const               { return( _callback ); }
        const Json::Value *  getPayload( );
        ServiceOperation     getOperation( ) const              { return( _operation ); }
        ServiceName          getService( ) const                { return( _service ); }
        std::string          getPostPayload( ) const            { return( _postPayload ); }
        void                 setPostPayload( std::string val )  { _postPayload = val; }
        bool                 isEndOfBundleMarker() const        { return _isEndOfBundleMarker; }
        void                 setEndOfBundleMarker(bool value)   { _isEndOfBundleMarker = value; }

    protected:

    private:

        IServerCallback *                   _callback;
        Json::Value                         _data;
        ServiceOperation                    _operation;
        ServiceName                         _service;
        Json::Value                         _payload;
        bool                                _isEndOfBundleMarker;

        // Only used for non-bundled posts... 
        std::string                         _postPayload;

    };  // end class


}  // end namespace


#if defined(__clang__)
#pragma clang diagnostic pop
#endif
