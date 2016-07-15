#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include <iostream>
#include <string>
#include <stdlib.h>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/asio/placeholders.hpp>

using boost::asio::ip::tcp;

class TCPConnection
    : public boost::enable_shared_from_this<TCPConnection>
{
public:
    typedef boost::shared_ptr<TCPConnection> pointer;
    tcp::socket& socket()
    {
        return _socket;
    }
    void start(int);
    int getId();

    TCPConnection(boost::asio::io_service& io_service,
                  const std::function<void(int)>& onDisconnectCallbackFn_,
                  const std::function<char(int,int)>& getPermissionCallbackFn_,
                  const std::function<bool(std::string,std::string)>& checkLoginCallbackFn_)
        : _socket(io_service),
          _onDisconnectCallbackFn(onDisconnectCallbackFn_),
          _getPermissionCallbackFn(getPermissionCallbackFn_),
          _checkLoginCallbackFn(checkLoginCallbackFn_)
    {
    }
private:
    void handle_read(boost::array<char,128>,const boost::system::error_code&);
    void handle_write(const boost::system::error_code&,
                      size_t)
    {
    }
    tcp::socket _socket;
    int _id;
    boost::array<char, 128> _buf;
    std::function<void(int)> _onDisconnectCallbackFn;
    std::function<char(int,int)> _getPermissionCallbackFn;
    std::function<bool(std::string,std::string)> _checkLoginCallbackFn;
};

#endif
