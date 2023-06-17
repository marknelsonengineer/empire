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
/// @param logSeverity The LogSeverity to convert
/// @return A `string_view` representation of the enum
consteval std::string_view LogSeverityToString( enum LogSeverity logSeverity ) {
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
         return "COUNT"sv;  // Consider raising an exception (but for this is `consteval`)
   }
}

} // namespace empire
