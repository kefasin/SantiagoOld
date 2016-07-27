#ifndef MARIADBCONNECTIONS_H
#define MARIADBCONNECTIONS_H

#include "Database/Records.h"
#include <mysql.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>

namespace Santiago{ namespace Database
{
    class MariaDBConnections
    {
    public:
        std::map<Records::UserPermission, std::string> _permission;
        
        MariaDBConnections();
        bool addUserProfileRecord(const std::string, const std::string);
        bool updateUserPassword(const std::string, const std::string, const std::string);
        bool addSessionRecord(const std::string, const std::string, ptime);
        bool updateSessionRecord(const std::string, ptime);
        bool addPermissionRecord(int, const std::string, Records::UserPermission);
        std::vector<Records::UserProfile> getUserProfileRecord();
        std::vector<Records::Session> getSessionRecord();
        std::vector<Records::Permission> getPermissionRecord();

    private:
        int _userProfileId, _sessionId, _permissionId;
    };
}}

#endif
