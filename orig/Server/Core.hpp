///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// The Core services of the Empire ][ server.
///
//  The documentation for classes in this file are in the .hpp file.
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
#include "../../src/lib/Singleton.hpp"


namespace empire {

/// This is the core services class for Empire.  It's intended to hold a number
/// of Singletons and to correctly initialize the game.  Some of its tasks include
/// For example, it should:
///     - Setup the logger
///     - Create Nations
///     - Create Sectors
///     - Unmartial saved game state
///     - Initiate timed activities such as updates
///     - Start the network server
///
/// In a Model-View-Controller architecture, this would be a Controller class
/// for Empire.
///
/// @pattern Singleton:  There can be only one Core...
///
class Core final : public Singleton<Core> {
public:  ////////////////////  Constructors / Destructors  ////////////////////
	/// Startup the game core.  Token is private to the Singleton so others
	/// can't call it.
	///
	/// It does the following:
	///    - Initialize the logger
	Core(token) ;

	/// Shutdown the core.
	~Core() ;

	void use() const;


private:  /////////////////////////////  Members  /////////////////////////////


private:  /////////////////////////////  Methods  /////////////////////////////

}; // class Core

}  // namespace empire
