#if ($HEADER_COMMENTS)
///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Comprehensive test of lorem ipsum
///
/// @file      ${FILE_NAME}
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) ${YEAR} Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#end
/// @cond Suppress Doxygen warnings

#[[#include]]#  <boost/test/unit_test.hpp>

#[[#include]]#  "../src/something.hpp"

using namespace empire;
using namespace std;


BOOST_AUTO_TEST_SUITE( Thing )

/// Test something important
BOOST_AUTO_TEST_CASE( Thing_general ) {

   BOOST_CHECK_EQUAL( true, true );
}


BOOST_AUTO_TEST_SUITE_END()
/// @endcond
