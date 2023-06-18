///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Global logging services for Empire V
///
/// Be sure to define `LOG_MODULE` before including Log.hpp
///
///     #define LOG_MODULE "My_Module"  ///< The name of the module for logging purposes @NOLINT( cppcoreguidelines-macro-usage ): `#define` is OK here
///
/// @file      lib/Log.hpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <cstring>
#include <format>
#include <iostream>
#include <string_view>

#include "LogSeverity.hpp"

namespace empire {

/// Add a new log entry to the circular log queue
///
/// @tparam Args The Variadic template for holding a list of types to be formatted
/// @param severity The severity of the log entry
/// @param module The module for the log entry
/// @param fmt The format string that may use `{}` substitute for `args`
/// @param args The optional set of arguments to be substituted in `fmt`
template<typename...Args>
inline void queueLogEntry( const LogSeverity severity, const std::string_view module, const std::format_string<Args...> fmt, Args&&... args ) {
   const std::string aString { std::format( fmt, std::forward<Args>(args)... ) };

   char aCharArray[10] ;

   std::strncpy( aCharArray, aString.c_str(), 10 );

   std::cout << aCharArray << std::endl;
}

/// Use for Boost Unit Tests
#define LOG_TEST( fmt, ... ) queueLogEntry( LogSeverity::test, LOG_MODULE, fmt __VA_OPT__(,) __VA_ARGS__ )

/// Use when trying follow the thread of execution through code
#define LOG_TRACE ( fmt, ... )

/// Information that is diagnostically helpful
#define LOG_DEBUG ( fmt, ... )

/// Generally useful information
#define LOG_INFO ( fmt, ... )

/// Anything that can potentially cause application oddities
#define LOG_WARN ( fmt, ... )

/// Any error which is fatal to an **operation**
#define LOG_ERROR ( fmt, ... )

/// Any error which is fatal to the **process**
#define LOG_FATAL ( fmt, ... )


} // namespace empire
