#define BOOST_TEST_MODULE Empire_Server
#include <boost/test/unit_test.hpp>

#include "../Commodities/Commodity.hpp"

using namespace empire;

BOOST_AUTO_TEST_SUITE( Commodities )


BOOST_AUTO_TEST_CASE( Commodity_01 ) {
    Commodity testCommodity( true, 100 );

    BOOST_REQUIRE_NO_THROW( testCommodity.validate() );
}

BOOST_AUTO_TEST_SUITE_END()
