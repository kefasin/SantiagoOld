#ifndef SANTIAGO_AUTHENTICATION_CONNECTIONREQUESTSCONTROLLER_H
#define SANTIAGO_AUTHENTICATION_CONNECTIONREQUESTSCONTROLLER_H

#include <memory>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "Message.h"
#include "TCPConnection.h"

namespace Santiago{ namespace Authentication
{
    class ConnectionRequestsController
    {
    public:

        typedef std::function<void(const ServerMessage&)> OnNewRequestCallbackFn;
        typedef std::function<void(const ServerMessage&)> OnRequestReplyCallbackFn;
        typedef std::function<void(unsigned)> OnDisconnectCallbackFn;

        ConnectionRequestsController(unsigned connectionId_,
                                     const MySocketPtr& socketPtr_,
                                     const OnDisconnectCallbackFn& onDisconnectCallbackFn_,
                                     const OnNewRequestCallbackFn& onNewRequestCallbackFn_,
                                     const OnRequestReplyCallbackFn& onRequestReplyCallbackFn_);

        void sendMessage(const ServerMessage& message_);
        

    protected:

        void handleTCPConnectionDisconnect();
        void handleTCPConnectionMessage(const RequestId& requestId_, const ConnectionMessage& message_);

        unsigned                      _connectionId;
        TCPConnection                 _tcpConnection;
        OnDisconnectCallbackFn        _onDisconnectCallbackFn;
        OnNewRequestCallbackFn        _onNewRequestCallbackFn;
        OnRequestReplyCallbackFn      _onRequestReplyCallbackFn;

        std::map<RequestId,unsigned>  _replyPendingRequestList; //the pair.second gives the number of pending replies

    };

}}

#endif
