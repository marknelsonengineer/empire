///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Boost global logging services for Empire ][
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
/// @file      Server/Log.hpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      16 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

//#include <iostream>
//#include <boost/log/expressions.hpp>
//#include <boost/log/sources/severity_channel_logger.hpp>
//#include <boost/log/sources/record_ostream.hpp>
//#include <boost/log/utility/setup/console.hpp>
//#include <boost/log/utility/setup/common_attributes.hpp>
//#include <boost/log/utility/setup/file.hpp>
//#include <boost/log/sinks.hpp>

//#include <boost/log/sources/severity_channel_logger.hpp>  // For BOOST_LOG_CHANNEL_SEV
#include <boost/log/common.hpp>

/// Empire-specific severity levels
enum severity_level {
    normal,
    notification,
    warning,
    error,
    critical
};


/// Narrow-char, thread-safe logger w/ severity and channel
typedef boost::log::sources::severity_channel_logger_mt< severity_level, std::string > logger_t;

extern void init();


namespace empire {


} // namespace empire
