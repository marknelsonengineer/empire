///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// The Core services of the Empire ][ server.
///
//  The documentation for classes in this file are in the .hpp file.
/// data that varies between instances of a commodity.
///
/// @file      Server/Core.hpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      16 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../lib/EmpireExceptions.hpp"
#include "../lib/Singleton.hpp"

namespace empire {
	
/// This is the core services class for Empire.  It's intended to hold a number
/// of Singletons and to correctly initialize the game.  For example, it should
/// setup the logger, create Nations, create Sectors, etc.  
///
/// In a Model-View-Controller architecture, this would be a Controller class
/// for Empire.
///
/// @pattern Singleton:  There can be only one Core...
///
class Core final : public Singleton<Core> {
public:
	Core(token) ;
	~Core();
	
	void use() const;
		
}; // class Core

}  // namespace empire
