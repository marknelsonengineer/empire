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
         DESCRIPTION "More is better"
         LANGUAGES CXX )

MESSAGE( STATUS "CMAKE_SOURCE_DIR = [${CMAKE_SOURCE_DIR}]" )

SET( CMAKE_CXX_STANDARD 20 )
SET( CMAKE_VERBOSE_MAKEFILE false )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}         -Wall -Wextra" )
SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native -O2" )
SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}   -march=x86-64 -mtune=generic -O0 -DDEBUG -fno-inline" )

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

ADD_EXECUTABLE( empire src/main.cpp src/version.hpp src/typedefs.h src/version.cpp )

ADD_CUSTOM_TARGET( update_version
                   COMMENT "Update version"
                   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                   COMMAND .doxygen/update_version.py )
ADD_DEPENDENCIES( empire update_version )

ADD_SUBDIRECTORY( tests )
