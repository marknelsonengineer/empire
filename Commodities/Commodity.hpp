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
#include "EmpireExceptions.hpp"


namespace empire {
   
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


/// Base class for all commodities (food, iron ore, civs, mil, etc.) that keeps
/// data that varies between instances of a commodity.
///
/// The management of Commodities is central to Empire.  This class endeavors
/// to support expressive code in the working logic by allowing expressions
/// like:
///
/// @code
///    someResource.civ += 10;
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
/// Commodities must be enabled or disabled (at the time it is constructed) and can't be changed.
/// Commodities must have a maximum value (at the time it is constructed) and can't be changed.
///
///
/// @pattern Flyweight:  Commodity and CommodityType work together in a Flyweight pattern.
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
      

   /// Override the += operator.  If the Commodity exceeds maxValue, then
   /// throw commodityOverflowException, and leave the Commodity in a 
   /// valid state, with value = maxValue.
   ///
   /// Throw commodityDisabledException if you try to modify a disabled
   /// Commodity.
   Commodity operator +=  ( const commodityValue increaseBy );


   /// Override the -= operator.  If the Commodity goes below 0, then
   /// throw commodityUnderflowException, and leave the Commodity in a 
   /// valid state, with value = 0.
   ///
   /// Throw commodityDisabledException if you try to modify a disabled
   /// Commodity.
   Commodity operator -=  ( const commodityValue decreaseBy );

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
   
   
   /// Return the current value of this Commodity.
   const commodityValue getValue();
   
   
   /// Validate the commodity.
   bool validate();
};


} // namespace empire;
