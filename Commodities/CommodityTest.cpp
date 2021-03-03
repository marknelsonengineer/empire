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
#include "CommodityType.hpp"   // This is just to be able to test CommodityType


using namespace empire;


/// @internal  Name the test suite after the directory that it's in.  Also,
/// the name should not conflict with other objects in the test suite.
BOOST_AUTO_TEST_SUITE( Commodities )


/// @internal Assume that the validate() function will verify the state of
///           the object's members.  These unit tests should focus on
///           exercising the methods, getters/setters and constructors.

/// Test an enabled Commodity constructor
BOOST_AUTO_TEST_CASE( Commodity_enabled_constructor ) {
   Commodity testCommodity( CIV, 100 );

   BOOST_CHECK_NO_THROW( testCommodity.validate() );
   BOOST_CHECK( testCommodity.isEnabled() );
   BOOST_CHECK( testCommodity.getMaxValue() == 100 );
   
   BOOST_CHECK( testCommodity.getValue() == 0 );
   testCommodity += 100;
   BOOST_CHECK( testCommodity.getValue() == 100 );
   BOOST_CHECK_NO_THROW( testCommodity.validate() );
   
   BOOST_CHECK( testCommodity.getName1() == 'c' );
   BOOST_CHECK( testCommodity.getName3() == "civ" );
   BOOST_CHECK( testCommodity.getName8() == "Civilian" );
   BOOST_CHECK( testCommodity.getPower() == 50 );
   BOOST_CHECK( testCommodity.getIsSellable() == false );
   BOOST_CHECK( testCommodity.getPrice() == 4 );
   BOOST_CHECK( testCommodity.getWeight() == 1 );
   BOOST_CHECK( testCommodity.getPackingInefficient() == 1 );
   BOOST_CHECK( testCommodity.getPackingNormal() == 10 );
   BOOST_CHECK( testCommodity.getPackingWarehouse() == 10 );
   BOOST_CHECK( testCommodity.getPackingUrban() == 10 );
   BOOST_CHECK( testCommodity.getPackingBank() == 10 );
   BOOST_CHECK( testCommodity.getName32() == "Civilians" );
}


/// Test a disabled constructor
BOOST_AUTO_TEST_CASE( Commodity_disabled_constructor ) {
   Commodity testCommodity( CIV, false );

   BOOST_CHECK_NO_THROW( testCommodity.validate() );
   BOOST_CHECK( testCommodity.isEnabled() == false );
   BOOST_CHECK( testCommodity.getMaxValue() == 0 );
   ///@todo test getValue()
}


/// Bounds test all possible constructors
BOOST_AUTO_TEST_CASE( Commodity_constructors ) {
   BOOST_CHECK_THROW(    new Commodity( CIV, -1 )                     , assertionException );
   BOOST_CHECK_NO_THROW( new Commodity( CIV, 0                       ) );
   // ...
   BOOST_CHECK_NO_THROW( new Commodity( CIV, MAX_COMMODITY_VALUE     ) );
   BOOST_CHECK_THROW(    new Commodity( CIV, MAX_COMMODITY_VALUE + 1 ), assertionException );
}


/// Exercise the += operator overloads
BOOST_AUTO_TEST_CASE( Commodity_overflow ) {
   // Test basic += overload operator
   Commodity testCommodity( CIV, 100 );

   BOOST_CHECK( testCommodity.getValue() == 0 );
   testCommodity += 100;
   BOOST_CHECK( testCommodity.getValue() == 100 );
   testCommodity -= 100;
   BOOST_CHECK( testCommodity.getValue() == 0 );
   testCommodity += 100;
   BOOST_CHECK( testCommodity.getValue() == 100 );
   
   // Test the overflow exception
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
   
   
   /// Bounds check the += increaseBy operator
   Commodity testCommodity2 ( CIV, MAX_COMMODITY_VALUE );

   BOOST_CHECK_THROW( testCommodity2 += -1, assertionException );
   testCommodity2 += 0;
   BOOST_CHECK( testCommodity2.getValue() == 0 );
   testCommodity2 += MAX_COMMODITY_VALUE;
   BOOST_CHECK_THROW( testCommodity2 += (MAX_COMMODITY_VALUE + 1), assertionException );
   BOOST_CHECK( testCommodity2.getValue() == MAX_COMMODITY_VALUE );
   BOOST_CHECK_NO_THROW( testCommodity2.validate() );


   /// Test using += on a disabled Commodity
   Commodity testCommodity3 ( CIV, false );
   BOOST_CHECK_THROW( testCommodity3 += 1, commodityDisabledException );
   
   BOOST_CHECK_NO_THROW( testCommodity3.validate() );
}


/// Exercise the -= operator overloads
BOOST_AUTO_TEST_CASE( Commodity_underflow ) {
   // Test basic -= overload operator
   Commodity testCommodity( CIV, 100 );

   BOOST_CHECK( testCommodity.getValue() == 0 );
   
   // Test the underflow exception
   try {
      testCommodity -= 1;
      BOOST_CHECK_MESSAGE( false, "The line above should have thrown an exception" );
   }
   catch( boost::exception & e ) {
      commodityValue const* oldValue = boost::get_error_info<errinfo_oldValue>( e );
      BOOST_CHECK( *oldValue == 0 );
      
      commodityValue const* requestedValue = boost::get_error_info<errinfo_requestedValue>( e );
      BOOST_CHECK( *requestedValue == -1 );

      /// @todo Check commodity name when we get that wired in...
   }

   BOOST_CHECK( testCommodity.getValue() == 0 );
   BOOST_CHECK_NO_THROW( testCommodity.validate() );
   
   
   /// Bounds check the += increaseBy operator
   Commodity testCommodity2 ( CIV, MAX_COMMODITY_VALUE );

   BOOST_CHECK_THROW( testCommodity2 += -1, assertionException );
   testCommodity2 -= 0;
   BOOST_CHECK( testCommodity2.getValue() == 0 );
   testCommodity2 += MAX_COMMODITY_VALUE;
   BOOST_CHECK( testCommodity2.getValue() == MAX_COMMODITY_VALUE );
   testCommodity2 -= MAX_COMMODITY_VALUE;
   BOOST_CHECK( testCommodity2.getValue() == 0 );
   testCommodity2 += MAX_COMMODITY_VALUE;
   BOOST_CHECK( testCommodity2.getValue() == MAX_COMMODITY_VALUE );
   BOOST_CHECK_THROW( testCommodity2 -= (MAX_COMMODITY_VALUE + 1), assertionException );
   // The above is an ** illegal ** -= because it's > than MAX_COMMODITY_VALUE, 
   // so the Commodity should be unchanged.
   BOOST_CHECK( testCommodity2.getValue() == MAX_COMMODITY_VALUE );
   BOOST_CHECK_NO_THROW( testCommodity2.validate() );

   /// Test using -= on a disabled Commodity
   Commodity testCommodity3 ( CIV, false );
   BOOST_CHECK_THROW( testCommodity3 -= 1, commodityDisabledException );
   BOOST_CHECK_NO_THROW( testCommodity3.validate() );
}


/// Exercise the CommodityTypes validate function
BOOST_AUTO_TEST_CASE( CommodityType_Basics ) {
	CommodityTypes::validate();
}

/// @todo build out the other tests


BOOST_AUTO_TEST_SUITE_END()
