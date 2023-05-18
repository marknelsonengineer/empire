///////////////////////////////////////////////////////////////////////////////
//  Empire V
//
/// Base class for all of Empire's exceptions.
///
/// Note:      We will declare class-specific exceptions with the classes...
///            so they won't always be here.
///
/// @file      lib/EmpireExceptions.hpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      29 Jan 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <boost/exception/all.hpp>


namespace empire {

/// Base class for all Empire exceptions
///
/// Note:  We will declare class-specific exceptions with the classes...
///        so they won't always be here.
struct empireException: virtual std::exception, virtual boost::exception { };


/// Thrown when we violate any of the validate() functons throughout
/// the codebase.
struct assertionException: virtual empireException { };


} // namespace empire;
