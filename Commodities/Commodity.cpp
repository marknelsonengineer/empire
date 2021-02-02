///////////////////////////////////////////////////////////////////////////////
//  Empire ][
/// Base class for all commodities (food, iron ore, civs, mil, etc.)
/// 
/// @file    Commodity.cpp
/// @version 1.0
///
/// @author Mark Nelson <mr_nelson@icloud.com>
/// @date   29 Jan 2021
///////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <cstdbool>


/// Standard signed integer for commodity values.
///
/// Commodities are integers that range from 0 to 1,000.  It's important
/// that intermediate calculations use signed arithmitic, so I think
/// commodities should be signed integers.  
///
/// For efficiency and marshaling purposes, I think it's a good idea to
/// use a fixed-width integer type.
typedef std::int16_t commodityValue;


/// Base class for all commodities (food, iron ore, civs, mil, etc.)
///
/// @internal
/// This is the "Extrinsic" part of a Flyweight design pattern.  
/// All commodities have a number of variables that are the same, so
/// they will be found in CommodityType.
///
class Commodity {
public:
   /// Constructor for Commodity.  Commodity has a few immutable values (isEnabled and maxValue),
   /// so they are required when Commodity is built.
	Commodity( const bool           inIsEnabled
	          ,const commodityValue inMaxValue 
	          ) :
  	           isEnabled           (inIsEnabled)
             ,maxValue            (inMaxValue)
   {
      // @todo Validate the object
   }
   
private: 

   /// Set if the commodity is used in a Resource.
   /// Commodities will be stored in Resources (another base class for ships,
   /// Sectors, etc.).  A submarine, for example, can not transport Rads, 
   /// in which case, we set isEnabled to false.  Once set, it should be a 
   /// constant.
   const bool isEnabled;


   /// Holds the maximum value of the commodity.  When isEnabled is true,
   /// it should range from 1 to 1,000.  Once set, it should be constant.
   const commodityValue maxValue;


   /// Holds the value of the commodity.  It should range from 0 to maxValue.
   commodityValue value;

};


