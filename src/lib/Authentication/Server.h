#ifndef SANTIAGO_AUTHENTICATION_SERVER_H
#define SANTIAGO_AUTHENTICATION_SERVER_H

#include <boost/asio/accector.hpp>

#include "TCPConnection.h"

namespace Santiago{ namespace Authentication
{
    class Server
    {
    public:

        typedef TCPConnection::Ptr TCPConnectionPtr;

        //  typedef UserController::Ptr UserControllerPtr;

        Server(boost::asio::io_service& ioService_,int port_)
            :_acceptor(ioService_, tcp::endpoint(tcp::v4(),port_),_nextConnectionId(1))
        {
            start();
        }

        void start();

    protected:


        void handleAccept(const TCPConnectionPtr connectionPtr_,
                          const boost::system::error_code& error_);
        void handleDisconnect(unsigned connectionId_);
        void handleClientMessage(unsigned connectionId_,
                                 const ConnectionMessage& connectionMessage_)
        {
            //for now assume all messages are USER messages.
            // Later when implementing ResourceController check if they are USER or RESOURCE message
            ServerMessage serverMessage(connectionId_,connectionMessage_);
            _userController.handleClientMessage(serverMessage_);
        }

        boost::asio::error_code sendMessageCallbackFn(const ServerMessage& serverMessage_)
        {
            //TODO need to check if such a connection still exists
            return _idConnectionPtrMap[serverMessage_._connectionId]->sendMessage(serverMessage_._connectionMessage);
        }
        
        tcp::acceptor                           _acceptor;
        std::map<unsigned,TCPConnectionPtr>          _idConnectionPtrMap;
        unsigned                                _nextConnectionId;
        UserController                          _userController;
        
    };
}}


#endif
