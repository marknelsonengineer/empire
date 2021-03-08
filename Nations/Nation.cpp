///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Concrete class for all nations.
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      Commodity.cpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      7 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include "Nation.hpp"

using namespace std;

namespace empire {


/////////////////////////////                      ////////////////////////////
/////////////////////////////  Nation Definitions  ////////////////////////////
/////////////////////////////                      ////////////////////////////

constexpr Nation::Nation( const uint8_t inNumber ) : number (inNumber) {
	
	// name = "nation_xx";
	
	status = NEW;
	
	validate();
}


///@TODO:  Make a decision:  Do validate() fail a BOOST_ASSERT or do they
///        return false?
constexpr bool Nation::validate() const {
	BOOST_ASSERT( number >= 0 );
	BOOST_ASSERT( number <= MAX_NATIONS );
	
	/// @TODO Build validation for all members
	return true;  // All tests pass
}


////////////////////////////                       ////////////////////////////
////////////////////////////  Nations Definitions  ////////////////////////////
////////////////////////////                       ////////////////////////////


/// Static array of Nations -- we will never have more or less Nation objects.
///
/// Because it's a static array, it needs to be set here.
constinit const Nation Nations::nations[MAX_NATIONS] = {
	Nation(0), Nation(1), Nation(2), Nation(3) 
};


/// @todo Create an appropriate function for Boost's "void assertion_failed"
bool Nations::validate() {
	///@ TODO Build validation
	for( int i = 0 ; i < MAX_NATIONS ; i++ ) { /// @TODO:  Convert to a proper iterator
		nations[i].validate();
	}
	
	return true;  // All tests pass
}

}  // namespace empire
