///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Program version and build tracker
///
/// Use Semantic Versioning 2.0.0:  https://semver.org/
///
/// #VERSION_BUILD is modified by update_version.py
///
/// @file    version.h
/// @author  Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#pragma once

/// Increments with __major__ functional changes
#define VERSION_MAJOR    5

/// Increments with minor functional changes
#define VERSION_MINOR    0

/// Increments with bugfixes
#define VERSION_PATCH    0

/// Monotonic counter, set by update_version.py, that tracks the number of
/// compilations.
#define VERSION_BUILD 1

#ifndef STRINGIFY_VALUE
/// C preprocesor trick that converts values into strings at compile time
/// @see https://stackoverflow.com/questions/12844364/stringify-c-preprocess
#define STRINGIFY_VALUE(a)  STRINGIFY_VALUE_(a)

/// Second step of the stringify process
#define STRINGIFY_VALUE_(a) #a
#endif

/// The full version number as a narrow string
#define FULL_VERSION     STRINGIFY_VALUE( VERSION_MAJOR ) \
                     "." STRINGIFY_VALUE( VERSION_MINOR ) \
                     "." STRINGIFY_VALUE( VERSION_PATCH ) \
                     "+" STRINGIFY_VALUE( VERSION_BUILD )

/// The full version number as a wide string
#define FULL_VERSION_W L"" FULL_VERSION