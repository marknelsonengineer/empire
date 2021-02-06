///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Base class for all of Empire's exceptions.
///
/// @file      EmpireExceptions.hpp
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
struct empireException: virtual std::exception, virtual boost::exception { };


/// Thrown when we violate any of the validate() functons throughout
/// the codebase.
struct assertionException: virtual empireException { };


} // namespace empire;
