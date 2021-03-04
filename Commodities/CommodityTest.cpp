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
   Commodity testCommodity( CIV, 100 );

   BOOST_CHECK_NO_THROW( testCommodity.validate() );
   BOOST_CHECK( testCommodity.isEnabled() );
   BOOST_CHECK( testCommodity.getMaxValue() == 100 );

   BOOST_CHECK( testCommodity.getValue() == 0 );
   testCommodity += 100;
   BOOST_CHECK( testCommodity.getValue() == 100 );
   BOOST_CHECK_NO_THROW( testCommodity.validate() );

   BOOST_CHECK( testCommodity.getName1()              == 'c' );
   BOOST_CHECK( testCommodity.getName3()              == "civ" );
   BOOST_CHECK( testCommodity.getName8()              == "Civilian" );
   BOOST_CHECK( testCommodity.getPower()              == 50 );
   BOOST_CHECK( testCommodity.getIsSellable()         == false );
   BOOST_CHECK( testCommodity.getPrice()              == 4 );
   BOOST_CHECK( testCommodity.getWeight()             == 1 );
   BOOST_CHECK( testCommodity.getPackingInefficient() == 1 );
   BOOST_CHECK( testCommodity.getPackingNormal()      == 10 );
   BOOST_CHECK( testCommodity.getPackingWarehouse()   == 10 );
   BOOST_CHECK( testCommodity.getPackingUrban()       == 10 );
   BOOST_CHECK( testCommodity.getPackingBank()        == 10 );
   BOOST_CHECK( testCommodity.getName32()             == "Civilians" );
}


/// Test a disabled constructor
BOOST_AUTO_TEST_CASE( Commodity_disabled_constructor ) {
   Commodity testCommodity( MIL, false );

   BOOST_CHECK_NO_THROW( testCommodity.validate() );
   BOOST_CHECK( testCommodity.isEnabled() == false );
   BOOST_CHECK( testCommodity.getMaxValue() == 0 );
   BOOST_CHECK_THROW( testCommodity.getValue(), commodityDisabledException );
}


/// Bounds test all possible constructors
BOOST_AUTO_TEST_CASE( Commodity_constructors ) {
   BOOST_CHECK_THROW(    new Commodity( SHELL, -1 )                        , assertionException );
   BOOST_CHECK_NO_THROW( new Commodity( GUN, 0                          ) );
   // ...
   BOOST_CHECK_NO_THROW( new Commodity( PETROL, MAX_COMMODITY_VALUE     ) );
   BOOST_CHECK_THROW(    new Commodity( IRON_ORE, MAX_COMMODITY_VALUE + 1 ), assertionException );
}


/// Exercise the += operator overloads
BOOST_AUTO_TEST_CASE( Commodity_overflow ) {
   // Test basic += overload operator
   Commodity testCommodity( GOLD_DUST, 100 );

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
      const commodityValue* oldValue = boost::get_error_info<errinfo_oldValue>( e );
      BOOST_CHECK( *oldValue == 100 );

      const commodityValue* requestedValue = boost::get_error_info<errinfo_requestedValue>( e );
      BOOST_CHECK( *requestedValue == 101 );

      const commodityValue* maxValue = boost::get_error_info<errinfo_maxValue>( e );
      BOOST_CHECK( *maxValue == 100 );
      
      char const* commodityName1 = boost::get_error_info<errinfo_commodityType>( e );
      BOOST_CHECK( *commodityName1 == 'd' );
   }

   BOOST_CHECK( testCommodity.getValue() == 100 );
   BOOST_CHECK_NO_THROW( testCommodity.validate() );


   /// Bounds check the += increaseBy operator
   Commodity testCommodity2 ( GOLD_BAR, MAX_COMMODITY_VALUE );

   BOOST_CHECK_THROW( testCommodity2 += -1, assertionException );
   testCommodity2 += 0;
   BOOST_CHECK( testCommodity2.getValue() == 0 );
   testCommodity2 += MAX_COMMODITY_VALUE;
   BOOST_CHECK_THROW( testCommodity2 += (MAX_COMMODITY_VALUE + 1), assertionException );
   BOOST_CHECK( testCommodity2.getValue() == MAX_COMMODITY_VALUE );
   BOOST_CHECK_NO_THROW( testCommodity2.validate() );


   /// Test using += on a disabled Commodity
   Commodity testCommodity3 ( FOOD, false );
   BOOST_CHECK_THROW( testCommodity3 += 1, commodityDisabledException );

   BOOST_CHECK_NO_THROW( testCommodity3.validate() );
}


