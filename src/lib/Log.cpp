///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Global logging services for Empire V
///
/// @file      lib/Log.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <array>    // For array<>
#include <atomic>   // For atomic_size_t
#include <cstring>  // For memset()

#define LOG_MODULE "Log"  ///< The name of the module for logging purposes @NOLINT( cppcoreguidelines-macro-usage ): `#define` is OK here

#include "../version.hpp"  // For CACHE_LINE_BYTES
#include "Log.hpp"

using namespace std;

namespace empire {

/// A ring buffer that holds an array of LogEntry records
///
/// Normally, this would be a member of a Singleton object.  However, for
/// performance and portability reasons, I'm choosing to make it a static
/// variable in Log.cpp.
///
/// The logger is not stateful between application restarts.  In other words,
/// the LogQueue does not try to save/restore anything to/from disk.
std::array< empire::LogEntry, empire::SIZE_OF_QUEUE > LogQueue;  /// @NOLINT( cppcoreguidelines-avoid-non-const-global-variables ): We are deliberately making this static to act like a member variable.

/// Essentially, this is a pointer to the next available entry in LogQueue.
/// In fact, it holds both the generation counter (the number of times LogQueue
/// has wrapped around) and the current index into LogQueue.
/// @NOLINTNEXTLINE( cppcoreguidelines-avoid-non-const-global-variables ): LogIndex is static, so it's not really a global
alignas( empire::CACHE_LINE_BYTES ) atomic_size_t LogIndex { 0 };


/// Mask the actual index into LogQueue from LogIndex
constinit const size_t LOG_QUEUE_INDEX_MASK { SIZE_OF_QUEUE - 1 };


/// Reset the logger
void logReset() {
   /// - Stop the handler threads

   /// - Zero out LogQueue
   // @NOLINTNEXTLINE( altera-unroll-loops ): No need to unroll this loop
   for( LogEntry& i : LogQueue ) {
      memset( &i, 0, sizeof( LogEntry ) );
   }

   /// - Reset the LogIndex
   LogIndex.store( 0 );

   /// - Start the handler threads

   cout << "Reset the logger" << endl;
   cout << "SIZE_OF_QUEUE=" << SIZE_OF_QUEUE << endl;
}


LogEntry& getNextLogEntry() {
   /// Get the LogEntry and increment the queue (thread safe because LogIndex is
   /// an atomic)
   /// @NOLINTNEXTLINE( cppcoreguidelines-pro-bounds-constant-array-index ): Temporarily disable static bounds checking
   LogEntry& thisEntry = LogQueue[ LogIndex++ & LOG_QUEUE_INDEX_MASK ];

   /// Disable the LogEntry
   thisEntry.ready = false;

   /// For performance reasons, we are not zeroing out the LogEntry
   BOOST_ASSERT_MSG( thisEntry.msg_end == 0, "LogEntry::msg_end marker is not 0" );
   BOOST_ASSERT_MSG( thisEntry.module_end == 0, "LogEntry::module_end marker is not 0" );

   /// Return a reference to the LogEntry
   return thisEntry;
}

/// Peek at the top of the log
///
/// @return A reference to a LogEntry record that's just before LogQueue's
///         head pointer
LogEntry& logPeek() {
   /// @NOLINTNEXTLINE( cppcoreguidelines-pro-bounds-constant-array-index ): Temporarily disable static bounds checking
   return LogQueue[ (LogIndex - 1) & LOG_QUEUE_INDEX_MASK ];
}

} // namespace empire
