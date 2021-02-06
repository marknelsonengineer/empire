///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Base class for all commodities (food, iron ore, civs, mil, etc.) that keeps
/// data that varies between instances of a commodity.
///
/// @file      Commodity.hpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      29 Jan 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <cstdbool>


namespace empire {
   
/// Standard signed integer for commodity values.
///
/// Commodities are integers that range from 0 to 1,000.  It's important
/// that intermediate calculations use signed arithmitic, so I think
/// commodities should be signed integers.
///
/// For efficiency and marshaling purposes, I think it's a good idea to
/// use a fixed-width integer type.
typedef std::int16_t commodityValue;
   

/// Absolute maximum value for any and all Commodities.
constexpr commodityValue MAX_COMMODITY_VALUE = 1000;


/// Base class for all commodities (food, iron ore, civs, mil, etc.) that keeps
/// data that varies between instances of a commodity.
///
/// @pattern Flyweight:  Commodity and CommodityType work together in a Flyweight pattern.
///
/// Commodities must be enabled or disabled (at the time it is constructed) and can't be changed.
/// Commodities must have a maximum value (at the time it is constructed) and can't be changed.
///
/// @internal
/// This is the "Extrinsic" part of a Flyweight design pattern.
/// All commodities have a number of variables that change with each instance,
/// which can be found in this class.  CommodityType will hold the values that
/// are common for all commodities of that type.  They will be linked via an
/// enum.
///
class Commodity {
public:
   /// Constructor for Commodity.  If the Commodity needs to exist (for inheritance
   /// and reference purposes) but is not actually used, then construct it
   /// with a maxValue of 0 or false.  For example:
   ///
   /// @code
   /// Commodity submarineMil( 100 );    // Subs can carry up to 100 mil
   /// Commodity submarineRad( false );  // Subs can't carry rad
   /// @endcode
   ///
   Commodity( const commodityValue inMaxValue );

private:
   /// Holds the maximum value of the commodity.  If the resource can not use
   /// the commodity, then set it to 0 (or false).  
   /// This can range from 0 to MAX_COMMODITY_VALUE.  Once set, it can't be changed.
   /// There is no default value, it must be set in the constructor.
   const commodityValue maxValue;


   /// Holds the value of the Commodity.  It should range from 0 to maxValue
   /// for a given instance of a Commodity.
   /// The default value is 0.
   commodityValue value = 0;

public:
   /// True if this Commodity is enabled... sometimes, we have Resources which
   /// not be allowed to have a certain Commodity, so it will be disabled in
   /// which case, this method will return false.
   ///
   /// @internal return true if maxValue is > 0
   bool isEnabled();
   
   
   /// Return the maximum allowed value for this Commodity.
   const commodityValue getMaxValue();
   
   
   /// @todo Write the setter and all of the overrides.
   
   /// Validate the commodity.
   bool validate();
};


} // namespace empire;
