#include "UserController.h"


void UserController::handleClientMessage(const ServerMessage& serverMessage_)
{
    
    switch(serverMessage_._connectionMessage._type)
    {
    case NEW_USER:
        addNewUser(serverMessage_._connectionMessage._parameters[0],
                   serverMessage_._connectionMessage._parameters[1]);
    case LOGIN_USER:
        loginUser(serverMessage_._connectionMessage._parameters[0],
                  serverMessage_._connectionMessage._parameters[1]);
    case LOGOUT_USER:
        logoutUser(serverMessage_._connectionMessage._parameters[0]);
    case CHANGE_PASS_USER:
        changePasswordAndLogoutUser(serverMessage_._connectionMessage._parameters[0],
                                    serverMessage_._connectionMessage._parameters[1]);
    case CHANGE_PASS_AND_LOGOUT_USER:
        changePasswordAndLogoutUser(serverMessage_._connectionMessage._parameters[0],
                                     serverMessage_._connectionMessage._parameters[1],
                                     serverMessage_._connectionMessage._parameters[2]);
    case VERIFY_USER:
        verifyUser(serverMessage_._connectionMessage._parameters[0],
                   serverMessage_._connectionMessage._parameters[1]);
    }

}

void addNewUser(const std::string& userId_, const std::string& password_)
{
    _databaseConnector.addUserProfileRecord(userId_, password_);
}

void loginUser(const std::string& userId_, const std::string& password_)
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

void logoutUser(const std::string& userId_)
{
    time_t rawTime;
    struct tm * timeInfo;
    time (&rawTime);
    timeInfo = localtime (&rawTime);
    time_t newTime = timegm(timeInfo);
    ptime logoutTime = from_time_t(newTime);
    _databaseConnector.updateSessionRecord(userId_, logoutTime);
}

void changePasswordForUser(const std::string& userId_,const std::string& newPassword_)
{
    _databaseConnector.updateUserPassword(userId_, newPassword_);
}

void changePasswordAndLogoutUser(const std::string& userId_,const std::string& newPassword_)
{
    _databaseConnector.updateUserPassword(userId_, newPassword_);
    logoutUser(userId_);
}

void verifyUser(const std::string& userId_, const std::string& cookieString_)
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
        ptime logoutTime = from_time_t(newTime);
        _databaseConnector.addSessionRecord(userId_, cookieId, loginTime);
    }
    
    else
    {
        //No matching userId_ and cookieString_
    }
}
