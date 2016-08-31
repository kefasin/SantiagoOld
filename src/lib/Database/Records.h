#ifndef RECORDS_H
#define RECORDS_H

#include <string>

//#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/date_time/gregorian/gregorian.hpp>

namespace Santiago{ namespace Database
{
    using namespace boost::posix_time;
    //using namespace boost::gregorian;

    
    enum UserPermission 
    {
        OWNER,
        READ,
        WRITE,
        READ_WRITE
    };
    
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
        int _resId;
        std::string _userName;
        UserPermission  _userPermission;
    };
}}

#endif
