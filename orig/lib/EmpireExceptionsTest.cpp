///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Test class for EmpireExceptions.cpp
///
/// @file      lib/EmpireExceptionsTest.cpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      11 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

/// The name of this test module is Empire_Server
#define BOOST_TEST_MODULE Empire_Server

#include <boost/assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "EmpireExceptions.hpp"


using namespace empire;


/// @internal  Name the test suite after the directory that it's in.  Also,
/// the name should not conflict with other objects in the test suite.
BOOST_AUTO_TEST_SUITE( EmpireExceptions_test_suite )

/// Test an enabled Commodity constructor
BOOST_AUTO_TEST_CASE( EmpireExceptions_general ) {
   BOOST_CHECK_THROW( boost::assertion_failed( "test", __FUNCTION__, __FILE__, __LINE__ ), assertionException);
}

BOOST_AUTO_TEST_SUITE_END()
