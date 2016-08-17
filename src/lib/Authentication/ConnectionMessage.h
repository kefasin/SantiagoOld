#ifndef SANTIAGO_AUTHENTICATION_CONNECTION_MESSAGE_H
#define SANTIAGO_AUTHENTICATION_CONNECTION_MESSAGE_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <string>

namespace Santiago{ namespace Authentication
{
    enum class ConnectionMessageType
    {
        /* 2 types of messages: Requests and Replies
         * Requests - CR - Client Request, SR - Server Request
         * Replies - Succeeded, Failed
         */
        SUCCEEDED,
        FAILED,

        CR_CREATE_USER, 
        CR_LOGIN_USER,
        CR_VERIFY_USER_FOR_COOKIE,
        CR_LOGOUT_USER_FOR_COOKIE,
        CR_LOGOUT_USER_FOR_ALL_COOKIES,
        CR_CHANGE_USER_PASSWORD,

        SR_LOGOUT_USER_FOR_COOKIE,
        SR_LOGOUT_USER_FOR_ALL_COOKIES,
    };

    struct ConnectionMessage
    {
        ConnectionMessage(const char* content_,unsigned size_);         

        ConnectionMessage(ConnectionMessageType type_,const std::vector<std::string>& parameters_);

        std::ostream& writeToStream(std::ostream& outStream_) const;
        unsigned getSize() const;

        ConnectionMessageType     _type;
        std::vector<std::string>  _parameters;
    };

    inline std::ostream& operator<<(std::ostream& ostream_, const ConnectionMessage& connectionMessage_)
    { return connectionMessage_.writeToStream(ostream_);}

}} //closing namespace Santiago::Authentication

#endif
