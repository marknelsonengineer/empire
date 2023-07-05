///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// A data structure that holds each entry in empire::LogQueue
///
/// @file      LogEntry.hpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2023 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <chrono>         // For time_t
#include <cstddef>        // For size_t
#include <cstdint>        // For uint64_t

#include "LogConfig.hpp"  // For LOG_ALIGNMENT, LOG_MSG_LENGTH, MODULE_NAME_LENGTH
#include "LogSeverity.hpp"

namespace empire {

/// A structure that holds each entry in empire::LogQueue
///
/// Every instance will be aligned to empire::LOG_ALIGNMENT.
///
/// @NOLINTBEGIN( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays ): `char[]` arrays are used in the Log module
/// @NOLINTBEGIN( altera-struct-pack-align ): We are not packing data as it's not standardized yet
struct alignas( LOG_ALIGNMENT ) LogEntry {
   /// The log message (aligned to the first half of each LogEntry)
   [[maybe_unused]] alignas( LOG_ALIGNMENT >> 1U ) char msg[ LOG_MSG_LENGTH ];

   /// A null byte to terminate LogEntry::msg
   [[maybe_unused]] uint64_t msg_end;

   /// The module that generated this LogEntry
   [[maybe_unused]] alignas( MODULE_NAME_LENGTH ) char module_name[ MODULE_NAME_LENGTH ];

   /// A null byte to terminate LogEntry::module_name
   [[maybe_unused]] uint64_t module_end;

   /// The severity of this LogEntry
   [[maybe_unused]] alignas( size_t ) LogSeverity logSeverity;

   /// The timestamp of this LogEntry
   [[maybe_unused]] alignas( size_t ) std::time_t logTimestamp;

   /// `true` if the LogEntry is ready to process.  `false` if it's being composed.
   bool ready;
};
// NOLINTEND( altera-struct-pack-align )
// NOLINTEND( cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays )

} // namespace empire
