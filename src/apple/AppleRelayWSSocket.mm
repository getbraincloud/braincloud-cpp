#include "braincloud/internal/apple/AppleRelayWSSocket.h"
#if (!defined(TARGET_OS_WATCH) || TARGET_OS_WATCH == 0)

@interface RelayWebSocket() <SRWebSocketDelegate>
{
    SRWebSocket* _webSocket;
    BrainCloud::IRelayRocketSocketCallback* _callback;
}
@end

@implementation RelayWebSocket

-(void)dealloc
{
}

- (id)initWithAddress:(const std::string&)address
                 port:(int)port
               useSSL:(bool)useSSL
             callback:(BrainCloud::IRelayRocketSocketCallback*)callback
{
    _callback = callback;

    NSString* url;
    if (address.find("://") != std::string::npos)
    {
        // A full URL (secureUrl) is used as given; port is ignored.
        url = [NSString stringWithUTF8String:address.c_str()];
    }
    else
    {
        NSString* scheme = useSSL ? @"wss" : @"ws";
        url = [NSString stringWithFormat:@"%@://%s:%i/", scheme, address.c_str(), port];
    }

    _webSocket = [[SRWebSocket alloc] initWithURL:[NSURL URLWithString:url]];
    _webSocket.delegate = self;

    [_webSocket open];

    return [super init];
}

- (void)send:(const uint8_t*)data size:(int)size
{
    if (_webSocket)
    {
        NSData* nsData = [NSData dataWithBytes:data length:size];
        [_webSocket sendData:nsData error:NULL];
    }
}

- (void)close
{
    if (_webSocket)
    {
        [_webSocket close];
        _webSocket = nil;
    }
}

///--------------------------------------
#pragma mark - SRWebSocketDelegate
///--------------------------------------

- (void)webSocketDidOpen:(SRWebSocket *)webSocket
{
    NSLog(@"Relay WebSocket connected");
    _callback->onOpen();
}

- (void)webSocket:(SRWebSocket *)webSocket didFailWithError:(NSError *)error
{
    NSLog(@"Relay WebSocket failed with error %@", error);
    _webSocket = nil;
    _callback->onError();
}

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessage:(id)message
{
    if ([message isKindOfClass:[NSData class]])
    {
        NSData* data = (NSData*)message;
        _callback->onRecv((const uint8_t*)data.bytes, (int)data.length);
    }
}

- (void)webSocket:(SRWebSocket *)webSocket didReceiveMessageWithData:(NSData *)data
{
    _callback->onRecv((const uint8_t*)data.bytes, (int)data.length);
}

- (void)webSocket:(SRWebSocket *)webSocket didCloseWithCode:(NSInteger)code reason:(NSString *)reason wasClean:(BOOL)wasClean
{
    NSLog(@"Relay WebSocket closed");
    _webSocket = nil;
    _callback->onClose();
}

@end

namespace BrainCloud
{
    IRelayWSSocket* IRelayWSSocket::create(const std::string& address, int port, int maxPacketSize, bool useSSL)
    {
        return new AppleRelayWSSocket(address, port, maxPacketSize, useSSL);
    }

    AppleRelayWSSocket::AppleRelayWSSocket(const std::string& address, int port, int maxPacketSize, bool useSSL)
        : _isValid(false)
        , _isConnecting(true)
        , _webSocket(NULL)
    {
        _webSocket = [[RelayWebSocket alloc] initWithAddress:address port:port useSSL:useSSL callback:this];
    }

    AppleRelayWSSocket::~AppleRelayWSSocket()
    {
        close();
    }

    bool AppleRelayWSSocket::isConnected()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return !_isConnecting && _isValid;
    }

    bool AppleRelayWSSocket::isValid()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _isConnecting || _isValid;
    }

    void AppleRelayWSSocket::updateConnection()
    {
        // SRWebSocket is event-driven; nothing to poll here
    }

    void AppleRelayWSSocket::send(const uint8_t* pData, int size)
    {
        if (_webSocket)
        {
            [_webSocket send:pData size:size];
        }
    }

    const uint8_t* AppleRelayWSSocket::peek(int& size)
    {
        size = 0;
        std::unique_lock<std::mutex> lock(_recvMutex);
        if (_recvQueue.empty()) return nullptr;
        _currentPacket = std::move(_recvQueue.front());
        _recvQueue.pop();
        size = (int)_currentPacket.size();
        return _currentPacket.data();
    }

    void AppleRelayWSSocket::close()
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _isValid = false;
            _isConnecting = false;
            _connectionCondition.notify_all();
        }

        {
            std::unique_lock<std::mutex> lock(_recvMutex);
            while (!_recvQueue.empty()) _recvQueue.pop();
        }

        if (_webSocket)
        {
            [_webSocket close];
            _webSocket = nil;
        }
    }

    void AppleRelayWSSocket::onOpen()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _isValid = true;
        _isConnecting = false;
        _connectionCondition.notify_all();
    }

    void AppleRelayWSSocket::onClose()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _isValid = false;
        _isConnecting = false;
        _connectionCondition.notify_all();
        _webSocket = nil;
    }

    void AppleRelayWSSocket::onError()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _isValid = false;
        _isConnecting = false;
        _connectionCondition.notify_all();
        _webSocket = nil;
    }

    void AppleRelayWSSocket::onRecv(const uint8_t* data, int size)
    {
        std::unique_lock<std::mutex> lock(_recvMutex);
        _recvQueue.push(std::vector<uint8_t>(data, data + size));
    }
};

#endif /* TARGET_OS_WATCH */
