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
                newConnection->start();
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
            ServerMessage serverMessage(connectionId_,connectionMessage_);
        }
        
    }
}// closing Santiago::Authentication
