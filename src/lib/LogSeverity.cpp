///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// The severity of a log message
///
/// @file      LogSeverity.cpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2023 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "LogSeverity.hpp"

namespace empire {

std::ostream& operator<<( std::ostream& stream, LogSeverity severity ) {
   return stream << LogSeverityToString( severity ) ;
}


} // namespace empire
