#ifndef SANTIAGO_AUTHENTICATION_CONNECTIONCONTROLLER_H
#define SANTIAGO_AUTHENTICATION_CONNECTIONCONTROLLER_H

#include <functional>

#include "ServerMessage.h"

namespace Santiago{ namespace Authentication
{
    class RequestHandlerBase
    {
    public:
        typedef std::function<void()> OnCompletedCallbackFn;

        RequestHandlerBase(const RequestId& requestId_,
                           ConnectionServer& connectionServer_,
                           const OnCompletedCallbackFn& onCompletedCallbackFn_);

        virtual void start() = 0;
        virtual void handleReplyMessage(const ServerMessage& serverMessage_) = 0;

    protected:
        
        ConnectionControllerStore    _connectionControllerStore;
        OnCompletedCallbackFn        _onCompletedCallbackFn;

    };

}}

#endif
