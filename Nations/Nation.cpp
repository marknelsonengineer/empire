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
#include "../lib/Log.hpp"


using namespace std;

namespace empire {


/////////////////////////////                      ////////////////////////////
/////////////////////////////  Nation Definitions  ////////////////////////////
/////////////////////////////                      ////////////////////////////

// Define & initialize the nationCounter static variable
Nation_ID Nation::nationCounter = 0;


Nation::Nation() : id(nationCounter) {
	if( nationCounter >= MAX_NATIONS ) {  // ...we have too many Nations
		throw nationLimitExceededException() << errinfo_currentNationCounter( nationCounter )
		                                     << errinfo_maxNations( MAX_NATIONS ) ;
	}

	rename( to_string( id ));  // Set the name

	status = NEW;

	nationCounter++;

	validate();
}


string Nation::fixupName( const string_view newName ) {
	string name = string( newName );
	
	// Map any non-alphanumeric characters to a space
	for( auto &c : name ) {
		if( !isalnum( c ) ) {
			c = ' ';
		}
	}
	
	// Trim whitespace before & after the string and
	// collapse repeating interior whitespace into one ' '
	boost::trim_all( name );
		
	return name;
}


void Nation::rename( const std::string_view newName ) {
	
	string trimmedNewName = fixupName( newName );
	
	if( trimmedNewName.length() <= 0 ) {
		throw invalid_argument( "A nation must have a name" );
	}
	
	if( trimmedNewName.length() > Nation::MAX_NAME ) {
		throw length_error( "Requested name is greater than the maximum of " + to_string( Nation::MAX_NAME ) + " characters" );
	}
	
	// At this point, trimmedNewName is the new, candidate name

//	Nations& nations = Nations::get();

//	Nation_ID mapped_ID = nations.nameMap["Boo"];

/*
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
*/
	name = trimmedNewName;
	
	/// @todo Implement logging
	/// @todo Implement nameMap validation
	
	/// @todo News:  Country [ID] changed their name from oldName to newName
	// BOOST_LOG_TRIVIAL(info) << "Nation [" << id << "] renamed from [" << "XXX" << "] to [" << trimmedNewName << "]" ;
}


/// Validate the health of the Nation object.  
/// @throws BOOST_ASSERT if the validation fails.  A server-level wrapper can
///         catch the exception -- so we can get a stack trace to the root
///         cause.  
/// @returns true if the object is valid -- so we can easily test for 
///          validity in BOOST_ASSERT().
const bool Nation::validate() const {
	BOOST_ASSERT( id >= 0 );
	BOOST_ASSERT( id < MAX_NATIONS );

	BOOST_ASSERT( nationCounter >= 0 );
	BOOST_ASSERT( nationCounter <= MAX_NATIONS );

	BOOST_ASSERT( name.size() > 0 );
	BOOST_ASSERT( name.size() <= MAX_NAME );

	/// @todo Build validation for all members
	return true;  // All tests pass
}


////////////////////////////                       ////////////////////////////
////////////////////////////  Nations Definitions  ////////////////////////////
////////////////////////////                       ////////////////////////////

Nations::Nations(token) {
	
	nations[0].rename( "Pogo" );
	nations[0].setStatus( Nation::Status::DEITY );
	
	/// @todo BUG:  MAX_NATIONS is not printing out	
	LOG_DEBUG << MAX_NATIONS << " nations constructed.";
}


Nation& Nations::operator[](const Nation_ID index ) {
	if( index < 0 || index >= MAX_NATIONS ) {
		throw out_of_range( "index" );
	}

	return nations[index];
}


/// @todo Create an appropriate function for Boost's "void assertion_failed"
bool Nations::validate() {
	for( Nation_ID i = 0 ; i < MAX_NATIONS ; i++ ) {
		BOOST_ASSERT( i == nations[i].getID() );
		nations[i].validate();
	}

	///@todo Build more validation

	return true;  // All tests pass
}

}  // namespace empire
