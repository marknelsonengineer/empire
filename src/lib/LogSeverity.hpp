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

#include <string_view>

#include <boost/assert.hpp>

using namespace std::string_view_literals;  /// @NOLINT( google-global-names-in-headers ): We need to use it here for `sv`

namespace empire {

/// Severity levels
///
/// This list is in order of severity (from lowest to highest).
enum class LogSeverity {
   test,     ///< Use for Boost Unit Tests
   trace,    ///< Use when trying follow the thread of execution through code
   debug,    ///< Information that is diagnostically helpful
   info,     ///< Generally useful information
   warning,  ///< Anything that can potentially cause application oddities
   error,    ///< Any error which is fatal to an **operation**
   fatal,    ///< Any error which is fatal to the **process**
   COUNT     ///< The number of log levels
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
