///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Boost global logging services for Empire V
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
/// @file      lib/Log.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////


#include "Log.hpp"

// Additional Boost Log #includes
#include <boost/log/expressions.hpp>                      // For BOOST_LOG_ATTRIBUTE_KEYWORD
#include <boost/log/utility/setup/file.hpp>               // For add_file_log
#include <boost/log/utility/setup/console.hpp>            // For add_console_log
#include <boost/log/utility/setup/common_attributes.hpp>  // For add_common_attributes
#include <boost/log/support/date_time.hpp>                // For timestamp in logs

namespace logging  = boost::log;
namespace expr     = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sinks    = boost::log::sinks;


namespace empire {


/// Define an override for the << operator that will map
/// severity_level to a text description.
///
/// @implementation This list must match (in the same order) as
///                 the severity_level enum.
std::ostream& operator<< (std::ostream& strm, severity_level level) {
	static const char* strings[] = {
		"trace"
	  ,"debug"
	  ,"info"
	  ,"warning"
	  ,"error"
	  ,"critical"
	  ,"fatal"
    };

    if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
        strm << strings[level];
    else
        strm << static_cast< int >(level);

    return strm;
}


// Define the attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(severity ,"Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel  ,"Channel", std::string)


/// Initialize the global logger.
///
///   - Adds a file logger
///   - Adds a console logger
///   - Initialize the default logging level
///
/// The Channel is the filename of the source of the log, which is a good proxy
/// for a channel.
///
/// LOG_CHANNEL should be set by -DLOG_CHANNEL=\"$*\" in the Makefile
///
/// @todo Create a custom log sink for testing log messages (consider unbounded_fifo_queue)
/// @todo Implement a log-level API to change the log level in realtime
///
BOOST_LOG_GLOBAL_LOGGER_INIT(empireLogger, logger_t) {
	logger_t lg;

	boost::log::add_common_attributes();

	logging::add_file_log (
		keywords::file_name = LOGFILE,
		keywords::rotation_size = 2 * 1024 * 1024,  // Rotate every 2MB or...
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 5, 0),  // at 12:05am
		keywords::enable_final_rotation = true,
		keywords::filter = severity >= default_log_severity_level,
		keywords::format = ( expr::stream
			<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
			<< ": <" << severity
			<< "> [" << channel << "] "
			<< expr::smessage
		)
	);

	logging::add_console_log (
		std::clog,
		keywords::filter = severity >= default_log_severity_level,
		keywords::format = ( expr::stream
			<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
			<< ": <" << severity
			<< "> [" << channel << "] "
			<< expr::smessage
		)
	);

	return lg;
}


} // namespace empire
