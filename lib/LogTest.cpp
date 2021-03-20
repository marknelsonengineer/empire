///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Test Boost global logging services for Empire ][
///
/// @see https://www.boost.org/doc/libs/1_75_0/libs/log/doc/html/index.html
/// @see https://github.com/snikulov/boost-log-example/blob/master/src/main.cpp
/// @see https://gist.github.com/silgon/9bbf3cff69aabe1027e8
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @internal  Compile with
///            g++ -std=c++11 test_log_default.cpp -DBOOST_LOG_DYN_LINK -lboost_log -lboost_thread -lpthread -lboost_system
///
/// @file      lib/LogTest.cpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      18 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

/// The name of this test module is Empire_Server
#define BOOST_TEST_MODULE Empire_Server

#include <boost/assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "Log.hpp"

using namespace empire;


/// @internal  Name the test suite after the directory that it's in.  Also,
/// the name should not conflict with other objects in the test suite.
BOOST_AUTO_TEST_SUITE( Log_test_suite )

/// Test an enabled Commodity constructor
BOOST_AUTO_TEST_CASE( Log_general ) {
	BOOST_LOG_CHANNEL_SEV(empireLogger::get(), __FILE__, trace) << "A trace severity level message";
	BOOST_LOG_CHANNEL_SEV(empireLogger::get(), __FILE__, debug) << "A debug severity level message";
	BOOST_LOG_CHANNEL_SEV(empireLogger::get(), __FILE__, info) << "A info severity level message";
	BOOST_LOG_CHANNEL_SEV(empireLogger::get(), __FILE__, warning) << "An warning severity level message";
	BOOST_LOG_CHANNEL_SEV(empireLogger::get(), __FILE__, error) << "A error severity level message";
	BOOST_LOG_CHANNEL_SEV(empireLogger::get(), __FILE__, critical) << "A critical severity level message";
	BOOST_LOG_CHANNEL_SEV(empireLogger::get(), __FILE__, fatal) << "A fatal severity level message";
}

BOOST_AUTO_TEST_SUITE_END()
