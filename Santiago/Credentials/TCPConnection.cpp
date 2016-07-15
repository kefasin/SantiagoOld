#include"TCPConnection.h"

void TCPConnection::start(int connId)
{
    _id = connId;
    boost::asio::async_read(_socket, boost::asio::buffer(_buf),
                            boost::bind(&TCPConnection::handle_read,this, _buf,
                                        boost::asio::placeholders::error));
    std::cout<<_buf.data();

    std::string msg="connected";
    boost::asio::write(_socket, boost::asio::buffer(msg,10)); 
}

int TCPConnection::getId()
{
    return _id;
}

void TCPConnection::handle_read(boost::array<char,128> buf_,const boost::system::error_code& error)
{
    if(error)
    {
        _onDisconnectCallbackFn(getId());
        _getPermissionCallbackFn(10,1);
        _checkLoginCallbackFn("junu","junu");
        return;
    }
    boost::asio::async_read(_socket, boost::asio::buffer(buf_),
                            boost::bind(&TCPConnection::handle_read,this, buf_,
                            boost::asio::placeholders::error));
}


