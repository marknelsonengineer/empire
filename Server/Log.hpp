///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// The Logging services of the Empire ][ server.
///
//  The documentation for classes in this file are in the .hpp file.
/// data that varies between instances of a commodity.
///
/// @file      Server/Log.hpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      16 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <boost/log/sources/logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/sources/channel_feature.hpp>
#include <boost/log/sources/channel_logger.hpp>

namespace logging  = boost::log;
namespace sinks    = boost::log::sinks;
namespace src      = boost::log::sources;
namespace expr     = boost::log::expressions;
namespace attrs    = boost::log::attributes;
namespace keywords = boost::log::keywords;

#define SYS_LOGFILE "./empire.log"

#define LOG_TRACE    BOOST_LOG_SEV(empireLogger::get(), boost::log::trivial::trace)
#define LOG_DEBUG    BOOST_LOG_SEV(empireLogger::get(), boost::log::trivial::debug)
#define LOG_INFO     BOOST_LOG_SEV(empireLogger::get(), boost::log::trivial::info)
#define LOG_WARN     BOOST_LOG_SEV(empireLogger::get(), boost::log::trivial::warning)
#define LOG_ERROR    BOOST_LOG_SEV(empireLogger::get(), boost::log::trivial::error)
#define LOG_CRITICAL BOOST_LOG_SEV(empireLogger::get(), boost::log::trivial::critical)
#define LOG_FATAL    BOOST_LOG_SEV(empireLogger::get(), boost::log::trivial::fatal)

/// Narrow-char thread-safe logger that contains a severity level (trace through fatal)
typedef boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> logger_t;

namespace empire {

/// A Boost global logger with custom initialization
BOOST_LOG_GLOBAL_LOGGER(empireLogger, logger_t)

}
