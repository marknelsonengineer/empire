///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Helper class that keeps constant values for commodities.
///
/// This header file will help modularize Commodity, but should not be included
/// anywhere in empire... except for Commodity.cpp.
///
/// It helps Commodity, so if you need some property of Commodity, the include
/// its .hpp file, not this one.  Essentially, these classes are private to
/// Commodity.
///
/// @file      CommodityType.hpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      29 Jan 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <cstdbool>
#include <string_view>

using namespace std;


namespace empire {

/// Heper class for all commodities (food, iron ore, civs, mil, etc.)
///
/// @pattern Flyweight:  Commodity and CommodityType work together in a Flyweight pattern.
///
/// @internal
/// This is the "Intrinsic" part of a Flyweight design pattern.
/// All commodities have a number of variables that are the same, so
/// they will be found in here.  Commodity will hold the variables that
/// change with each instance.
///
class CommodityType final {
public:
   /// Constructor for CommodityType.
   ///
   /// @internal
   /// Because this class holds all of its members as const, we need to set them
   /// in a constructor and use an initializer list to set them.
   ///
   CommodityType(const char             inName1
                ,const std::string_view inName3
                ,const std::string_view inName8
                ,const uint16_t         inPower
                ,const bool             inIsSellable
                ,const uint16_t         inPrice
                ,const uint8_t          inWeight
                ,const uint8_t          inPackingInefficient
                ,const uint8_t          inPackingNormal
                ,const uint8_t          inPackingWarehouse
                ,const uint8_t          inPackingUrban
                ,const uint8_t          inPackingBank
                ,const std::string_view inName32
                ) ;

private:
   /// The 1 character mnemonic for this commodity.
   const char name1;

   /// The 3 character mnemonic for this commodity.
   const std::string_view name3;

   /// The 8 character name for this commodity.
   const std::string_view name8;

   /// How much 1000 units of this item contribute to power (see info power).
   const uint16_t power;

   /// Whether you can sell the item on the market.
   const bool isSellable;

   /// The price if the item is mortgaged.  Also known as the "Melt Denominator".
   const uint16_t price;

   /// The weight of the item, which determines how much mobility it takes to move it.
   const uint8_t weight;

   /// The packing bonus the item receives in inefficient (<60%) sectors.
   const uint8_t packingInefficient;

   /// The packing bonus the item receives in normal sectors.
   const uint8_t packingNormal;

   /// The packing bonus the item receives in warehouse sectors.
   const uint8_t packingWarehouse;

   /// The packing bonus the item receives in urban sectors.
   const uint8_t packingUrban;

   /// The packing bonus the item receives in bank sectors.
   const uint8_t packingBank;

   /// The up-to-32 character name for this commodity.
   const std::string_view name32;


public:  /////////////////////////// Getters //////////////////////////////////
	
   /// Return the 1-character mnemonic for this commodity.
   const char getName1() const;

   /// Return the 3-character mnemonic for this commodity.
   const std::string_view getName3() const;

   /// Return the 8-character mnemonic for this commodity.
   const std::string_view getName8() const;

	/// Return the power factor for this commodity
	const uint16_t getPower() const;
	
	/// Return weather you can sell the item on the market.
	const bool getIsSellable() const;
	
   /// Return the price if the item is mortgaged.  Also known as the "Melt Denominator".
	const uint16_t getPrice() const;
	
   /// Return the weight of the item, which determines how much mobility it takes to move it.
	const uint8_t getWeight() const;
	
   /// Return the packing bonus the item receives in inefficient (<60%) sectors.
	const uint8_t getPackingInefficient() const;
	
   /// Return the packing bonus the item receives in normal sectors.
	const uint8_t getPackingNormal() const;
	
   /// Return the packing bonus the item receives in warehouse sectors.
	const uint8_t getPackingWarehouse() const;
	
   /// Return the packing bonus the item receives in urban sectors.
	const uint8_t getPackingUrban() const;
	
   /// Return the packing bonus the item receives in bank sectors.
	const uint8_t getPackingBank() const;
	
   /// Return the up-to-32 character name for this commodity.
	const std::string_view getName32() const;
 

public:  /////////////////////////// Methods /////////////////////////////////

   /// Validate the health of the CommodityType
   void validate() const;

};  // class CommodityType



/// Container class of CommodityType
///
/// The commodityArray will be held as a static, so it's easy to get to
///
class CommodityTypes final {
private:
   

public:
   /// Static srray of CommodityTypes -- the intrinsic values of various Commodities.
   static const CommodityType CommodityArray[COMMODITY_COUNT];

   static void validate();

   static void print();

};  // class CommodityTypes

}   // namespace empire;
