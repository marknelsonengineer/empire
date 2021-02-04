///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Base class for all commodities (food, iron ore, civs, mil, etc.) that keeps
/// data that varies between instances of a commodity.
///
/// @internal
/// This is the "Extrinsic" part of a Flyweight design pattern.
/// All commodities have a number of variables that change with each instance,
/// which can be found in this class.  CommodityType will hold the values that
/// are common for all commodities of that type.
///
/// @file      Commodity.cpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      29 Jan 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <cstdbool>

#include "Commodity.hpp"

namespace empire {
   
/// Constructor for Commodity.  Commodity has a few immutable values (isEnabled
/// and maxValue), so they are required when Commodity is built.
Commodity::Commodity( const bool           inIsEnabled
	                  ,const commodityValue inMaxValue
	                 ) :
  	                   isEnabled           (inIsEnabled)
                     ,maxValue            (inMaxValue)
   {
      value = 0;
      // @todo Validate the object
   }


/// Validate the commodity.
bool Commodity::validate() {
   if (maxValue < 0)
      return false;
   
   /// @todo More tests
   
   // All tests pass
   return true;
}


}  // namespace empire
