set(Authentication_SRCS
  ConnectionMessage.cpp
  ServerMessage.cpp
  ServerV1.cpp
  ConnectionServer.cpp
  ConnectionRequestsController.cpp
  ConnectionMessageSocket.cpp
  CreateUserRequestHandler.cpp
  LoginUserRequestHandler.cpp
  VerifyUserForCookieRequestHandler.cpp
  ChangeUserPasswordRequestHandler.cpp
  LogoutUserForCookieRequestHandler.cpp
  LogoutUserForAllCookiesRequestHandler.cpp)

if(WIN32)
  set(Authentication_SRCS ${Authentication_SRCS}   
  ConnectionMessage.h
  ServerMessage.h
  ServerV1.h
  ConnectionServer.h
  ConnectionRequestsController.h
  ConnectionMessageSocket.h
  RequestHandlerBase.h
  CreateUserRequestHandler.h
  LoginUserRequestHandler.h
  VerifyUserForCookieRequestHandler.h
  ChangeUserPasswordRequestHandler.h
  LogoutUserForCookieRequestHandler.h
  LogoutUserForAllCookiesRequestHandler.h)
endif(WIN32)

add_library(Authentication STATIC ${Authentication_SRCS})
target_link_libraries(Authentication Fastcgi ${Boost_LIBRARIES})
