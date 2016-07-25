#include"TCPConnection.h"

namespace Santiago{ namespace Authentication
{

    TCPConnection::TCPConnection(const MySocketPtr& socketPtr_,
                  const OnDisconnectCallbackFn& onDisconnectCallbackFn_,
                  const OnMessageCallbackFn& onMessageCallbackFn_)
        :_socketPtr(socketPtr_)
        ,_onDisconnectCallbackFn(onDisconnectCallbackFn_)
        ,_onMessageCallbackFn(onMessageCallbackFn_)
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
            //check for error and do cleanup.
            close();
        }
        else
        {
            while (true)
            {
                const char* inputBufferData = boost::asio::buffer_cast<const char*>(_inputBuffer.data());
                unsigned contentSize = *(int*)inputBufferData;
                
                if(_inputBuffer.size() >= 4)
                {
                    //  std::string myString(inputBufferData);
                    //  std::string size = myString.substr(0,4);
                    //  unsigned contentSize = *(int*)size.c_str();    
                    _inputBuffer.consume(4);
                    const char* content = inputBufferData + 4;
                    contentSize -=4;
                    while(contentSize >= _inputBuffer.size())
                    { 
                        std::string myString(content);
                        _inputBuffer.consume(contentSize-4);
                        ConnectionMessage message(myString);
                        _onMessageCallbackFn(message);
                    }
                   
                        break;
                }
                else
                {
                    break;
                }
                
                //do the processing in a while loop
                //and for each complete case call the onMessageCallbackFn        
                //in the end chain the asyc_read_some call. 
                
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
    
    }
}//closing namespace Santiago::Authentication