/// Exercise the -= operator overloads
BOOST_AUTO_TEST_CASE( Commodity_underflow ) {
   // Test basic -= overload operator
   Commodity testCommodity( OIL, 100 );

   BOOST_CHECK( testCommodity.getValue() == 0 );

   // Test the underflow exception
   try {
      testCommodity -= 1;
      BOOST_CHECK_MESSAGE( false, "The line above should have thrown an exception" );
   }
   catch( boost::exception & e ) {
      const commodityValue* oldValue = boost::get_error_info<errinfo_oldValue>( e );
      BOOST_CHECK( *oldValue == 0 );

      const commodityValue* requestedValue = boost::get_error_info<errinfo_requestedValue>( e );
      BOOST_CHECK( *requestedValue == -1 );
      
		const char* commodityName1 = boost::get_error_info<errinfo_commodityType>( e );
      BOOST_CHECK( *commodityName1 == 'o' );
   }

   BOOST_CHECK( testCommodity.getValue() == 0 );
   BOOST_CHECK_NO_THROW( testCommodity.validate() );


   /// Bounds check the += increaseBy operator
   Commodity testCommodity2 ( LCM, MAX_COMMODITY_VALUE );

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
   Commodity testCommodity3 ( HCM, false );
   BOOST_CHECK_THROW( testCommodity3 -= 1, commodityDisabledException );
   BOOST_CHECK_NO_THROW( testCommodity3.validate() );
}


/// Exercise the CommodityTypes validate function
BOOST_AUTO_TEST_CASE( CommodityType_Basics ) {
	CommodityTypes::validate();
}


/// Exercise the CommodityTypes validate function
BOOST_AUTO_TEST_CASE( CommodityType_Values ) {
   Commodity civ ( CIV      ,1000 );
   Commodity mil ( MIL      ,1000 );
   Commodity shl ( SHELL    ,1000 );
   Commodity gun ( GUN      ,1000 );
   Commodity pet ( PETROL   ,1000 );
   Commodity ore ( IRON_ORE ,1000 );
   Commodity gld ( GOLD_DUST,1000 );
   Commodity bar ( GOLD_BAR ,1000 );
   Commodity eat ( FOOD     ,1000 );
   Commodity oil ( OIL      ,1000 );
   Commodity lcm ( LCM      ,1000 );
   Commodity hcm ( HCM      ,1000 );
   Commodity ucw ( UCW      ,1000 );
   Commodity rad ( RAD      ,1000 );

	// The first...
   BOOST_CHECK( civ.getName1()              == 'c' );
   BOOST_CHECK( civ.getName3()              == "civ" );
   BOOST_CHECK( civ.getName8()              == "Civilian" );
   BOOST_CHECK( civ.getPower()              == 50 );
   BOOST_CHECK( civ.getIsSellable()         == false );
   BOOST_CHECK( civ.getPrice()              == 4 );
   BOOST_CHECK( civ.getWeight()             == 1 );
   BOOST_CHECK( civ.getPackingInefficient() == 1 );
   BOOST_CHECK( civ.getPackingNormal()      == 10 );
   BOOST_CHECK( civ.getPackingWarehouse()   == 10 );
   BOOST_CHECK( civ.getPackingUrban()       == 10 );
   BOOST_CHECK( civ.getPackingBank()        == 10 );
   BOOST_CHECK( civ.getName32()             == "Civilians" );

	// Spot check the middle
   BOOST_CHECK( mil.getName1()              == 'm' );
   BOOST_CHECK( shl.getName3()              == "shl" );
   BOOST_CHECK( gun.getName8()              == "Guns" );
   BOOST_CHECK( pet.getPower()              == 7 );
   BOOST_CHECK( ore.getIsSellable()         == true );
   BOOST_CHECK( gld.getPrice()              == 100 );
   BOOST_CHECK( bar.getWeight()             == 50 );
   BOOST_CHECK( eat.getPackingInefficient() == 1 );
   BOOST_CHECK( oil.getPackingNormal()      == 1 );
   BOOST_CHECK( lcm.getPackingWarehouse()   == 10 );
   BOOST_CHECK( hcm.getPackingUrban()       == 1 );
   BOOST_CHECK( ucw.getPackingBank()        == 1 );
   BOOST_CHECK( mil.getName32()             == "Military" );

	// The last...
   BOOST_CHECK( rad.getName1()              == 'r' );
   BOOST_CHECK( rad.getName3()              == "rad" );
   BOOST_CHECK( rad.getName8()              == "RAD" );
   BOOST_CHECK( rad.getPower()              == 50 );
   BOOST_CHECK( rad.getIsSellable()         == true );
   BOOST_CHECK( rad.getPrice()              == 1000 );
   BOOST_CHECK( rad.getWeight()             == 8 );
   BOOST_CHECK( rad.getPackingInefficient() == 1 );
   BOOST_CHECK( rad.getPackingNormal()      == 1 );
   BOOST_CHECK( rad.getPackingWarehouse()   == 10 );
   BOOST_CHECK( rad.getPackingUrban()       == 1 );
   BOOST_CHECK( rad.getPackingBank()        == 1 );
   BOOST_CHECK( rad.getName32()             == "Radioactive material" );
}

BOOST_AUTO_TEST_SUITE_END()
