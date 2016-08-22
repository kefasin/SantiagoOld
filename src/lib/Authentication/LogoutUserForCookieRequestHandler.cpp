#include "LogoutUserForCookieRequestHandler.h"

namespace Santiago{ namespace Authentication
{

    LogoutUserForCookieRequestHandler::LogoutUserForCookieRequestHandler(const RequestId& requestId_,
                                                                         ConnectionServer& connectionServer_,
                                                                         const OnCompletedCallbackFn& onCompletedCallbackFn_,
                                                                         const ServerMessage& initiatingMessage_)
        :RequestHandlerBase(connectionServer_,onCompletedCallbackFn_,initiatingMessage_)
    {}
    
    virtual void LogoutUserForCookieRequestHandler::start()
    {
        if(_databaseConnector.logoutUserForCookie(_message._connectionMessage._parameters[0]))
        {
            ServerMessage serverMessage(_initiatingMessage._connectionId
                                        ,_initiatingMessage._requestId
                                        ,_initiatingMessage._type.CONNECTION_MESSAGE_REPLY
                                        ,_initiatingMessage->_connectionMessage(SUCCEEDED,std::vector<std::string>));
            
            _onCompletedCallbackFn(_initiatingMessage._requestId);
        }
        else
        {
            ServerMessage serverMessage(_initiatingMessage._connectionId
                                        ,_initiatingMessage._requestId
                                        ,_initiatingMessage._type.CONNECTION_MESSAGE_REPLY
                                        ,_initiatingMessage->_connectionMessage(FAILED,std::vector<std::string>));
        }
    }
    
    virtual void LogoutUserForCookieRequestHandler::handleReplyMessage(const ServerMessage& serverMessage)
    {
        BOOST_ASSERT(false);
    }
    
}}
