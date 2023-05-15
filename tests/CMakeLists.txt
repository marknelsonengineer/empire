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

set( Boost_USE_STATIC_LIBS OFF )

# see https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package( Boost REQUIRED COMPONENTS unit_test_framework )
include_directories ( ${Boost_INCLUDE_DIRS} )

if( Boost_UNIT_TEST_FRAMEWORK_FOUND )
   message( STATUS "Generating unit tests for BOOST Unit Test Framework" )

   add_executable( All_Boost_Tests test_Singleton.cpp ../src/lib/Singleton.hpp ../src/lib/Singleton.cpp )
   target_link_libraries( All_Boost_Tests ${Boost_LIBRARIES} )

else( Boost_UNIT_TEST_FRAMEWORK_FOUND )
   message( "BOOST Test framework is missing" )
endif( Boost_UNIT_TEST_FRAMEWORK_FOUND )

set(ENV{DEBUGINFOD_URLS} "https://debuginfod.archlinux.org")

add_custom_target( Valgrind
        COMMENT "Grinding the tests"
        COMMAND valgrind --leak-check=full --track-origins=yes --error-exitcode=1 --quiet ./All_Boost_Tests --build_info --color_output --show_progress --report_format=HRF --report_level=short
        )
