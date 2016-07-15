#include <mysql.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>
#include <sstream>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

class MariaDBConnections
{
    int _userProfileId, _sessionId, _permissionId;

public:
    enum UserPermission
    {
        READ,
        WRITE,
        READ_WRITE
    };

    std::map<UserPermission, std::string> _permission;

    struct UserProfile
    {
        int _id;
        std::string _userName;
        std::string _password;
    };

    struct Session
    {
        int _id;
        std::string _userName;
        std::string _cookieId;
        ptime _loginTime;
        ptime _logoutTime;
        
    };

    struct Permission
    {
        int _id;
        std::string _resId;
        std::string _userName;
        UserPermission  _userPermission;
    };
    
    MariaDBConnections();
    bool addUserProfileRecord(std::string, std::string);
    bool addSessionRecord(std::string, std::string, ptime, ptime);
    bool addPermissionRecord(int, std::string, UserPermission);
    std::vector<MariaDBConnections::UserProfile> getUserProfileRecord();
    std::vector<MariaDBConnections::Session> getSessionRecord();
    std::vector<MariaDBConnections::Permission> getPermissionRecord(); 
}; 
