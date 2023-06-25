///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// A command-line client for Empire V
///
/// @file      main_empire_client.cpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2023 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <iostream>  // For cout & endl

#include "version.hpp"

using namespace empire;

/// A sample Hello World program
///
/// @return Zero
int main() {
   std::cout << PROGRAM_TITLE << " version " << FULL_VERSION << std::endl;
   std::cout << std::endl;
   std::cout << LEGAL_NOTICE;

   return 0;
}
