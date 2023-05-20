///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// A command-line client for Empire V
///
/// @file      main_empire_client.cpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2023 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "version.hpp"

using namespace empire;

/// A sample Hello World program
///
/// @return Zero
int main() {
   std::cout << "Empire version " << FULL_VERSION << std::endl;
   return 0;
}
