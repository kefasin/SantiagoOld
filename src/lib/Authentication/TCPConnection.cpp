#include"TCPConnection.h"

namespace Santiago{ namespace Authentication
{

    TCPConnection::TCPConnection(const MySocketPtr& socketPtr_,
                  const OnDisconnectCallbackFn& onDisconnectCallbackFn_,
                  const OnMessageCallbackFn& onMessageCallbackFn_)
        :_socketPtr(socketPtr_)
        ,_onDisconnectCallbackFn(onDisconnectCallbackFn_)
        ,_onMessageCallbackFn(onMessageCallbackFn_)
        ,_readFlag(true)
    {
        BOOST_ASSERT(_socketPtr);
        _socketPtr->async_read_some(
            _inputBuffer.prepare(BUFFER_INCREMENT_SIZE),
            std::bind(&TCPConnection::handleRead,
                      this->shared_from_this(),
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
    }

    /* 
       1) Pls try and fully understand Fastcgi::handleRead before writing this fn.
      2) Pls add check to ensure that all the bytes reqd are there before reading from input buffer
      3) Use boost::asio::buffer_cast to peek into buffer data before moving to std::string
      4) strtol will not work in this circumstance. Use reinterpret_cast. see Fastcgi::hangleRead for examples
    */
    
    void TCPConnection::handleRead(const boost::system::error_code& error_,size_t bytesTransferred_)
    {
        _inputBuffer.commit(bytesTransferred_);
        if(error_)
        {
            close();   //check for error and do cleanup.
        }
        else
        {
            const char* inputBufferData = boost::asio::buffer_cast<const char*>(_inputBuffer.data()); 
            while (_inputBuffer.size())
            {
                if(_inputBuffer.size() >= 4 && _readFlag)
                {
                    _inputBuffer.consume(4);
                    unsigned bufferSize = *(int*)inputBufferData;
                    const char* content = inputBufferData + 4;
                    std::string myString(content);
                    unsigned contentSize = myString.size();
                    _readFlag =false;
                    if(bufferSize <= contentSize+4)
                    {
                        _readFlag = true;
                    }
                }
                else
                {
                    const char* content = inputBufferData;
                    std::string myString(content);
                    unsigned contentSize = myString.size();
                }
                ConnectionMessage message(myString);
                _onMessageCallbackFn(message);
                _inputBuffer.consume(contentSize);      
            }
        }
    
        _socketPtr->async_read_some(
            _inputBuffer.prepare(BUFFER_INCREMENT_SIZE),
            std::bind(&TCPConnection::handleRead,
                      this->shared_from_this(),
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
    
    }
    
    unsigned TCPConnection:: intReceive(std::string str)
    {
        unsigned num;
        num = strtol(str.c_str(), NULL, 2);
        return num;
    }
    void TCPConnection::close()
    {
        _onDisconnectCallbackFn();
    } 
    
    }//closing namespace Santiago::Authentication
}
