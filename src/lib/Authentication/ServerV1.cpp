#include "ServerV1.h"

namespace Santiago{ namespace Authentication
{
    Server::Server(boost::asio::io_service& ioService_,int port_)
        :_ioService(ioService_)
        ,_port(port_)
        ,_connectionServer(_ioService
                           ,_port
                           ,std::bind(&Server::handleDisconnect,this,std::placeholders::_1)
                           ,std::bind(&Server::handleRequestNew,this,std::placeholders::_1)
                           ,std::bind(&Server::handleRequestReply,this,std::placeholders::_1))
    {}

    void Server::start()
    {
        _connectionServer.start();
    }
    
    
    void Server::handleDisconnect(unsigned connectionId_)
    {
        //TODO
    }

    void Server::handleRequestNew(const ServerMessage& message_)
    {
        //TODO: create request handler shared_ptr of the correct type using message_._connectionMessage->_type
        //push that into the ServerData._activeRrequestHandlersList
        //call requestHandlerPtr->start()
        RequestHandlerBasePtr requestHandlerPtr;
        ServerData serverData;
        serverData._activeRequestHandlersList.insert
        requestHandlerPtr->start()
        
    }

    void Server::handleRequestReply(const ServerMessage& message_)
    {
        std::map<RequestId,RequestHandlerBasePtr>::iterator iter =
            _serverData._activeRequestHandlersList.find(message_._requestId);

        BOOST_ASSERT(iter == _serverData._activeRequestHandlersList.end());
        
        iter->second->handleReplyMessage(message_);
    }

    void Server::handleRequestCompleted(const RequestId& requestId_)
    {
        std::map<RequestId,RequestHandlerBasePtr>::iterator iter =
            _serverData._activeRequestHandlersList.find(message_._requestId);
        
        BOOST_ASSERT(iter == _serverData._activeRequestHandlersList.end());

        _serverData._activeRequestHandlersList.erase(iter);
    }


}} //closing namespace Santiago::Authentication
