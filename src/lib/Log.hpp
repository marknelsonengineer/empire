///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Global logging services for Empire V
///
/// There's usually a tradeoff between performance and abstraction.  In the case
/// of a logging utility embedded in complex code, we are prioritizing
/// performance.  To that end, we are breaking a few modern C++ rules:
///   1. Log uses macros like `#define LOG_INFO()`.  This allows us to embed
///      `LOG_INFO( "something" )` into code and then use the precompiler to
///       either keep it or cleanly remove it, depending on the value of
///       `MIN_LOG_SEVERITY`.
///   2. Log uses `printf()` style substitution.  I did a performance analysis
///      of `format()` vs. `printf()` and `format()` incurred a 60% penalty in
///      CPU cycles.  `printf()` uses varargs, so we need to disable a few
///      modern C++ warnings about that.
///   3. Log uses old-school, C-style `char[]` arrays.  Again, this is to maximize
///      performance.  Log pre-allocates an array of #empire::LogEntry structures,
///      managed in a circular queue.  Each #empire::LogEntry is aligned on a
///      large boundary and its largest element #empire::LogEntry::msg is aligned
///      on the same half-boundary. We also map several char-arrays to `(void*)`,
///      which is generally considered bad practice in modern C++.
///
/// Be sure to define `LOG_MODULE` and `MIN_LOG_SEVERITY` before including Log.hpp:
///
///     /// The name of the module for logging purposes
///     /// NOLINTNEXTLINE( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays ): A `char[]` array is acceptable here
///     [[maybe_unused]] alignas(32) static constinit const char LOG_MODULE[32] { "test_Log" };
///
///     /// Logs at and above `MIN_LOG_SEVERITY` will be available.  Logs below
///     /// `MIN_LOG_SEVERITY` will not be compiled into the source file.
///     #define MIN_LOG_SEVERITY LOG_SEVERITY_INFO
///
///     #include "../src/lib/Log.hpp"
///
/// @file      lib/Log.hpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2023 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <cstdarg>  // For va_list, va_start() va_end()
#include <cstdint>  // For uint16_t, uint64_t
#include <cstdio>   // For vsnprintf()
#include <cstring>  // For memcpy()

#include <boost/assert.hpp>  // For BOOST_ASSERT_MSG()

#include "LogSeverity.hpp"

namespace empire {

/// The alignment of each LogEntry
constinit const uint16_t LOG_ALIGNMENT { 256 };

/// The maximum size of LogEntry::msg is exactly 1/2 of the size of a LogEntry
constinit const size_t LOG_MSG_LENGTH { LOG_ALIGNMENT >> 1U };

/// The maximum size of LogEntry::module_name
constinit const size_t MODULE_NAME_LENGTH { 32 };


/// A structure that holds each entry in the log
///
/// Every instance will be aligned to empire::LOG_ALIGNMENT.
///
/// @NOLINTBEGIN( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays ): We use `char[]` arrays here
/// @NOLINTBEGIN( altera-struct-pack-align ): We are not packing data as it's not standardized yet
struct alignas( LOG_ALIGNMENT ) LogEntry {
   /// The log message (aligned to the first half of each LogEntry)
   alignas( LOG_ALIGNMENT >> 1U ) char msg[ LOG_MSG_LENGTH ];

   /// A null byte to terminate LogEntry::msg
   [[maybe_unused]] uint64_t msg_end;

   /// The module that generated this LogEntry
   alignas( MODULE_NAME_LENGTH ) char module_name[ MODULE_NAME_LENGTH ];

   /// A null byte to terminate LogEntry::module_name
   [[maybe_unused]] uint64_t module_end;

   /// The severity of this LogEntry
   [[maybe_unused]] alignas( size_t ) LogSeverity logSeverity;

