///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Program version and build tracker
///
/// Use Semantic Versioning 2.0.0:  https://semver.org/
///
/// #empire::VERSION_BUILD is modified by update_version.py
///
/// @file      version.cpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <string>

#include "version.hpp"  // Just cuz

namespace empire {

[[maybe_unused]] alignas( int ) const_version_number_t VERSION_MAJOR {  5 };
[[maybe_unused]] alignas( int ) const_version_number_t VERSION_MINOR {  0 };
[[maybe_unused]] alignas( int ) const_version_number_t VERSION_PATCH {  0 };
[[maybe_unused]] alignas( int ) const_build_number_t   VERSION_BUILD { 34 };

[[maybe_unused]] alignas( const std::string ) constinit const std::string FULL_VERSION { "5.0.0+34" };

} // namespace empire
