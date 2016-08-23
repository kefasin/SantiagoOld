#ifndef SANTIAGO_AUTHENTICATION_CONNECTIONCONTROLLER_H
#define SANTIAGO_AUTHENTICATION_CONNECTIONCONTROLLER_H

#include <functional>

#include "ServerMessage.h"
#include "Database/MariaDBConnections.h"

namespace Santiago{ namespace Authentication
{
    class RequestHandlerBase
    {
    public:
        typedef std::function<void()> OnCompletedCallbackFn;

        RequestHandlerBase(ConnectionServer& connectionServer_
                           ,const OnCompletedCallbackFn& onCompletedCallbackFn_
                           ,const ServerMessage& initiatingMessage_)
            :_connectionServer(connectionServer_)
            ,_onCompletedCallbackFn(onCompletedCallbackFn_)
            ,_initiatingMessage(initiatingMessage_)
        {}

        virtual void start() = 0;
        virtual void handleReplyMessage(const ServerMessage& serverMessage_) = 0;

    protected:

        ConnectionServer&              _connectionServer;
        OnCompletedCallbackFn          _onCompletedCallbackFn;
        ServerMessage                  _initiatingMessage;

        Database::MariaDBConnections   _databaseConnector; 

    };

}}

#endif
