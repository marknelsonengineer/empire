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
//[ example_expressions_channel_severity_filter
// We define our own severity levels
//enum severity_level
//{
//    normal,
//    notification,
//    warning,
//    error,
//    critical
//};

// Define the attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id  ,"LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity ,"Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel  ,"Channel", std::string)

//<-
std::ostream& operator<< (std::ostream& strm, severity_level level) {
    static const char* strings[] =
    {
        "normal",
        "notification",
        "warning",
        "error",
        "critical"
    };

    if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
        strm << strings[level];
    else
        strm << static_cast< int >(level);

    return strm;
}
//->

void init()
{
    // Create a minimal severity table filter
    typedef expr::channel_severity_filter_actor< std::string, severity_level > min_severity_filter;
    min_severity_filter min_severity = expr::channel_severity_filter(channel, severity);

    // Set up the minimum severity levels for different channels
    min_severity["general"] = notification;
    min_severity["network"] = warning;
    min_severity["gui"] = error;

    logging::add_file_log
    (
        keywords::file_name = "sample_%N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::filter = min_severity || severity >= critical,
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
        keywords::filter = min_severity || severity >= critical,
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

}
