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

#include "Commodity.hpp"

#include <boost/assert.hpp>

namespace empire {


Commodity::Commodity( const commodityValue inMaxValue ) : maxValue ( inMaxValue ) {
   this->validate();
}


Commodity Commodity::operator += ( const commodityValue increaseBy ) {
   
   if( !isEnabled() ) {
      throw commodityDisabledException();
   }
   
   // These will bound the size of the increase to a small enough 
   // number to prevent any wraparound issues.
   BOOST_ASSERT( increaseBy >= 0 );
   BOOST_ASSERT( increaseBy <= MAX_COMMODITY_VALUE );
   
   commodityValue newValue = this->value + increaseBy;
   
   if( newValue >= 0 && newValue <= this->maxValue ) {  // Is the new value OK?
      this->value = newValue;
      return this->value;
   }
   
   if( newValue > this->maxValue ) {  // If we overflow...
      this->value = maxValue;         // set value to maxValue and...
      throw commodityOverflowException() << errinfo_oldValue( this->value ) 
                                         << errinfo_requestedValue( newValue )
                                         << errinfo_maxValue( this->maxValue );
                                         /// @todo Add CommodityType when its wired in
   }
   
   return this->value;
}


Commodity Commodity::operator -= ( const commodityValue decreaseBy ) {
   
   if( !isEnabled() ) {
      throw commodityDisabledException();
   }
   
   // These will bound the size of the decrease to a small enough 
   // number to prevent any wraparound issues.
   BOOST_ASSERT( decreaseBy >= 0 );
   BOOST_ASSERT( decreaseBy <= MAX_COMMODITY_VALUE );
   
   commodityValue newValue = this->value - decreaseBy;
   
   if( newValue >= 0 && newValue <= this->maxValue ) {  // Is the new value OK?
      this->value = newValue;
      return this->value;
   }
   
   if( newValue < 0 ) {               // If we underflow...
      this->value = 0;                // set value to 0 and...
      throw commodityUnderflowException() << errinfo_oldValue( this->value ) 
                                          << errinfo_requestedValue( newValue );
                                         /// @todo Add CommodityType when its wired in
   }
   
   return this->value;
}


bool Commodity::isEnabled() {
   if ( maxValue >= 1 ) 
      return true;
   
   return false;
}


const commodityValue Commodity::getMaxValue() {
   return maxValue;
}


const commodityValue Commodity::getValue() {
   return value;
}


/// @todo Create an appropriate function for Boost's "void assertion_failed"
/// @internal  It's OK to directly access member values here as we are validating
///            the data structure.  The Unit Test Framework will validate the 
///            getters and setters.
bool Commodity::validate() {
   if( isEnabled() ) {
      BOOST_ASSERT( maxValue <= MAX_COMMODITY_VALUE );
      BOOST_ASSERT( value >= 0 );
      BOOST_ASSERT( value <= maxValue );
   }

   if( !isEnabled() ) {
      BOOST_ASSERT( maxValue == 0 );
      BOOST_ASSERT( value == 0 );
   }
   
   return true;  // All tests pass
}


}  // namespace empire
