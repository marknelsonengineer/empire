###############################################################################
### Empire ][ - What you do is what you do
###
### Tells cmake about the project's source files and targets
###
### @see https://cmake.org/cmake/help/latest/manual/cmake-language.7.html
###
### @file      tests/CMakeLists.txt
### @author    Mark Nelson <mr_nelson@icloud.com>
### @copyright (c) 2021 Mark Nelson.  All rights reserved.
###############################################################################

SET( Boost_USE_STATIC_LIBS OFF )

# see https://cmake.org/cmake/help/latest/module/FindBoost.html
FIND_PACKAGE( Boost REQUIRED COMPONENTS unit_test_framework )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )

IF( Boost_UNIT_TEST_FRAMEWORK_FOUND )
   MESSAGE( STATUS "Generating unit tests for BOOST Unit Test Framework" )

   ADD_EXECUTABLE( All_Boost_Tests test_Singleton.cpp ../src/lib/Singleton.hpp ../src/lib/Singleton.cpp )
   TARGET_LINK_LIBRARIES( All_Boost_Tests ${Boost_LIBRARIES} )

ELSE( Boost_UNIT_TEST_FRAMEWORK_FOUND )
   MESSAGE( "BOOST Test framework is missing" )
ENDIF( Boost_UNIT_TEST_FRAMEWORK_FOUND )

SET( ENV{DEBUGINFOD_URLS} "https://debuginfod.archlinux.org" )

ADD_CUSTOM_TARGET( Valgrind
                   COMMENT "Grinding the tests"
                   COMMAND valgrind --leak-check=full --track-origins=yes --error-exitcode=1 --quiet ./All_Boost_Tests --build_info --color_output --show_progress --report_format=HRF --report_level=short
                   )
