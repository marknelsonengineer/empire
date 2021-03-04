///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Base class for all commodities (food, iron ore, civs, mil, etc.) that keeps
/// data that varies between instances of a commodity.
///
/// @internal  Commodities are fundamental to Empire.  To that end, I wanted to
///            make this efficient and type safe.  A lot of effort has gone
///            into inlining functions and getting the Flyweight class to be
///            built statically at compile-time and elidgable for inlining.
///
/// @file      Commodity.hpp
/// @version   1.0 - Initial version
/// @version   1.1 - Combined with CommodityTest to support inlining, 
///                  constinit and constexpr
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      29 Jan 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <cstdbool>
#include <string_view>

#include "EmpireExceptions.hpp"

namespace empire {

////////////////////                                      ////////////////////
////////////////////  Fundamental Commodity Declarations  ////////////////////
////////////////////                                      ////////////////////

/// Standard signed integer for commodity values.
///
/// Commodities are integers that range from 0 to MAX_COMMODITY_VALUE.  
/// It's important that intermediate calculations use signed arithmitic, 
/// so I think commodities should be signed integers.
///
/// For efficiency and marshaling purposes, I think it's a good idea to
/// use a fixed-width integer type.
typedef std::int16_t commodityValue;
   

/// Absolute maximum value for any and all Commodities.
constexpr commodityValue MAX_COMMODITY_VALUE = 1000;


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
 
 
///////////////////////////                        ///////////////////////////
///////////////////////////  Commodity Exceptions  ///////////////////////////
///////////////////////////                        ///////////////////////////
 
/// Thrown when we overflow a commodity.  This can usually be absorbed.
/// When this is thown, we are telling the caller that Commodity X went
/// over by Y and now contains Z.  It's up to the caller to take that 
/// information and ignore it or take appropriate action.
/// 
/// In either case, when this is called, the Commodity is still valid
/// and will be set to maxValue.
struct commodityOverflowException: virtual empireException { };

/// On a commodityOverflowException and commodityUnderflowException, this 
/// holds the original value of the Commodity in the exception.
typedef boost::error_info<struct tag_oldValue, commodityValue> errinfo_oldValue;

/// On a commodityOverflowException and commodityUnderflowException, this
/// holds the new/requested value of the Commodity in the exception.
typedef boost::error_info<struct tag_requestedValue, commodityValue> errinfo_requestedValue;
   
/// On a commodityOverfloewException, this holds the maxValue for the Commodity.
typedef boost::error_info<struct tag_maxValue, commodityValue> errinfo_maxValue;
/// @todo Add a reference to the CommodityType when they are wired together


/// Thrown when we underflow a commodity.  This can usually be absorbed.
/// When this is thown, we are telling the caller that Commodity X went
/// under by Y.  It's up to the caller to take that 
/// information and ignore it or take appropriate action.
///
/// oldValue and requestedValue are set.
/// 
/// In either case, when this is called, the Commodity is still valid
/// and will be set to maxValue.
struct commodityUnderflowException: virtual empireException { };


/// Thrown when you try to use a += or -= operator on a disabled Commodity.
struct commodityDisabledException: virtual empireException { };


//////////////////////                                   //////////////////////
//////////////////////  CommodityType Class Declaration  //////////////////////
//////////////////////                                   //////////////////////

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
   constexpr CommodityType(
   	const char             inName1
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

private:  /////////////////////////  Private Members  /////////////////////////
	
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
	constexpr const uint16_t getPower() const;
	
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
   constexpr void validate() const ;

};  // class CommodityType


////////////////                                               ////////////////
////////////////  CommodityTypes Static Container Declaration  ////////////////
////////////////                                               ////////////////

/// Container class of CommodityType
///
/// The commodityArray will be held as a constinit (initialized at compile-time
/// array.  As a static array, it's easy to get to.
///
class CommodityTypes final {
public:
   /// Static srray of CommodityTypes -- the intrinsic values of various Commodities.
   static constinit const CommodityType CommodityArray[COMMODITY_COUNT];

   /// Validate the health of the CommodityTypes class
   static void validate();

	/// Print the CommodityTypes array
   static void print();

};  // class CommodityTypes


////////////////////////                               ////////////////////////
////////////////////////  Commodity Class Declaration  ////////////////////////
////////////////////////                               ////////////////////////

/// Base class for all commodities (food, iron ore, civs, mil, etc.) that keeps
/// data that varies between instances of a commodity.
///
/// The management of Commodities is central to Empire.  This class endeavors
/// to support expressive code in the working logic by allowing expressions
/// like:
///
/// @code
///    someResource.civ += 10;
///    someResource.lcm.getWeight();
/// @endcode
///
/// Essentially, this class is intended to be a "smart integer" in the context
/// of Empire.  If we overflow (go over maxValue), the class should throw
/// a commodityOverflowException or commodityUnderflowException.  The exception
/// will contain information like Commodity X went over by Y and is now Z.  
/// It's up to the caller to decide weather to ignore it or take appropriate
/// action.
///
/// To this end, many arithmitc operators are disabled.
///    += Works as expected:  someResource.civ += 10;
///    -= Works as expected:  someResource.civ -= 10;
///
/// Commodities must have a maximum value (at the time it is constructed) and 
/// can't be changed.  If the Commodity needs to exist (for inheritance
/// and reference purposes) but is not actually used, then construct it
/// with a maxValue of 0 or false.
///
/// @pattern Flyweight:  Commodity and CommodityType work together in a 
///          Flyweight pattern.
///
/// @internal
/// This is the "Extrinsic" part of a Flyweight design pattern.
/// All commodities have a number of variables that change with each instance,
/// which can be found in this class.  CommodityType will hold the values that
/// are common for all commodities of that type.  The Commodity constructor
/// takes an enum and then it stores a reference to the CommodityType entry in
/// the CommodityTypes array.
///
class Commodity {
public:  ////////////////  Constructor and Operator Overrides  ////////////////
	
