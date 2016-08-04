#include"TCPConnection.h"

namespace Santiago{ namespace Authentication
{

    TCPConnection::TCPConnection(const MySocketPtr& socketPtr_,
                                 const OnDisconnectCallbackFn& onDisconnectCallbackFn_,
                                 const OnMessageCallbackFn& onMessageCallbackFn_)
        :_socketPtr(socketPtr_)
        ,_ioService(_socketPtr->get_io_service())
        ,_strandPtr(new boost::asio::strand(_ioService))
        ,_onDisconnectCallbackFn(onDisconnectCallbackFn_)
        ,_onMessageCallbackFn(onMessageCallbackFn_)
    {
        BOOST_ASSERT(_socketPtr);
        start();
    }
    
    void TCPConnection::start()
    {

        _socketPtr->async_read_some(_inputBuffer.prepare(BUFFER_INCREMENT_SIZE),
                                    _strandPtr->wrap(boost::bind(&TCPConnection::handleRead,
                                                                 this->shared_from_this(),
                                                                 boost::asio::placeholders::error,
                                                                 boost::asio::placeholders::bytes_transferred)));
    }
    
    void TCPConnection::handleRead(const boost::system::error_code& error_,size_t bytesTransferred_)
    {
        if(error_)
        {
            close();   //check for error and do cleanup.
        }
        else
        {
            _inputBuffer.commit(bytesTransferred_);

            while (_inputBuffer.size())
            {
                const char* inputBufferData = boost::asio::buffer_cast<const char*>(_inputBuffer.data());
                unsigned messageSize = *(reinterpret_cast<const unsigned*>(inputBufferData));
                if(_inputBuffer.size() >= messageSize)
                {
                    ConnectionMessage message(inputBufferData+4,messageSize-4);
                    _inputBuffer.consume(messageSize);

                    _onMessageCallbackFn(message);
                }
                else
                {
                    break;
                }
            }
        }
        start();
    }
    
    void TCPConnection::close()
    {
        _socketPtr.reset();
        _onDisconnectCallbackFn();
    }

    void TCPConnection::sendMessage(const ConnectionMessage& message_)
    {
        _strandPtr->dispatch(boost::bind(&TCPConnection::sendMessageImpl,this,message_));
    }

    void TCPConnection::sendMessageImpl(const ConnectionMessage& message_)
    {
        boost::asio::streambuf outputBuffer;
        std::ostream outStream(&outputBuffer);
        outStream<<message_;

        boost::system::error_code errorCode;
        BOOST_ASSERT(_socketPtr);
        unsigned size = boost::asio::write(*_socketPtr,outputBuffer,errorCode);
        if(!errorCode)
        {
            BOOST_ASSERT(message_.getSize() == size);
        }
        else
        {
            close();
        }
    }
    
}}      //closing namespace Santiago::Authentication

