#include "Authentication/Message.h"
#include <iostream>

int main()
{
    std::stringstream str;
    int temp, tp, ps1, ps2, size = 30, nop = 2;
    std::string para1, para2;
    para1 = "Junais";
    para2 = "Arun";
    ps1 = para1.size();
    ps2 = para2.size();
    MessageType type = MessageType::NEW_USER;
    std::map<int, MessageType> intenum;
    intenum = {{65, MessageType::NEW_USER},
               {66, MessageType::LOGIN_USER},
               {67, MessageType::LOGOUT_USER},
               {68, MessageType::CHANGE_PASS_USER},
               {69, MessageType::CHANGE_PASS_AND_LOGOUT_USER},
               {70, MessageType::VERIFY_USER}};
    std::map<MessageType, int> enumint;
    enumint = {{MessageType::NEW_USER, 65},
               {MessageType::LOGIN_USER, 66},
               {MessageType::LOGOUT_USER, 67},
               {MessageType::CHANGE_PASS_USER, 68},
               {MessageType::CHANGE_PASS_AND_LOGOUT_USER, 69},
               {MessageType::VERIFY_USER, 70}};
    tp = enumint.find(type)->second;
    str.write((const char*)&size, sizeof(size));
    str.write((const char*)&tp, sizeof(tp));
    str.write((const char*)&nop, sizeof(nop));
    str.write((const char*)&ps1, sizeof(ps1));
    str << para1;
    str.write((const char*)&ps2, sizeof(ps2));
    str << para2;
    Santiago::Authentication::ConnectionMessage message(str.str());
    std::cout << enumint.find(message._type)->second << "\n";
    for(auto it = message._parameters.begin(); it != message._parameters.end(); ++it)
    {
        temp = (*it).size();
        std::cout << temp << "\t" << *it << "\n";
    }
    std::string msg = message.getMessageString();
    std::cout << msg;
    return 0;
}
