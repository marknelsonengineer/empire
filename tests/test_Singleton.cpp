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


/// Create a basic Singleton class
class TestSingleton1 final : public Singleton< TestSingleton1 > {
public:
   explicit TestSingleton1( [[maybe_unused]] token singletonToken ) {
      // cout << "TestSingleton1 constructed" << endl;
   }

   // Do something useful with this Singleton
   void use() const {
      // cout << "TestSingleton1 in use" << endl;
   }
};


/// Create another Singleton with a destructor.
class TestSingleton2 final : public Singleton< TestSingleton2 > {
public:
   explicit TestSingleton2( [[maybe_unused]] token singletonToken ) {
      // cout << "TestSingleton2 constructed" << endl;
   }

protected:
   ~TestSingleton2() override {
      cout << "Overridden Destructor " << TestSingleton2::info() << endl;
   }

public:
   void use() const {
      // cout << "TestSingleton2 " << to_string ( TestSingleton2::get().getUUID() ) << " in use" << endl;
   }
};


BOOST_AUTO_TEST_SUITE( Singleton )

BOOST_AUTO_TEST_CASE( Singleton_general ) {

   // Populate t11 w/ TestSingleton1
   const TestSingleton1& t11 = TestSingleton1::get();
   BOOST_CHECK_NO_THROW( TestSingleton1::validate() );
   BOOST_CHECK_NO_THROW( t11.use() );
   BOOST_CHECK( !TestSingleton1::info().empty() );
   BOOST_CHECK_NE( TestSingleton1::info().find( "UUID=" ), std::string::npos );
   BOOST_CHECK_NE( TestSingleton1::info().find( "constructed" ), std::string::npos );
   BOOST_CHECK_NE( TestSingleton1::info().find( "destroyed" ), std::string::npos );

   const boost::uuids::uuid uuid_t11 = t11.getUUID();
   BOOST_CHECK_NE( uuid_t11, boost::uuids::nil_generator()() );

   // Populate t12 with the same singleton
   const TestSingleton1& t12 = TestSingleton1::get();
   BOOST_CHECK_NO_THROW( t12.validate() );
   BOOST_CHECK_NO_THROW( t12.use() );

   const boost::uuids::uuid uuid_t12 = t12.getUUID();

   // Ensure they are the same object
   BOOST_CHECK_EQUAL( &t11, &t12 );
   BOOST_CHECK_EQUAL( uuid_t11, uuid_t12 );


   // Test our second Singleton
   const TestSingleton2& t21 = TestSingleton2::get();
   BOOST_CHECK_NO_THROW( t21.validate() );
   BOOST_CHECK_NO_THROW( t21.use() );

   const boost::uuids::uuid uuid_t21 = t21.getUUID();
   BOOST_CHECK_NE( uuid_t21, boost::uuids::nil_generator()() );

   const TestSingleton2& t22 = TestSingleton2::get();
   BOOST_CHECK_NO_THROW( t22.validate() );
   BOOST_CHECK_NO_THROW( t22.use() );

   const boost::uuids::uuid uuid_t22 = t22.getUUID();

   // Ensure they are the same object
   BOOST_CHECK_EQUAL( &t21, &t22 );
   BOOST_CHECK_EQUAL( uuid_t21, uuid_t22 );

   // Ensure the two Singletons are different objects
   BOOST_CHECK_NE( (void*) &t11, (void*) &t21 );
   BOOST_CHECK_NE( uuid_t11, uuid_t21 );
}  // Singleton_general


BOOST_AUTO_TEST_CASE( Singleton_erase ) {
   BOOST_CHECK_NO_THROW( TestSingleton1::get().validate() );
   const boost::uuids::uuid uuid_t11 = TestSingleton1::get().getUUID();

   TestSingleton1::erase();

   // Get TestSingleton1 after erasing it (it should create a new one)
   BOOST_CHECK_NO_THROW( TestSingleton1::get().validate() );
   const boost::uuids::uuid uuid_t12 = TestSingleton1::get().getUUID();

   BOOST_CHECK_NE( uuid_t11, uuid_t12 );
}


