///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Comprehensive test of version.cpp
///
/// @file      test_version.cpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2023 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
/// @cond Suppress Doxygen warnings

#include <boost/test/unit_test.hpp>

#include <format>

#include "../src/version.hpp"

using namespace empire;
using namespace std;

BOOST_AUTO_TEST_SUITE( Version )

/// Test the version numbers
BOOST_AUTO_TEST_CASE( Version_general ) {
   BOOST_CHECK_EQUAL( VERSION_MAJOR, 5 );

   BOOST_CHECK_EQUAL( FULL_VERSION, std::format( "{}.{}.{}+{}", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH
                                                 , VERSION_BUILD ) );
}


BOOST_AUTO_TEST_SUITE_END()
/// @endcond
