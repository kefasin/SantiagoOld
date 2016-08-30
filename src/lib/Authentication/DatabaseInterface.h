#ifndef SANTIAGO_AUTHENTICATION_DATABASEINTERFACE_H
#define SANTIAGO_AUTHENTICATION_DATABASEINTERFACE_H

#include <vector>
#include <cstdlib>
#include <string>

#include "Database/MariaDBConnections.h"

namespace Santiago{ namespace Authentication
{
    class DatabaseInterface
    {
    public:
        
        bool createUser(const std::string& userId_, const std::string& password_);
        bool loginUser(const std::string& userId_, const std::string& password_);
        bool verifyUserForCookie(const std::string& cookie_);
        bool logoutUserForCookie(const std::string& cookie_);
        bool logoutUserForAllCookies(const std::string& userId_);
        bool changeUserPassword(const std::string& userId_,const std::string& oldPassword_,
                                const std::string& newPassword_);
        bool addResource(int resId_,std::string userName_,
                         UserPermission permission_);

    protected:
        
        Database::MariaDBConnections            _databaseConnector;
        

    };


}} //closing namespace Santiago::Authentication
#endif
