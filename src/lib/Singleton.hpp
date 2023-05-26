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

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <memory>

#include "../typedefs.h"  // For singleton_counter_t

namespace empire {

/// Template for a Singleton class
///
/// @pattern Singleton:  This is a base class for Singleton
template< typename T >
class Singleton {
public:
   Singleton( Singleton& ) = delete;  // Copy constructor
   Singleton( const Singleton& ) = delete;  // Copy constructor
   Singleton( const Singleton&& ) = delete;  // Move constructor

   Singleton& operator=( const Singleton ) = delete;  // Copy assignment (One of these is wrong)
   Singleton& operator=( const Singleton& ) = delete;  // Copy assignment (One of these is wrong)
   Singleton& operator=( const Singleton&& ) = delete;  // Move assignment

   virtual ~Singleton() {
      destructCounter += 1;
      uuid = boost::uuids::nil_generator()();
      /// @todo: Actually destroy the instance
   };

   static T& get();

   static void erase();

   /// @return A Universally Unique IDentifier or UUID for the Singleton
   [[nodiscard]] boost::uuids::uuid getUUID() const {
      return uuid;
   }

   /// @return Number of times the Singleton has been constructed
   [[nodiscard]] singleton_counter_t getConstructedCount() const {
      return constructCounter;
   }

   /// @return Number of times the Singleton has been destroyed
   [[nodiscard]] singleton_counter_t getDestroyedCount() const {
      return destructCounter;
   }

   /// Validate the health of the Singleton
   ///
   ///
   void validate() const {
      /// @throws range_error if the #constructCounter is <= 0
      if( constructCounter <= 0 ) {
         throw std::range_error( "constructCounter is <= 0" );
      }

      /// @throws range_error if the #destructCounter is anything other than #constructCounter - 1
      if( destructCounter != constructCounter - 1 ) {
         throw std::range_error( "destructCounter is not one less than constructCounter" );
      }

      /// @throws logic_error if the #uuid is not set
      if( uuid == boost::uuids::nil_generator()() ) {
         throw std::logic_error( "Singleton UUID is not set and it should be");
      }
   }

protected:
   static T* s_pStaticInstance;  ///< Pointer to the Singleton instance

   /// Construct a Singleton (on first use or after erase())
   Singleton() {
      uuid = boost::uuids::random_generator()();
      constructCounter += 1;

      // std::cout << "Singleton " << to_string( uuid ) << " constructed" << std::endl;
   }


   /// Inherited, concrete singleton classes will use `token` to call the base
   /// class without having to be a friend class.
   struct token {
   };

private:
   static boost::uuids::uuid uuid;               ///< UUID for this Singleton
   singleton_counter_t constructCounter  { 0 };  ///< Number of times this Singleton has been constructed
   singleton_counter_t destructCounter { 0 };    ///< Number of times this Singleton has been destroyed

}; // Singleton


template< typename T >
T* Singleton< T >::s_pStaticInstance = nullptr;

template< typename T >
boost::uuids::uuid Singleton< T >::uuid = boost::uuids::nil_generator()();


/// Get an instance of a Singleton
///
/// @tparam T The Singleton class
/// @return The one and only instance of `T`
template< typename T >
inline T& Singleton< T >::get() {

   if( s_pStaticInstance == nullptr ) {
      s_pStaticInstance = new T( token { } );
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
   if( s_pStaticInstance == nullptr ) {
      return;
   }

   delete s_pStaticInstance;
   s_pStaticInstance = nullptr;
   // std::cout << "Singleton " << to_string( uuid ) << " erased" << std::endl;
   uuid = boost::uuids::nil_generator()();
}

}  // namespace empire
