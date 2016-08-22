#include "ChangeUserPasswordRequestHandler.h"

namespace Santiago{ namespace Authentication
{

    ChangeUserPasswordRequestHandler::ChangeUserPasswordRequestHandler(const RequestId& requestId_,
                                                                       ConnectionServer& connectionServer_,
                                                                       const OnCompletedCallbackFn& onCompletedCallbackFn_,
                                                                       const ServerMessage& initiatingMessage_)
        :RequestHandlerBase(requestId_, connectionServer_, onCompletedCallbackFn_), message_(_message)
    {}
    
    virtual void ChangeUserPasswordRequestHandler::start()
        {
            if(_databaseConnector.updateUserPassword(_message._connectionMessage._parameters[0],
                                                  _message._connectionMessage._parameters[1],
                                                     _message._connectionMessage._parameters[2]))
            {
                ServerMessage serverMessage(_initiatingMessage._connectionId
                                            ,_initiatingMessage._requestId
                                            ,_initiatingMessage._type.CONNECTION_MESSAGE_REPLY
                                            ,_initiatingMessage->_connectionMessage(SUCCEEDED,
                                                                                    std::vector<std::string>));
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

    virtual void ChangeUserPasswordRequestHandler::handleReplyMessage(const ServerMessage& serverMessage)
    {
        BOOST_ASSERT(false);
    }
    
}}
