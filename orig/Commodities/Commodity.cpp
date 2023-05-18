///////////////////////////////////////////////////////////////////////////////
//  Empire V
//
/// Base class for all commodities (food, iron ore, civs, mil, etc.) that keeps
/// data that varies between instances of a commodity.
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      Commodities/Commodity.cpp
/// @version   1.0 - Initial version
/// @version   1.1 - Combined with CommodityTest to support inlining,
///                  constinit and constexpr
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      29 Jan 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include "Commodity.hpp"

#include <boost/assert.hpp>

using namespace std;

namespace empire {

/////////////////////////                             /////////////////////////
/////////////////////////  CommodityType Definitions  /////////////////////////
/////////////////////////                             /////////////////////////

/// Constructor for CommodityType.
///
/// @internal
/// Because this class holds all of its members as const, we need to set them
/// in a constructor and use an initializer list to set them.
///
constexpr CommodityType::CommodityType(
	const char        inName1
  ,const string_view inName3
  ,const string_view inName8
  ,const uint16_t    inPower
  ,const bool        inIsSellable
  ,const uint16_t    inPrice
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
  ,price              (inPrice)
  ,weight             (inWeight)
  ,packingInefficient (inPackingInefficient)
  ,packingNormal      (inPackingNormal)
  ,packingWarehouse   (inPackingWarehouse)
  ,packingUrban       (inPackingUrban)
  ,packingBank        (inPackingBank)
  ,name32             (inName32)
{
   validate();
}


////////////////////////                              ////////////////////////
////////////////////////  CommodityTypes Definitions  ////////////////////////
////////////////////////                              ////////////////////////

/// Static array of CommodityTypes -- the intrinsic values of various Commodities.
///
/// Because it's a static array, it needs to be set here.  It's built at
/// compile-time.
constinit const CommodityType CommodityTypes::CommodityArray[COMMODITY_COUNT] = {
   //                                    power sellable price weight    packing           long name
   //                                                                 in  no  wh  ur  bk
    CommodityType( 'c', "civ", "Civilian",  50,   false,    4,     1,  1, 10, 10, 10, 10, "Civilians" )
   ,CommodityType( 'm', "mil", "Military", 100,    true,   20,     1,  1,  1,  1,  1,  1, "Military" )
   ,CommodityType( 's', "shl", "Shells",   125,    true,   80,     1,  1,  1, 10,  1,  1, "Shells" )
   ,CommodityType( 'g', "gun", "Guns",     950,    true,  100,    10,  1,  1, 10,  1,  1, "Guns" )
   ,CommodityType( 'p', "pet", "Petrol",     7,    true,   50,     1,  1,  1, 10,  1,  1, "Petrolium" )
   ,CommodityType( 'i', "ore", "Ore",       10,    true,  100,     1,  1,  1, 10,  1,  1, "Iron ore" )
   ,CommodityType( 'd', "gld", "Dust",     200,    true,  100,     5,  1,  1, 10,  1,  1, "Gold dust" )
   ,CommodityType( 'b', "bar", "Bars",    2500,    true,  200,    50,  1,  1,  5,  1,  4, "Bars of gold" )
   ,CommodityType( 'f', "eat", "Food",       0,    true,    2,     1,  1,  1, 10,  1,  1, "Food" )
   ,CommodityType( 'o', "oil", "Oil",       50,    true,   50,     1,  1,  1, 10,  1,  1, "Oil" )
   ,CommodityType( 'l', "lcm", "LCM",       20,    true,  100,     1,  1,  1, 10,  1,  1, "Light products" )
   ,CommodityType( 'h', "hcm", "HCM",       40,    true,  100,     1,  1,  1, 10,  1,  1, "Heavy products" )
   ,CommodityType( 'u', "ucw", "UCW",       50,    true,    2,     2,  1,  1, 10,  1,  1, "Uncompensated workers" )
   ,CommodityType( 'r', "rad", "RAD",       50,    true, 1000,     8,  1,  1, 10,  1,  1, "Radioactive material" )
};


void CommodityTypes::validate() {
   BOOST_ASSERT( CommodityArray[CIV].getName1()       == 'c' );
   BOOST_ASSERT( CommodityArray[MIL].getName1()       == 'm' );
   BOOST_ASSERT( CommodityArray[SHELL].getName1()     == 's' );
   BOOST_ASSERT( CommodityArray[GUN].getName1()       == 'g' );
   BOOST_ASSERT( CommodityArray[PETROL].getName1()    == 'p' );
   BOOST_ASSERT( CommodityArray[IRON_ORE].getName1()  == 'i' );
   BOOST_ASSERT( CommodityArray[GOLD_DUST].getName1() == 'd' );
   BOOST_ASSERT( CommodityArray[GOLD_BAR].getName1()  == 'b' );
   BOOST_ASSERT( CommodityArray[FOOD].getName1()      == 'f' );
   BOOST_ASSERT( CommodityArray[OIL].getName1()       == 'o' );
   BOOST_ASSERT( CommodityArray[LCM].getName1()       == 'l' );
   BOOST_ASSERT( CommodityArray[HCM].getName1()       == 'h' );
   BOOST_ASSERT( CommodityArray[UCW].getName1()       == 'u' );
   BOOST_ASSERT( CommodityArray[RAD].getName1()       == 'r' );

   CommodityArray[CIV].validate();
   CommodityArray[MIL].validate();
   CommodityArray[SHELL].validate();
   CommodityArray[GUN].validate();
   CommodityArray[PETROL].validate();
   CommodityArray[IRON_ORE].validate();
   CommodityArray[GOLD_DUST].validate();
   CommodityArray[GOLD_BAR].validate();
   CommodityArray[FOOD].validate();
   CommodityArray[OIL].validate();
   CommodityArray[LCM].validate();
   CommodityArray[HCM].validate();
   CommodityArray[UCW].validate();
   CommodityArray[RAD].validate();
}


/// Print the CommodityTypes array
void CommodityTypes::print() {
   /// @todo Print the CommodityTypes list
}


///////////////////////////                         ///////////////////////////
///////////////////////////  Commodity Definitions  ///////////////////////////
///////////////////////////                         ///////////////////////////

Commodity::Commodity( const CommodityEnum inCommodityEnum
	                  ,const commodityValue inMaxValue          )
	                 : commodityType ( CommodityTypes::CommodityArray[inCommodityEnum])
	                  ,maxValue ( inMaxValue ) {
   validate();
}


/// @todo Create an appropriate function for Boost's "void assertion_failed"
/// @internal  It's OK to directly access member values here as we are validating
///            the data structure.  The Unit Test Framework will validate the
///            getters and setters.
const bool Commodity::validate() const {
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
