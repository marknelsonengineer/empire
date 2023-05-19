///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// The Core services of the `Empire V` server.
///
/// @file      main_empire_server.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "version.hpp"

using namespace empire;

/// A sample Hello World program
///
/// @return Zero
int main() {
   std::cout << "Empire version " << FULL_VERSION << std::endl;
   std::cout << std::endl;
   std::cout << LEGAL_NOTICE;
   return 0;
}
