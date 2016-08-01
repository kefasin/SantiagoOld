#include "Message.h"

namespace Santiago{ namespace Authentication
{   
    ConnectionMessage::ConnectionMessage(const std::string& inputString_)
    {
        int size = inputString_.size();
        std::string typeString = inputString_.substr(0, 4);
        std::map<int, MessageType> type;
        type = {{65, MessageType::NEW_USER},
                {66, MessageType::LOGIN_USER},
                {67, MessageType::LOGOUT_USER},
                {68, MessageType::CHANGE_PASS_USER},
                {69, MessageType::CHANGE_PASS_AND_LOGOUT_USER},
                {70, MessageType::VERIFY_USER}};
        _type = type.find(*(int*)typeString.c_str())->second;
        int startPos = 8;
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
        int parasize = _parameters.size();
        messageString.write((const char*)&(parasize), sizeof(parasize));
        for(auto it = _parameters.begin(); it != _parameters.end(); ++it)
        {
            int size = (*it).size();
            messageString.write((const char*)&(size), sizeof(size));
            messageString << *it; 
        }
        return messageString.str();
    }
}}//closing Santiago::Authentication 
