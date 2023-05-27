###############################################################################
### Empire V - What you do is what you do
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
set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
FIND_PACKAGE( Boost REQUIRED COMPONENTS unit_test_framework log log_setup thread system )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )

IF( Boost_UNIT_TEST_FRAMEWORK_FOUND )
   # SET( Boost_USE_STATIC_LIBS ON)  # I'll need to compile my own Boost libraries, then we can get into this

   ADD_EXECUTABLE( All_Boost_Tests test_Singleton.cpp ../src/version.hpp ../src/typedefs.h ../src/version.cpp test_version.cpp test_Log.cpp )
   TARGET_LINK_LIBRARIES( All_Boost_Tests ${Boost_LIBRARIES} )
   TARGET_LINK_LIBRARIES( All_Boost_Tests empire )
   ADD_DEPENDENCIES( All_Boost_Tests update_version )
   SET_TARGET_PROPERTIES(All_Boost_Tests PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE True)

   ADD_CUSTOM_TARGET( Valgrind
                      COMMENT "Grinding the tests"
                      DEPENDS All_Boost_Tests
                      COMMAND ${CMAKE_COMMAND} -E env DEBUGINFOD_URLS=https://debuginfod.archlinux.org valgrind --leak-check=full --track-origins=yes --error-exitcode=1 --quiet ./All_Boost_Tests --build_info --color_output --show_progress --report_format=HRF --report_level=short
                      )

   SET_TARGET_PROPERTIES(Valgrind PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_RELEASE True)

ELSE( Boost_UNIT_TEST_FRAMEWORK_FOUND )
   MESSAGE( "BOOST Test framework is missing" )
ENDIF( Boost_UNIT_TEST_FRAMEWORK_FOUND )
