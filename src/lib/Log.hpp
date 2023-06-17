///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Boost global logging services for Empire V
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      lib/Log.hpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <string_view>

namespace empire {

/// Use for Boost Unit Tests
#define LOG_TEST     (void)

/// Use when trying follow the thread of execution through code
#define LOG_TRACE    (void)

/// Information that is diagnostically helpful
#define LOG_DEBUG    (void)

/// Generally useful information
#define LOG_INFO     (void)

/// Anything that can potentially cause application oddities
#define LOG_WARN     (void)

/// Any error which is fatal to an **operation**
#define LOG_ERROR    (void)

/// Any error which is fatal to the **process**
#define LOG_FATAL    (void)


} // namespace empire
