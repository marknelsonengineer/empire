///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Compile-time configuration options for the Empire logger
///
/// @file      LogConfig.hpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2023 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstddef>  // For size_t
#include <cstdint>  // For uint16_t

namespace empire {

/// The alignment of each LogEntry
constinit const uint16_t LOG_ALIGNMENT { 256 };

/// The maximum size of LogEntry::msg is exactly 1/2 of the size of a LogEntry
constinit const size_t LOG_MSG_LENGTH { LOG_ALIGNMENT >> 1U };

/// The maximum size of LogEntry::module_name
constinit const size_t MODULE_NAME_LENGTH { 32 };

/// empire::LogQueue is a ring buffer modeled after the Linux kernel's DMESG buffer.
/// The size of empire::LogQueue must be a power of 2 (8, 16, 32, ...) entries.
/// This variable enforces that rule.
///
/// | empire::SIZE_OF_QUEUE_BASE_2 | empire::SIZE_OF_QUEUE |
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
constinit const unsigned char SIZE_OF_QUEUE_BASE_2 { 7 };

/// The size of the empire::LogQueue ring buffer
constinit const size_t SIZE_OF_QUEUE { 1U << SIZE_OF_QUEUE_BASE_2 };

/// Mask the actual index into empire::LogQueue from empire::LogIndex
constinit const size_t LOG_QUEUE_INDEX_MASK { SIZE_OF_QUEUE - 1 };

} // namespace empire
