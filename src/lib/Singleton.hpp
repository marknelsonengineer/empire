///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// A generic Singleton template
///
/// @see https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template
///
/// @file      lib/Singleton.hpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>

#include <boost/core/type_name.hpp>        // For type_name
#include <boost/uuid/uuid.hpp>             // For uuid
#include <boost/uuid/uuid_generators.hpp>  // For nil_generator() random_generator()
#include <boost/uuid/uuid_io.hpp>          // For uuid::to_string()

#include <memory>                          // For unique_ptr make_unique()

#include "../typedefs.h"                   // For singleton_counter_t

namespace empire {

/// Template for a Singleton class
///
/// This is more like a Singleton helper.  Users of Singleton will inherit this
/// class and create their own Singleton like this:
///
///     /// Create a basic Singleton class
///     class TestSingleton1 final : public Singleton< TestSingleton1 > {
///     public:
///        explicit TestSingleton1( [[maybe_unused]] token singletonToken ) {
///           cout << "Constructor1  " << TestSingleton1::info() << endl;
///        }
///
///        // Do something useful with this Singleton
///        void use() const {
///           cout << "Use1 " << TestSingleton1::info() << endl;
///        }
///     };
///
/// This template not a guarantee of Singleness.  There are many ways you can
/// override this and create multiple instances.  This Singleton, on the other
/// hand, should detect if it happens and throw an exception.  Unfortunately,
/// these programs will compile and run.  We can't detect the problem until it
/// happens.
///
/// The assurances of singleness are at runtime and (I'd estimate) to be
/// relatively weak against a determined hacker.
///
/// This class is also not thread-safe.
///
/// Singletons that need configuration parameters are tricky.  You don't really
/// want to pass them in with every get() call.  That's wasteful as you really
/// only need them when you instantiate the underlying object.
///
/// You can't override get() to call a constructor with parameters.  Instead,
/// the Boost Test `Singleton_with_parameters` demonstrates a technique where you
/// set the parameters as statics before instantiating the Singleton.
///
/// @pattern Singleton:  This is a base class for Singleton
/// @tparam T This derived class will be a Singleton
template< typename T >
class Singleton {
public:  // /////////////////// Constructors & Destructors /////////////////////

   /// The mechanism to ensure objects inherit from Singleton.
   ///
   /// Inherited, concrete Singleton classes should pass `token` to their
   /// constructors.  If you can't access #Singleton.token, then you are not
   /// overriding Singleton.
   using token = struct token {};

   Singleton( Singleton& ) = delete;  ///< Disable copy constructor
   Singleton( const Singleton& ) = delete;  ///< Disable copy constructor
   Singleton( const Singleton&& ) = delete;  ///< Disable move constructor

   Singleton& operator=( const Singleton ) = delete;  ///< Disable copy assignment
   Singleton& operator=( const Singleton& ) = delete;  ///< Disable copy assignment
   Singleton& operator=( const Singleton&& ) = delete;  ///< Disable move assignment

   /// Destructor for Singleton
   virtual ~Singleton() {
      std::cout << "Destructor for " << info() << std::endl;

      uuid = boost::uuids::nil_generator()();
      destructCounter += 1;
      // validate();  // It's not safe to call validate() yet because
      // the derived class's destructor has not fired yet.
   };


public:  // ///////////////////////// Static Methods ///////////////////////////

   static T& get();  // Defined below, so it can use `inline`

   /// Get some details about this object
   ///
   ///     TestSingleton1 UUID=0982ec0e-34c2-4769-837e-abd90834e407 constructed 14 times destroyed 13 times
   ///
   /// @return An info string
   static std::string info() {
      std::string infoString {} ;

      infoString += boost::core::type_name<T>();
      infoString += " UUID=" + to_string( uuid );
      infoString += " constructed " + std::to_string( constructCounter ) + " times";
      infoString += " destroyed " + std::to_string( destructCounter ) + " times";
      return infoString;
   }

   /// Get the number of times this Singleton has been constructed
   ///
   /// @return Number of times this Singleton has been constructed
   [[nodiscard]] static singleton_counter_t getConstructedCount() {
      return constructCounter;
   }

   /// Get the number of times this Singleton has been destroyed
   ///
   /// @return Number of times this Singleton has been destroyed
   [[nodiscard]] static singleton_counter_t getDestroyedCount() {
      return destructCounter;
   }

