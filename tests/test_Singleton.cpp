///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Comprehensive tests for #empire::Singleton
///
/// @file      tests/test_Singleton.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
/// @cond Suppress Doxygen warnings
/// @NOLINTBEGIN( cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers ): Magic numbers are OK in tests

#include <boost/test/unit_test.hpp>

#define LOG_CHANNEL "test_Singleton"  /// @NOLINT( cppcoreguidelines-macro-usage ): A #define is OK here
#include "../src/lib/Log.hpp"

#include "../src/lib/Singleton.hpp"

using namespace std;
using namespace empire;


/// Create a basic Singleton class
class TestSingleton1 final : public Singleton< TestSingleton1 > {
public:
   explicit TestSingleton1( [[maybe_unused]] token singletonToken ) {
      // cout << "Constructor1  " << TestSingleton1::info() << endl;
   }

   // Do something useful with this class
   void use() const {
      // cout << "Use1 " << TestSingleton1::info() << endl;
   }
};


/// Create another Singleton with a destructor.
class TestSingleton2 final : public Singleton< TestSingleton2 > {  /// @NOLINT( cppcoreguidelines-special-member-functions, hicpp-special-member-functions ): Copy and Move assignment constructors are in the template
public:  // /////////////////// Constructors & Destructors /////////////////////
   explicit TestSingleton2( [[maybe_unused]] token singletonToken ) {
      cout << "Constructor2  " << TestSingleton2::info() << endl;
   }

   ~TestSingleton2() override {
      (void) 0;  // Avoid a lint warning
      // cout << "Overridden Destructor " << TestSingleton2::info() << endl;
   }

   TestSingleton2( TestSingleton2& ) = delete;  ///< Disable copy constructor
   TestSingleton2( const TestSingleton2& ) = delete;  ///< Disable copy constructor
   TestSingleton2( const TestSingleton2&& ) = delete;  ///< Disable move constructor

public:  // ///////////////////////// Public Methods ///////////////////////////
   void use() const {
      // cout << "Use2 " << TestSingleton2::info() << endl;
   }
};


BOOST_AUTO_TEST_SUITE( Singleton )

BOOST_AUTO_TEST_CASE( Singleton_general ) {

   BOOST_CHECK( !TestSingleton1::isInstantiated() );

   // Populate t11 w/ TestSingleton1
   const TestSingleton1& t11 = TestSingleton1::get();
   BOOST_CHECK_NO_THROW( TestSingleton1::validate() );
   BOOST_CHECK( TestSingleton1::isInstantiated() );
   BOOST_CHECK_NO_THROW( t11.use() );
   BOOST_CHECK( !TestSingleton1::info().empty() );
   BOOST_CHECK_NE( TestSingleton1::info().find( "UUID=" ), std::string::npos );
   BOOST_CHECK_NE( TestSingleton1::info().find( "constructed" ), std::string::npos );
   BOOST_CHECK_NE( TestSingleton1::info().find( "destroyed" ), std::string::npos );

   const boost::uuids::uuid uuid_t11 = t11.getUUID();
   BOOST_CHECK_NE( uuid_t11, boost::uuids::nil_generator()() );

   // Populate t12 with the same singleton
   BOOST_CHECK( TestSingleton1::isInstantiated() );
   const TestSingleton1& t12 = TestSingleton1::get();
   BOOST_CHECK_NO_THROW( t12.validate() );
   BOOST_CHECK( TestSingleton1::isInstantiated() );
   BOOST_CHECK_NO_THROW( t12.use() );

   const boost::uuids::uuid uuid_t12 = t12.getUUID();

   // Ensure they are the same object
   BOOST_CHECK_EQUAL( &t11, &t12 );
   BOOST_CHECK_EQUAL( uuid_t11, uuid_t12 );


   // Test our second Singleton
   BOOST_CHECK( !TestSingleton2::isInstantiated() );

   const TestSingleton2& t21 = TestSingleton2::get();
   BOOST_CHECK_NO_THROW( t21.validate() );
   BOOST_CHECK( TestSingleton2::isInstantiated() );
   BOOST_CHECK_NO_THROW( t21.use() );

   const boost::uuids::uuid uuid_t21 = t21.getUUID();
   BOOST_CHECK_NE( uuid_t21, boost::uuids::nil_generator()() );

   // Populate t22 with the same singleton
   BOOST_CHECK( TestSingleton2::isInstantiated() );
   const TestSingleton2& t22 = TestSingleton2::get();
   BOOST_CHECK_NO_THROW( t22.validate() );
   BOOST_CHECK( TestSingleton2::isInstantiated() );
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
   BOOST_CHECK( TestSingleton1::isInstantiated() );

   TestSingleton1::erase();

   // Get TestSingleton1 after erasing it (it should create a new one)
   BOOST_CHECK( !TestSingleton1::isInstantiated() );
   BOOST_CHECK_NO_THROW( TestSingleton1::get().validate() );
   const boost::uuids::uuid uuid_t12 = TestSingleton1::get().getUUID();
   BOOST_CHECK( TestSingleton1::isInstantiated() );

   BOOST_CHECK_NE( uuid_t11, uuid_t12 );
}


