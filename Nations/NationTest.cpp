///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Test class for Nation.cpp
///
/// @file      Nations/NationTest.cpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      8 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

/// The name of this test module is Empire_Server
#define BOOST_TEST_MODULE Empire_Server

#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>
#include <string>
#include <string_view>

#include "Nation.hpp"


using namespace empire;


/// @internal  Name the test suite after the directory that it's in.  Also,
/// the name should not conflict with other objects in the test suite.
BOOST_AUTO_TEST_SUITE( Nations_test_suite )


/// Test the Nation::fixupName() function
BOOST_AUTO_TEST_CASE( Nation_fixupName ) {
	BOOST_CHECK( Nation::fixupName( "   Sam" ) == "Sam" );
	BOOST_CHECK( Nation::fixupName( "Sam   " ) == "Sam" );
	BOOST_CHECK( Nation::fixupName( "  Sam  " ) == "Sam" );
	BOOST_CHECK( Nation::fixupName( "Sam  I  am" ) == "Sam I am" );
	BOOST_CHECK( Nation::fixupName( "Sam\t\tI\t\tam" ) == "Sam I am" );
	BOOST_CHECK( Nation::fixupName( "  1  2  3  4  " ) == "1 2 3 4" );
}



/// Basic test of a simple Nations getter
BOOST_AUTO_TEST_CASE( Nations_getter ) {
	Nations& nations = Nations::get();
	nations.validate();

	Nation& nation = nations[0];

	nation.validate();
	BOOST_CHECK( nation.getID() == 0 );
	BOOST_CHECK( nation.getName() == "Pogo" );
	BOOST_CHECK( nation.getStatus() == Nation::Status::DEITY );
}



/// Test that an attempt to create another Nation (beyond MAX_NATIONS) will
/// throw an exception
BOOST_AUTO_TEST_CASE( Nations_exceeded ) {
   BOOST_CHECK_THROW( Nation(), nationLimitExceededException );

   // Test the overflow exception
   try {
		Nation illegalNation;
      BOOST_CHECK_MESSAGE( false, "The line above should have thrown an exception" );
   }
   catch( boost::exception & e ) {
      const Nation_ID* maxValue = boost::get_error_info<errinfo_maxNations>( e );
      BOOST_CHECK( *maxValue == MAX_NATIONS );

      const Nation_ID* currentValue = boost::get_error_info<errinfo_currentNationCounter>( e );
      BOOST_CHECK( *currentValue == MAX_NATIONS );
   }
}



/// Test Nations.get( id ) bounds
BOOST_AUTO_TEST_CASE( Nations_get_bounds_on_index ) {
	Nations& nations = Nations::get();

	BOOST_CHECK_THROW( nations[-1], std::out_of_range );
	BOOST_CHECK_NO_THROW( nations[0] );
	BOOST_CHECK_NO_THROW( nations[MAX_NATIONS - 1] );
	BOOST_CHECK_THROW( nations[MAX_NATIONS], std::out_of_range );
}



/// Test Nations.get( name ) and Nations.contains( name )
BOOST_AUTO_TEST_CASE( Nations_get_name ) {
	Nations& nations = Nations::get();

	Nation& nation0 = nations["Pogo"];
	BOOST_CHECK( nation0.getID() == 0 );

	Nation& nation1 = nations["1"];
	BOOST_CHECK( nation1.getID() == 1 );

	// This demonstrates another way to get a Nation by name
	BOOST_CHECK( Nations::get()["2"].getID() == 2 );

	BOOST_CHECK_THROW( nations["Not a nation"], std::out_of_range );

	BOOST_CHECK_THROW( nations[" Pogo "], std::invalid_argument );

	BOOST_CHECK( nations.contains(  "Pogo" ) );
	BOOST_CHECK( !nations.contains( " Pogo") );
	BOOST_CHECK( !nations.contains( "Pogo ") );

	BOOST_CHECK( !nations.contains( "Not a nation") );
}



/// Test basic Nations range iterator
BOOST_AUTO_TEST_CASE( Nations_basic_range_iterator ) {
	Nations& nations = Nations::get();

	int i = 0;
	for( const auto& nation : nations ) {
		nation.validate();
		i++;
	}
	BOOST_CHECK( i == MAX_NATIONS );
}


/*
BOOST_AUTO_TEST_CASE( Nation_rename ) {
	Nation nation = Nations::get(0);

	// Basic rename
	nation.rename( "Sam" );
	BOOST_CHECK( nation.getName() == "Sam" );

	// Rename to ""
	BOOST_CHECK_THROW( nation.rename(""), std::invalid_argument );

	// Rename to largest possible name
	std::string newName = std::string( Nation::MAX_NAME, 'x' );
	nation.rename( newName );
	BOOST_CHECK( nation.getName() == newName );

	newName += "X";  // Add one more characrter...
	BOOST_CHECK_THROW( nation.rename(newName), std::length_error );
	// std::cout << newName;

	// Test the trim_all functionality
	nation.rename( "  Sam   I\t\t am  " );
	BOOST_CHECK( nation.getName() == "Sam I am" );
	std::cout << "The new name is [" << nation.getName() << "]";

	// Test renaming a nation to itself
	nation.rename( "Sam" );
	BOOST_CHECK( nation.getName() == "Sam" );
	nation.rename( "Sam" );
	BOOST_CHECK( nation.getName() == "Sam" );

	// Test duplicate naming

	Nation nation5 = Nations::get(5);
	Nation nation7 = Nations::get(7);

	nation7.rename( "Popular" );
   // Test renaming a "later" Nation
   try {
		Nation nation9 = Nations::get(9);
		nation9.rename( "Popular" );
      BOOST_CHECK_MESSAGE( false, "The line above should have thrown an exception" );
   }
   catch( boost::exception & e ) {
      const Nation_ID* nationID = boost::get_error_info<errinfo_NationID>( e );
      BOOST_CHECK( *nationID == 7 );

      const std::string_view* nationName = boost::get_error_info<errinfo_NationName>( e );
      BOOST_CHECK( *nationName == "Popular" );
   }

}

*/


/// Test basic Nations dump()
BOOST_AUTO_TEST_CASE( Nations_dump ) {
	Nations& nations = Nations::get();
	nations.dump();
}



BOOST_AUTO_TEST_SUITE_END()
