#include "CreateUserRequestHandler.h"


namespace Santiago{namespace Authentication
{
    CreateUserRequestHandler::CreateUserRequestHandler(ConnectionServer& connectionServer_
                                                       ,const OnCompletedCallbackFn& onCompletedCallbackFn_
                                                       ,const ServerMessage& initiatingMessage_)
        :RequestHandlerBase(connectionServer_,onCompletedCallbackFn_,initiatingMessage_)
    {}

    virtual void CreateUserRequestHandler::start()
    {
        if(_databaseConnector.addUserProfileRecord(_initiatingMessage._connectionMessage._parameters[0],
                                                   _initiatingMessage._connectionMessage._parameters[1]))
        {

            ServerMessage serverMessage(_initiatingMessage._connectionId
                                        ,_initiatingMessage._requestId
                                        ,_initiatingMessage._type.CONNECTION_MESSAGE_REPLY
                                        ,_initiatingMessage->_connectionMessage(SUCCEEDED,std::vector<std::string>));
            
            _connectionServer.sendMessage(serverMessage);
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

    virtual void CreateUserRequestHandler::handleReplyMessage(const ServerMessage& serverMessage)
    {
        BOOST_ASSERT(false);
    }

    
}}