BOOST_AUTO_TEST_CASE( Singleton_bulk_get ) {
   const TestSingleton1& t11 = TestSingleton1::get();
   BOOST_CHECK_NO_THROW( t11.validate() );
   BOOST_CHECK( t11.isInstantiated() );
   BOOST_CHECK_NO_THROW( t11.use() );
   const boost::uuids::uuid uuid_t11 = t11.getUUID();
   const singleton_counter_t t11_constructed = TestSingleton1::getConstructedCount();
   const singleton_counter_t t11_destructed = TestSingleton1::getDestroyedCount();
   BOOST_CHECK_EQUAL( t11_constructed, t11_destructed + 1 );

   for( int i = 0 ; i < 16 ; i++ ) {
      const TestSingleton1& t12 = TestSingleton1::get();
      BOOST_CHECK_NO_THROW( t12.validate() );
      BOOST_CHECK( t12.isInstantiated() );
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
      const TestSingleton2& t21 = TestSingleton2::get();
      BOOST_CHECK_NO_THROW( t21.validate() );
      BOOST_CHECK( t21.isInstantiated() );
      BOOST_CHECK_NO_THROW( t21.use() );
      const boost::uuids::uuid uuid_t21 = t21.getUUID();
      const singleton_counter_t t21_constructed = TestSingleton2::getConstructedCount();
      const singleton_counter_t t21_destructed = TestSingleton2::getDestroyedCount();

      TestSingleton2::erase();

      BOOST_CHECK( !TestSingleton2::isInstantiated() );

      const TestSingleton2& t22 = TestSingleton2::get();
      BOOST_CHECK_NO_THROW( t22.validate() );
      BOOST_CHECK( TestSingleton2::isInstantiated() );
      BOOST_CHECK_NO_THROW( t22.use() );
      const boost::uuids::uuid uuid_t22 = t22.getUUID();
      const singleton_counter_t t22_constructed = TestSingleton2::getConstructedCount();
      const singleton_counter_t t22_destructed = TestSingleton2::getDestroyedCount();


      // Ensure they are different objects
      BOOST_CHECK_NE( uuid_t21, uuid_t22 );
      BOOST_CHECK_EQUAL( t21_constructed, t22_constructed - 1 );
      BOOST_CHECK_EQUAL( t21_destructed, t22_destructed - 1 );
      BOOST_CHECK_EQUAL( t21_constructed, t21_destructed + 1 );
   }
}


BOOST_AUTO_TEST_CASE( Singleton_bulk_erase ) {
   TestSingleton1::erase();
   BOOST_CHECK( !TestSingleton1::isInstantiated() );

   const singleton_counter_t t11_constructed = TestSingleton1::getConstructedCount();
   const singleton_counter_t t11_destructed = TestSingleton1::getDestroyedCount();

   for( int i = 0 ; i < 16 ; i++ ) {
      TestSingleton1::erase();
      BOOST_CHECK_NO_THROW( TestSingleton1::validate() );
      BOOST_CHECK( !TestSingleton1::isInstantiated() );
      BOOST_CHECK_EQUAL( t11_constructed, TestSingleton1::getConstructedCount() );
      BOOST_CHECK_EQUAL( t11_destructed, TestSingleton1::getDestroyedCount() );
   }
}


/// Create a Singleton class where the constructor needs parameters.
///
/// Singletons that need configuration parameters are tricky.  You don't really
/// want to pass them in with every get() call.  That's wasteful as you really
/// only need them when you instantiate the underlying object.
///
/// If you make a constructor with parameters, it will never get called unless
/// you override get(), which is a terrible idea.
///
/// You can't override get() to call a constructor with parameters.  Instead,
/// the Boost Test `Singleton_with_parameters` demonstrates a technique where you
/// set the parameters as statics before instantiating the Singleton.
class TestSingleton3 final : public empire::Singleton< TestSingleton3 > {
public:
   explicit TestSingleton3( int parm1, string_view parm2 ) {
      (void) parm1;  // Avoid lint warnings
      (void) parm2;
      // cout << "Construct (param1, param2) ";
      // cout << TestSingleton3::info();
      // cout << " param1=" << sm_parameter1;
      // cout << " param2=" << sm_parameter2;
      // cout << endl;
   }