BOOST_AUTO_TEST_CASE( Singleton_bulk_get ) {
   const TestSingleton1& t11 = TestSingleton1::get();
   BOOST_CHECK_NO_THROW( t11.validate() );
   BOOST_CHECK_NO_THROW( t11.use() );
   const boost::uuids::uuid uuid_t11 = t11.getUUID();
   const singleton_counter_t t11_constructed = TestSingleton1::getConstructedCount();
   const singleton_counter_t t11_destructed = TestSingleton1::getDestroyedCount();
   BOOST_CHECK_EQUAL( t11_constructed, t11_destructed + 1 );

   for( int i = 0 ; i < 16 ; i++ ) {
      const TestSingleton1& t12 = TestSingleton1::get();
      BOOST_CHECK_NO_THROW( t12.validate() );
      BOOST_CHECK_NO_THROW( t12.use() );
      const boost::uuids::uuid uuid_t12 = t12.getUUID();

      // Ensure they are the same object
      BOOST_CHECK_EQUAL( &t11, &t12 );
      BOOST_CHECK_EQUAL( uuid_t11, uuid_t12 );
      BOOST_CHECK_EQUAL( t11_constructed, TestSingleton1::getConstructedCount() );
      BOOST_CHECK_EQUAL( t11_destructed, TestSingleton1::getDestroyedCount() );
   }
}

BOOST_AUTO_TEST_CASE( Singleton_bulk_get_and_erase ) {
   for( int i = 0 ; i < 16 ; i++ ) {
      const TestSingleton1& t21 = TestSingleton1::get();
      BOOST_CHECK_NO_THROW( t21.validate() );
      BOOST_CHECK_NO_THROW( t21.use() );
      const boost::uuids::uuid uuid_t21 = t21.getUUID();
      const singleton_counter_t t21_constructed = TestSingleton1::getConstructedCount();
      const singleton_counter_t t21_destructed = TestSingleton1::getDestroyedCount();

      TestSingleton1::erase();

      const TestSingleton1& t22 = TestSingleton1::get();
      BOOST_CHECK_NO_THROW( t22.validate() );
      BOOST_CHECK_NO_THROW( t22.use() );
      const boost::uuids::uuid uuid_t22 = t22.getUUID();
      const singleton_counter_t t22_constructed = TestSingleton1::getConstructedCount();
      const singleton_counter_t t22_destructed = TestSingleton1::getDestroyedCount();


      // Ensure they are different objects
      BOOST_CHECK_NE( uuid_t21, uuid_t22 );
      BOOST_CHECK_EQUAL( t21_constructed, t22_constructed - 1 );
      BOOST_CHECK_EQUAL( t21_destructed, t22_destructed - 1 );
      BOOST_CHECK_EQUAL( t21_constructed, t21_destructed + 1 );
   }
}


BOOST_AUTO_TEST_CASE( Singleton_bulk_erase ) {
   TestSingleton1::erase();
   const singleton_counter_t t11_constructed = TestSingleton1::getConstructedCount();
   const singleton_counter_t t11_destructed = TestSingleton1::getDestroyedCount();

   for( int i = 0 ; i < 16 ; i++ ) {
      TestSingleton1::erase();
      BOOST_CHECK_NO_THROW( TestSingleton1::validate() );
      BOOST_CHECK_EQUAL( t11_constructed, TestSingleton1::getConstructedCount() );
      BOOST_CHECK_EQUAL( t11_destructed, TestSingleton1::getDestroyedCount() );
   }
}


/// Create a Singleton class where the constructor takes parameters.
///
/// Introducing Singletons with parameters is tricky.  You don't really want
/// to pass them in with every get() call.  That's wasteful as you really only
/// need it when you instantiate the underlying object.
///
/// If you make a constructor with parameters, it will never get called unless
/// you override get(), which is a terrible idea.
class TestSingleton3 final : public empire::Singleton< TestSingleton3 > {
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
