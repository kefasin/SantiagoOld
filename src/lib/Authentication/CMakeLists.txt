set(Authentication_SRCS Message.cpp Server.cpp TCPConnection.cpp UserController.cpp)

if(WIN32)
  set(Authentication_SRCS ${fastcgi_devkit_SRCS}
    Message.h
    Server.h
    TCPConnection.h
    UserController.h)
endif(WIN32)

add_library(Authentication STATIC ${Authentication_SRCS})
target_link_libraries(Authentication Fastcgi ${Boost_LIBRARIES})