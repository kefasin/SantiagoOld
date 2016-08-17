#include"ConnectionMessageSocket.h"

namespace Santiago{ namespace Authentication
{

    ConnectionMessageSocket::ConnectionMessageSocket(const MySocketPtr& socketPtr_,
                                                     const OnDisconnectCallbackFn& onDisconnectCallbackFn_,
                                                     const OnMessageCallbackFn& onMessageCallbackFn_)
        :_socketPtr(socketPtr_)
        ,_ioService(_socketPtr->get_io_service())
//        ,_strandPtr(new boost::asio::strand(_ioService))
        ,_onDisconnectCallbackFn(onDisconnectCallbackFn_)
        ,_onMessageCallbackFn(onMessageCallbackFn_)
    {
        BOOST_ASSERT(_socketPtr);
    }
    
    void ConnectionMessageSocket::start()
    {

        _socketPtr->async_read_some(_inputBuffer.prepare(BUFFER_INCREMENT_SIZE),
                                    /*_strandPtr->wrap(*/boost::bind(&ConnectionMessageSocket::handleRead,
                                                                 this->shared_from_this(),
                                                                 boost::asio::placeholders::error,
                                                                 boost::asio::placeholders::bytes_transferred)/*)*/);
    }
    
    void ConnectionMessageSocket::handleRead(const boost::system::error_code& error_,size_t bytesTransferred_)
    {
        /*
         * TODO: Pls change to include the RequestId
         * Format - <Total size><RequestId-8 bytes><ConnectionMessage>
         * Parse the RequestId separately and pass it in onMessageCallbackFn - Pls note the
         * changed format of the onMessageCallbackFn
         */

        if(error_)
        {
            close();   //check for error and do cleanup.
            return;
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
    
    void ConnectionMessageSocket::close()
    {
        _socketPtr.reset();
        _onDisconnectCallbackFn();
    }

    void ConnectionMessageSocket::sendMessage(const RequestId& requestId_,const ConnectionMessage& message_)
    {
//        _strandPtr->dispatch(boost::bind(&TCPConnection::sendMessageImpl,this,message_));
        sendMessageImpl(requestId_,message_);
    }

    void ConnectionMessageSocket::sendMessageImpl(const RequestId& requestId_, const ConnectionMessage& message_)
    {
        /*
         * TODO: Make code to write the size and the requestId to the buffer before
         * writing the ConnectionMessage.
         * size = 4 + 8(for RequestId) + connectionMessage size 
         */
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

