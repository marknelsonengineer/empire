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

#include <x86gprintrin.h>

#include <cmath>
#include <cstring>

#include "version.hpp"

using namespace empire;
using namespace std;

/// A datetype that represents the number of bits.
using log_2_t = unsigned char;

/// The number of bits in the buffer we are testing.
constinit const log_2_t MAX_BUF_SIZE_LOG_2 { 10 };

/// The maximum size of the buffer we are testing.  Should be a power of 2
constinit const size_t MAX_BUF_SIZE { 1U << MAX_BUF_SIZE_LOG_2 };

/// Base class for methods under test
class BaseRunner {
private:
   uint64_t m_overhead { 0 };    ///< An optional overhead time that can be deducted from the results

protected:
   uint64_t m_start_time { 0 };  ///< The start time of the test
   uint64_t m_end_time { 0 };    ///< The end time of the test

   void setOverhead( uint64_t overhead ) { m_overhead = overhead; }
   [[nodiscard]] uint64_t getOverhead() const { return m_overhead; }
   [[nodiscard]] uint64_t getDuration() const { return m_end_time - m_start_time - m_overhead; }

public:
   virtual ~BaseRunner() = default;
   virtual uint64_t doRun() = 0;
};


/// Support a run with a source & destination buffer and a guard buffer
class BufferRunner : public BaseRunner {
private:
   size_t m_buffer_size;
   size_t m_guard_size;

protected:
   void* m_src { nullptr };
   void* m_guard { nullptr };
   void* m_dest { nullptr };

public:
   BufferRunner( size_t buffer_size, size_t guard_size )
   : m_buffer_size( buffer_size )
   , m_guard_size( guard_size ) {

      m_src = malloc( m_buffer_size );
      m_guard = malloc( m_guard_size );
      m_dest = malloc( m_buffer_size );

      // Efficiently fill the source & dest buffers with random data
      unsigned char step_size = sizeof( long long unsigned int );
      size_t remaining_buffer = buffer_size;
      while( remaining_buffer >= step_size ) {
         size_t current_position = buffer_size - remaining_buffer;
         _rdrand64_step( (long long unsigned int*)( (char*)m_src + current_position ) );
         _rdrand64_step( (long long unsigned int*)( (char*)m_dest + current_position ) );
         remaining_buffer -= step_size;
      }
      while( remaining_buffer > 0 ) {
         remaining_buffer--;
         long long unsigned int random_int;
         _rdrand64_step( &random_int );
         *((char*)(m_src) + remaining_buffer) = (char)random_int;
      }
   };


};


/// Supports a run where the dataset is a power of 2
class LogRunner : public BufferRunner {
private:
   log_2_t m_buffer_size_Log_2 { 0 };
   log_2_t m_guard_size_Log_2 { 0 };

public:
   LogRunner( log_2_t buffer_size_log_2, log_2_t guard_size_log_2 )
         : BufferRunner( 1U << buffer_size_log_2, 1U << guard_size_log_2 )
           , m_buffer_size_Log_2( buffer_size_log_2 )
           , m_guard_size_Log_2( guard_size_log_2 )
   {

   };
};

char src[MAX_BUF_SIZE];
char guard[MAX_BUF_SIZE * 10];
char dest[MAX_BUF_SIZE];

void fillBuffers() {
   for( size_t i = 0 ; i < sizeof( src ) ; i += 8 ) {
      _rdrand64_step( (long long unsigned int*)(&src[ i ]) );
   }

   for( size_t i = 0 ; i < sizeof( dest ) ; i += 8 ) {
      _rdrand64_step( (long long unsigned int*)(&dest[ i ]) );
   }

   for( size_t i = 0 ; i < sizeof( guard ) ; i += 8 ) {
      _rdrand64_step( (long long unsigned int*)(&guard[ i ]) );
   }

}


/// This test develops a baseline that represents the overhead of conducting the tests.
class Test0_Nothing : public BaseRunner {
public:
   uint64_t doRun() override {
      m_start_time = __rdtsc();

      m_end_time = __rdtsc();

      return getDuration();
   }
};


uint64_t test0_nothing() {
   fillBuffers();

   uint64_t start_time { __rdtsc() };

   uint64_t end_time { __rdtsc() };

   return end_time - start_time;
}

