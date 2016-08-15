#ifndef SANTIAGO_AUTHENTICATION_SERVER_H
#define SANTIAGO_AUTHENTICATION_SERVER_H

//#include <boost/asio/acceptor.hpp>
#include <boost/asio/socket_acceptor_service.hpp>
#include <boost/asio/error.hpp>

#include "ConnectionServer.h"
#include "RequestHandlerBase.h"

using boost::asio::ip::tcp;

namespace Santiago{ namespace Authentication
{
    class Server
    {
    public:

        typedef std::shared_ptr<RequestHandlerBase> RequestHandlerBasePtr;

        Server(boost::asio::io_service& ioService_,int port_);

        void start();

    protected:
        
        void handleDisconnect(unsigned connectionId_);
        void handleRequestNew(const ServerMessage& message_);
        void handleRequestReply(const ServerMessage& message_);
        void handleRequestCompleted(const RequestId& requestId_);
        
        std::map<std::string,CookieData>           _cookieCookieDataMap;
        std::map<std::string,UserIdData>           _userIdUserIdDataMap;
        std::map<RequestId,RequestHandlerBasePtr>  _activeRequestHandlersList;

    };
}}


#endif
