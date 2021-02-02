///////////////////////////////////////////////////////////////////////////////
/// Empire ][
///
/// @brief   Base class for all commodities (food, iron ore, civs, mil, etc.)
///
///          This is the "Extrinsic" part of a Flyweight design pattern.  
///          All commodities have a number of variables that are the same, so
///          they will be found in CommodityType.
///
/// @file    Commodity.cpp
/// @version 1.0
///
/// @author @todo Mark Nelson <@todo mr_nelson@icloud.com>
/// @date   @todo 29 Jan 2021
///////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <cstdbool>


/// @typedef Commodities are integers that range from 0 to 1,000.  It's important
///          that intermediate calculations use signed arithmitic, so I think
///          commodities should be signed integers.  
///
///          For efficiency and marshaling purposes, I think it's a good idea to
///          use a fixed-width integer type.
typedef std::int16_t commodityValue;


/// @class Base class for all commodities (food, iron ore, civs, mil, etc.)
class Commodity {

private: 

   /// @var Commodities will be stored in Resources (another base class for ships,
   ///      Sectors, etc.).  A submarine, for example, can not transport Rads, 
   ///      in which case, we set isEnabled to false.  Once set, it should be a 
   ///      constant.
   const bool isEnabled;


   /// @var Holds the maximum value of the commodity.  When isEnabled is true,
   ///      it should range from 1 to 1,000.  Once set, it should be constant.
   const commodityValue maxValue;


   /// @var Holds the value of the commodity.  It should range from 0 to 1,000.
   commodityValue value;

};


