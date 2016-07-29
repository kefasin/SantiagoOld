#include "UserController.h"

namespace Santiago{ namespace Authentication
    {
        void UserController::handleClientMessage(const ServerMessage& serverMessage_)
        {
            
            switch(serverMessage_._connectionMessage._type)
            {
            case MessageType::NEW_USER:
                addNewUser(serverMessage_._connectionMessage._parameters[0],
                           serverMessage_._connectionMessage._parameters[1]);
                break;
            case MessageType::LOGIN_USER:
                loginUser(serverMessage_._connectionMessage._parameters[0],
                          serverMessage_._connectionMessage._parameters[1]);
                break;
            case MessageType::LOGOUT_USER:
                logoutUser(serverMessage_._connectionMessage._parameters[0]);
                break;
            case MessageType::CHANGE_PASS_USER:
                changePasswordForUser(serverMessage_._connectionMessage._parameters[0],
                                            serverMessage_._connectionMessage._parameters[1],
                                            serverMessage_._connectionMessage._parameters[2]);
                break;
            case MessageType::CHANGE_PASS_AND_LOGOUT_USER:
                changePasswordAndLogoutUser(serverMessage_._connectionMessage._parameters[0],
                                            serverMessage_._connectionMessage._parameters[1],
                                            serverMessage_._connectionMessage._parameters[2]);
                break;
            case MessageType::VERIFY_USER:
                verifyUser(serverMessage_._connectionMessage._parameters[0],
                           serverMessage_._connectionMessage._parameters[1]);
            }
            
        }
        
        void UserController:: addNewUser(const std::string& userId_, const std::string& password_)
        {
            _databaseConnector.addUserProfileRecord(userId_, password_);
        }
        
        void UserController::loginUser(const std::string& userId_, const std::string& password_)
        {
            std::string cookieId;
            //TODO Initialize cookieId
            time_t rawTime;
            struct tm * timeInfo;
            time (&rawTime);
            timeInfo = localtime (&rawTime);
            time_t newTime = timegm(timeInfo);
            ptime loginTime = from_time_t(newTime);
            _databaseConnector.addSessionRecord(userId_, cookieId, loginTime);
        }
        
        void UserController::logoutUser(const std::string& userId_)
        {
            time_t rawTime;
            struct tm * timeInfo;
                time (&rawTime);
                timeInfo = localtime (&rawTime);
                time_t newTime = timegm(timeInfo);
                ptime logoutTime = from_time_t(newTime);
                _databaseConnector.updateSessionRecord(userId_, logoutTime);
        }
        
        void UserController::changePasswordForUser(const std::string& userId_,
                                                   const std::string oldPassword_,const std::string& newPassword_)
        {
            _databaseConnector.updateUserPassword(userId_, oldPassword_, newPassword_);
            //update only if oldPassword_ is matching
        }
        
        void UserController::changePasswordAndLogoutUser(const std::string& userId_,
                                                         const std::string oldPassword_,
                                                         const std::string& newPassword_)
        {
            _databaseConnector.updateUserPassword(userId_, oldPassword_, newPassword_);
            logoutUser(userId_);
             //update only if oldPassword_ is matching
        }
        
        
        void UserController::verifyUser(const std::string& userId_, const std::string& cookieString_)
        {    
            bool match = 0;
            auto searchCookie = _userIdConnectionPtrCookiePairMap.find(userId_);
            if(searchCookie != _userIdConnectionPtrCookiePairMap.end())
            {
                if(searchCookie->second->second == cookieString_)
                {
                    match = 1;
                }
            }
            
            if(match)
            {
                time_t rawTime;
                struct tm * timeInfo;
                time (&rawTime);
                timeInfo = localtime (&rawTime);
                time_t newTime = timegm(timeInfo);
                ptime loginTime = from_time_t(newTime);
                _databaseConnector.addSessionRecord(userId_, cookieString_, loginTime);
            }
            
            else
            {
                //No matching userId_ and cookieString_
            }
        }
    }}//closing Santiago::Authentication
