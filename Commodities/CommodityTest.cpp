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

#include "../Commodities/Commodity.hpp"

using namespace empire;

BOOST_AUTO_TEST_SUITE( Commodities )

/// @internal Assume that the validate() function will verify the state of
///           the object's membrers.  These unit tests should focus on
///           exercising the methods, getters/setters and constructors.

/// Test the enabled Commodity constructor
BOOST_AUTO_TEST_CASE( Commodity_enabled_constructor ) {
    Commodity testCommodity( 100 );

    BOOST_CHECK_NO_THROW( testCommodity.validate() );
    BOOST_CHECK( testCommodity.isEnabled() );
    BOOST_CHECK( testCommodity.getMaxValue() == 100 );
}


BOOST_AUTO_TEST_CASE( Commodity_disabled_constructor ) {
    Commodity testCommodity( false );

    BOOST_CHECK_NO_THROW( testCommodity.validate() );
    BOOST_CHECK( testCommodity.isEnabled() == false );
    BOOST_CHECK( testCommodity.getMaxValue() == 0 );
}


BOOST_AUTO_TEST_CASE( Commodity_bad_constructor ) {
    BOOST_CHECK_THROW( new Commodity( -1 ), boost::execution_exception );
}


BOOST_AUTO_TEST_SUITE_END()
