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
         VERSION 5.0.0
         DESCRIPTION "What you do is what you do"
         LANGUAGES CXX )

# MESSAGE( STATUS "CMAKE_SOURCE_DIR = [${CMAKE_SOURCE_DIR}]" )
# MESSAGE( STATUS "CMAKE_BUILD_TYPE = [${CMAKE_BUILD_TYPE}]" )
# MESSAGE( STATUS "CMAKE_CXX_COMPILER = [${CMAKE_CXX_COMPILER}]" )

SET( CMAKE_CXX_STANDARD 20 )
SET( CMAKE_CXX_STANDARD_REQUIRED true )
SET( CMAKE_VERBOSE_MAKEFILE false )

ADD_COMPILE_OPTIONS( -Wall -Wextra )

IF( "${CMAKE_CXX_COMPILER}" STREQUAL "/usr/bin/c++" )
   MESSAGE( STATUS "Compile with g++" )
   ADD_COMPILE_OPTIONS( -lstdc++ -fuse-ld=gold )
ELSEIF( "${CMAKE_CXX_COMPILER}" STREQUAL "/usr/bin/clang++" )
   MESSAGE( STATUS "Compile with clang++" )
   ADD_COMPILE_OPTIONS( -std=c++20 )
ENDIF()

SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native -Ofast -funroll-loops -static -mfma" )
SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -march=x86-64 -mtune=generic -O0 -DDEBUG -fno-inline" )
SET( CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_DEBUG} -march=native -mtune=native -Oz -mfma" )

# Boost Library Configuration
# see https://cmake.org/cmake/help/latest/module/FindBoost.html
SET( Boost_USE_RELEASE_LIBS ON )  # Boost is production-for-us (we're not developing Boost code), so it's always release
SET( Boost_USE_DEBUG_LIBS OFF )  # Boost is production-for-us (we're not developing Boost code), so it's never debug
SET( Boost_USE_STATIC_LIBS OFF )  # By default, use shared libraries, but on Release builds, use static libs

IF( ${CMAKE_BUILD_TYPE} STREQUAL "Release" )  # We build our own Boost libraries to get these static libraries
   SET( Boost_USE_STATIC_LIBS ON )  # By default, use shared libraries, but on Release builds, use static libs
ENDIF()

FIND_PACKAGE( Boost REQUIRED COMPONENTS unit_test_framework serialization )  # Needed for libraries (not Boost header-only components).

INCLUDE_DIRECTORIES( BEFORE SYSTEM ${Boost_INCLUDE_DIRS} )

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

ADD_CUSTOM_TARGET( PublishXX
                   COMMENT "Pushing files to UHUnix"
                   COMMAND rsync --recursive --checksum --delete --compress --stats --chmod=o+r,Do+x ${CMAKE_SOURCE_DIR}/.doxygen/generated_docs/html/ marknels@uhunix.hawaii.edu:~/public_html/${CMAKE_PROJECT_NAME}
                   )

FILE( GLOB_RECURSE ALL_SOURCES ./src/*.cpp ./tests/*.cpp )
MESSAGE( STATUS "Sources to lint ${ALL_SOURCES}" )

ADD_CUSTOM_TARGET( Lint
                   COMMENT "Linting source"
                   COMMAND clang-tidy ${ALL_SOURCES} --quiet --extra-arg-before=-std=c++20 --
                   )

IF( ${CMAKE_BUILD_TYPE} STREQUAL "Release" )  # We build our own Boost libraries to get these static libraries
   SET( BUILD_SHARED_LIBS FALSE )
ELSE()
   SET( BUILD_SHARED_LIBS TRUE )
   ADD_COMPILE_DEFINITIONS( BOOST_TEST_DYN_LINK )
ENDIF()

ADD_LIBRARY( empire src/lib/Singleton.hpp src/lib/Singleton.cpp src/lib/Log.hpp src/lib/Log.cpp )


ADD_EXECUTABLE( empire_server src/main_empire_server.cpp src/version.hpp src/typedefs.h src/version.cpp )
ADD_EXECUTABLE( empire_client src/main_empire_client.cpp src/version.hpp src/typedefs.h src/version.cpp )
TARGET_LINK_LIBRARIES( empire_client ${Boost_LIBRARIES} )

ADD_CUSTOM_TARGET( update_version
                   COMMENT "Update version"
                   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                   COMMAND .doxygen/update_version.py )
ADD_DEPENDENCIES( empire_server update_version )
ADD_DEPENDENCIES( empire_client update_version )

ADD_EXECUTABLE( All_Boost_Tests tests/test_Log.cpp tests/test_Singleton.cpp tests/test_version.cpp src/typedefs.h src/version.hpp src/version.cpp )
TARGET_LINK_LIBRARIES( All_Boost_Tests ${Boost_LIBRARIES} )
TARGET_LINK_LIBRARIES( All_Boost_Tests empire )
ADD_DEPENDENCIES( All_Boost_Tests update_version )
