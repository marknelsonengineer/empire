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

#include <atomic>
#include <thread>
#include <threads.h>
#include <vector>

#include "version.hpp"

using namespace std;

using namespace empire;

std::atomic_flag lock = ATOMIC_FLAG_INIT;  ///< A lock type (maybe)

mtx_t mutex1;  ///< A mutex for the worker
cnd_t cnd1;    ///< A condition variable for the worker

/// A worker function that's waiting for something to do
///
/// @param i An index to the worker thread
void worker( int i ) {
   cout << "Waiting " << i << endl;
   cnd_wait( &cnd1, &mutex1 );
   cout << "Awake " << i << endl;
}


/// A sample Hello World program
///
/// @return Zero
int main() {
   cout << PROGRAM_TITLE << " version " << FULL_VERSION << endl;
   cout << endl;
   cout << LEGAL_NOTICE;

   mtx_init( &mutex1, mtx_plain );
   cnd_init( &cnd1 );

   std::vector<std::thread> pool;
   for( int i = 0; i < 10; ++i ) {
      pool.emplace_back(worker, i);
   }

   std::this_thread::sleep_for(2000ms);
   cout << "Wake up" << endl;

   cnd_signal( &cnd1 );

   for (auto& i : pool) {
      i.join();
   }


   return 0;
}
