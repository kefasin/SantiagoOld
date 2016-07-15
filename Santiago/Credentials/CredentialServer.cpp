#include"CredentialServer.h"

void CredentialServer::start_accept()
{
    std::function<void(int)> onDisconnectCallbackFn = 
        std::bind(&CredentialServer::handleDisconnect,this,std::placeholders::_1);
    std::function<char(int,int)> getPermissionCallbackFn = 
        std::bind(&CredentialServer::handleGetPermission,this,std::placeholders::_1,std::placeholders::_2);
    std::function<bool(std::string,std::string)> checkLoginCallbackFn = 
        std::bind(&CredentialServer::handleCheckLogin,this,std::placeholders::_1,std::placeholders::_2);


    TCPConnection::pointer new_connection(new TCPConnection(_acceptor.get_io_service(),onDisconnectCallbackFn,
                                                            getPermissionCallbackFn,checkLoginCallbackFn));
    _acceptor.async_accept(new_connection->socket(),
                           boost::bind(&CredentialServer::handle_accept, this, new_connection,
                                       boost::asio::placeholders::error));
}

void CredentialServer::handle_accept(TCPConnection::pointer new_connection,
                                     const boost::system::error_code& error)
{
    if (!error)
    {
        int connId=rand();
        new_connection->start(connId);
        std::cout<<connId<<std::endl;
        _idMap.insert(std::pair<int,TCPConnection::pointer>(connId,new_connection));
    } 
    start_accept();
}

void CredentialServer::handleDisconnect(int connectionId_)
{
    _idMap.erase(connectionId_);
}


char CredentialServer::handleGetPermission(int resId_, int clientId_)
{
    std::map<int, std::vector<std::pair<int, char>>>::iterator it;
    it = _permissionMap.find(resId_);
    if(it == _permissionMap.end())
    {
        return 'N';
    }
    else
    {
        for(auto itr = it->second.begin(); itr != it->second.end(); ++itr)
        {
            if(itr->first == clientId_)
            {
                return itr->second;
            }
        }
        return 'N';
    }
}


bool CredentialServer::handleCheckLogin(std::string user_,std::string pass_)
{
    std::map<std::string,std::string>::iterator it;
    it=_loginMap.find(user_);
    if(it == _loginMap.end())
    {
        return false;
    }
    else
    {
        if(it->second == pass_)
        {
            return true;
        }
    }
}
