///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// A generic Singleton template & class.
///
//  The documentation for classes in this file are in the .hpp file.
/// data that varies between instances of a commodity.
///
/// @file      lib/Singleton.hpp
/// @version   1.0 - Initial version
///
/// @see https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      16 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#pragma once


namespace empire {

/// Create a template of a Singleton class
///
/// @pattern Singleton:  This is a base class for Singletons
///
/// @todo Need to improve this documentation
template<typename T>
class Singleton {
public:
	Singleton( const Singleton&) = delete;
	Singleton& operator= (const Singleton) = delete;

	static T& get();	
	
protected:
	struct token {};  // Inherited, concrete singleton classes will use this 
	                  // to call the base class without having to be a 
	                  // friend class.
	Singleton() {}		// The empty constructor may be overridden
}; // template class Singleton


/// Get an instance of a Singleton
///
/// @todo Need to improve this documentation
template<typename T>
inline T& Singleton<T>::get() {
	static T instance{ token{} };
	return instance;
}  // template T


}  // namespace empire
