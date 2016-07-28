#include "TCPConnection.h"
using boost::asio::ip::tcp;

namespace Santiago{namespace Authentication
{
   
    std::function<void()> onDisconnectCallbackFn;
    std::function<void(const ConnectionMessage)> onMessageCallbackFn;
    int main()
    {
        boost::asio::io_service io_service;
        tcp::acceptor _acceptor(io_service, tcp::endpoint(tcp::v4(), 3000));
        TCPConnection::MySocketPtr socketPtr(new TCPConnection::MySocket(_acceptor.get_io_service()));
        TCPConnection::Ptr newConnection(new TCPConnection(socketPtr
                                                           ,onDisconnectCallbackFn
                                                           ,onMessageCallbackFn));
    }
    }
}
