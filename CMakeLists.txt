###############################################################################
### Empire V - What you do is what you do
###
### Tells cmake about the project's source files and targets
###
### @see https://cmake.org/cmake/help/latest/manual/cmake-language.7.html
###
### @file      CMakeLists.txt
### @author    Mark Nelson <marknels@hawaii.edu>
### @copyright (c) 2021 Mark Nelson.  All rights reserved.
###############################################################################

CMAKE_MINIMUM_REQUIRED( VERSION 3.26 )

PROJECT( Empire
         VERSION 2.0
         DESCRIPTION "What you do is what you do"
         LANGUAGES CXX )

MESSAGE( STATUS "CMAKE_SOURCE_DIR = [${CMAKE_SOURCE_DIR}]" )

SET( CMAKE_CXX_STANDARD 20 )
SET( CMAKE_VERBOSE_MAKEFILE false )

MESSAGE( STATUS "CMAKE_CXX_COMPILER = [${CMAKE_CXX_COMPILER}]" )

IF( "${CMAKE_CXX_COMPILER}" STREQUAL "/usr/bin/c++" )
   MESSAGE( STATUS "Compile with g++" )
   SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++ -fuse-ld=gold" )
ELSEIF( "${CMAKE_CXX_COMPILER}" STREQUAL "/usr/bin/clang++" )
   MESSAGE( STATUS "Compile with clang++" )
   SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20" )
ENDIF()


SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra" )
SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native -Ofast -funroll-loops -static -mfma" )
SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -march=x86-64 -mtune=generic -O0 -DDEBUG -fno-inline" )
SET( CMAKE_CXX_FLAGS_MINSIZEREL  "${CMAKE_CXX_FLAGS_DEBUG} -march=native -mtune=native -Oz -mfma" )

ADD_CUSTOM_TARGET( Document
                   COMMENT "Generating documentation"
                   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                   COMMAND .doxygen/stats.py
                   COMMAND rsync --recursive --checksum --delete --mkpath --chmod=o+r,Do+x ${CMAKE_SOURCE_DIR}/images ${CMAKE_SOURCE_DIR}/.doxygen/generated_docs/html/
                   COMMAND doxygen .doxygen/Doxyfile
                   )

SET_TARGET_PROPERTIES( Document
                       PROPERTIES
                       ADDITIONAL_CLEAN_FILES "${CMAKE_SOURCE_DIR}/.doxygen/generated_docs"
                       )

ADD_CUSTOM_TARGET( Publish
                   COMMENT "Pushing files to UHUnix"
                   COMMAND rsync --recursive --checksum --delete --compress --stats --chmod=o+r,Do+x ${CMAKE_SOURCE_DIR}/.doxygen/generated_docs/html/ marknels@uhunix.hawaii.edu:~/public_html/${CMAKE_PROJECT_NAME}
                   )

FILE( GLOB_RECURSE ALL_SOURCES ./src/*.cpp ./tests/*.cpp )
MESSAGE( STATUS "Sources to lint ${ALL_SOURCES}" )

ADD_CUSTOM_TARGET( Lint
                   COMMENT "Linting source"
                   COMMAND clang-tidy ${ALL_SOURCES} --quiet --extra-arg-before=-std=c++20 --
                   )

ADD_EXECUTABLE( empire_server src/main_empire_server.cpp src/version.hpp src/typedefs.h src/version.cpp )
ADD_EXECUTABLE( empire_client src/main_empire_client.cpp src/version.hpp src/typedefs.h src/version.cpp )
ADD_LIBRARY( empire STATIC src/lib/Singleton.hpp src/lib/Singleton.cpp )

FIND_PACKAGE( Boost REQUIRED COMPONENTS serialization )
IF( Boost_SERIALIZATION_FOUND )
   TARGET_LINK_LIBRARIES( empire_client ${Boost_LIBRARIES} )
ELSE( Boost_SERIALIZATION_FOUND )
   MESSAGE( "BOOST Serialization library is missing" )
ENDIF( Boost_SERIALIZATION_FOUND )

ADD_CUSTOM_TARGET( update_version
                   COMMENT "Update version"
                   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                   COMMAND .doxygen/update_version.py )
ADD_DEPENDENCIES( empire_server update_version )
ADD_DEPENDENCIES( empire_client update_version )

ADD_SUBDIRECTORY( tests )
