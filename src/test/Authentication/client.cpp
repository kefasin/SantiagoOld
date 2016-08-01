#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <unistd.h>

using boost::asio::ip::tcp;

int main()
{
  try
  {
   
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("localhost","5000");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);
    for (;;)
    {
        boost::array<char, 128> buf;
        std::string msg="hai";
        boost::system::error_code error;
        boost::asio::write(socket, boost::asio::buffer(msg),
                           boost::asio::transfer_all(),error);
        if(!error)
            std::cout<<"send";
    }
  }
  catch (std::exception& e)
  {
      std::cerr << e.what() << std::endl;
  }
  return 0;
}
