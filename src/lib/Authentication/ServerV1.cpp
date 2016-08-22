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
        switch(message_._connectionMessage->_type)
        {
        case CR_CREATE_USER:
            RequestHandlerBasePtr requestHandlerPtr(new CreateUserRequestHandler
                                                    (_connectionServer
                                                     ,std::bind(&Server:handleRequestCompleted
                                                                ,this,std::placeholders::_1)
                                                     ,message_._connectionMessage));
            
            break;
        case CR_LOGIN_USER:
            RequestHandlerBasePtr requestHandlerPtr(new LoginUserRequestHandler
                                                    (_connectionServer
                                                     ,std::bind(&Server:handleRequestCompleted
                                                                ,this,std::placeholders::_1)
                                                     ,message_._connectionMessage));
            break;
        case CR_VERIFY_USER_FOR_COOKIE:
             RequestHandlerBasePtr requestHandlerPtr(new VerifyUserForCookieRequestHandler
                                                    (_connectionServer
                                                     ,std::bind(&Server:handleRequestCompleted
                                                                ,this,std::placeholders::_1)
                                                     ,message_._connectionMessage));
            break;
        case CR_LOGOUT_USER_FOR_COOKIE:
             RequestHandlerBasePtr requestHandlerPtr(new LogoutUserForCookieRequestHandler
                                                     (_connectionServer
                                                     ,std::bind(&Server:handleRequestCompleted
                                                                ,this,std::placeholders::_1)
                                                      ,message_._connectionMessage));
            break;
        case CR_LOGOUT_USER_FOR_ALL_COOKIES:
            RequestHandlerBasePtr requestHandlerPtr(new LogoutUserForAllCookiesRequestHandler
                                                    (_connectionServer
                                                     ,std::bind(&Server:handleRequestCompleted
                                                                ,this,std::placeholders::_1)
                                                     ,message_._connectionMessage));
            break;
        case CR_CHANGE_USER_PASSWORD:
             RequestHandlerBasePtr requestHandlerPtr(new ChangeUserPasswordRequestHandler
                                                     (_connectionServer
                                                      ,std::bind(&Server:handleRequestCompleted
                                                                 ,this,std::placeholders::_1)
                                                      ,message_._connectionMessage));
             break;
        case SR_LOGOUT_USER_FOR_COOKIE:
            RequestHandlerBasePtr requestHandlerPtr(new LogoutUserForCookieRequestHandler
                                                    (_connectionServer
                                                     ,std::bind(&Server:handleRequestCompleted
                                                                ,this,std::placeholders::_1)
                                                     ,message_._connectionMessage));
            break;
        case SR_LOGOUT_USER_FOR_ALL_COOKIES:
            RequestHandlerBasePtr requestHandlerPtr(new LogoutUserForAllCookiesRequestHandler
                                                    (_connectionServer
                                                     ,std::bind(&Server:handleRequestCompleted
                                                                ,this,std::placeholders::_1)
                                                     ,message_._connectionMessage));
            break;
        }
        _serverData._activeRequestHandlersList.insert(std::make_pair(message_.requestId,requestHandlerPtr));
        requestHandlerPtr->start();
        
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