   /// Constructor for Commodity.  If the Commodity needs to exist (for 
   /// inheritance and reference purposes) but is not actually used, then 
   /// construct it with a maxValue of 0 or false.  For example:
   ///
   /// @code
   /// Commodity submarineMil( 100 );    // Subs can carry up to 100 mil
   /// Commodity submarineRad( false );  // Subs can't carry rad
   /// @endcode
   ///
   Commodity( const enum CommodityEnum inCommodityEnum, const commodityValue inMaxValue );
      

   /// Override the += operator.  If the Commodity exceeds maxValue, then
   /// throw commodityOverflowException, and leave the Commodity in a 
   /// valid state, with value = maxValue.
   ///
   /// Throw commodityDisabledException if you try to modify a disabled
   /// Commodity.
   Commodity& operator += ( const commodityValue increaseBy );


   /// Override the -= operator.  If the Commodity goes below 0, then
   /// throw commodityUnderflowException, and leave the Commodity in a 
   /// valid state, with value = 0.
   ///
   /// Throw commodityDisabledException if you try to modify a disabled
   /// Commodity.
   Commodity& operator -= ( const commodityValue decreaseBy );

private:  /////////////////////////////  Members  /////////////////////////////
	
   /// Holds the type of commodity.  This is the reference into the Flyweight
   /// intrinsic data.
   ///
   /// @internal The reason CommodityType is declared above *in this file*
   ///           is to support inlining and the exotic constexpr declarations 
   const CommodityType &commodityType;
   // const enum CommodityEnum commodityEnum;


   /// Holds the maximum value of the commodity.  If the resource can not use
   /// the commodity, then set it to 0 (or false).  
   /// This can range from 0 to MAX_COMMODITY_VALUE.  Once set, it can't be changed.
   /// There is no default value, it must be set in the constructor.
   const commodityValue maxValue;


   /// Holds the value of the Commodity.  It should range from 0 to maxValue
   /// for a given instance of a Commodity.
   /// The default value is 0.
   commodityValue value = 0;
      
public:  /////////////////////////////  Methods  /////////////////////////////
	
   /// True if this Commodity is enabled... sometimes, we have Resources which
   /// not be allowed to have a certain Commodity, so it will be disabled in
   /// which case, this method will return false.
   ///
   /// @internal return true if maxValue is > 0
   const bool isEnabled() const;
   
   
   /// Return the maximum allowed value for this Commodity.
   const commodityValue getMaxValue() const;
   
   
   /// Return the current value of this Commodity.
   const commodityValue getValue() const;
   
    /// Validate the commodity.
   const bool validate() const;
   

 public:  /////////////////////////////  Getters  /////////////////////////////
 	
   /// Return the 1-character mnemonic for this commodity.
   const char getName1() const;

   /// Return the 3-character mnemonic for this commodity.
   const std::string_view getName3() const;

   /// Return the 8-character mnemonic for this commodity.
   const std::string_view getName8() const;

	/// Return the power factor for this commodity
	constexpr const uint16_t getPower() const;
	
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
};


///////////////////////                                ///////////////////////
///////////////////////  Inline CommodityType Getters  ///////////////////////
///////////////////////                                ///////////////////////

/// Return the power factor for this commodity
constexpr const uint16_t CommodityType::getPower() const {
	return power;
}


/// Validate the health of the CommodityType    
/// 
/// @internal This is declared as an inline constexpr in order to allow 
///           the constinit CommodityType constructor to call it.
//
/// This is a little pointless as these are locked down pretty tight via 
/// const.
constexpr void CommodityType::validate() const {
   BOOST_ASSERT( name3.length()  <=  3 );
   BOOST_ASSERT( name8.length()  <=  8 );
   BOOST_ASSERT( name32.length() <= 32 );
   BOOST_ASSERT( power  >= 0           && power  <= 2500 );
   BOOST_ASSERT( price  >= 2           && price  <= 1000 );
   BOOST_ASSERT( weight >= 1           && weight <=   50 );
   BOOST_ASSERT( packingInefficient == 1 );
   BOOST_ASSERT( packingNormal    >= 1 && packingNormal    <= 10 );
   BOOST_ASSERT( packingWarehouse >= 1 && packingWarehouse <= 10 );
   BOOST_ASSERT( packingUrban     >= 1 && packingUrban     <= 10 );
   BOOST_ASSERT( packingBank      >= 1 && packingBank      <= 10 );
}


/////////////////////////                            /////////////////////////
/////////////////////////  Inline Commodity Getters  /////////////////////////
/////////////////////////                            /////////////////////////

constexpr const uint16_t Commodity::getPower() const {
   return commodityType.getPower();
}

} // namespace empire;
