#ifndef SANTIAGO_AUTHENTICATION_CONNECTIONSERVER_H
#define SANTIAGO_AUTHENTICATION_CONNECTIONSERVER_H

//#include <boost/asio/acceptor.hpp>
#include <boost/asio/socket_acceptor_service.hpp>
#include <boost/asio/error.hpp>

#include "ConnectionController.h"
using boost::asio::ip::tcp;

namespace Santiago{ namespace Authentication
{
    class ConnectionServer
    {
    public:

        typedef TCPConnection::Ptr TCPConnectionPtr;
        typedef std::function<void(const ServerMessage&)> OnNewRequestCallbackFn;
        typedef std::function<void(const ServerMessage&)> OnRequestReplyCallbackFn;
        typedef std::function<void(unsigned)> OnDisconnectCallbackFn;

        ConnectionServer(boost::asio::io_service& ioService_,
                         int port_,
                         const OnDisconnectCallbackFn& onDisconnectCallbackFn_,
                         const OnNewRequestCallbackFn& onNewRequestCallbackFn_,
                         const OnRequestReplyCallbackFn& onRequestReplyCallbackFn_);

        void start();

    protected:
        
        void handleAccept(const TCPConnection::MySocketPtr& socketPtr_,
                          const boost::system::error_code& error_);

        void handleDisconnect(unsigned connectionId_);

        void sendMessage(const ServerMessage& serverMessage_);
       
        
        tcp::acceptor                           _acceptor;
        std::map<unsigned,ConnectionController> _idConnectionPtrMap;
        unsigned                                _nextConnectionId;        
    };
}}
#endif
