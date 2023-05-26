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

#include <memory>

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

   static T& get();

   static void erase();

   virtual ~Singleton() = default;

   static T* s_pStaticInstance;  ///< Holds a pointer to the Singleton class

protected:
   Singleton() = default;  ///< The empty constructor may be overridden


   /// Inherited, concrete singleton classes will use `token` to call the base
   /// class without having to be a friend class.
   struct token {
   };

}; // Singleton


template< typename T >
T* Singleton< T >::s_pStaticInstance = nullptr;


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
   if( !s_pStaticInstance ) {
      return;
   }

   delete s_pStaticInstance;
   s_pStaticInstance = nullptr;
}

}  // namespace empire
