set(Authentication_SRCS Message.cpp
TCPConnection.cpp Server.cpp  UserController.cpp)

if(WIN32)
  set(Authentication_SRCS ${Authentication_SRCS}
    Message.h
    Server.h
    TCPConnection.h
    UserController.h)
endif(WIN32)

add_library(Authentication STATIC ${Authentication_SRCS})
target_link_libraries(Authentication Fastcgi ${Boost_LIBRARIES})
