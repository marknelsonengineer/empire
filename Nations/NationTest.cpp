///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Test class for Nation.cpp
///
/// @file      NationTest.cpp
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

//#include "EmpireExceptions.hpp"
#include "Nation.hpp"


using namespace empire;


/// @internal  Name the test suite after the directory that it's in.  Also,
/// the name should not conflict with other objects in the test suite.
BOOST_AUTO_TEST_SUITE( Nations_test_suite )

/// Test an enabled Commodity constructor
BOOST_AUTO_TEST_CASE( Nations_getter ) {
	Nations::validate();
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
      BOOST_CHECK( *currentValue == 25 );
   }
}



BOOST_AUTO_TEST_SUITE_END()
