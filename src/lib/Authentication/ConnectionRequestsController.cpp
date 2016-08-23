#include "ConnectionRequestsController.h"

namespace Santiago{ namespace Authentication
{

    ConnectionRequestsController::ConnectionRequestsController(unsigned connectionId_,
                                                               const MySocketPtr& socketPtr_,
                                                               const OnDisconnectCallbackFn& onDisconnectCallbackFn_,
                                                               const OnNewRequestCallbackFn& onNewRequestCallbackFn_,
                                                               const OnRequestReplyCallbackFn&
                                                               onRequestReplyCallbackFn_)
        :_connectionId(connectionId_)
        ,_onDisconnectCallbackFn(onDisconnectCallbackFn_)
        ,_onNewRequestCallbackFn(onNewRequestCallbackFn_)
        ,_onRequestReplyCallbackFn(onRequestReplyCallbackFn_)
    {
         std::function<void(const ConnectionMessage)> onMessageCallbackFn = 
             std::bind(&ConnectionRequestsController:: handleConnectionMessageSocketMessage,this,
                       std::placeholders::_1,std::placeholders::_2);
          ConnectionMessageSocketPtr newConnectionMessageSocket(new ConnectionMessageSocket
                                                                (socketPtr_,_onDisconnectCallbackFn,
                                                                 onMessageCallbackFn));
          newConnectionMessageSocket->start();
    } 


    
    void ConnectionRequestsController::handleConnectionMessageSocketDisconnect()
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
    handleConnectionMessageSocketMessage(const RequestId& requestId_, const ConnectionMessage& message_)
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
            _connectionmessagesocket.sendMessage(message_._requestId,*message_._connectionMessage);
        }
    }

}}
