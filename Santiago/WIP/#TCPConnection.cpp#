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
    
    void TCPConnection::handleRead(const boost::system::error_code& error_,size_t bytesTransferred_)
    {
        //check for error and do cleanup.
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

