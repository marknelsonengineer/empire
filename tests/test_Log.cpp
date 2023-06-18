///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Test Boost global logging services for Empire V
///
/// @file      tests/test_Log.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
/// @cond Suppress Doxygen warnings

#define BOOST_TEST_MODULE Empire
#define BOOST_TEST_MAIN  // Include this to get Boost Test's main()

#include <boost/test/unit_test.hpp>

#define LOG_MODULE "test_Log"  ///< The name of the module for logging purposes @NOLINT( cppcoreguidelines-macro-usage ): `#define` is OK here
#include "../src/lib/Log.hpp"

using namespace empire;

BOOST_AUTO_TEST_SUITE( Log )


BOOST_AUTO_TEST_CASE( Log_LogSeverityToString ) {
   BOOST_CHECK_EQUAL( LogSeverityToString( LogSeverity::test ), "test" );
   BOOST_CHECK_EQUAL( LogSeverityToString( LogSeverity::trace ), "trace" );
   BOOST_CHECK_EQUAL( LogSeverityToString( LogSeverity::debug ), "debug" );
   BOOST_CHECK_EQUAL( LogSeverityToString( LogSeverity::info ), "info" );
   BOOST_CHECK_EQUAL( LogSeverityToString( LogSeverity::warning ), "warning" );
   BOOST_CHECK_EQUAL( LogSeverityToString( LogSeverity::error ), "error" );
   BOOST_CHECK_EQUAL( LogSeverityToString( LogSeverity::fatal ), "fatal" );

   BOOST_CHECK_EQUAL( (int) LogSeverity::COUNT, 7 );
}


BOOST_AUTO_TEST_CASE( Log_general ) {
   LOG_TEST( "Test log entry with no parameters" );
   LOG_TEST( "Test log entry with {} parameter", 1 );
   LOG_TEST( "Test log entry with {} parameter, same datatype, but different value", 2 );
   LOG_TEST( "Test log entry with {} parameter, different datatype", 3.0 );
   LOG_TEST( "Test log entry with {}, 2 parameters", 1, 2 );
}


BOOST_AUTO_TEST_SUITE_END()
/// @endcond
