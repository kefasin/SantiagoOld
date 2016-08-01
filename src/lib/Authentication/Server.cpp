#include "Server.h"

namespace Santiago{ namespace Authentication
    {
        void Server::startAccept()
        {
            TCPConnection::MySocketPtr socketPtr(new TCPConnection::MySocket(_acceptor.get_io_service()));
            _acceptor.async_accept(*socketPtr,
                                   boost::bind(&Server::handleAccept, this, socketPtr,
                                               boost::asio::placeholders::error));
        }
        
        void Server::handleAccept(const TCPConnection::MySocketPtr socketPtr_,
                                  const boost::system::error_code& error_)
        {
            if (!error_)
            {
                std::function<void()> onDisconnectCallbackFn = 
                    std::bind(&Server::handleDisconnect,this,_nextConnectionId);
                std::function<void(const ConnectionMessage)> onMessageCallbackFn = 
                    std::bind(&Server::handleClientMessage,this,_nextConnectionId,std::placeholders::_1);
                
                TCPConnectionPtr newConnection(new TCPConnection(socketPtr_,onDisconnectCallbackFn,
                                                                 onMessageCallbackFn));
                newConnection->startRead();
                _idConnectionPtrMap[_nextConnectionId] = newConnection;
                ++_nextConnectionId;
            } 
            
            startAccept();
        }
        
        void Server::handleDisconnect(unsigned connectionId_)
        {
            _idConnectionPtrMap.erase(connectionId_);
        }

        
        void handleClientMessage(unsigned connectionId_,
                                 const ConnectionMessage& connectionMessage_)
        {
             //for now assume all messages are USER messages.
            // Later when implementing ResourceController check if they are USER or RESOURCE message
            ServerMessage serverMessage(connectionId_,connectionMessage_);
        }

        
        boost::system::error_code sendMessageCallbackFn(const ServerMessage& serverMessage_)
        {
            //TODO need to check if such a connection still exists
            _userController.handleClientMessage(serverMessage_);
            return _idConnectionPtrMap[serverMessage_._connectionId]->sendMessage(serverMessage_._connectionMessage);
        }
        
    }
}// closing Santiago::Authentication
