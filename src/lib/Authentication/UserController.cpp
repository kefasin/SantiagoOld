#include "UserController.h"

namespace Santiago{ namespace Authentication
{
    void UserController::handleClientMessage(const ServerMessage& serverMessage_)
    {
        switch(serverMessage_._connectionMessage._type)
        {
            
        case MessageType::CREATE_USER:
            createUser(serverMessage_._connectionMessage._parameters[0],
                       serverMessage_._connectionMessage._parameters[1]);
            break;

        case MessageType::LOGIN_USER:
            loginUser(serverMessage_._connectionId,
                      serverMessage_._connectionMessage._parameters[0],
                      serverMessage_._connectionMessage._parameters[1]);
            break;
            
        case MessageType::VERIFY_USER_FOR_COOKIE:
            verifyUserForCookie(serverMessage_._connectionMessage._parameters[0]);
            break;
            
        case MessageType::LOGOUT_USER_FOR_COOKIE:
            logoutUserForCookie(serverMessage_._connectionMessage._parameters[0]);
            break;
            
        case MessageType::LOGOUT_USER_FOR_ALL_COOKIES:
            logoutUserForAllCookies(serverMessage_._connectionMessage._parameters[0]);
                break;
                
        case MessageType::CHANGE_USER_PASSWORD:
            changeUserPassword(serverMessage_._connectionMessage._parameters[0],
                               serverMessage_._connectionMessage._parameters[1],
                               serverMessage_._connectionMessage._parameters[2]);
            break;
            
        default:
            BOOST_ASSERT(false);
            break;
        }            
    }
    
    void UserController:: createUser(const std::string& userId_, const std::string& password_)
    {
        //TODO: the addUserProfileRecord should check for already existing users of the same id.
        //TODO: addUserProfileRecord should return true if succees. Else return failed
        //TODO: inform the result to the client using the _sendMessageCallbackFn
        _databaseConnector.addUserProfileRecord(userId_, password_);
        }
    
    void UserController::loginUser(unsigned connectionId_,
                                   const std::string& userId_,
                                   const std::string& password_)
    {
        //TODO: find a way to generate unique cookie. Use connectionId_ to differentiate simultaneous
        //calls if we are using time based generation
        //TODO: check if the user name password matches
        //TODO: use _sendMessageCallbackFn to return the result. FAILED has no parameter
        // SUCCESS has cookie as the parameter
        
        std::string cookieId;
        //TODO Initialize cookieId
        time_t rawTime;
        struct tm * timeInfo;
        time (&rawTime);
        timeInfo = localtime (&rawTime);
        time_t newTime = timegm(timeInfo);
        ptime loginTime = from_time_t(newTime);
        _databaseConnector.addSessionRecord(userId_, cookieId, loginTime);

        //TODO: Modify the _cookiedCookieDataMap and _userIdUserIdDataMap like below
/*
        std::lock_guard<std::mutex>  lock(mtx);
        _cookieCookieDataMap[cookie] = CookieData{userId_,connectionId_};
        _userIdUserDataMap[userId_]._cookieList.push_back(cookie);
*/
        }

    void UserController::verifyUserForCookie(const std::string& cookie_)
    {    
        bool match = (_cookieCookieDataMap.find(cookie_) != _cookieCookieDataMap.end());
        //TODO: return the SUCCEEDED with userid 
        //TODO: if the verify call from another connection add that connection to the _userIdUserIdDataMap
        
        if(match)
        {
            time_t rawTime;
            struct tm * timeInfo;
            time (&rawTime);
            timeInfo = localtime (&rawTime);
            time_t newTime = timegm(timeInfo);
            ptime loginTime = from_time_t(newTime);
            _databaseConnector.addSessionRecord(_cookieCookieDataMap[cookie_]._userId, cookie_, loginTime);
        }
        
        else
        {
            //No matching userId_ and cookieString_
        }
    }

    void UserController::logoutUserForCookie(const std::string& userId_)
    {
        //TODO: has some work..skip this for now
        time_t rawTime;
        struct tm * timeInfo;
        time (&rawTime);
        timeInfo = localtime (&rawTime);
        time_t newTime = timegm(timeInfo);
        ptime logoutTime = from_time_t(newTime);
        _databaseConnector.updateSessionRecord(userId_, logoutTime);
    }

    void logoutUserForAllCookies(const std::string& userId_)
    {
        //TODO: has some work..skip for now
    }

    void UserController::changeUserPassword(const std::string& userId_,
                                            const std::string& oldPassword_,
                                            const std::string& newPassword_)
    {
        _databaseConnector.updateUserPassword(userId_, oldPassword_, newPassword_);
        //update only if oldPassword_ is matching
        //TODO: send SUCEEDED or FAILED message
    }


}}//closing Santiago::Authentication
