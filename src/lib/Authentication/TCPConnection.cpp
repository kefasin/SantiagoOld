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
    }
    
    void TCPConnection::start()
    {
        BOOST_ASSERT(_socketPtr);
        
        _socketPtr->async_read_some(_inputBuffer.prepare(BUFFER_INCREMENT_SIZE),
                                    boost::bind(&TCPConnection::handleRead,
                                                this->shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        
    }
    
    void TCPConnection::handleRead(const boost::system::error_code& error_,size_t bytesTransferred_)
    {
        _inputBuffer.commit(bytesTransferred_);
        if(error_)
        {
            close();   //check for error and do cleanup.
        }
        else
        {
            unsigned bufferSize = _inputBuffer.size();
            while (bufferSize)
            {
                const char* inputBufferData = boost::asio::buffer_cast<const char*>(_inputBuffer.data());
                unsigned messageStringSize = *(reinterpret_cast<const unsigned*>(inputBufferData));
                if(bufferSize >= messageStringSize)
                {
                    std::string myString( reinterpret_cast<char const*>(inputBufferData+4), messageStringSize-4);
                    _inputBuffer.consume(messageStringSize);
                    ConnectionMessage message(myString);
                    _onMessageCallbackFn(message);
                    bufferSize -= messageStringSize;  
                }
                else
                {
                    break;
                }
            }
            return;
        }
        _socketPtr->async_read_some(
            _inputBuffer.prepare(BUFFER_INCREMENT_SIZE),
            boost::bind(&TCPConnection::handleRead,
                      this->shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
        
    }
    
    void TCPConnection::close()
    {
        _onDisconnectCallbackFn();
    } 
    
    }      //closing namespace Santiago::Authentication
}
