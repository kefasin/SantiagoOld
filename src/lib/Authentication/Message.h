#ifndef SANTIAGO_AUTHENTICATION_MESSAGE_H
#define SANTIAGO_AUTHENTICATION_MESSAGE_H

#include <string>
#include <vector>

namespace Santiago{ namespace Authentication
{
    enum class MessageType
    {
       /* First 4 bits used to signify the type of message.
          First bit signifies USER authentication message.
          Second bit signifies RESOURCE authentication message*/

        NEW_USER                        = 65, //0b1000001 
        LOGIN_USER                      = 66, //0b1000010
        LOGOUT_USER                     = 67, //0b1000011
        CHANGE_PASS_USER                = 68, //0b1000100
        CHANGE_PASS_AND_LOGOUT_USER     = 69, //0b1000101
        VERIFY_USER                     = 70, //0b1000110
    };

    struct ConnectionMessage
    {
        ConnectionMessage(const std::string& inputString_); //TODO implement in .cpp. this one throws
        

        ConnectionMessage(MessageType type_,
                          const std::vector<std::string>& parameters_)
            :_type(type_)
            ,_parameters(parameters_)
        {}

        std::string getMessageString() const;

        MessageType               _type;
        std::vector<std::string>  _parameters;
    };

    struct ServerMessage
    {
        ServerMessage(unsigned connectionId_,
                      const ConnectionMessage& connectionMessage_)
            :_connectionId(connectionId_)
            ,_connectionMessage(connectionMessage_)
        {}

        unsigned             _connectionId;
        ConnectionMessage    _connectionMessage;
    };

}} //closing namespace Santiago::Authentication

#endif
