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
/// @NOLINTBEGIN( cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers ): Tests will have magic numbers

#define BOOST_TEST_MODULE Empire
#define BOOST_TEST_MAIN  // Include this to get Boost Test's main()

#include <boost/test/unit_test.hpp>

#include "../src/lib/LogSeverity.hpp"

/// The name of the module for logging purposes
/// @NOLINTNEXTLINE( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays ): A `char[]` array is acceptable here
[[maybe_unused]] static constinit const char LOG_MODULE[32] { "test_Log" };

/// Logs at and above this will be available.  Logs below this will not be compiled into this source file.
[[maybe_unused]] static constinit const empire::LogSeverity MIN_LOG_SEVERITY { empire::LogSeverity::trace };
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
   LOG_TEST( "Test log entry with %d parameter", 1 );
   LOG_TEST( "Test log entry with %d parameter, same datatype, but different value", 2 );
   LOG_TEST( "Test log entry with %f parameter, different datatype", 3.0 );
   LOG_TEST( "Test log entry with [%d, %d], 2 parameters", 1, 2 );
}


BOOST_AUTO_TEST_SUITE_END()
// NOLINTEND( cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers )
/// @endcond