   explicit TestSingleton3( [[maybe_unused]] token singletonToken ) : TestSingleton3( sm_parameter1, sm_parameter2 ) {
      // cout << "Construct (token)" << TestSingleton3::info() << endl;
   }

   static TestSingleton3& get() {
      // std::cout << "Instantiate3 " << info() << std::endl;

      return Singleton< TestSingleton3 >::get();
   }

   void use() const {
      // cout << "Use " << TestSingleton3::info() << endl;
   }

   static void setParameter1( int parameter1 ) {
      sm_parameter1 = parameter1;
   }

   static int getParameter1() {
      return sm_parameter1;
   }

   static void setParameter2( string_view parameter2 ) {
      sm_parameter2 = parameter2;
   }

   static string getParameter2() {
      return sm_parameter2;
   }

protected:
   static int    sm_parameter1;  ///< @NOLINT( cppcoreguidelines-avoid-non-const-global-variables ): This is not really a global
   static string sm_parameter2;  ///< @NOLINT( cppcoreguidelines-avoid-non-const-global-variables ): This is not really a global
};

int    TestSingleton3::sm_parameter1 { 0 };  ///< @NOLINT( cppcoreguidelines-avoid-non-const-global-variables ): This is not really a global
string TestSingleton3::sm_parameter2 {};     ///< @NOLINT( cppcoreguidelines-avoid-non-const-global-variables ): This is not really a global

BOOST_AUTO_TEST_CASE( Singleton_with_parameters ) {
   TestSingleton3::setParameter1( 16 );
   TestSingleton3::setParameter2( "I am Sam" );

   BOOST_CHECK( !TestSingleton3::isInstantiated() );

   // Populate t31 w/ TestSingleton3
   const TestSingleton3& t31 = TestSingleton3::get();
   BOOST_CHECK_NO_THROW( TestSingleton3::validate() );
   BOOST_CHECK( TestSingleton3::isInstantiated() );
   BOOST_CHECK_NO_THROW( t31.use() );
   BOOST_CHECK( !TestSingleton3::info().empty() );
   BOOST_CHECK_NE( TestSingleton3::info().find( "UUID=" ), std::string::npos );
   BOOST_CHECK_NE( TestSingleton3::info().find( "constructed" ), std::string::npos );
   BOOST_CHECK_NE( TestSingleton3::info().find( "destroyed" ), std::string::npos );
   BOOST_CHECK_EQUAL( t31.getParameter1(), 16 );
   BOOST_CHECK_EQUAL( t31.getParameter2(), "I am Sam" );

   const boost::uuids::uuid uuid_t31 = t31.getUUID();
   BOOST_CHECK_NE( uuid_t31, boost::uuids::nil_generator()() );

   TestSingleton3::erase();
   BOOST_CHECK( !TestSingleton3::isInstantiated() );

   TestSingleton3::setParameter1( 32 );
   TestSingleton3::setParameter2( "Sam I am" );

   // Populate t32 w/ TestSingleton3
   const TestSingleton3& t32 = TestSingleton3::get();
   BOOST_CHECK_NO_THROW( TestSingleton3::validate() );
   BOOST_CHECK( TestSingleton3::isInstantiated() );
   BOOST_CHECK_NO_THROW( t32.use() );
   BOOST_CHECK_EQUAL( t31.getParameter1(), 32 );
   BOOST_CHECK_EQUAL( t31.getParameter2(), "Sam I am" );

   const boost::uuids::uuid uuid_t32 = t32.getUUID();
   BOOST_CHECK_NE( uuid_t31, boost::uuids::nil_generator()() );

   // Ensure the two Singletons are different objects
   BOOST_CHECK_NE( uuid_t31, uuid_t32 );


   // The following will compile and run, but will fail at runtime.
   // TestSingleton3 x1 = TestSingleton3( 100 );
   // TestSingleton3 x2 = TestSingleton3( 100 );

   BOOST_CHECK( true );
}

BOOST_AUTO_TEST_SUITE_END()
/// @NOLINTEND( cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers )
/// @endcond