uint64_t test1_memcpy( size_t size ) {
   fillBuffers();

   uint64_t start_time { __rdtsc() };

   std::memcpy( dest, src, size );

   uint64_t end_time { __rdtsc() };

   return end_time - start_time;
}


uint64_t test2_memset( size_t size ) {
   fillBuffers();

   uint64_t start_time { __rdtsc() };

   std::memset( dest, 0, size );

   uint64_t end_time { __rdtsc() };

   return end_time - start_time;
}


uint64_t test3_xxxx( size_t size ) {
   fillBuffers();

   uint64_t start_time { __rdtsc() };

   asm( ".intel_syntax noprefix;"
        "xor rax, rax;"
        "mov rdi, 0;"
        ".att_syntax prefix;"
      :  // Output
      :  "r"(dest) // Input
      :  "rax" // Clobbered
   );

   uint64_t end_time { __rdtsc() };

   return end_time - start_time;
}


/// A sample Hello World program
///
/// @return Zero
int main() {
   uint64_t result_times[128][4][11];

   for( size_t j = 3 ; j <= 10 ; j++ ) {
      size_t size = 1 << j ;
      cout << j << " " << size << endl;

      for( size_t i = 0 ; i < 128 ; i++ ) {
         result_times[ i ][ 0 ][0] = test0_nothing();
         result_times[ i ][ 1 ][j] = test1_memcpy( size );
         result_times[ i ][ 2 ][j] = test2_memset( size );
         result_times[ i ][ 3 ][j] = test3_xxxx( size );
      }
   }


//   cout << "Nothing" << endl;
//   for( size_t i = 0 ; i < 128 ; i++ ) {
//      cout << result_times[ i ][ 0 ][0];
//      cout << endl;
//   }

   cout << "test1_memcpy" << endl;
   for( size_t j = 3 ; j <=10 ; j++ ) {
      double sum { 0 };
      uint64_t minVal { UINT64_MAX };
      uint64_t maxVal { 0 };
      for( size_t i = 0 ; i < 128 ; i++ ) {
         sum += result_times[ i ][ 1 ][ j ];
         minVal = min( minVal, result_times[ i ][ 1 ][ j ] );
         maxVal = max( maxVal, result_times[ i ][ 1 ][ j ] );
      }
      double mean { sum / 128.0 };
      double stdev { 0 };

      for( size_t i = 0 ; i < 128 ; i++ ) {
         stdev += pow( result_times[ i ][ 1 ][ j ] - mean, 2 );
      }
      stdev = sqrt( stdev / 128.0 );

      cout << "memcpy-" << ( 1 << j ) << ":" ;
      cout << mean << ":" ;
      cout << stdev << ":" ;
      cout << minVal << ":" ;
      cout << maxVal << ":" ;
      cout << endl;
   }


   cout << "test3_xxxx" << endl;
   for( size_t j = 3 ; j <=10 ; j++ ) {
      double sum { 0 };
      uint64_t minVal { UINT64_MAX };
      uint64_t maxVal { 0 };
      for( size_t i = 0 ; i < 128 ; i++ ) {
         sum += result_times[ i ][ 3 ][ j ];
         minVal = min( minVal, result_times[ i ][ 3 ][ j ] );
         maxVal = max( maxVal, result_times[ i ][ 3 ][ j ] );
      }
      double mean { sum / 128.0 };
      double stdev { 0 };

      for( size_t i = 0 ; i < 128 ; i++ ) {
         stdev += pow( result_times[ i ][ 3 ][ j ] - mean, 2 );
      }
      stdev = sqrt( stdev / 128.0 );

      cout << "test3-" << ( 1 << j ) << ":" ;
      cout << mean << ":" ;
      cout << stdev << ":" ;
      cout << minVal << ":" ;
      cout << maxVal << ":" ;
      cout << endl;
   }


//   for( size_t i = 0 ; i < 128 ; i++ ) {
//      cout << "test1_memcpy( " << size << " ) = " << result_times[i][1] << "  ";
//   }

//      cout << "test2_memset( " << size << " ) = " << result_times[i][2] << "  ";
//   }

   std::cout << PROGRAM_TITLE << " version " << FULL_VERSION << std::endl;
   return 0;
}
