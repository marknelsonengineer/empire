///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// The severity of a log message
///
/// @file      LogSeverity.hpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2023 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <string_view>

#include <boost/assert.hpp>

using namespace std::string_view_literals;  /// @NOLINT( google-global-names-in-headers ): We need to use it here for `sv`

namespace empire {

/// @NOLINTBEGIN( modernize-macro-to-enum ): We need to use macros

#define LOG_SEVERITY_TEST    ( 0 )  ///< Use for Boost Unit Tests
#define LOG_SEVERITY_TRACE   ( 1 )  ///< Use when trying follow the thread of execution through code
#define LOG_SEVERITY_DEBUG   ( 2 )  ///< Information that is diagnostically helpful
#define LOG_SEVERITY_INFO    ( 3 )  ///< Generally useful information
#define LOG_SEVERITY_WARNING ( 4 )  ///< Anything that can potentially cause application oddities
#define LOG_SEVERITY_ERROR   ( 5 )  ///< Any error which is fatal to an **operation**
#define LOG_SEVERITY_FATAL   ( 6 )  ///< Any error which is fatal to the **process**

// NOLINTEND( modernize-macro-to-enum )



/// Severity levels
///
/// This list is in order of severity (from lowest to highest).
enum class LogSeverity {
   test    = LOG_SEVERITY_TEST,     ///< Use for Boost Unit Tests
   trace   = LOG_SEVERITY_TRACE,    ///< Use when trying follow the thread of execution through code
   debug   = LOG_SEVERITY_DEBUG,    ///< Information that is diagnostically helpful
   info    = LOG_SEVERITY_INFO,     ///< Generally useful information
   warning = LOG_SEVERITY_WARNING,  ///< Anything that can potentially cause application oddities
   error   = LOG_SEVERITY_ERROR,    ///< Any error which is fatal to an **operation**
   fatal   = LOG_SEVERITY_FATAL,    ///< Any error which is fatal to the **process**
   COUNT                            ///< The number of log severity levels
};


/// Convert LogSeverity to a string_view
///
/// Is `constexpr` so it can be used at both compile-time *and* runtime.
///
/// @param logSeverity The LogSeverity to convert
/// @return A `string_view` representation of the enum
constexpr std::string_view LogSeverityToString( enum LogSeverity logSeverity ) {
   switch( logSeverity ) {
      case LogSeverity::test:
         return "test"sv;
      case LogSeverity::trace:
         return "trace"sv;
      case LogSeverity::debug:
         return "debug"sv;
      case LogSeverity::info:
         return "info"sv;
      case LogSeverity::warning:
         return "warning"sv;
      case LogSeverity::error:
         return "error"sv;
      case LogSeverity::fatal:
         return "fatal"sv;
      case LogSeverity::COUNT:
         BOOST_ASSERT_MSG( false, "Invalid use of LogSeverityToString()" );
   }

   BOOST_ASSERT_MSG( false, "No valid case in LogSeverityToString() switch statement" );
}


/// Put to operator override for LogSeverity
///
/// @param stream The stream to write to
/// @param severity The severity to output
/// @return A stream that's been written to
extern std::ostream& operator<<( std::ostream& stream, LogSeverity severity );


/// Const LogSeverity used for Boost Unit Tests
[[maybe_unused]] constinit static const LogSeverity constLogSeverityTest { LogSeverity::test };

/// Const LogSeverity used when trying follow the thread of execution through code
[[maybe_unused]] constinit static const LogSeverity constLogSeverityTrace { LogSeverity::trace };

/// Const LogSeverity used for information that is diagnostically helpful
[[maybe_unused]] constinit static const LogSeverity constLogSeverityDebug { LogSeverity::debug };

/// Const LogSeverity used for generally useful information
[[maybe_unused]] constinit static const LogSeverity constLogSeverityInfo { LogSeverity::info };

/// Const LogSeverity used for anything that can potentially cause application oddities
[[maybe_unused]] constinit static const LogSeverity constLogSeverityWarning { LogSeverity::warning };

/// Const LogSeverity used for any error which is fatal to an **operation**
[[maybe_unused]] constinit static const LogSeverity constLogSeverityError { LogSeverity::error };

/// Const LogSeverity used for any error which is fatal to the **process**
[[maybe_unused]] constinit static const LogSeverity constLogSeverityFatal { LogSeverity::fatal };


} // namespace empire
