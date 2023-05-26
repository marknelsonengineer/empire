///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Comprehensive tests for Singleton
///
/// @file      tests/test_Singleton.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
/// @cond Suppress Doxygen warnings

#define BOOST_TEST_MODULE Empire_Server
#define BOOST_TEST_MAIN  // Include this to get Boost Test's main()

#include <boost/test/unit_test.hpp>

#include "../src/lib/Singleton.hpp"

using namespace std;
using namespace empire;


/// Create a basic Singleton class.
class TestSingleton1 final : public Singleton< TestSingleton1 > {
public:
   explicit TestSingleton1( [[maybe_unused]] token singletonToken ) {
      // cout << "TestSingleton1 constructed" << endl;
   }


   void use() const {
      // cout << "TestSingleton1 in use" << endl;
   }
};


/// Create a 2nd Singleton class... to verify that we can have
/// multiple derived classes of Singleton.  This instance has a bit more
/// functionality
class TestSingleton2 final : public Singleton< TestSingleton2 > {
public:
   explicit TestSingleton2( [[maybe_unused]] token singletonToken ) {
      // cout << "TestSingleton1 constructed" << endl;
   }


   void use() const {
      // cout << "TestSingleton2 in use" << endl;
   }
};


BOOST_AUTO_TEST_SUITE( Singleton )

BOOST_AUTO_TEST_CASE( Singleton_general ) {

   // Populate t11 w/ TestSingleton1
   const TestSingleton1& t11 = TestSingleton1::get();
   BOOST_CHECK_NO_THROW( t11.use() );

   const boost::uuids::uuid uuid_t11 = t11.getUUID();
   BOOST_CHECK_NE( uuid_t11, boost::uuids::nil_generator()() );

   // Populate t12 with the same singleton
   const TestSingleton1& t12 = TestSingleton1::get();
   BOOST_CHECK_NO_THROW( t12.use() );

   const boost::uuids::uuid uuid_t12 = t12.getUUID();

   // Ensure they are the same object
   BOOST_CHECK_EQUAL( &t11, &t12 );
   BOOST_CHECK_EQUAL( uuid_t11, uuid_t12 );

   // Test our second Singleton
   const TestSingleton2& t21 = TestSingleton2::get();
   BOOST_CHECK_NO_THROW( t21.use() );

   const boost::uuids::uuid uuid_t21 = t21.getUUID();
   BOOST_CHECK_NE( uuid_t21, boost::uuids::nil_generator()() );

   const TestSingleton2& t22 = TestSingleton2::get();
   BOOST_CHECK_NO_THROW( t22.use() );

   const boost::uuids::uuid uuid_t22 = t22.getUUID();

   // Ensure they are the same object
   BOOST_CHECK_EQUAL( &t21, &t22 );
   BOOST_CHECK_EQUAL( uuid_t21, uuid_t22 );

   // Ensure they are different objects
   BOOST_CHECK_NE( (void*) &t11, (void*) &t21 );
   BOOST_CHECK_NE( uuid_t11, uuid_t21 );
}  // Singleton_general


BOOST_AUTO_TEST_CASE( Singleton_erase ) {
   const boost::uuids::uuid uuid_t11 = TestSingleton2::get().getUUID();

   TestSingleton2::erase();

   // Get TestSingleton2 after erasing it (it should create a new one)
   const boost::uuids::uuid uuid_t12 = TestSingleton2::get().getUUID();

   BOOST_CHECK_NE( uuid_t11, uuid_t12 );
}

// Test calling erase a use & bunch of times


/// Create a Singleton class where the constructor takes parameters.
class TestSingleton3 final : public Singleton< TestSingleton3 > {
public:
   explicit TestSingleton3( [[maybe_unused]] token singletonToken ) {
      // cout << "TestSingleton1 constructed" << endl;
   }


   void use() const {
      // cout << "TestSingleton3 in use" << endl;
   }
};


BOOST_AUTO_TEST_SUITE_END()
/// @endcond
