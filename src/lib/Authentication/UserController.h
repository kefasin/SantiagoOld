#ifndef SANTIAGO_AUTHENTICATION_USERCONTROLLER_H
#define SANTIAGO_AUTHENTICATION_USERCONTROLLER_H

#include <functional>
#include <ctime>
#include "Database/MariaDBConnections.h"
#include "Message.h"
#include "TCPConnection.h"


namespace Santiago{ namespace Authentication
{
    class UserController
    {

    public:

        typedef std::function<boost::system::error_code(ServerMessage)> SendMessageCallbackFn;
       
        UserController(SendMessageCallbackFn sendMessageCallbackFn_)
            :_sendMessageCallbackFn(sendMessageCallbackFn_)
        {}

        void handleClientMessage(const ServerMessage& serverMessage_);
            //here make a large switch of message type,
            //parse the parameter strings into integers for relevant cases, 
            //redirect to different functions for each of the message type
           
    protected:

        void addNewUser(const std::string& userId_, const std::string& password_);
        void loginUser(const std::string& userId_, const std::string& password_);
        void logoutUser(const std::string& userId_);
        void changePasswordForUser(const std::string& userId_,
                                   const std::string oldPassword_,const std::string& newPassword_);
        void changePasswordAndLogoutUser(const std::string& userId_,
                                         const std::string oldPassword_,const std::string& newPassword_);
        void verifyUser(const std::string& userId_, const std::string& cookieString_);

        SendMessageCallbackFn                   _sendMessageCallbackFn;
        std::map<std::string,std::string>       _userIdCookieMap;
        std::map<std::string,std::vector<std::pair<TCPConnection::Ptr,std::string>>> _userIdConnectionPtrCookiePairMap;
        Database::MariaDBConnections _databaseConnector;

    };


}} //closing namespace Santiago::Authentication
#endif
