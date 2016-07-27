#include "Authentication/Message.h"
#include <iostream>

int main()
{
    std::stringstream str;
    int temp, tp, size, nop;
    std::string tempStr;
    std::cout << "Enter number of parameters:";
    std::cin >> nop;
    size = 12 + (nop * 4); 
    std::vector<int> parameterSize;
    std::vector<std::string> parameter;
    for(int i = 0 ; i < nop ; ++i)
    {
        std::cin >> tempStr;
        parameterSize.push_back(tempStr.size());
        parameter.push_back(tempStr);
    }
    for(auto it = parameterSize.begin(); it != parameterSize.end(); ++it)
    {
        size += *it;
    }
    MessageType type = MessageType::NEW_USER;
    std::map<MessageType, int> enumInt;
    enumInt = {{MessageType::NEW_USER, 65},
               {MessageType::LOGIN_USER, 66},
               {MessageType::LOGOUT_USER, 67},
               {MessageType::CHANGE_PASS_USER, 68},
               {MessageType::CHANGE_PASS_AND_LOGOUT_USER, 69},
               {MessageType::VERIFY_USER, 70}};
    tp = enumInt.find(type)->second;
    //str.write(reinterpret_cast<char*>(&size), sizeof(int)); //if no.of bytes is there in constructor input string
    str.write(reinterpret_cast<char*>(&tp), sizeof(int));
    str.write(reinterpret_cast<char*>(&nop), sizeof(int));
    auto it2 = parameter.begin();
    for(auto it1 = parameterSize.begin(); it1 != parameterSize.end(); ++it1, ++it2)
    {
        int paraSize = *it1;
        str.write(reinterpret_cast<char*>(&paraSize), sizeof(int));
        str << *it2;
    }
    //std::cout << "\n" << str.str(); 
    Santiago::Authentication::ConnectionMessage message(str.str());
    std::cout << "\n" << enumInt.find(message._type)->second << "\n";
    for(auto it = message._parameters.begin(); it != message._parameters.end(); ++it)
    {
        temp = (*it).size();
        std::cout << temp << "\t" << *it << "\n";
    }
    std::string msg = message.getMessageString();
    std::cout << msg; //printing str.str() and msg give same result
    return 0;
}
