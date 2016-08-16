#include "ServerV1.h"

namespace Santiago{ namespace Authentication
{

    void Server::handleDisconnect(unsigned connectionId_)
    {
        //TODO
    }

    void Server::handleRequestNew(const ServerMessage& message_)
    {
        //TODO: create request handler shared_ptr of the correct type using message_._connectionMessage->_type
        //push that into the ServerData._axtiveRrequestHandlersList
        //call requestHanglerPtr->start()
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
