///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Boost Logging services
///
/// @see https://www.boost.org/doc/libs/1_75_0/libs/log/doc/html/index.html
/// @see https://github.com/snikulov/boost-log-example/blob/master/src/main.cpp
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      Server/Log.cpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      17 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include "Log.hpp"

using namespace std;

namespace empire {

/// Initialize the global logger.
///
///   - Adds a file logger
///   - Adds a console logger
///   - Initialize the default logging level
///
/// @todo I'd like to add channels, but I think a better way is to just add the
///       filename of the source of the log, which is a good proxy for channel.
/// @todo Get into rotating logfiles
/// @todo Miminize the number of includes, namespaces
/// @todo Find a way to configure the initial log level
///
BOOST_LOG_GLOBAL_LOGGER_INIT(empireLogger, logger_t) {
	logger_t lg;

	logging::add_common_attributes();

	logging::add_file_log (
            boost::log::keywords::file_name = SYS_LOGFILE,
            boost::log::keywords::format = (
                    expr::stream << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                                 << " [" << expr::attr<  boost::log::trivial::severity_level >("Severity") << "]: "
                                 << expr::smessage
            )
	);

	logging::add_console_log (
            std::cout,
            boost::log::keywords::format = (
                    expr::stream << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                                 << " [" << expr::attr<     boost::log::trivial::severity_level >("Severity") << "]: "
                                 << expr::smessage
            )
	);

	// This is where the default logging level is set
	logging::core::get()->set_filter (
            logging::trivial::severity >= logging::trivial::info
	);

	return lg;
}



}  // namespace empire
