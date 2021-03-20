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
/// @file      lib/Log.cpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      17 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks.hpp>

#include "Log.hpp"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;



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
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id  ,"LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity ,"Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel  ,"Channel", std::string)

/// Initialize the global logger.
///
///   - Adds a file logger
///   - Adds a console logger
///   - Initialize the default logging level
///
/// @todo I'd like to add channels, but I think a better way is to just add the
///       filename of the source of the log, which is a good proxy for channel.
/// @todo Miminize the number of includes, namespaces
/// @todo Find a way to configure the initial log level
/// @todo Create a LogTest unit test and exercise every one of these...
/// @todo Create a custom log sink for testing log messages
///
BOOST_LOG_GLOBAL_LOGGER_INIT(empireLogger, logger_t) {
	logger_t lg;

	logging::add_file_log
    (
        keywords::file_name = LOGFILE,
        keywords::rotation_size = 2 * 1024 * 1024,  // Rotate every 2MB or...
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 5, 0),  // at 12:05am 
    	keywords::enable_final_rotation = true,
        keywords::filter = severity >= default_log_severity_level,
        keywords::format =
        (
            expr::stream
                << line_id
                << ": <" << severity
                << "> [" << channel << "] "
                << expr::smessage
        )
    );


    logging::add_console_log
    (
        std::clog,
        keywords::filter = severity >= default_log_severity_level,
        keywords::format =
        (
            expr::stream
                << line_id
                << ": <" << severity
                << "> [" << channel << "] "
                << expr::smessage
        )
    );
    
	boost::log::add_common_attributes();
		
	return lg;
}

} // namespace empire
