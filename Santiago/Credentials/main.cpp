#include"CredentialServer.h"
#include<iostream>
#include<stdlib.h>

int main(int argc, char* argv[])
{
  try
  {
       if (argc !=2 )
    {
      std::cerr << "Usage: server port" << std::endl;
      return 1;
    }
      
    boost::asio::io_service io_service;
    int port=atoi(argv[1]);
    CredentialServer server(io_service,port);

   
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
