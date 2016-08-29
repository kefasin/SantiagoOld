#include " AddResourceRequestHandler.h"

namespace Santiago{ namespace Authentication
{

    AddResourceRequestHandler:: AddResourceRequestHandler(ConnectionServer& connectionServer_
                                                          ,const OnCompletedCallbackFn& onCompletedCallbackFn_
                                                          ,const ServerMessage& initiatingMessage_)
        :RequestHandlerBase(connectionServer_,onCompletedCallbackFn_,initiatingMessage_)
    {}
    
    virtual void  AddResourceRequestHandler::start()
    {
        if(_databaseConnector.addPermissionrecord(_initiatingMessage._connectionMessage._parameters[0],
                                                  _initiatingMessage._connectionMessage._parameters[1],
                                                  Database::UserPermission::OWNER))
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
    
    virtual void  AddResourceRequestHandler::handleReplyMessage(const ServerMessage& serverMessage)
    {
        BOOST_ASSERT(false);
    }

}}
