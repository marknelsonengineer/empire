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

consteval Nation::Nation( const Nation_ID inNumber ) : number (inNumber) {
	
	name = "Unknown"; //to_string( inNumber );
	
	status = NEW;
	
	validate();
}


constexpr const Nation_ID Nation::getID() const {
	return number;
}


///@todo:  Make a decision:  Do validate() fail a BOOST_ASSERT or do they
///        return false?
constexpr bool Nation::validate() const {
	BOOST_ASSERT( number >= 0 );
	BOOST_ASSERT( number <= MAX_NATIONS );
	
	BOOST_ASSERT( name.size() <= MAX_NAME );
	
	/// @todo Build validation for all members
	return true;  // All tests pass
}


////////////////////////////                       ////////////////////////////
////////////////////////////  Nations Definitions  ////////////////////////////
////////////////////////////                       ////////////////////////////


/// Static array of Nations -- we will never have more or less Nation objects.
///
/// Because it's a static array, it needs to be set here.
constinit const Nation Nations::nations[MAX_NATIONS] = {
	Nation( 0), Nation( 1), Nation( 2), Nation( 3), Nation( 4)
  ,Nation( 5), Nation( 6), Nation( 7), Nation( 8), Nation( 9)
  ,Nation(10), Nation(11), Nation(12), Nation(13), Nation(14)
  ,Nation(15), Nation(16), Nation(17), Nation(18), Nation(19) 
  ,Nation(20), Nation(21), Nation(22), Nation(23), Nation(24) 
};


/// @todo Create an appropriate function for Boost's "void assertion_failed"
bool Nations::validate() {
	///@todo Build validation
	for( Nation_ID i = 0 ; i < MAX_NATIONS ; i++ ) {
		BOOST_ASSERT( i == nations[i].getID() );
		nations[i].validate();
	}
	
	return true;  // All tests pass
}

}  // namespace empire
