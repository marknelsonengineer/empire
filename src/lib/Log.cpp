///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Global logging services for Empire V
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      lib/Log.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <array>

#define LOG_MODULE "Log"  ///< The name of the module for logging purposes @NOLINT( cppcoreguidelines-macro-usage ): `#define` is OK here

#include "Log.hpp"

using namespace std;

namespace { // Anonymous namespace

/// A ring buffer that holds an array of LogEntry records
///
/// Normally, this would be a member of a Singleton object.  However, for
/// performance and portability reasons, I'm choosing to make it a static
/// variable in Log.cpp.
std::array< empire::LogEntry, empire::SIZE_OF_QUEUE > LogQueue;  /// @NOLINT( cppcoreguidelines-avoid-non-const-global-variables ): We are deliberately making this static act like a member variable.

} // namespace


namespace empire {



LogEntry& getNextLogEntry() {
   LogEntry& thisEntry = LogQueue[0];
   /// Increment the queue

   /// Zero out the LogEntry
   thisEntry.ready = false;

   /// Return a reference to the LogEntry
   return LogQueue[0];
}

} // namespace empire
