///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// The Core services of the Empire ][ server.
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      Server/Core.cpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      16 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#include "Core.hpp"
#include "../lib/Log.hpp"


using namespace std;

namespace empire {




Core::Core(token) {
	// This has the benefit of initializing the logger
	LOG_INFO << "Empire core services starting" ;
}


Core::~Core() {
	LOG_INFO << "Empire core services shutting down" ;
}


void Core::use() const {
	LOG_TRACE << "Core in use" ;
}


}  // namespace empire
