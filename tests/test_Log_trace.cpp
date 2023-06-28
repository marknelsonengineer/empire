///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Test setting MIN_LOG_SEVERITY to LOG_SEVERITY_TRACE
///
/// @file      tests/test_Log.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
/// @cond Suppress Doxygen warnings
/// @NOLINTBEGIN( cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers ): Tests will have magic numbers

#include <boost/test/unit_test.hpp>

#include "../src/lib/LogSeverity.hpp"  // For LOG_SEVERITY #defines

/// The name of the module for logging purposes
/// @NOLINTNEXTLINE( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays ): A `char[]` array is acceptable here
[[maybe_unused]] static constinit const char LOG_MODULE[32] { "test_Log_trace" };

/// Logs at and above `MIN_LOG_SEVERITY` will be available.  Logs below
/// `MIN_LOG_SEVERITY` will not be compiled into the source file.
#define MIN_LOG_SEVERITY LOG_SEVERITY_TRACE
#include "../src/lib/Log.hpp"


/* ****************************************************************************
   White Box Test Declarations

   These declarations may contain duplicate code or code that needs to be in
   sync with the code under test.  Because these are white box tests, it's on
   the tester to ensure the code is in sync.                                 */

namespace empire {
   extern LogEntry& logPeek();
} // namespace empire

/* ***************************************************************************/


using namespace empire;

BOOST_AUTO_TEST_SUITE( Log )

BOOST_AUTO_TEST_CASE( Log_level_trace ) {
   LOG_TEST( "Test log entry with no parameters" );
   BOOST_CHECK_NE( logPeek().msg, "Test log entry with no parameters" );

   LOG_TRACE( "Test trace entry with %d parameter", 1 );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test trace entry with 1 parameter" );

   LOG_DEBUG( "Test debug entry with %d parameter, same datatype, but different value", 2 );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test debug entry with 2 parameter, same datatype, but different value" );

   LOG_INFO( "Test info entry with %06.3f parameter, different datatype", 3.0 );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test info entry with 03.000 parameter, different datatype" );

   LOG_WARN( "Test warn entry with [%d, %d], 2 parameters", 1, 2 );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test warn entry with [1, 2], 2 parameters" );

   LOG_ERROR( "Test error entry with [%s, %s], 2 string parameters", "one", "two" );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test error entry with [one, two], 2 string parameters" );

   LOG_FATAL( "Test fatal entry with [%s, %d], 2 different parameters", "one", 2 );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test fatal entry with [one, 2], 2 different parameters" );
}


BOOST_AUTO_TEST_SUITE_END()
// NOLINTEND( cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers )
/// @endcond
