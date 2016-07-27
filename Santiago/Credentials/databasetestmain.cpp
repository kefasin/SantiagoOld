#include "MariaDBConnections.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace Santiago::Database;

int main()
{
    Santiago::Database::MariaDBConnections connection;
    Records::UserPermission x = Records::UserPermission::READ_WRITE;
    connection.addPermissionRecord(34, "safek", x);
    connection.updateUserPassword("junais", "saitan","junki");
    connection.addSessionRecord("dfd", "kefas",
    {date{2016, 5, 12}, time_duration{12, 0, 0}});
    connection.updateSessionRecord("dfd", {date{2014, 6, 12}, time_duration{13, 0, 3}});
    connection.addUserProfileRecord("junais", "pakistan");
    std::vector<Records::UserProfile> records1 = connection.getUserProfileRecord();
    for(auto it = records1.begin(); it != records1.end(); ++it)
    {
        std::cout << "\n" << (*it)._id << "\t" << (*it)._userName << "\t" << (*it)._password << "\n";
    }
    std::vector<Records::Session> records2 = connection.getSessionRecord();
    for(auto it = records2.begin(); it != records2.end(); ++it)
    {
        std::cout << "\n" << (*it)._id << "\t" << (*it)._cookieId <<
            "\t" << (*it)._userName << "\t" << (*it)._loginTime << "\t" << (*it)._logoutTime << "\n";
            }
    std::vector<Records::Permission> records3 = connection.getPermissionRecord();
    for(auto it = records3.begin(); it != records3.end(); ++it)
    {
        std::cout << "\n" << (*it)._id << "\t" << (*it)._resId <<
            "\t" << (*it)._userName << "\t" <<
            connection._permission.find((*it)._userPermission)->second << "\n";
    }
    return 0;
}
    
