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

#include <iostream>  // REMOVE

#include "Core.hpp"

using namespace std;

namespace empire {

Core::Core(token) {
	cout << "Core constructed" << endl;
}


Core::~Core() {
	cout << "Core destroyed" << endl;
}

	
void Core::use() const {
	cout << "Core in use" << endl;
}


}  // namespace empire
