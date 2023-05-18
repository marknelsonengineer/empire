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

namespace empire {

/// Template for a Singleton class
///
/// @pattern Singleton:  This is a base class for Singleton
template< typename T >
class Singleton {
public:
   Singleton( const Singleton& ) = delete;  // Copy constructor
   Singleton( const Singleton&& ) = delete;  // Move constructor

   Singleton& operator=( const Singleton ) = delete;  // Copy assignment (One of these is wrong)
   Singleton& operator=( const Singleton& ) = delete;  // Copy assignment (One of these is wrong)
   Singleton& operator=( const Singleton&& ) = delete;  // Move assignment

   static T& get();

   virtual ~Singleton() = default;

protected:
   Singleton() = default;  ///< The empty constructor may be overridden

   /// Inherited, concrete singleton classes will use `token` to call the base
   /// class without having to be a friend class.
   struct token {
   };

}; // Singleton


/// Get an instance of a Singleton
///
/// @tparam T The Singleton class
/// @return The one and only instance of `T`
template< typename T >
inline T& Singleton< T >::get() {
   static T instance { token { } };
   return instance;
}

}  // namespace empire
