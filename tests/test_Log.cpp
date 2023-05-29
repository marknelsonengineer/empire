///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Test Boost global logging services for Empire V
///
/// @see https://www.boost.org/doc/libs/1_82_0/libs/log/doc/html/index.html
/// @see https://github.com/snikulov/boost-log-example/blob/master/src/main.cpp
/// @see https://gist.github.com/silgon/9bbf3cff69aabe1027e8
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @internal  Compile with
///            g++ -std=c++11 test_log_default.cpp -DBOOST_LOG_DYN_LINK -lboost_log -lboost_thread -lpthread -lboost_system
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

#include "../src/lib/Log.hpp"

using namespace empire;


BOOST_AUTO_TEST_SUITE( Log )


BOOST_AUTO_TEST_CASE( Log_general ) {
//   LOG_TEST  <<  "A test message";
//   LOG_TRACE << "A trace message";
//   LOG_DEBUG << "A debug message";
//   LOG_INFO  << "An info message";
//   LOG_WARN  << "A warning message";
//   LOG_ERROR << "An error message";
//   LOG_FATAL << "A fatal message";
}


BOOST_AUTO_TEST_SUITE_END()
/// @endcond
