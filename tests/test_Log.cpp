///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Test Boost global logging services for Empire V
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      tests/test_Log.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
/// @cond Suppress Doxygen warnings

#define BOOST_TEST_MODULE Empire
#define BOOST_TEST_MAIN  // Include this to get Boost Test's main()

#include <boost/test/execution_monitor.hpp>
#include <boost/test/unit_test.hpp>

#define LOG_CHANNEL "test_Log"  /// @NOLINT( cppcoreguidelines-macro-usage ): A #define is OK here
#include "../src/lib/LogSeverity.hpp"

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
}


BOOST_AUTO_TEST_CASE( Log_general ) {
}


BOOST_AUTO_TEST_SUITE_END()
/// @endcond
