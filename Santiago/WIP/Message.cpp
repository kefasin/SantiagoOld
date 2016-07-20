#include "Message.h"


ConnectionMessage::ConnectionMessage(const std::string& inputString_)
{
    int size = inputString_.size();
    std::string typeString = inputString_.substr(32, 32);
    std::map<int, MessageType> type;
    type = {{65, MessageType::NEW_USER}, {66, MessageType::LOGIN_USER},
            {67, MessageType::LOGOUT_USER},
            {68, MessageType::CHANGE_PASS_USER},
            {69, MessageType::CHANGE_PASS_AND_LOGOUT_USER},
            {70, MessageType::VERIFY_USER}};
    _type = type.find(intReceive(typeString)) -> second;
    std::string parametersNumString = inputString_.substr(64, 32);
    int parametersNum = intReceive(parametersNumString);
    int startPos = 96;
    while(startPos < size)
    {
        std::string parameterSizeString = inputString_.substr(startPos, 32);
        int parameterSize = intReceive(parameterSizeString);
        startPos += 32;
        std::string parameter = inputString_.substr(startPos, parameterSize);
        _parameters.push_back(parameter);
        startPos += parameterSize;
    }
    
}

std::string::ConnectionMessage getMessageString() const
{
    std::vector<std::string>::iterator it;
    std::string messageString = "";
    int size = 96;   //No.of characters in string + Message Type(enum) + No.of parameters
    for(it = _parameters.begin(); it != _parameters.end(); ++it)
    {
        size += 32 + (*it).size(); 
    }
    messageString += intSend(size) + intSend(_type) + intSend(_parameters.size());
    for(it = _parameters.begin(); it != _parameters.end(); ++it)
    {
        messageString += intSend((*it).size()) + *it; 
    }
    return messageString;
}
