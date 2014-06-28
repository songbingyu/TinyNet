# add by bingyu.song  for TinyNet 


cmake_minimum_required(VERSION 2.6)

project( TinyNet  CXX )


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS " -g -Wall -Werror ")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

aux_source_directory(./ SRC_LIST)
include_directories( ./ )

MESSAGE(STATUS "This is SOURCE dir "${TinyNet_SOURCE_DIR})

ADD_DEFINITIONS(-DDEBUG)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/lib)


ADD_LIBRARY(TinyNet  SHARED ${SRC_LIST})
#add_executable(server_test  ${SRC_LIST} )



