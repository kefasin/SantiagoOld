#ifndef SANTIAGO_AUTHENTICATION_SERVER_H
#define SANTIAGO_AUTHENTICATION_SERVER_H

//#include <boost/asio/acceptor.hpp>
#include <boost/asio/socket_acceptor_service.hpp>
#include <boost/asio/error.hpp>

#include "TCPConnection.h"
#include "UserController.h"
using boost::asio::ip::tcp;

namespace Santiago{ namespace Authentication
{
    class Server
    {
    public:

        typedef TCPConnection::Ptr TCPConnectionPtr;

        Server(boost::asio::io_service& ioService_,int port_)
            :_acceptor(ioService_, tcp::endpoint(tcp::v4(),port_)),_nextConnectionId(1),
             _userController(std::bind(&Server::sendMessageCallbackFn,this,std::placeholders::_1))
        {
            startAccept();
        }

        void startAccept();

    protected:
        
        void handleAccept(const TCPConnection::MySocketPtr socketPtr_,
                          const boost::system::error_code& error_);
        void handleDisconnect(unsigned connectionId_);
        void handleClientMessage(unsigned connectionId_,
                                 const ConnectionMessage& connectionMessage_);     
            //for now assume all messages are USER messages.
            // Later when implementing ResourceController check if they are USER or RESOURCE message
        boost::system::error_code sendMessageCallbackFn(const ServerMessage& serverMessage_)
        {
            //TODO need to check if such a connection still exists
             _userController.handleClientMessage(serverMessage_);
            return _idConnectionPtrMap[serverMessage_._connectionId]->sendMessage(serverMessage_._connectionMessage);
        }
        
        tcp::acceptor                           _acceptor;
        std::map<unsigned,TCPConnectionPtr>          _idConnectionPtrMap;
        unsigned                                _nextConnectionId;
        UserController                          _userController;
        
    };
}}


#endif