   /// `true` if the LogEntry is ready to process.  `false` if it's being composed.
   bool ready;
};
// NOLINTEND( altera-struct-pack-align )
// NOLINTEND( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays )


/// empire::LogQueue is a ring buffer modeled after the Linux kernel's DMESG buffer.
/// The size of empire::LogQueue must be a power of 2 (8, 16, 32, ...) entries.
/// This variable enforces that rule.
///
/// | empire::BASE_2_SIZE_OF_QUEUE | empire::SIZE_OF_QUEUE |
/// |:----------------------------:|----------------------:|
/// |               1              |                     1 |
/// |               2              |                     2 |
/// |               3              |                     4 |
/// |               4              |                     8 |
/// |               5              |                    16 |
/// |               6              |                    32 |
/// |               7              |                    64 |
/// |               8              |                   128 |
///
constinit const unsigned char BASE_2_SIZE_OF_QUEUE { 7 };


/// The size of the Log's ring buffer
constinit const size_t SIZE_OF_QUEUE { 1U << BASE_2_SIZE_OF_QUEUE };


/// Retrieve the next available LogEntry from empire::LogQueue
///
/// @return A reference to a LogEntry record that's ready to be written to
extern LogEntry& getNextLogEntry();


/// Add a new log entry to empire::LogQueue
///
/// The function is marked `inline`, however:
/// 1. If disassembled, it won't ever inline because it uses varargs
/// 2. That's OK, because the function is customised for each module with
///    the `LOG_MODULE` and `MIN_LOG_SEVERITY` definitions.
///
/// @param severity The severity of the LogEntry
/// @param module_name The name of the module responsible for this LogEntry
/// @param fmt The `printf`-style format string
/// @NOLINTBEGIN( cert-dcl50-cpp, cppcoreguidelines-pro-type-vararg, hicpp-vararg ): We will allow a C-style variadic function
/// @NOLINTBEGIN( cppcoreguidelines-pro-bounds-array-to-pointer-decay, hicpp-no-array-decay ): For performance reasons, we cast arrays to pointers
/// @NOLINTBEGIN( cert-err33-c ): No need to check the return value from `vsnprintf()`
inline void queueLogEntry( const LogSeverity severity
                         , const char* module_name
                         , const char* fmt
                         , ... ) {

   BOOST_ASSERT_MSG( module_name != nullptr , "Module name can't be NULL" );
   BOOST_ASSERT_MSG( strlen( module_name ) < MODULE_NAME_LENGTH, "Module name must be < MODULE_NAME_LENGTH" );
   BOOST_ASSERT_MSG( fmt != nullptr, "Log format parameter can't be NULL" );
   BOOST_ASSERT_MSG( severity >= LogSeverity::test && severity <= LogSeverity::fatal, "Log severity not in range" );

   LogEntry& thisEntry = getNextLogEntry();
   BOOST_ASSERT_MSG( thisEntry.msg_end == 0, "LogEntry::msg_end marker is not 0" );
   BOOST_ASSERT_MSG( thisEntry.module_end == 0, "LogEntry::module_end marker is not 0" );
   BOOST_ASSERT_MSG( thisEntry.ready == false, "LogEntry::ready is not false" );

   thisEntry.logSeverity = severity;

   /// Use an efficient copy command to populate the module_name
   // @API{ memcpy, https://en.cppreference.com/w/cpp/string/byte/memcpy }
   // memcpy( thisEntry.module_name, module_name, MODULE_NAME_LENGTH );

   asm( "vmovdqa (%0), %%ymm8 ;"  // Copy module_name into YMM8
        "vmovdqa %%ymm8, (%1) ;"  // Copy YMM8 into thisEntry.module_name
         :
         : "a" (module_name)            // Input %0
         , "b" (thisEntry.module_name)  //       %1
         :  "%ymm8", "memory"           // Clobbered
   );

   /// @API{ va_list, https://en.cppreference.com/w/cpp/utility/variadic/va_list }
   va_list args;
   va_start( args, fmt );
   /// @API{ vsnprintf, https://en.cppreference.com/w/cpp/io/c/vfprintf }
   vsnprintf( thisEntry.msg, LOG_MSG_LENGTH, fmt, args );  /// @NOLINT( clang-analyzer-valist.Uninitialized ): `va_start()` initializes `args`.
   va_end( args );

   thisEntry.ready = true;  // Tell the LogHandler routines that this LogEntry is ready
}


/// Use for Boost Unit Tests
#if MIN_LOG_SEVERITY <= LOG_SEVERITY_TEST
    /// @NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_TEST( fmt, ... ) queueLogEntry( LogSeverity::test, LOG_MODULE, fmt __VA_OPT__(,) __VA_ARGS__ )
#else
   /// @NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_TEST( fmt, ... )
#endif

/// Use when trying follow the thread of execution through code
#if MIN_LOG_SEVERITY <= LOG_SEVERITY_TRACE
    /// @NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_TRACE( fmt, ... ) queueLogEntry( LogSeverity::trace, LOG_MODULE, fmt __VA_OPT__(,) __VA_ARGS__ )
#else
   /// @NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_TRACE( fmt, ... )
#endif

/// Information that is diagnostically helpful
#if MIN_LOG_SEVERITY <= LOG_SEVERITY_DEBUG
    /// NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_DEBUG( fmt, ... ) queueLogEntry( LogSeverity::debug, LOG_MODULE, fmt __VA_OPT__(,) __VA_ARGS__ )
#else
   /// NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_DEBUG( fmt, ... )
#endif

/// Generally useful information
#if MIN_LOG_SEVERITY <= LOG_SEVERITY_INFO
    /// NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_INFO( fmt, ... ) queueLogEntry( LogSeverity::info, LOG_MODULE, fmt __VA_OPT__(,) __VA_ARGS__ )
#else
   /// NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_INFO( fmt, ... )
#endif

/// Anything that can potentially cause application oddities
#if MIN_LOG_SEVERITY <= LOG_SEVERITY_WARNING
    /// NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_WARN( fmt, ... ) queueLogEntry( LogSeverity::warning, LOG_MODULE, fmt __VA_OPT__(,) __VA_ARGS__ )
#else
   /// NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_WARN( fmt, ... )
#endif

/// Any error which is fatal to an **operation**
#if MIN_LOG_SEVERITY <= LOG_SEVERITY_ERROR
    /// NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_ERROR( fmt, ... ) queueLogEntry( LogSeverity::error, LOG_MODULE, fmt __VA_OPT__(,) __VA_ARGS__ )
#else
   /// NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_ERROR( fmt, ... )
#endif

/// Any error which is fatal to the **process**
#if MIN_LOG_SEVERITY <= LOG_SEVERITY_FATAL
    /// NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_FATAL( fmt, ... ) queueLogEntry( LogSeverity::fatal, LOG_MODULE, fmt __VA_OPT__(,) __VA_ARGS__ )
#else
   /// NOLINTNEXTLINE( cppcoreguidelines-macro-usage ): We intend to use a macro here
   #define LOG_FATAL( fmt, ... )
#endif

// NOLINTEND( cert-err33-c )
// NOLINTEND( cppcoreguidelines-pro-bounds-array-to-pointer-decay, hicpp-no-array-decay )
// NOLINTEND( cert-dcl50-cpp, cppcoreguidelines-pro-type-vararg, hicpp-vararg )

} // namespace empire
