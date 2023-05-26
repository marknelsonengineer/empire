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

// #include <memory>  // For unique_ptr make_unique()

#include "../typedefs.h"  // For singleton_counter_t

namespace empire {

/// Template for a Singleton class
///
/// @pattern Singleton:  This is a base class for Singleton
template< typename T >
class Singleton {
public:
   Singleton( Singleton& ) = delete;  ///< Disable copy constructor
   Singleton( const Singleton& ) = delete;  ///< Disable copy constructor
   Singleton( const Singleton&& ) = delete;  ///< Disable move constructor

   Singleton& operator=( const Singleton ) = delete;  ///< Disable copy assignment
   Singleton& operator=( const Singleton& ) = delete;  ///< Disable copy assignment
   Singleton& operator=( const Singleton&& ) = delete;  ///< Disable move assignment

   /// Destructor for Singleton
   virtual ~Singleton() {
      std::cout << "Destructor for " << info() << std::endl;

      s_pStaticInstance = nullptr;
      uuid = boost::uuids::nil_generator()();
      destructCounter += 1;
      validate();
   };

   static T& get();

   static void erase();

   /// Return a string about this object
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

   /// @return Number of times the Singleton has been constructed
   [[nodiscard]] static singleton_counter_t getConstructedCount() {
      return constructCounter;
   }

   /// @return Number of times the Singleton has been destroyed
   [[nodiscard]] static singleton_counter_t getDestroyedCount() {
      return destructCounter;
   }

   /// Validate the health of the Singleton
   static void validate() {
      /// @throws logic_error if the info() string is not constructed correctly
      if( info().find( "UUID" ) == std::string::npos ) {
         throw std::logic_error( "The Singleton's info string was not constructed correctly");
      }

      if( s_pStaticInstance == nullptr ) {
         if( uuid != boost::uuids::nil_generator()() ) {
            /// @throws logic_error if an empty Singleton has a populated #uuid
            throw std::logic_error( "UUID should be nil for an empty Singleton");
         }
         if( constructCounter != destructCounter ) {
            /// @throws range_error if an empty Singleton's #constructCounter and #destructCounter are not equal
            throw std::range_error( "The constructCounter should equal the destructCounter in an empty Singleton" );
         }
      } else {
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
      }
   }

   /// @return A Universally Unique IDentifier or UUID for the Singleton
   [[nodiscard]] boost::uuids::uuid getUUID() const {
      return uuid;
   }

protected:
   static T* s_pStaticInstance;  ///< Pointer to the Singleton instance

   /// Construct an empty Singleton
   ///
   /// The Singleton is initially empty and is only instantiated when
   /// a client calls get().  This way, we don't pay for what we don't use.
   Singleton() {
      std::cout << "Constructor for " << info() << std::endl;
      validate();
   }


   /// This is the Singleton enforcement mechanism.  Inherited, concrete
   /// Singleton classes constructors should use `token` to access the base
   /// class without having to be a friend class.
   struct token {
   };

private:
   static boost::uuids::uuid uuid;               ///< UUID for this Singleton
   static singleton_counter_t constructCounter;  ///< Number of times this Singleton has been constructed
   static singleton_counter_t destructCounter;   ///< Number of times this Singleton has been destroyed

}; // Singleton


template< typename T >
T* Singleton< T >::s_pStaticInstance = nullptr;

template< typename T >
boost::uuids::uuid Singleton< T >::uuid = boost::uuids::nil_generator()();

template< typename T >
singleton_counter_t Singleton< T >::constructCounter = 0;

template< typename T >
singleton_counter_t Singleton< T >::destructCounter = 0;


/// Get an instance of a Singleton
///
/// @tparam T The Singleton class
/// @return The one and only instance of `T`
template< typename T >
inline T& Singleton< T >::get() {

   if( s_pStaticInstance == nullptr ) {
      /// @todo Implement smart pointers
      s_pStaticInstance = new T( token { } );
      uuid = boost::uuids::random_generator()();
      constructCounter += 1;
      std::cout << "Instantiate " << info() << std::endl;
   }

   return *s_pStaticInstance;
}


/// Delete the Singleton instance
///
/// Singleton guarantees that only one instance of the object can exist at a
/// time, but it does not guarantee that it can't be deleted and re-created.
/// The ability to delete and recreate a Singleton is beneficial for unit
/// testing and is one of the (few) things that makes having a Singleton
/// palatable.
///
/// @tparam T The Singleton class
template< typename T >
void Singleton< T >::erase() {
   std::cout << "Erase " << info() << std::endl;

   if( s_pStaticInstance == nullptr ) {
      return;
   }

   // Fires ~Singleton which resets all the member variables
   delete s_pStaticInstance;
}

}  // namespace empire
