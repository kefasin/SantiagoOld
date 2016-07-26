#define the variable echo-external_SRCS
set (Authentication_SRCS MessageTest.cpp)

add_executable(Authentication ${Authentication_SRCS}) 
target_link_libraries(Authentication ${Boost_LIBRARIES})
