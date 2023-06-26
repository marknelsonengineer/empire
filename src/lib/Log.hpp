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

[[maybe_unused]] constinit const u_int16_t LOG_ALIGNMENT { 256 };  ///< The alignment of each LogEntry
constinit const size_t MODULE_NAME_LENGTH { 32 };     ///< The length of the Module string
constinit const size_t LOG_MSG_LENGTH { 128 };        ///< The maximum length of the log message (should be a power of 2)

/// A structure that holds each log entry
///
/// Every instance will be aligned to 128-byte boundary.
///
/// @NOLINTBEGIN( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays ): We use character arrays here
/// @NOLINTBEGIN( altera-struct-pack-align ): We are not packing data as it's not standardized yet.
struct alignas( LOG_ALIGNMENT ) LogEntry {
   alignas( LOG_ALIGNMENT >> 1U ) char msg[LOG_MSG_LENGTH];  ///< The log message (aligned to the first half of each LogEntry)
   [[maybe_unused]] u_int64_t msg_end;         ///< A null byte to terminate LogEntry::msg
   alignas( size_t ) char module_name[MODULE_NAME_LENGTH];   ///< The module that generated this entry
   [[maybe_unused]] u_int64_t module_end;      ///< A null byte to terminate LogEntry::module_name
   alignas( size_t ) LogSeverity logSeverity;  ///< The severity of this LogEntry
   bool ready;                                 ///< `true` if the LogEntry is ready to process.  `false` if it's being composed.
};
// NOLINTEND( altera-struct-pack-align )
// NOLINTEND( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays )


/// The Log queue is a ring buffer modeled after the Linux kernel's DMESG buffer.
/// The size of the queue must be a power of 2 (8, 16, 32, ...) entries.
/// This variable enforces that rule
constinit const unsigned char BASE_2_SIZE_OF_QUEUE { 7 };


/// The size of the Log's ring buffer
constinit const size_t SIZE_OF_QUEUE { 1U << BASE_2_SIZE_OF_QUEUE };


/// Retrieve the next available LogEntry from the queue
///
/// @return A reference to a LogEntry record that's ready to be written to
extern LogEntry& getNextLogEntry();


/// Add a new log entry to the circular log queue
///
/// @tparam Args The Variadic template for holding a list of types to be formatted
/// @param severity The severity of the LogEntry
/// @param module_name The name of the module responsible for this LogEntry
/// @param fmt The `printf`-style format string
/// @param args The optional set of arguments to be substituted in `fmt`
template< typename...Args >
inline void
queueLogEntry( const LogSeverity severity
               , const char* module_name
               , const std::format_string< Args... > fmt
               , Args&& ... args ) {

   LogEntry& thisEntry = getNextLogEntry();
   thisEntry.logSeverity = severity;

   // Do efficient string copies to populate the module_name & log entry

//   #ifdef NDEBUG
      _mm256_store_si256 ((__m256i*)&thisEntry.module_name[0], _mm256_load_si256((__m256i const*) &module_name[0]));
//   #else
//      memcpy( thisEntry.module_name, module_name, MODULE_NAME_LENGTH );
//   #endif
/*
   va_list args;
   va_start(args, fmt);

   const std::string aString { std::format( fmt, std::forward< Args >( args )... ) };

   std::strncpy( static_cast<char*>(thisEntry.msg), aString.c_str(), LOG_MSG_LENGTH );

// std::cout << aCharArray << std::endl;
   thisEntry.ready = true;  // Tell the LogHandler routines that this LogEntry is ready
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
