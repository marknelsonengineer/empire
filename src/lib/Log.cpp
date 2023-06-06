///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Boost global logging services for Empire V
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      lib/Log.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////


#define LOG_CHANNEL "Log"  /// @NOLINT( cppcoreguidelines-macro-usage ): A #define is OK here
#include "Log.hpp"

// Additional Boost Log #includes

namespace empire {


/// Define an override for the << operator that will map severity_level to a
/// text description.
///
/// @internal This list must match (in the same order) as
///           the severity_level enum.
///
/// @param strm  Stream @todo
/// @param level Log level @todo
/// @return Not sure, but we'll sort it out @todo
//std::ostream& operator<< (std::ostream& strm, severity_level level) {
//	static const char* strings[] = {
//      "test"
//	  ,"trace"
//	  ,"debug"
//	  ,"info"
//	  ,"warning"
//	  ,"error"
//	  ,"critical"
//	  ,"fatal"
//    };
//
//    if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
//        strm << strings[level];
//    else
//        strm << static_cast< int >(level);
//
//    return strm;
//}


// Define the attribute keywords
//BOOST_LOG_ATTRIBUTE_KEYWORD(severity ,"Severity", severity_level)
//BOOST_LOG_ATTRIBUTE_KEYWORD(channel  ,"Channel", std::string)


/// Initialize the global logger.
///
///   - Add a file logger
///   - Add a console logger
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
//BOOST_LOG_GLOBAL_LOGGER_INIT(empireLogger, logger_t) {
//
//	return logger;
//}


} // namespace empire
