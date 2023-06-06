///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Boost global logging services for Empire V
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      lib/Log.hpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <string_view>

namespace empire {

/// Severity levels
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
/// all channels.
constinit const severity_level default_log_severity_level = test;


/// LOG_CHANNEL should be set by each user of Log before the `#include`
#ifndef LOG_CHANNEL
   #pragma GCC warning "LOG_CHANNEL should be set by each user of Log before the #include"
   #define LOG_CHANNEL "Unknown"
#endif

/// Define the Log sinks...
///
/// ## Usage
///     LOG_INFO << "Starting " << PROGRAM_TITLE << " core services";
///

/// @todo Identify when we are doing a Release build and zero out LOG_TEST, LOG_TRACE and LOG_DEBUG

/// Use for Boost Unit Tests
#define LOG_TEST     (void)

/// Use when trying follow the thread of execution through code
#define LOG_TRACE    (void)

/// Information that is diagnostically helpful
#define LOG_DEBUG    (void)

/// Generally useful information
#define LOG_INFO     (void)

/// Anything that can potentially cause application oddities
#define LOG_WARN     (void)

/// Any error which is fatal to an **operation**
#define LOG_ERROR    (void)

/// Any error which is fatal to the **process**
#define LOG_FATAL    (void)


} // namespace empire
