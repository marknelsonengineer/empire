///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Test class for Singleton.cpp
///
/// @file      lib/SingletonTest.cpp
/// @version   1.0
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      16 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

/// The name of this test module is Empire_Server
#define BOOST_TEST_MODULE Empire_Server

#include <boost/assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

#include "Singleton.hpp"


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
class TestSingleton2 final : public Singleton<TestSingleton2> {
public:
	TestSingleton2(token) ;
	~TestSingleton2();
	
	void use() const;
		
}; // class TestSingleton2

TestSingleton2::TestSingleton2(token) {
	// cout << "TestSingleton1 constructed" << endl;
}


TestSingleton2::~TestSingleton2() {
	// cout << "TestSingleton1 destroyed" << endl;
}

	
void TestSingleton2::use() const {
	// cout << "TestSingleton2 in use" << endl;
}





/// @internal  Name the test suite after the directory that it's in.  Also,
/// the name should not conflict with other objects in the test suite.
BOOST_AUTO_TEST_SUITE( Singleton_test_suite )

/// Test an enabled Commodity constructor
BOOST_AUTO_TEST_CASE( Singleton_general ) {
	auto const& t1 = TestSingleton1::instance();
	t1.use();

	auto const& t2 = TestSingleton2::instance();
	t2.use();	
}

BOOST_AUTO_TEST_SUITE_END()
