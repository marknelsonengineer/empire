
cmake_minimum_required(VERSION 3.26)

project(Empire
      VERSION     2.0
      DESCRIPTION "More is better"
      LANGUAGES   CXX)

message(STATUS "CMAKE_SOURCE_DIR = [${CMAKE_SOURCE_DIR}]")

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_executable(empire main.cpp)
