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
#include <string_view>
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
   // @todo Validate the object
}
   

/// Return the 1-character mnemonic for this commodity.
constexpr char CommodityType::getName1() const {
   return name1;
}


/// Return the 3-character mnemonic for this commodity.
constexpr string_view CommodityType::getName3() const {
   return name3;
}


/// Return the 8-character mnemonic for this commodity.
constexpr string_view CommodityType::getName8() const {
   return name8;
}

/// Return the power factor for this commodity
constexpr uint16_t CommodityType::getPower() const {
	return power;
}

/// Return weather you can sell the item on the market.
constexpr bool CommodityType::getIsSellable() const {
	return isSellable;
}

/// Return the price if the item is mortgaged.  Also known as the "Melt Denominator".
constexpr uint16_t CommodityType::getPrice() const { 
	return price;
}

/// Return the weight of the item, which determines how much mobility it takes to move it.
constexpr uint8_t CommodityType::getWeight() const {
	return weight;
}

/// Return the packing bonus the item receives in inefficient (<60%) sectors.
constexpr uint8_t CommodityType::getPackingInefficient() const {
	return packingInefficient;
}

/// Return the packing bonus the item receives in normal sectors.
constexpr uint8_t CommodityType::getPackingNormal() const {
	return packingNormal;
}

/// Return the packing bonus the item receives in warehouse sectors.
constexpr uint8_t CommodityType::getPackingWarehouse() const {
	return packingWarehouse;
}

/// Return the packing bonus the item receives in urban sectors.
constexpr uint8_t CommodityType::getPackingUrban() const {
	return packingUrban;
}

/// Return the packing bonus the item receives in bank sectors.
constexpr uint8_t CommodityType::getPackingBank() const {
	return packingBank;
}

/// Return the up-to-32 character name for this commodity.
constexpr std::string_view CommodityType::getName32() const {
	return name32;
}


/// Validate the health of the CommodityType
void CommodityType::validate() const {
   BOOST_ASSERT(  name3.length() <=  3 );
   BOOST_ASSERT(  name8.length() <=  8 );
   BOOST_ASSERT( name32.length() <= 32 );
   BOOST_ASSERT( power >=0 && power <=2500 );
   BOOST_ASSERT( price >= 2 && price <= 1000 );
 
   /// @todo More validations
}



///////////////////////////////////////////////////////////////////////////////
//                             Commodity Types
///////////////////////////////////////////////////////////////////////////////


/// Static srray of CommodityTypes -- the intrinsic values of various Commodities.
const CommodityType CommodityTypes::CommodityArray[COMMODITY_COUNT] = {
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


/// Disallow access by anything other than via enum.
/// <span style="color:red"> Calling this operator will throw an exception! </span>
//CommodityType &CommodityTypes::operator[] (int i) {
   /// @todo Throw exception...
//   return CommodityArray[i];  // Will never get here
//}


/// Use the CommodityEnum as the index to retrieve elements from CommodityArray.
//CommodityType &CommodityTypes::operator[](enum CommodityEnum i) {
//   return CommodityArray[i];
//}


void CommodityTypes::validate() const {
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
}


void CommodityTypes::print() const {
   /// @todo Print the commodity type list
}


} // namespace empire;
