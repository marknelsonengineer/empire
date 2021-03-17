///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Concrete class for all nations.
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      Nation/Nation.cpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      7 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include <boost/assert.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <iostream> //REMOVE

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

	cout << to_string(id) << endl;
	rename( to_string( id ));  // Set the name

	status = NEW;

	validate();
}


constexpr const Nation_ID Nation::getID() const {
	return id;
}


const string_view Nation::getName() const {
	return string_view( name );
}


void Nation::rename( const std::string_view newName ) {
	
	string trimmedNewName = string( newName );
	// Map any non-alphanumeric characters to a space
	for( auto &c : trimmedNewName) {
		if( !isalnum( c ) ) {
			c = ' ';
		}
	}
	// Trim whitespace before & after the string
	// Collapse repeating interior whitespace into one ' '
	boost::trim_all( trimmedNewName );
	
	if( trimmedNewName.length() <= 0 ) {
		throw invalid_argument( "A nation must have a name" );
	}
	
	if( trimmedNewName.length() > Nation::MAX_NAME ) {
		throw length_error( "Requested name is greater than the maximum of " + to_string( Nation::MAX_NAME ) + " characters" );
	}
	
	// At this point, trimmedNewName is the new, candidate name
	
	// Iterate over nameMap and see if there are any duplicates (other than our own)
	for( auto &nation : Nations::nameMap ) {
		if( nation.first == trimmedNewName ) {
			if( nation.second == id ) { // We are about to rename a nation to the same name... whatever
				continue;
			} else {
				throw nationNameTakenException() << errinfo_NationID( nation.second )
				                                 << errinfo_NationName( nation.first );
			}
		}
	}

	// Nations::nameMap.empty();
	Nations::nameMap.erase( trimmedNewName );
	Nations::nameMap.emplace( trimmedNewName, id );
	name = trimmedNewName;
	
	/// @todo Implement logging
	/// @todo Implement nameMap validation
	
	// BOOST_LOG_TRIVIAL(info) << "Nation [" << id << "] renamed from [" << "XXX" << "] to [" << trimmedNewName << "]" ;
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
Nation Nations::nations[MAX_NATIONS] = {
	Nation(), Nation(), Nation(), Nation(), Nation()
  ,Nation(), Nation(), Nation(), Nation(), Nation()
  ,Nation(), Nation(), Nation(), Nation(), Nation()
  ,Nation(), Nation(), Nation(), Nation(), Nation()
  ,Nation(), Nation(), Nation(), Nation(), Nation()
};


// Allocate nameMap
map<std::string_view, Nation_ID> Nations::nameMap = map<std::string_view, Nation_ID>();


Nation& Nations::get ( const Nation_ID index ) {
	if( index >= MAX_NATIONS ) {
		throw out_of_range( "index" );
	}

	return nations[index];
}


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
