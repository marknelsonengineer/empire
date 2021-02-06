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

/// Test the enabled Commodity constructor
BOOST_AUTO_TEST_CASE( Commodity_enabled_constructor ) {
    Commodity testCommodity( 100 );

    BOOST_CHECK_NO_THROW( testCommodity.validate() );
    BOOST_CHECK( testCommodity.isEnabled() );
    BOOST_CHECK( testCommodity.getMaxValue() == 100 );
    ///@todo test getValue()

}


BOOST_AUTO_TEST_CASE( Commodity_disabled_constructor ) {
    Commodity testCommodity( false );

    BOOST_CHECK_NO_THROW( testCommodity.validate() );
    BOOST_CHECK( testCommodity.isEnabled() == false );
    BOOST_CHECK( testCommodity.getMaxValue() == 0 );
    ///@todo test getValue()
}


BOOST_AUTO_TEST_CASE( Commodity_bad_constructor ) {
    BOOST_CHECK_THROW( new Commodity( -1 ), assertionException );
}

/// @todo build out the other tests


BOOST_AUTO_TEST_SUITE_END()
