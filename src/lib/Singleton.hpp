///////////////////////////////////////////////////////////////////////////////
//  Empire ][ - What you do is what you do
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
///
/// @todo Need to improve this documentation
template< typename T >
class Singleton {
public:
   Singleton( const Singleton& ) = delete;

   Singleton& operator=( const Singleton ) = delete;

   static T& get();

protected:
   struct token {};  ///< Inherited, concrete singleton classes will use this
                     ///< to call the base class without having to be a
                     ///< friend class.

   Singleton() = default;  ///< The empty constructor may be overridden
}; // Singleton


/// Get an instance of a Singleton
///
/// @tparam T The Singleton class
/// @return The one and only instance of `T`
template< typename T >
inline T& Singleton< T >::get() {
   static T instance { token {} };
   return instance;
}

}  // namespace empire
