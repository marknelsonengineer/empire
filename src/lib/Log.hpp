///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Boost global logging services for Empire V
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
/// @file      lib/Log.hpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <boost/log/common.hpp>

#include <string_view>

namespace empire {

/// Define the name of the logfile
///
/// Date and time placeholders are specified by Boost.DateTime library.
/// @see https://www.boost.org/doc/libs/1_82_0/doc/html/date_time/date_time_io.html
/// ...and...
/// File counter (%N) with an optional width specification (%3N) in a
/// printf-like format. The file counter will always be decimal, zero filled
/// to the specified width.
/// ...or a percent sign (%%).
constinit const char LOGFILE[] = "./empire_%Y%m%d_%H%M.log";


/// Empire-specific severity levels.
///
/// This list is in order of severity (from lowest to highest).
enum severity_level {
   test,     ///< Use for Boost Unit Tests
   trace,    ///< Use when trying follow the thread of execution through code
	debug,    ///< Information that is diagnostically helpful
	info,     ///< Generally useful information
	warning,  ///< Anything that can potentially cause application oddities
	error,    ///< Any error which is fatal to an **operation**
	fatal     ///< Any error which is fatal to the **process**
};


/// Define a default log severity level.  Logs at or above
/// this level will be printed.  Logs below this level
/// will be suppressed.
///
/// This default severity level is the same for both console
/// and file-based logging.  Furthermore, it's the same for
/// all channels.  Future programmers may want to create
/// custom log levels for different sinks and/or channels.
constinit const severity_level default_log_severity_level = test;


/// Narrow-char, thread-safe logger w/ severity and channel
using logger_t = boost::log::sources::severity_channel_logger_mt<severity_level, std::string>;


/// A Boost global logger
///
/// Define the BOOST_LOG_GLOBAL_LOGGER_INIT function in Log.cpp
///
/// This function is automatically called the first time a log is
/// generated
BOOST_LOG_GLOBAL_LOGGER(empireLogger, logger_t)


//#ifndef LOG_CHANNEL
/// LOG_CHANNEL should be set by each user of Log before the `#include`
//#define LOG_CHANNEL "Unknown"

#define INCLUDE_LOG( channelName ) \
namespace empire { \
/** LOG_CHANNEL should be set by each user of Log before `#include "Log.hpp"` **/ \
constexpr const std::string LOG_CHANNEL { "test_Singleton" }; \
}


//constinit const std::string LOG_CHANNEL { "Unknown" };
//#endif

/// Define the Log sinks...
///
/// ## Usage
///     LOG_INFO << "Starting " << PROGRAM_TITLE << " core services";
///

/// @todo Identify when we are doing a Release build and zero out LOG_TEST, LOG_TRACE and LOG_DEBUG

/// Use for Boost Unit Tests
#define LOG_TEST     BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), LOG_CHANNEL, test)

/// Use when trying follow the thread of execution through code
#define LOG_TRACE    BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), LOG_CHANNEL, trace)

/// Information that is diagnostically helpful
#define LOG_DEBUG    BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), LOG_CHANNEL, debug)

/// Generally useful information
#define LOG_INFO     BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), LOG_CHANNEL, info)

/// Anything that can potentially cause application oddities
#define LOG_WARN     BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), LOG_CHANNEL, warning)

/// Any error which is fatal to an **operation**
#define LOG_ERROR    BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), LOG_CHANNEL, error)

/// Any error which is fatal to the **process**
#define LOG_FATAL    BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), LOG_CHANNEL, fatal)


} // namespace empire