   /// Determine if this Singleton has been instantiated or not.
   ///
   /// @return `true` if this Singleton has been instantiated.  `false` if not.
   [[nodiscard]] static bool isInstantiated() {
      return s_instance ? true : false;
   }

   /// Validate the health of this Singleton
   static void validate() {
      /// @throws logic_error if the info() string is not constructed correctly
      if( info().find( "UUID" ) == std::string::npos ) {
         throw std::logic_error( "The Singleton's info string was not constructed correctly");
      }

      if( isInstantiated() ) {
         /// @throws range_error if a populated Singleton's #constructCounter is <= 0
         if( constructCounter <= 0 ) {
            throw std::range_error( "A Singleton's constructCounter should be > 0" );
         }

         /// @throws range_error if a populated Singleton's #destructCounter is anything other than #constructCounter - 1
         if( destructCounter != constructCounter - 1 ) {
            throw std::range_error( "A Singleton's constructCounter should be one more than it's destructCounter" );
         }

         /// @throws logic_error if a populated Singleton's #uuid is not set
         if( uuid == boost::uuids::nil_generator()() ) {
            throw std::logic_error( "A Singleton's UUID should be set");
         }
      } else {
         if( uuid != boost::uuids::nil_generator()() ) {
            /// @throws logic_error if an empty Singleton has a populated #uuid
            throw std::logic_error( "UUID should be nil for an empty Singleton");
         }
         if( constructCounter != destructCounter ) {
            /// @throws range_error if an empty Singleton's #constructCounter and #destructCounter are not equal
            throw std::range_error( "The constructCounter should equal the destructCounter in an empty Singleton" );
         }
      }
   }

   /// Get the Universally Unique IDentifier or UUID for this Singleton
   ///
   /// @return A Universally Unique IDentifier or UUID for this Singleton or
   ///         `00000000-0000-0000-0000-000000000000` if it hasn't been
   ///         instantiated yet.
   [[nodiscard]] boost::uuids::uuid getUUID() const {
      return uuid;
   }


   /// Erase this Singleton instance
   ///
   /// Singleton guarantees that only one instance of the object can exist at a
   /// time, but it does not guarantee that it can't be deleted and re-created.
   /// The ability to delete and recreate a Singleton is beneficial for unit
   /// testing and is one of the (few) things that makes having a Singleton
   /// palatable.
   static void erase() {
      std::cout << "Erase " << info() << std::endl;

      if( !isInstantiated() ) {
         return;
      }

      // Fires ~Singleton which resets the member variables
      s_instance.reset(nullptr);

      validate();
   }


protected:  // /////////////// Protected Methods & Members /////////////////////

   /// Construct an empty Singleton
   ///
   /// The Singleton is initially empty and is only instantiated when
   /// a client calls get().  This way, we don't pay for what we don't use.
   ///
   /// This is protected and `<T>` must override it.
   Singleton() {
      std::cout << "Base class constructor for " << info() << std::endl;

      if( isInstantiated() ) {
         throw std::logic_error( "Attempt to create a new Singleton on top of an existing one" );
      }

      uuid = boost::uuids::random_generator()();
      constructCounter += 1;

      std::cout << "Instantiated " << info() << std::endl;
      // validate();  // It's not safe to call validate() at this point because
                      // the derived class's constructor has not run yet.
   }

private:  // //////////////////// Private Static Members ///////////////////////
   static std::unique_ptr<T> s_instance;         ///< A unique "smart pointer" to an instance of this Singleton

   static boost::uuids::uuid uuid;               ///< Universally Unique IDentifier for this Singleton
   static singleton_counter_t constructCounter;  ///< Number of times this Singleton has been constructed
   static singleton_counter_t destructCounter;   ///< Number of times this Singleton has been destroyed

}; // Singleton


template< typename T >
std::unique_ptr< T > Singleton< T >::s_instance;

template< typename T >
boost::uuids::uuid Singleton< T >::uuid = boost::uuids::nil_generator()();

template< typename T >
singleton_counter_t Singleton< T >::constructCounter = 0;

template< typename T >
singleton_counter_t Singleton< T >::destructCounter = 0;


/// Get an instance of this Singleton
///
/// Uses `inline` to maximize performance
///
/// @tparam T The Singleton class
/// @return The one and only instance of `T`
template< typename T >
inline T& Singleton< T >::get() {

   if( !isInstantiated() ) {
      s_instance = std::make_unique<T>( token {} ) ;
   }

   validate();

   return *s_instance;
}

}  // namespace empire
