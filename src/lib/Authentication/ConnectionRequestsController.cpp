#include "ConnectionRequestsController.h"

namespace Santiago{ namespace Authentication
{
    void ConnectionRequestsController::handleTCPConnectionDisconnect()
    {
        while(_replyPendingRequestList.begin() != _replyPendingRequestList.end())
        {
            RequestId requestId = _replyPendingRequestList.begin()->first;
            _replyPendingRequestList.erase(_replyPendingRequestList.begin());

            ServerMessage serverMessage(_connectionId, requestId, ServerMessage::CONNECTION_DISCONNECT,boost::none);
            _onRequestReplyCallbackFn(serverMessage);
        }
        _onDisconnectCallbackFn(_connectionId);
    }

    void ConnectionRequestsController::
    handleTCPConnectionMessage(const RequestId& requestId_, const ConnectionMessage& message_)
    {
        if((ConnectionMessageType::SUCCEEDED == message_._type) ||
           (ConnectionMessageType::FAILED == message_._type))
        {
            std::map<RequestId,unsigned>::iterator iter = _replyPendingRequestList.find(requestId_);
            if(_replyPendingRequestList.end() == iter)
            {
                BOOST_ASSERT(false);
                std::runtime_error("Unexpected requestId received");
            }

            --(iter->second);
            if(0 == iter->second)
            {
                _replyPendingRequestList.erase(iter);
            }

            ServerMessage serverMessage(_connectionId,
                                        requestId_,
                                        ServerMessageType::CONNECTION_MESSAGE_REPLY,
                                        message_);

            _onRequestReplyCallbackFn(serverMessage);
        }
        else
        {
            ServerMessage serverMessage(_connectionId,
                                        requestId_,
                                        ServerMessageType::CONNECTION_MESSAGE_NEW,
                                        message_);

            _onRequestNewCallbackFn(serverMessage);
        }
    }

    void ConnectionRequestsController::sendMessage(const ServerMessage& message_)
    {
        BOOST_ASSERT(message_._connectionMessage);
        if(ServerMessageType::CONNECTION_MESSAGE_NEW == message_._type)
        {
            std::map<RequestId,unsigned>::iterator iter = _replyPendingRequestList.find(requestId_);
            if(_replyPendingRequestList.end() == iter)
            {
                _replyPendingRequestList[message_._requestId] = 1;
            }
            else
            {
                ++(iter->second);
            }
        }
        else
        {
            _tcpConnection.sendMessage(message_._requestId,*message_._connectionMessage);
        }
    }

}}
