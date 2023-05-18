///////////////////////////////////////////////////////////////////////////////
//  Empire V
//
/// Base class for all of Empire's exceptions.
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      lib/EmpireExceptions.cpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      29 Jan 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include <boost/assert.hpp>

#include "EmpireExceptions.hpp"

namespace empire {


}  // namespace empire


namespace boost {

/// Empire specific handler when we fail BOOST_ASSERT().
///
/// @internal
/// boost::assertion_failed is declared in <boost/assert.hpp> but it is never
/// defined.  The application is expected to supply an appropriate definition.
///
/// @see https://www.boost.org/doc/libs/1_73_0/libs/assert/doc/html/assert.html
///
void assertion_failed(char const * expr, char const * function, char const * file, long line) {
   // printf( "BOOO!!\n" ) ;
   throw empire::assertionException();
}

}  // namespace boost
