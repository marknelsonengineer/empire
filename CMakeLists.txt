###############################################################################
### Empire ][ - What you do is what you do
###
### Tells cmake about the project's source files and targets
###
### @see https://cmake.org/cmake/help/latest/manual/cmake-language.7.html
###
### @file      CMakeLists.txt
### @author    Mark Nelson <marknels@hawaii.edu>
### @copyright All rights reserved
###############################################################################

cmake_minimum_required(VERSION 3.26)

project(Empire
      VERSION     2.0
      DESCRIPTION "More is better"
      LANGUAGES   CXX)

message(STATUS "CMAKE_SOURCE_DIR = [${CMAKE_SOURCE_DIR}]")

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_custom_target(Document
        COMMENT           "Generating documentation"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMAND           doxygen .doxygen/Doxyfile
        )

add_custom_target(Publish
        COMMENT "Pushing files to UHUnix"
        COMMAND rsync --recursive --checksum --delete --compress --stats --chmod=o+r,Do+x ${CMAKE_SOURCE_DIR}/.doxygen/generated_docs/html/ marknels@uhunix.hawaii.edu:~/public_html/${CMAKE_PROJECT_NAME}
        )

add_executable(empire main.cpp)
