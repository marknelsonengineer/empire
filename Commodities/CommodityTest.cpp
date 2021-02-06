///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Test class for Commodity.cpp
///
/// @file      CommodityTest.cpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      29 Jan 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////


/// The name of this test module is Empire_Server
#define BOOST_TEST_MODULE Empire_Server

#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "EmpireExceptions.hpp"
#include "Commodity.hpp"

using namespace empire;


/// @internal  Name the test suite after the directory that it's in.  Also,
/// the name should not conflict with other objects in the test suite.
BOOST_AUTO_TEST_SUITE( Commodities )


/// @internal Assume that the validate() function will verify the state of
///           the object's members.  These unit tests should focus on
///           exercising the methods, getters/setters and constructors.

/// Test an enabled Commodity constructor
BOOST_AUTO_TEST_CASE( Commodity_enabled_constructor ) {
   Commodity testCommodity( 100 );

   BOOST_CHECK_NO_THROW( testCommodity.validate() );
   BOOST_CHECK( testCommodity.isEnabled() );
   BOOST_CHECK( testCommodity.getMaxValue() == 100 );
   
   BOOST_CHECK( testCommodity.getValue() == 0 );
   testCommodity += 100;
   BOOST_CHECK( testCommodity.getValue() == 100 );
}


/// Test a disabled constructor
BOOST_AUTO_TEST_CASE( Commodity_disabled_constructor ) {
   Commodity testCommodity( false );

   BOOST_CHECK_NO_THROW( testCommodity.validate() );
   BOOST_CHECK( testCommodity.isEnabled() == false );
   BOOST_CHECK( testCommodity.getMaxValue() == 0 );
   ///@todo test getValue()
}


/// Bounds test all possible constructors
BOOST_AUTO_TEST_CASE( Commodity_constructors ) {
   BOOST_CHECK_THROW(    new Commodity( -1 )                     , assertionException );
   BOOST_CHECK_NO_THROW( new Commodity( 0                       ) );
   // ...
   BOOST_CHECK_NO_THROW( new Commodity( MAX_COMMODITY_VALUE     ) );
   BOOST_CHECK_THROW(    new Commodity( MAX_COMMODITY_VALUE + 1 ), assertionException );
}


/// Exercise the operator overloads
BOOST_AUTO_TEST_CASE( Commodity_overloads ) {
   Commodity testCommodity( 100 );

   BOOST_CHECK( testCommodity.getValue() == 0 );
   testCommodity += 100;
   BOOST_CHECK( testCommodity.getValue() == 100 );
   try {
      testCommodity += 1;
      BOOST_CHECK_MESSAGE( false, "The line above should have thrown an exception" );
   }
   catch( boost::exception & e ) {
      commodityValue const* oldValue = boost::get_error_info<errinfo_oldValue>( e );
      BOOST_CHECK( *oldValue == 100 );
      
      commodityValue const* requestedValue = boost::get_error_info<errinfo_requestedValue>( e );
      BOOST_CHECK( *requestedValue == 101 );

      commodityValue const* maxValue = boost::get_error_info<errinfo_maxValue>( e );
      BOOST_CHECK( *maxValue == 100 );
      
      /// @todo Check commodity name when we get that wired in...
   }
   
   BOOST_CHECK( testCommodity.getValue() == 100 );
   BOOST_CHECK_NO_THROW( testCommodity.validate() );
}

/// @todo build out the other tests


BOOST_AUTO_TEST_SUITE_END()
