///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Concrete class for all nations.
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      Nations/Nation.cpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      7 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include <boost/assert.hpp>
#include <boost/algorithm/string/trim_all.hpp>

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

	name = to_string( id );
	// rename( fixupName( to_string( id )));  // DO NOT USE THIS
	/// Here's the lifecycle for nation.name:
	///   1. All of the `Nation` objects are created in the constructor of `Nations`
	///      but `Nations` is not fully built yet, hence `nameMap` isn't there.
	///   2. Just set Name in the constructor for Nation
	///   3. In `Nations` constructor, call `refreshNameMap()`

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
	Nations& nations = Nations::get();

	string trimmedNewName = fixupName( newName );

	if( trimmedNewName.length() <= 0 ) {
		throw invalid_argument( "A nation must have a name" );
	}

	if( trimmedNewName.length() > Nation::MAX_NAME ) {
		throw length_error( "Requested name is greater than the maximum of " + to_string( Nation::MAX_NAME ) + " characters" );
	}

	// At this point, trimmedNewName is the new, candidate name

	// Iterate over nameMap and see if there are any duplicates (other than our own)
	for( const auto &mapEntry : nations.nameMap ) {
		if( mapEntry.first == trimmedNewName ) {
			if( mapEntry.second == id ) { // We are about to rename a nation to the same name... whatever
				continue;
			} else {
				throw nationNameTakenException() << errinfo_NationID( mapEntry.second )
				                                 << errinfo_NationName( mapEntry.first );
			}
		}
	}

	// Ideally, these three operations would be done as an atomic operation
	nations.nameMap.erase( name );
	name = trimmedNewName;
	nations.nameMap.emplace( name, id );

	/// @todo Implement logging
	/// @todo Implement nameMap validation

	/// @todo News:  Country [ID] changed their name from oldName to newName
	// BOOST_LOG_TRIVIAL(info) << "Nation [" << id << "] renamed from [" << "XXX" << "] to [" << trimmedNewName << "]" ;
}


const bool Nation::validate() const {
	BOOST_ASSERT( id >= 0 );
	BOOST_ASSERT( id < MAX_NATIONS );

	BOOST_ASSERT( nationCounter >= 0 );
	BOOST_ASSERT( nationCounter <= MAX_NATIONS );

	BOOST_ASSERT( name.size() > 0 );
	BOOST_ASSERT( name.size() <= MAX_NAME );

	BOOST_ASSERT( fixupName( name ) == name );

	// No duplicate names (put in Nations)

	/// @todo Build validation for all members
	return true;  // All tests pass
}


void Nation::dump() const {
	LOG_TRACE << to_string( id ) << " of " << to_string( nationCounter ) << ": \"" << name << "\"";
}


////////////////////////////                       ////////////////////////////
////////////////////////////  Nations Definitions  ////////////////////////////
////////////////////////////                       ////////////////////////////

Nations::Nations(token) {
	nations[0].name = "Pogo" ;

	nations[0].setStatus( Nation::Status::DEITY );
	/// @todo When the time comes, we need to find a way to set Pogo's credentials
	///       uniquely for each instance of the game.  No default creds.

	refreshNameMap();

	LOG_DEBUG << to_string( MAX_NATIONS ) << " nations constructed.";
}


Nation& Nations::operator[](const Nation_ID index ) {
	if( index < 0 || index >= MAX_NATIONS ) {
		throw out_of_range( "index" );
	}

	return nations[index];
}


Nation& Nations::operator[](const std::string_view name ) {
	string fixedName = Nation::fixupName( name );
	BOOST_ASSERT( fixedName.length() > 0 );
	BOOST_ASSERT( fixedName.length() <= Nation::MAX_NAME );

	if( fixedName != name ) {
		throw invalid_argument( "name" );
	}

	Nation_ID id = nameMap.at( name );

	return nations[id];
}


bool Nations::contains( const std::string_view name ) const {
	BOOST_ASSERT( name.length() > 0 );
	BOOST_ASSERT( name.length() <= Nation::MAX_NAME );

	return( nameMap.contains( name ));
}


void Nations::refreshNameMap() {
	nameMap.clear();

	for( Nation_ID i = 0 ; i < MAX_NATIONS ; i++ ) {
		auto name = nations[i].getName();
		BOOST_ASSERT( name.length() > 0 );
		BOOST_ASSERT( name == Nation::fixupName( name ));

		// If this fails, then there's a duplicate name in nations
		BOOST_ASSERT( !nameMap.contains( name ));

		nameMap.insert( {name, i} );
	}

	validate();

	LOG_TRACE << __PRETTY_FUNCTION__ << " completed successfully";
}


/// @todo Create an appropriate function for Boost's "void assertion_failed"
bool Nations::validate() const {
	// Use a tradational for loop so we can compare getID and i
	for( Nation_ID i = 0 ; i < MAX_NATIONS ; i++ ) {
		BOOST_ASSERT( i == nations[i].getID() );
		nations[i].validate();
	}

	///@todo Build more validation

	return true;  // All tests pass
}


/// Dump the current state of all of the Nations to the console/log.
void Nations::dump() const {
	LOG_TRACE << "Nations =====================";
	LOG_TRACE << "MAX_NATIONS = [" << to_string( MAX_NATIONS ) << "]";
	LOG_TRACE << "Nation::MAX_NAME = [" << to_string( Nation::MAX_NAME ) << "]";

	for( const auto& nation : nations ) {
		nation.dump();
	}
}

}  // namespace empire
