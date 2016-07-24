#ifndef SANTIAGO_AUTHENTICATION_USERCONTROLLER_H
#define SANTIAGO_AUTHENTICATION_USERCONTROLLER_H

#include <function>
#include <ctime>
#include "Database/MariaDBConnections.h"
#include "Message.h"

namespace Santiago{ namespace Authentication
{
    class UserController
    {

    public:

        typedef std::function<boost::asio::error_code(ServerMessage)> SendMessageCallbackFn;
        //  typedef std::shared_ptr<UserController> Ptr;

        UserController(SendMessageCallbackFn sendMessageCallbackFn_)
            :_sendMessageCallbackFn(sendMessageCallbackFn_)
        {}

        void handleClientMessage(const ServerMessage& serverMessage_)
        //{
            //here make a large switch of message type,
            //parse the parameter strings into integers for relevant cases, 
            //redirect to different functions for each of the message type
            //something like
            /*
              switch(serverMessage_._connectionMessage._type)
              {
                 case NEW_USER:
                    addNewUser(serverMessage_._connectionMessage._parameters[0],
                               serverMessage_._connectionMessage._parameters[1]);
                 case LOGIN_USER;
                    loginUser(..,..);
                 //so on
              }
             */
        //  }

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
        std::map<std::string,std::vector<std::pair<TCPConnection::pointer,std::string>>> _userIdConnectionPtrCookiePairMap;
        MariaDBConnections _databaseConnector;

    };


}} //closing namespace Santiago::Authentication

#endif
