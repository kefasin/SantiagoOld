#include "ServerMessage.h"

namespace Santiago{ namespace Authentication
{
    RequestId::RequestId(unsigned initiatingConnectionId_,unsigned requestNo_)
        :_initiatingConnectionId(initiatingConnectionId_)
        ,_requestNo(requestNo_)
    {}
   
    ServerMessage::ServerMessage(unsigned connectionId_,
                                 const RequestId& requestId_,
                                 ServerMessageType type_,
                                 const boost::optional<ConnectionMessage>& connectionMessage_)
        :_connectionId(connectionId_)
        ,_requestId(requestId_)
        ,_type(type_)
        ,_connectionMessage(connectionMessage_)
    {}

}}//closing Santiago::Authentication 
