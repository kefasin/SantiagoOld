#include "LoginUserRequestHandler.h"

namespace Santiago{ namespace Authentication
{

    LoginUserRequestHandler::LoginURequestHandler(ConnectionServer& connectionServer_
                                                            ,const OnCompletedCallbackFn& onCompletedCallbackFn_
                                                            ,const ServerMessage& initiatingMessage_)
        :RequestHandlerBase(connectionServer_,onCompletedCallbackFn_,initiatingMessage_)
    {}
    
    virtual void LoginUserRequestHandler::start()
    {
        if(_databaseConnector.loginUser(_initiatingMessage._connectionMessage._parameters[0]
                                        ,_initiatingMessage._connectionMessage._parameters[1]))
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
    
    virtual void LoginUserRequestHandler::handleReplyMessage(const ServerMessage& serverMessage)
    {
        BOOST_ASSERT(false);
    }
    
}}
