#include"TCPConnection.h"

namespace Santiago{ namespace Authentication
{

    TCPConnection(const MySocketPtr& socketPtr_,
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
        //check for error and do cleanup.
        int readSize;
        if(error_)
        {
            close();
        }

        //format of the input <no_of_bytes><1 space><input bytes to be parsed ConnectionMessage::ConnectionMessage(std::string)>
        while(1)
        {
            // boost::asio::read(_socketPtr, _inputBuffer, 
            //  boost::asio::transfer_exactly(4),boost::system::error_code& error);
            std::string myString;  
            
            // Convert streambuf to std::string  
            std::istream(&_inputBuffer) >> myString; 
            unsigned readSize=intReceive(myString.substr(0,32));
            if(readSize >_inputBuffer.size())
            {
                ConnectionMessage message(myString);
                _onMessageCallbackFn(message);
                // std::function<void(unsigned)> onDisconnectCallbackFn = 
                //   std::bind(&Server::handleDisconnect,this,_nextConnectionId);
            }
            else
            {
                break;
            }
            //do the processing in a while loop
            //and for each complete case call the onMessageCallbackFn        
            //in the end chain the asyc_read_some call. 
            
        }       
        
        _socketPtr.async_read_some(
            boost::asio::buffer(_inputBuffer),
            boost::bind(&TCPConnection::handleRead, shared_from_this(),
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

