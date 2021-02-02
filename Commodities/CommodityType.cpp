///////////////////////////////////////////////////////////////////////////////
/// Empire ][
///
/// @brief   Helper class for all commodities (food, iron ore, civs, mil, etc.)
///
///          This is the "Intrinsic" part of a Flyweight design pattern.  
///          All commodities have a number of variables that are the same, so
///          they will be found in here.  Commodity will hold the variables that 
///          change with each instance.
///
/// @file    CommodityType.cpp
/// @version 1.0
///
/// @author Mark Nelson <@todo mr_nelson@icloud.com>
/// @date   29 Jan 2021
///////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <cstdbool>
#include <cstring>


/// @enum Identifies the Commodity by type.  Acts as an index into the Commodities array
enum CommodityEnum {CIV, MIL, SHELL, GUN, PETROL, IRON_ORE, GOLD_DUST, GOLD_BAR, FOOD, OIL, LCM, HCM, UCW, RAD };



/// @def The number of commodity types.  Should match the number of elements in the enum and in the CommodityTypes array.
#define NUMBER_OF_COMMODITY_TYPES (14)




/// @class Heper class for all commodities (food, iron ore, civs, mil, etc.)
class CommodityType {
public:
   CommodityType(const char     inName1 
                ,const char*    inName3
                ,const char*    inName8
                ,const uint16_t inPower
                ,const bool     inIsSellable
                ,const uint16_t inValue
                ,const uint8_t  inWeight
                ,const uint8_t  inPackingInefficient
                ,const uint8_t  inPackingNormal
                ,const uint8_t  inPackingWarehouse
                ,const uint8_t  inPackingUrban
                ,const uint8_t  inPackingBank
                ,const char*    inName32
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

private: 
   /// @var The 1 character mnemonic for this commodity.
   const char name1;


   /// @var The 3 character mnemonic for this commodity.
   const char* name3;


   /// @var The 8 character name for this commodity.
   const char* name8;
   

   /// @var How much 1000 units of this item contribute to power (see info power).
   const uint16_t power;


   /// @var Whether you can sell the item on the market.
   const bool isSellable;


   /// @var The value if the item is mortgaged.  Also known as the "Melt Denominator".
   const uint16_t value;


   /// @var The weight of the item, which determines how much mobility it takes to move it.
   const uint8_t weight;


   /// @var The packing bonus the item receives in inefficient (<60%) sectors.
   const uint8_t packingInefficient;


   /// @var The packing bonus the item receives in normal sectors.
   const uint8_t packingNormal;


   /// @var The packing bonus the item receives in warehouse sectors.
   const uint8_t packingWarehouse;


   /// @var The packing bonus the item receives in urban sectors.
   const uint8_t packingUrban;


   /// @var The packing bonus the item receives in bank sectors.
   const uint8_t packingBank;


   /// @var The up-to-32 character name for this commodity.
   const char* name32;


public:
   /// @brief Return the 1-character mnemonic for this commodity.
   const char getName1() {
      return name1;
   }


   /// @brief Validate the health of the CommodityType
   void validate() {
      if( strlen(name3) > 3 ) {
         /// @todo throw an exception
      }

      if (strlen(name8) > 8 ) {
         /// @tod throw an exception
      }

      if (strlen(name32) > 32 ) {
         /// @todo throw an exception
      }

      /// @todo More validations
   }
};



/// Container class of CommodityType
/// This should use the Singleton design pattern
class CommodityTypes {
private:
   /// @var Array of CommodityTypes -- the intrinsic values of various Commodities.
   CommodityType CommodityArray[9] = {
      //                                  power sellable value weight    packing          long name
      //                                                               in  no  wh  ur  bk
      CommodityType('c', "civ", "Civilian",  50,   false,    4,     1,  1, 10, 10, 10, 10, "Civilians") 
     ,CommodityType('m', "mil", "Military", 100,    true,   20,     1,  1,  1,  1,  1,  1, "Military")
     ,CommodityType('s', "shl", "Shells",   125,    true,   80,     1,  1,  1, 10,  1,  1, "Shells")
     ,CommodityType('g', "gun", "Guns",     950,    true,  100,    10,  1,  1, 10,  1,  1, "Guns")
     ,CommodityType('p', "pet", "Petrol",     7,    true,   50,     1,  1,  1, 10,  1,  1, "Petrolium")
     ,CommodityType('i', "ore", "Ore",       10,    true,  100,     1,  1,  1, 10,  1,  1, "Iron ore")
     ,CommodityType('d', "gld", "Dust",     200,    true,  100,     5,  1,  1, 10,  1,  1, "Gold dust")
     ,CommodityType('b', "bar", "Bars",    2500,    true,  200,    50,  1,  1,  5,  1,  4, "Bars of gold")
     ,CommodityType('f', "eat", "Food",       0,    true,    2,     1,  1,  1, 10,  1,  1, "Food")
     /// @todo the rest of the commodities
};


public:
   // Disallow access by anything other than via enum
   CommodityType &operator[](int i) {
      /// @todo Throw exception...
      return CommodityArray[i];  // Will never get here
   }

   CommodityType &operator[](enum CommodityEnum i) {
      return CommodityArray[i];
   }

/// @todo Figure out how to return the number of Commodities... I'm thinking hardcode it for ease

   void validate() {
      if ( CommodityArray[CIV].getName1() != 'c' ) {
         /// @todo throw CommodityValidationExceptionb
      }
   }

   void print() {
      /// @todo Print the commodity type list
   }

};
