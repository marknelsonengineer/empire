///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// The Core services of the `Empire V` server.
///
/// @file      main.cpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "version.hpp"

/// A sample Hello World program
///
/// @return Zero
   int main() {
      std::cout << "Empire version " << FULL_VERSION << std::endl;
      return 0;
   }
