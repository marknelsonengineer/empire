///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Helper class that keeps constant values for commodities.
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      CommodityType.cpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      29 Jan 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <cstdbool>
#include <string>
#include <boost/assert.hpp>


#include "CommodityType.hpp"

namespace empire {
   
/// Constructor for CommodityType.
///
/// @internal
/// Because this class holds all of its members as const, we need to set them
/// in a constructor and use an initializer list to set them.
///
CommodityType::CommodityType(const char        inName1 
                            ,const string_view inName3
                            ,const string_view inName8
                            ,const uint16_t    inPower
                            ,const bool        inIsSellable
                            ,const uint16_t    inValue
                            ,const uint8_t     inWeight
                            ,const uint8_t     inPackingInefficient
                            ,const uint8_t     inPackingNormal
                            ,const uint8_t     inPackingWarehouse
                            ,const uint8_t     inPackingUrban
                            ,const uint8_t     inPackingBank
                            ,const string_view inName32
                            ) : 
                             name1              (inName1)
                            ,name3              (inName3)
                            ,name8              (inName8) 
                            ,power              (inPower)
                            ,isSellable         (inIsSellable)
                            ,value              (inValue)
                            ,weight             (inWeight)
                            ,packingInefficient (inPackingInefficient)
                            ,packingNormal      (inPackingNormal)
                            ,packingWarehouse   (inPackingWarehouse)
                            ,packingUrban       (inPackingUrban)
                            ,packingBank        (inPackingBank)
                            ,name32             (inName32)
{
   // @todo Validate the object
}
   

/// Return the 1-character mnemonic for this commodity.
constexpr char CommodityType::getName1() {
   return name1;
}


/// Return the 3-character mnemonic for this commodity.
constexpr string_view CommodityType::getName3() {
   return name3;
}


/// Validate the health of the CommodityType
void CommodityType::validate() {
   BOOST_ASSERT(  name3.length() <=  3 );
   BOOST_ASSERT(  name8.length() <=  8 );
   BOOST_ASSERT( name32.length() <= 32 );
 
   /// @todo More validations
}



///////////////////////////////////////////////////////////////////////////////
//                             Commodity Types
///////////////////////////////////////////////////////////////////////////////

/// Disallow access by anything other than via enum.
/// <span style="color:red"> Calling this operator will throw an exception! </span>
CommodityType &CommodityTypes::operator[] (int i) {
   /// @todo Throw exception...
   return CommodityArray[i];  // Will never get here
}


/// Use the CommodityEnum as the index to retrieve elements from CommodityArray.
CommodityType &CommodityTypes::operator[](enum CommodityEnum i) {
   return CommodityArray[i];
}


void CommodityTypes::validate() {
   BOOST_ASSERT( CommodityArray[CIV].getName1() == 'c' );
}


void CommodityTypes::print() {
   /// @todo Print the commodity type list
}


} // namespace empire;
