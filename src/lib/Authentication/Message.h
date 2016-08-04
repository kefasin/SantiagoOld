#ifndef SANTIAGO_AUTHENTICATION_MESSAGE_H
#define SANTIAGO_AUTHENTICATION_MESSAGE_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <string>
namespace Santiago{ namespace Authentication
{

    /***********************************************************
     * MessageType
     ***********************************************************/

    enum class MessageType
    {
       /* First 4 bits used to signify the type of message.
          First bit signifies USER authentication message.
          Second bit signifies RESOURCE authentication message*/

        SUCCEEDED                       = 1,
        FAILED                          = 0,
        CREATE_USER                     = 65, //0b1000001 
        LOGIN_USER                      = 66, //0b1000010
        VERIFY_USER_FOR_COOKIE          = 67, //0b1000011
        LOGOUT_USER_FOR_COOKIE          = 68, //0b1000100
        LOGOUT_USER_FOR_ALL_COOKIES     = 69, //0b1000101
        CHANGE_USER_PASSWORD            = 70, //0b1000110
    };

    /***********************************************************
     * ConnectionMessage
     ***********************************************************/

    struct ConnectionMessage
    {
        ConnectionMessage(const char* content_,unsigned size_);         

        ConnectionMessage(MessageType type_,const std::vector<std::string>& parameters_);

        std::ostream& writeToStream(std::ostream& outStream_) const;
        unsigned getSize() const;
        
        MessageType               _type;
        std::vector<std::string>  _parameters;
    };

    inline std::ostream& operator<<(std::ostream& ostream_, const ConnectionMessage& connectionMessage_)
    { return connectionMessage_.writeToStream(ostream_);}

    /***********************************************************
     * ServerMessage
     ***********************************************************/

    struct ServerMessage
    {
        ServerMessage(unsigned connectionId_,const ConnectionMessage& connectionMessage_);

        unsigned             _connectionId;
        ConnectionMessage    _connectionMessage;
    };

}} //closing namespace Santiago::Authentication

#endif
