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

#include <memory>


namespace empire {

/// Create a template of a Singleton class
/// @todo Need to improve this documentation
template<typename T>
class Singleton {
public:
	static T& instance();
	
	Singleton( const Singleton&) = delete;
	Singleton& operator= (const Singleton) = delete;
	
protected:
	struct token {};
	Singleton() {}
}; // template class Singleton


/// Use the template to create an actual Singleton class
/// @todo Need to improve this documentation
template<typename T>
T& Singleton<T>::instance() {
	// static const std::unique_ptr<T> instance{ new T{token{}}};
	static T instance{ token{} };
	return instance;
}  // template T


}  // namespace empire
