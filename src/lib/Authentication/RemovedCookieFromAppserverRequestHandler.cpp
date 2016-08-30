#include "RemovedCookieFromAppserverRequestHandler.h"


namespace Santiago{ namespace Authentication
{

    RemovedCookieFromAppserverRequestHandler::LogoutUserForCookieRequestHandler(ConnectionServer& connectionServer_
                                                                                ,const OnCompletedCallbackFn& onCompletedCallbackFn_
                                                                                ,const ServerMessage& initiatingMessage_)
        :RequestHandlerBase(connectionServer_,onCompletedCallbackFn_,initiatingMessage_)
    {}
    
    virtual void RemovedCookieFromAppserverRequestHandler::start()
    {
        std::string removedCookie = _initiatingMessage._connectionMessage._parameters[0];
        
        CookieData cookieData = _serverData._cookieCookieDataMap.find(removedCookie)->second;
        if(cookieData._connectionIds.erase(_connectionServer._nextConnectionId))
        {
            std::string userId = cookieData._userId;

            UserIdData userIdData = _serverData._userIdUserIdDataMap.find(userId)->second;
            userIdData._cookieList.erase(removedCookie);
        }
    }
    
    virtual void RemovedCookieFromAppserverRequestHandler::handleReplyMessage(const ServerMessage& serverMessage)
    { 
        BOOST_ASSERT(false);
    }
    
}}
