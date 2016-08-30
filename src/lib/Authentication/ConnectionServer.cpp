#include "ConnectionServer.h"



namespace Santiago{ namespace Authentication
{
    ConnectionServer::ConnectionServer(boost::asio::io_service& ioService_,
                                       unsigned port_,
                                       const OnDisconnectCallbackFn& onDisconnectCallbackFn_,
                                       const OnNewRequestCallbackFn& onNewRequestCallbackFn_,
                                       const OnRequestReplyCallbackFn& onRequestReplyCallbackFn_)
        :_acceptor(ioService_, tcp::endpoint(tcp::v4(),port_))
        ,_onDisconnectCallbackFn(onDisconnectCallbackFn_)
        ,_onNewRequestCallbackFn(onNewRequestCallbackFn_)
        ,_onRequestReplyCallbackFn(onRequestReplyCallbackFn_)
        ,_nextconnectionId(1)
    {}
    
    void ConnectionServer::start()
    {
        ConnectionMessageSocket::MySocketPtr socketPtr
            (new ConnectionMessageSocket::MySocket(_acceptor.get_io_service()));
        _acceptor.async_accept(*socketPtr,
                               boost::bind(&ConnectionServer::handleAccept, this, socketPtr,
                                           boost::asio::placeholders::error));
    }

    void ConnectionServer::handleAccept(const ConnectionMessageSocket::MySocketPtr& socketPtr_,
                                        const boost::system::error_code& error_)
    {
        
        ConnectionRequestsControllerPtr newConnection(new ConnectionRequestsController(_nextconnectionId
                                                                                       ,socketPtr_
                                                                                       ,_onDisconnectCallbackFn
                                                                                       ,_onNewRequestCallbackFn
                                                                                       ,_onRequestReplyCallbackFn));
        BOOST_ASSERT(_idConnectionPtrMap.find(_nextConnectionId) == _idConnectionPtrMap.end());
        _idConnectionPtrMap[_nextConnectionId] = newConnection;
        // newConnection->start();
        ++_nextConnectionId;
    }

    void ConnectionServer handleDisconnect(unsigned connectionId_)
    {
         std::map< std::map<unsigned,ConnectionRequestController>>::iterator iter =
             _idConnectionPtrMap.find(connectionId_);
         
         BOOST_ASSERT(iter == _idConnectionPtrMap.end());
         _idConnectionPtrMap.erase(iter);
    }
    
    
}}
