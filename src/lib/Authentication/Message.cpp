#include "Message.h"

namespace Santiago{ namespace Authentication
{   
    /***********************************************************
     * ConnectionMessage
     ***********************************************************/

    ConnectionMessage::ConnectionMessage(const char* content_, unsigned size_)
    {
        unsigned curPos = 0;
        //parse the type
        _type = *reinterpret_cast<const MessageType*>(content_ + curPos);
        curPos += sizeof(MessageType);
        //parse the noOfParameters
        unsigned noOfParameters = *reinterpret_cast<const unsigned*>(content_ + curPos);        
        curPos += sizeof(unsigned);
        //parse the parameters
        while(curPos < size_)
        {
            //parse the parameter size
            unsigned parameterSize = *reinterpret_cast<const unsigned*>(content_ + curPos);
            curPos += sizeof(unsigned);
            //check for parameter size inconsistency
            if(curPos + parameterSize > size_)
            {
                throw std::runtime_error("Invalid message format: Parameter size does not match.");
            }

            _parameters.push_back(std::string(content_ + curPos, parameterSize));
            curPos += parameterSize;          
        }
        //check for no of parameters inconsistency.
        if(_parameters.size() < noOfParameters)
        {
            throw std::runtime_error("Invalid message format: Number of paramters does not match.");
        }
    }

    ConnectionMessage::ConnectionMessage(MessageType type_, const std::vector<std::string>& parameters_)
        :_type(type_)
        ,_parameters(parameters_)
    {}
      
    std::ostream& ConnectionMessage::writeToStream(std::ostream& outStream_) const
    {
        outStream_.write(reinterpret_cast<const char*>(&_type), sizeof(_type));
        unsigned noOfParameters = _parameters.size();
        outStream_.write(reinterpret_cast<const char*>(&noOfParameters), sizeof(noOfParameters));

        for(auto it = _parameters.begin(); it != _parameters.end(); ++it)
        {
            unsigned parameterSize = it->size();
            outStream_.write(reinterpret_cast<const char*>(&parameterSize), sizeof(parameterSize));
            outStream_ << *it; 
        }

        return outStream_;
    }

    unsigned ConnectionMessage::getSize() const
    {
        unsigned size = sizeof(MessageType) + sizeof(unsigned);
        for(unsigned i=0;i<_parameters.size();i++)
        {
            size += _parameters[i].size();
        }
        return size;
    }

    /***********************************************************
     * ServerMessage
     ***********************************************************/

    ServerMessage::ServerMessage(unsigned connectionId_,const ConnectionMessage& connectionMessage_)
        :_connectionId(connectionId_)
        ,_connectionMessage(connectionMessage_)
    {}

}}//closing Santiago::Authentication 
