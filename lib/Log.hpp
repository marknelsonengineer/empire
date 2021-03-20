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
#include <string_view>

namespace empire {

/// Define the name of the logfile
///
/// Allowed date and time placeholders conform to the ones specified by
/// Boost.DateTime library.
/// @see https://www.boost.org/doc/libs/1_75_0/doc/html/date_time/date_time_io.html#date_time.format_flags
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
///
/// This default severity level is the same for both console
/// and file-based logging.  Furthermore, it's the same for
/// all channels.  Future programmers may want to create
/// custom log levels for different sinks and/or channels.
constinit const severity_level default_log_severity_level = trace;


/// Narrow-char, thread-safe logger w/ severity and channel
typedef boost::log::sources::severity_channel_logger_mt<
	severity_level // The type of severity
  ,std::string    // The channel name
> logger_t;


/// A Boost global logger
///
/// This defines the BOOST_LOG_GLOBAL_LOGGER_INIT function in Log.cpp
///
/// This function gets automatically called the first time a log is
/// generated.
BOOST_LOG_GLOBAL_LOGGER(empireLogger, logger_t)


consteval const char* fixup( const char* x ) {
	if( strcmp( x, "LogTest.cpp" ) == 0 ) return "LogTest";
	if( strcmp( x, "EmpireExceptions.cpp" ) == 0 ) return "EmpireExceptions";
	if( strcmp( x, "Singleton.cpp" ) == 0 ) return "Singleton";
	if( strcmp( x, "Core.cpp" ) == 0 ) return "Core";

	return "Unregistered";
}

/// Define the Log sinks...
///
/// @usage
///     LOG_INFO << "Empire core services starting";
///
#define LOG_TRACE    BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), fixup(__FILE__), trace)
#define LOG_DEBUG    BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), __FILE__, debug)
#define LOG_INFO     BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), __FILE__, info)
#define LOG_WARN     BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), __FILE__, warning)
#define LOG_ERROR    BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), __FILE__, error)
#define LOG_CRITICAL BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), __FILE__, critical)
#define LOG_FATAL    BOOST_LOG_STREAM_CHANNEL_SEV(empireLogger::get(), __FILE__, fatal)


} // namespace empire
