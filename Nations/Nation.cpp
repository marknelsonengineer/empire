///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Concrete class for all nations.
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      Nation.cpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      7 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include <boost/assert.hpp>

#include "Nation.hpp"

using namespace std;

namespace empire {


/////////////////////////////                      ////////////////////////////
/////////////////////////////  Nation Definitions  ////////////////////////////
/////////////////////////////                      ////////////////////////////

// Define & initialize the nationCounter static variable
Nation_ID Nation::nationCounter = 0;


Nation::Nation() {

	if( nationCounter < MAX_NATIONS ) {
		id = nationCounter++;
	} else { // ...then we have too many Nations
		throw nationLimitExceededException() << errinfo_currentNationCounter( nationCounter )
		                                     << errinfo_maxNations( MAX_NATIONS ) ;
	}

	name = to_string( id );

	status = NEW;

	validate();
}


constexpr const Nation_ID Nation::getID() const {
	return id;
}


///@todo:  Make a decision:  Do validate() fail a BOOST_ASSERT or do they
///        return false?
const bool Nation::validate() const {
	BOOST_ASSERT( id >= 0 );
	BOOST_ASSERT( id <= MAX_NATIONS );

	BOOST_ASSERT( nationCounter >= 0 );
	BOOST_ASSERT( nationCounter <= MAX_NATIONS );

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
const Nation Nations::nations[MAX_NATIONS] = {
	Nation(), Nation(), Nation(), Nation(), Nation()
  ,Nation(), Nation(), Nation(), Nation(), Nation()
  ,Nation(), Nation(), Nation(), Nation(), Nation()
  ,Nation(), Nation(), Nation(), Nation(), Nation()
  ,Nation(), Nation(), Nation(), Nation(), Nation()
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
