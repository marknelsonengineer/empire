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

#include <boost/log/common.hpp>

namespace empire {

/// Define the name of the logfile
constinit const char LOGFILE[] = "./empire_%N.log";


/// Empire-specific severity levels.  
///
/// This list is in order of severity (from lowest to highest).
enum severity_level {
	trace,
	debug,
	info,
	warning,
	error,
	critical,
	fatal
};


/// Define a default log severity level.  Logs at or above
/// this level will be printed.  Logs below this level
/// will be suppressed.
constinit const severity_level default_log_severity_level = debug;



/// Narrow-char, thread-safe logger w/ severity and channel
typedef boost::log::sources::severity_channel_logger_mt< severity_level, std::string > logger_t;


/// A Boost global logger
BOOST_LOG_GLOBAL_LOGGER(empireLogger, logger_t)


} // namespace empire
