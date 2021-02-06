///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Base class for all commodities (food, iron ore, civs, mil, etc.) that keeps
/// data that varies between instances of a commodity.
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      Commodity.cpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      29 Jan 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include <cstdbool>
#include <stdio.h>
#include <boost/assert.hpp>

#include "Commodity.hpp"

namespace empire {


Commodity::Commodity( const commodityValue inMaxValue ) : maxValue ( inMaxValue ) {
   this->validate();
}


bool Commodity::isEnabled() {
   if ( maxValue >= 1 ) 
      return true;
   
   return false;
}


const commodityValue Commodity::getMaxValue() {
   return maxValue;
}


/// @todo Create an appropriate function for Boost's "void assertion_failed"
/// @internal  It's OK to directly access member values here as we are validating
///            the data structure.  The Unit Test Framework will validate the 
///            getters and setters.
bool Commodity::validate() {
   if( maxValue >= 1 ) {
      BOOST_ASSERT( maxValue <= MAX_COMMODITY_VALUE );
      BOOST_ASSERT( value >= 0 );
      BOOST_ASSERT( value <= maxValue );
   }

   if( maxValue < 1 ) {
      BOOST_ASSERT( maxValue == 0 );
      BOOST_ASSERT( value == 0 );
   }
   
   return true;  /// All tests pass
}


}  // namespace empire


//struct empireException : public boost::exception, public std::exception
//{
//  const char *what() const noexcept { return "Empire Exceptio"; }
//};


//namespace boost {
//    void assertion_failed(char const * expr, char const * function,
//        char const * file, long line) {
//         printf( "BOOO!!\n" ) ;
//          return ;
//        }
//}
