///////////////////////////////////////////////////////////////////////////////
//  Empire ][ - What you do is what you do
//
/// Test class for Singleton.cpp
///
/// @file      tests/test_Singleton.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
/// @cond Suppress Doxygen warnings

/// The name of this test module is Empire_Server
#define BOOST_TEST_MODULE Empire_Server
#define BOOST_TEST_MAIN  // Include this to get Boost Test's main()

#include <boost/assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "../src/lib/Singleton.hpp"


using namespace empire;


class TestSingleton1 final : public Singleton<TestSingleton1> {
public:
	TestSingleton1(token) ;
	~TestSingleton1();

	void use() const;

}; // class TestSingleton1


TestSingleton1::TestSingleton1(token) {
	// cout << "TestSingleton1 constructed" << endl;
}


TestSingleton1::~TestSingleton1() {
	// cout << "TestSingleton1 destroyed" << endl;
}


void TestSingleton1::use() const {
	// cout << "TestSingleton1 in use" << endl;
}


/// Create a 2nd test class... to verify that we can have
/// multiple dervied classes of Singleton
///
/// Note:  This Singleton does not have a destructor
class TestSingleton2 final : public Singleton<TestSingleton2> {
public:
	TestSingleton2(token) ;

	void use() const;

}; // class TestSingleton2

TestSingleton2::TestSingleton2(token) {
	// cout << "TestSingleton1 constructed" << endl;
}

void TestSingleton2::use() const {
	// cout << "TestSingleton2 in use" << endl;
}



/// @internal  Name the test suite after the directory that it's in.  Also,
/// the name should not conflict with other objects in the test suite.
BOOST_AUTO_TEST_SUITE( Singleton_test_suite )

/// Test an enabled Commodity constructor
BOOST_AUTO_TEST_CASE( Singleton_general ) {

	// Populate t11 w/ TestSingleton1
	const TestSingleton1& t11 = TestSingleton1::get();
   BOOST_CHECK_NO_THROW( t11.use() );

	// Populate t12 with the same singleton
   const TestSingleton1& t12 = TestSingleton1::get();
   BOOST_CHECK_NO_THROW( t12.use() );

	// Ensure they are the same object
   BOOST_CHECK( &t11 == &t12 );

	// Test our second Singleton
	const TestSingleton2& t2 = TestSingleton2::get();
	BOOST_CHECK_NO_THROW( t2.use() );

	// Ensure they are different objecgts
   BOOST_CHECK( (void*)&t11 != (void*)&t2 );
}

BOOST_AUTO_TEST_SUITE_END()
/// @endcond
