#include "Message.h"

namespace Santiago{ namespace Authentication
{
    /*
      TODO:
      1) TCPConnection::intReceive cannot be called here...better not make a separate fn for this at all and do at that
      in place. TCPConnection::intReceive uses strtol...this also will not work since we are not parsing the string.
      Instead try int size = *(reinterpret_cast<unsigned*>(string.c_str())) for reading. For writing pls use strncpy
      with reinterpret_cast. Pls make a separate free fn for this

      2) Make a small test program in the src/test/Authentication directory to test this 2 fns.
     */

    ConnectionMessage::ConnectionMessage(const std::string& inputString_)
    {
        int size = inputString_.size();
        std::string typeString = inputString_.substr(4, 4);
        std::map<int, MessageType> type;
        type = {{65, MessageType::NEW_USER}, {66, MessageType::LOGIN_USER},
                {67, MessageType::LOGOUT_USER},
                {68, MessageType::CHANGE_PASS_USER},
                {69, MessageType::CHANGE_PASS_AND_LOGOUT_USER},
                {70, MessageType::VERIFY_USER}};
        _type = type.find(*(int*)typeString.c_str())->second;
        // std::string parametersNumString = inputString_.substr(8, 4);
        //  int parametersNum = *(int*)parametersNumString.c_str();
        int startPos = 12;
        while(startPos < size)
        {
            std::string parameterSizeString = inputString_.substr(startPos, 4);
            int parameterSize = *(int*)parameterSizeString.c_str();
            startPos += 4;
            std::string parameter = inputString_.substr(startPos, parameterSize);
            _parameters.push_back(parameter);
            startPos += parameterSize;
        }
        
    }
      
    std::string ConnectionMessage::getMessageString() const
    {
        std::vector<std::string>::iterator it;
        std::stringstream messageString;
        std::map<MessageType, int> enumType;
        enumType = {{MessageType::NEW_USER, 65},
                {MessageType::LOGIN_USER, 66},
                {MessageType::LOGOUT_USER, 67},
                {MessageType::CHANGE_PASS_USER, 68},
                {MessageType::CHANGE_PASS_AND_LOGOUT_USER, 69},
                {MessageType::VERIFY_USER, 70}};
        int type = enumType.find(_type)->second;
        messageString.write((const char*)&type, sizeof(type));
        messageString.write((const char*)&(_parameters.size()), sizeof(_parameters.size()));
        for(it = _parameters.begin(); it != _parameters.end(); ++it)
        {
            it size = *it.size();
            messageString.write((const char*)&(size), sizeof(size));
            messageString << *it; 
        }
        return messageString.str();
    }
}}//closing Santiago::Authentication 
