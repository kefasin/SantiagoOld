cmake_minimum_required(VERSION 2.8)
project (Santiago)

#commands specific to unix
if (UNIX)
add_definitions (-D__UNIX__)
add_definitions("-std=c++0x -Wall -pedantic")
endif (UNIX)

#Make tie src/lib directory in the include path for the whole project
include_directories (${CMAKE_SOURCE_DIR})

#find the boost library
find_package(Boost COMPONENTS thread system REQUIRED)
include_directories( ${Boost_INCLUDE_DIR}/../ )
set(Boost_LIBRARIES ${Boost_LIBRARIES} pthread)

add_subdirectory (test)

