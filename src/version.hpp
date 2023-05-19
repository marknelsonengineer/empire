///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Program version and build tracker
///
/// Use Semantic Versioning 2.0.0:  https://semver.org/
///
/// #empire::VERSION_BUILD is modified by update_version.py
///
/// @file      version.hpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

#include "typedefs.h"  // For const_version_number_t

/// All things Empire
namespace empire {

/// Increments with __major__ functional changes
[[maybe_unused]] extern const_version_number_t VERSION_MAJOR;

/// Increments with minor functional changes
[[maybe_unused]] extern const_version_number_t VERSION_MINOR;

/// Increments with bugfixes
[[maybe_unused]] extern const_version_number_t VERSION_PATCH;

/// Monotonic counter, set by update_version.py, to track the number of
/// compilations.
[[maybe_unused]] extern const_build_number_t VERSION_BUILD;

/// The full version number as a string
[[maybe_unused]] extern const std::string FULL_VERSION;

} // namespace empire
