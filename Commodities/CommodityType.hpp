///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Helper class that keeps constant values for commodities.
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

/// Identifies the Commodity by type.  Acts as an index into the Commodities
/// array and therefore must be a global enum (not an enum class).
///
/// The last element is COUNT, which is the number of commodities in the enum.
///
/// I'm deliberately keeping this in the global namespace for convenience.  I 
/// want to refer to Commodities by CIV or LCM not CommodityType.CIV
///
enum CommodityEnum { CIV   =0 ,MIL     =1 ,SHELL    =2  ,GUN     =3
                    ,PETROL=4 ,IRON_ORE=5 ,GOLD_DUST=6  ,GOLD_BAR=7
                    ,FOOD  =8 ,OIL     =9 ,LCM      =10 ,HCM     =11
                    ,UCW  =12 ,RAD    =13 ,COMMODITY_COUNT       =14 };
 

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
class CommodityType {
public:
	
   /// Constructor for CommodityType.
   ///
   /// @internal
   /// Because this class holds all of its members as const, we need to set them
   /// in a constructor and use an initializer list to set them.
   ///
   CommodityType(const char        inName1
                ,const std::string_view inName3
                ,const std::string_view inName8
                ,const uint16_t    inPower
                ,const bool        inIsSellable
                ,const uint16_t    inValue
                ,const uint8_t     inWeight
                ,const uint8_t     inPackingInefficient
                ,const uint8_t     inPackingNormal
                ,const uint8_t     inPackingWarehouse
                ,const uint8_t     inPackingUrban
                ,const uint8_t     inPackingBank
                ,const std::string_view inName32
                );

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


   /// The value if the item is mortgaged.  Also known as the "Melt Denominator".
   const uint16_t value;


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
   constexpr char getName1();

   /// Return the 3-character mnemonic for this commodity.
   constexpr std::string_view getName3();

   /// Return the 8-character mnemonic for this commodity.
   constexpr std::string_view getName8();

	/// Return the power factor for this commodity
	constexpr uint16_t getPower();
	
	/// Return weather you can sell the item on the market.
	constexpr bool getIsSellable();
	
   /// Return the value if the item is mortgaged.  Also known as the "Melt Denominator".
	constexpr uint16_t getValueXXX();  /// @TODO Need to rename Value!!!  COnflicts with Commodty::getValue
	
   /// Return the weight of the item, which determines how much mobility it takes to move it.
	constexpr uint8_t getWeight();
	
   /// Return the packing bonus the item receives in inefficient (<60%) sectors.
	constexpr uint8_t getPackingInefficient();
	
   /// Return the packing bonus the item receives in normal sectors.
	constexpr uint8_t getPackingNormal();
	
   /// Return the packing bonus the item receives in warehouse sectors.
	constexpr uint8_t getPackingWarehouse();
	
   /// Return the packing bonus the item receives in urban sectors.
	constexpr uint8_t getPackingUrban();
	
   /// Return the packing bonus the item receives in bank sectors.
	constexpr uint8_t getPackingBank();
	
   /// Return the up-to-32 character name for this commodity.
	constexpr std::string_view getName32();
 

public:  /////////////////////////// Methods /////////////////////////////////

   /// Validate the health of the CommodityType
   void validate();

};  // class CommodityType



/// Container class of CommodityType
///
/// The commodityArray will be held as a static, so it's easy to get to
///
class CommodityTypes {
private:
   

public:
   /// Static srray of CommodityTypes -- the intrinsic values of various Commodities.
   static CommodityType CommodityArray[COMMODITY_COUNT];


   /// Disallow access by anything other than via enum.
   /// <span style="color:red"> Calling this operator will throw an exception! </span>
//   CommodityType &operator[] (int i);

   /// Use the CommodityEnum as the index to retrieve elements from CommodityArray.
//   CommodityType &operator[] (enum CommodityEnum i);

   void validate();

   void print();

};  // class CommodityTypes

}   // namespace empire;
