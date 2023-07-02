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
/// @NOLINTBEGIN( cppcoreguidelines-pro-bounds-array-to-pointer-decay, hicpp-no-array-decay ): For performance reasons, we cast arrays to pointers
/// @NOLINTBEGIN( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays ): We use `char[]` arrays here

#define BOOST_TEST_MODULE Empire
#define BOOST_TEST_MAIN  // Include this to get Boost Test's main()

#include <boost/test/unit_test.hpp>

#include "../src/lib/LogSeverity.hpp"  // For LOG_SEVERITY #defines

/// The name of the module for logging purposes
/// @NOLINTNEXTLINE( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays ): A `char[]` array is acceptable here
[[maybe_unused]] alignas(32) static constinit const char LOG_MODULE[32] { "test_Log" };

/// Logs at and above `MIN_LOG_SEVERITY` will be available.  Logs below
/// `MIN_LOG_SEVERITY` will not be compiled into the source file.
#define MIN_LOG_SEVERITY LOG_SEVERITY_TEST
#include "../src/lib/Log.hpp"


/* ****************************************************************************
   White Box Test Declarations

   These declarations may contain duplicate code or code that needs to be in
   sync with the code under test.  Because these are white box tests, it's on
   the tester to ensure the code is in sync.                                 */

namespace empire {
   extern void logReset();
   extern LogEntry& logPeek();
} // namespace empire

/* ***************************************************************************/


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
   logReset();
   LOG_TEST( "Test log entry with no parameters" );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test log entry with no parameters" );

   LOG_TEST( "Test log entry with %d parameter", 1 );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test log entry with 1 parameter" );

   LOG_TEST( "Test log entry with %d parameter, same datatype, but different value", 2 );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test log entry with 2 parameter, same datatype, but different value" );

   LOG_TEST( "Test log entry with %06.3f parameter, different datatype", 3.0 );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test log entry with 03.000 parameter, different datatype" );

   LOG_TEST( "Test log entry with [%d, %d], 2 parameters", 1, 2 );
   BOOST_CHECK_EQUAL( logPeek().msg, "Test log entry with [1, 2], 2 parameters" );
}

BOOST_AUTO_TEST_CASE( Log_reset ) {
   logReset();
   const LogEntry& emptyEntry = logPeek();
   BOOST_CHECK_EQUAL( emptyEntry.ready, false );

   const char zeroMsgBuffer[ LOG_MSG_LENGTH ] { 0 };
   BOOST_CHECK_EQUAL( memcmp( emptyEntry.msg, zeroMsgBuffer, LOG_MSG_LENGTH ), 0 );
   BOOST_CHECK_EQUAL( emptyEntry.msg_end, 0 );

   const char zeroModuleBuffer[ MODULE_NAME_LENGTH ] { 0 };
   BOOST_CHECK_EQUAL( memcmp( emptyEntry.module_name, zeroModuleBuffer, MODULE_NAME_LENGTH ), 0 );
   BOOST_CHECK_EQUAL( emptyEntry.module_end, 0 );

   // Not testing emptyEntry.severity
}

BOOST_AUTO_TEST_CASE( Log_no_parameters ) {
   LOG_TEST( "Test log entry with no parameters" );

   const LogEntry& anEntry = logPeek();
   BOOST_CHECK_EQUAL( anEntry.ready, true );

   BOOST_CHECK_EQUAL( anEntry.msg, "Test log entry with no parameters" );
   BOOST_CHECK_EQUAL( anEntry.msg_end, 0 );
   BOOST_CHECK_EQUAL( anEntry.module_name, "test_Log");
   BOOST_CHECK_EQUAL( anEntry.module_end, 0 );
   BOOST_CHECK_EQUAL( anEntry.logSeverity, LogSeverity::test );
}

BOOST_AUTO_TEST_CASE( Log_one_int_parameter ) {
   LOG_TEST( "Test log entry with %d parameter", 1 );

   const LogEntry& anEntry = logPeek();
   BOOST_CHECK_EQUAL( anEntry.ready, true );

   BOOST_CHECK_EQUAL( anEntry.msg, "Test log entry with 1 parameter" );
   BOOST_CHECK_EQUAL( anEntry.msg_end, 0 );
   BOOST_CHECK_EQUAL( anEntry.module_name, "test_Log");
   BOOST_CHECK_EQUAL( anEntry.module_end, 0 );
   BOOST_CHECK_EQUAL( anEntry.logSeverity, LogSeverity::test );
}

BOOST_AUTO_TEST_CASE( Log_one_string_parameter ) {
   LOG_TEST( "Test log entry with %s parameter", "one string" );

   const LogEntry& anEntry = logPeek();
   BOOST_CHECK_EQUAL( anEntry.ready, true );

   BOOST_CHECK_EQUAL( anEntry.msg, "Test log entry with one string parameter" );
   BOOST_CHECK_EQUAL( anEntry.msg_end, 0 );
   BOOST_CHECK_EQUAL( anEntry.module_name, "test_Log");
   BOOST_CHECK_EQUAL( anEntry.module_end, 0 );
   BOOST_CHECK_EQUAL( anEntry.logSeverity, LogSeverity::test );
}



BOOST_AUTO_TEST_SUITE_END()
// NOLINTEND( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays )
// NOLINTEND( cppcoreguidelines-pro-bounds-array-to-pointer-decay, hicpp-no-array-decay )
// NOLINTEND( cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers )
/// @endcond
