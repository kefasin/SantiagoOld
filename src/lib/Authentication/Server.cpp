#include "Server.h"

namespace Santiago{ namespace Authentication
    {
        void Server::start()
        {
            MySocketPtr socketPtr(new MySocket(_acceptor.get_io_service()));
            _acceptor.async_accept(*socketPtr,
                                   boost::bind(&Server::handleAccept, this, socketPtr,
                                               boost::asio::placeholders::error));
        }
        
        void Server::handleAccept(MySocketPtr socketPtr_,
                                  const boost::system::error_code& error_)
        {
            if (!error_)
            {
                std::function<void(unsigned)> onDisconnectCallbackFn = 
                    std::bind(&Server::handleDisconnect,this,_nextConnectionId);
                std::function<void(unsigned,const ConnectionMessage)> onMessageCallbackFn = 
                    std::bind(&Server::handleClientMessage,this,_nextConnectionId,std::placeholders::_2);
                
                TCPConnectionPtr newConnection(new TCPConnection(socketPtr_,onDisconnectCallbackFn,
                                                                 onMessageCallbackFn));
                //_idConnectionPtrMap.insert(std::pair<unsigned,TCPConnectionPtr>
                //(_nextConnectionId,newConnection));
                _idConnectionPtrMap[_nextConnectionId] = newConnection;
                ++_nextConnectionId;
            } 
            
            start();
        }
        
        void Server::handleDisconnect(unsigned connectionId_)
        {
            _idConnectionPtrMap.erase(connectionId_);
        }
    }} // closing Santiago::Authentication
