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
#include "CommodityType.hpp"


#include <boost/assert.hpp>

namespace empire {


Commodity::Commodity( const enum CommodityEnum inCommodityEnum, const commodityValue inMaxValue ) : commodityType (CommodityTypes::CommodityArray[inCommodityEnum]), maxValue ( inMaxValue ) {
   this->validate();
}


Commodity& Commodity::operator += ( const commodityValue increaseBy ) {
   
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
      return *this;
   }
   
   if( newValue > this->maxValue ) {  // If we overflow...
      this->value = maxValue;         // set value to maxValue and...
      throw commodityOverflowException() << errinfo_oldValue( this->value ) 
                                         << errinfo_requestedValue( newValue )
                                         << errinfo_maxValue( this->maxValue );
                                         /// @todo Add CommodityType when its wired in
   }
   
   return *this;
}


Commodity& Commodity::operator -= ( const commodityValue decreaseBy ) {
   
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
      return *this;
   }
   
   if( newValue < 0 ) {               // If we underflow...
      this->value = 0;                // set value to 0 and...
      throw commodityUnderflowException() << errinfo_oldValue( this->value ) 
                                          << errinfo_requestedValue( newValue );
                                         /// @todo Add CommodityType when its wired in
   }
   
   return *this;
}


const bool Commodity::isEnabled() const {
   if ( maxValue >= 1 ) 
      return true;
   
   return false;
}


const commodityValue Commodity::getMaxValue() const {
   return maxValue;
}


const commodityValue Commodity::getValue() const {
   return value;
}


const char Commodity::getName1() const {
   return commodityType.getName1();
}


const string_view Commodity::getName3() const {
   return commodityType.getName3();
}


const string_view Commodity::getName8() const {
   return commodityType.getName8();
}


const bool Commodity::getIsSellable() const {
   return commodityType.getIsSellable();
}


const uint16_t Commodity::getPrice() const { 
   return commodityType.getPrice();
}


const uint8_t Commodity::getWeight() const {
   return commodityType.getWeight();
}


const uint8_t Commodity::getPackingInefficient() const {
   return commodityType.getPackingInefficient();
}


const uint8_t Commodity::getPackingNormal() const {
   return commodityType.getPackingNormal();
}


const uint8_t Commodity::getPackingWarehouse() const {
   return commodityType.getPackingWarehouse();
}


const uint8_t Commodity::getPackingUrban() const {
   return commodityType.getPackingUrban();
}


const uint8_t Commodity::getPackingBank() const {
   return commodityType.getPackingBank();
}


const string_view Commodity::getName32() const {
   return commodityType.getName32();
}


/// @todo Create an appropriate function for Boost's "void assertion_failed"
/// @internal  It's OK to directly access member values here as we are validating
///            the data structure.  The Unit Test Framework will validate the 
///            getters and setters.
bool Commodity::validate() const {
   if( isEnabled() ) {
      BOOST_ASSERT( maxValue <= MAX_COMMODITY_VALUE );
      BOOST_ASSERT( value >= 0 );
      BOOST_ASSERT( value <= maxValue );
   }

   if( !isEnabled() ) {
      BOOST_ASSERT( maxValue == 0 );
      BOOST_ASSERT( value == 0 );
   }
   
   commodityType.validate();

   return true;  // All tests pass
}


}  // namespace empire
