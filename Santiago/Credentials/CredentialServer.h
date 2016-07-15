#ifndef CREDENTIALSERVER_H
#define CREDENTIALSERVER_H
#include"TCPConnection.h"
#include<stdlib.h>
#include<map>
#include<vector>
#include<utility>
#include <mysql.h>

class CredentialServer
{
public:
    
    CredentialServer(boost::asio::io_service& io_service,int port_)
        : _acceptor(io_service, tcp::endpoint(tcp::v4(),port_))
    {
        _permissionMap.insert(std::pair<int, std::vector<std::pair<int, char>>>(10, {{1, 'r'}, {2, 'r'}}));
        _loginMap.insert(std::pair<std::string,std::string>("junu","junu"));
        start_accept();
    }
private:
    void start_accept();
    void handle_accept(TCPConnection::pointer,
                       const boost::system::error_code&);
    void handleDisconnect(int);
    char handleGetPermission(int,int);
    bool handleCheckLogin(std::string,std::string);
    tcp::acceptor _acceptor;
    std::map<int,TCPConnection::pointer> _idMap;
    std::map<std::string,std::string> _cookieMap;
    std::map<int,std::vector<std::pair<int,char>>> _permissionMap;
    std::map<std::string,std::string> _loginMap;
    std::map<std::string,std::vector<std::pair<TCPConnection::pointer,std::string>>> _userConnectionMap;
    MYSQL *_con;
};

#endif
