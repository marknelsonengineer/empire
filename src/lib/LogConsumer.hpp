///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// Abstract class for log consumers
///
/// @file      LogConsumer.hpp
/// @author    Mark Nelson <marknels@hawaii.edu>
/// @copyright (c) 2023 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <thread>

#include "LogConsumer.hpp"

namespace empire {

///
/// @pattern Consumer-Producer
/// The descendents of this class are the consumers for this pattern.
class [[maybe_unused]] LogConsumer : public std::thread {
public:
   LogConsumer(LogConsumer &src)                    = delete; // Copy constructor
   LogConsumer(const LogConsumer &src)              = delete; // Const copy constructor
   LogConsumer &operator=(LogConsumer &src)         = delete; // Copy assignment
   LogConsumer &operator=(const LogConsumer &src)   = delete; // Const copy assignment
   LogConsumer (LogConsumer&& src)                  = delete; // Move constructor
   LogConsumer (const LogConsumer&& src)            = delete; // Const move constructor
   LogConsumer& operator= (LogConsumer&& src)       = delete; // Move assignment operator
   LogConsumer& operator= (const LogConsumer&& src) = delete; // Const move assignment operator

private:
   /// The ID of the thread or 0 if the thread is not running
   [[maybe_unused]] alignas( size_t ) std::thread::id thread_id { 0 };

   /// A flag to indicate if the thread should continue running.  Set to
   /// `false` if you want the thread to stop
   [[maybe_unused]] alignas( size_t ) bool continueRunning { true };

   /// The thread-specific tail pointer into empire::LogQueue
   [[maybe_unused]] alignas( size_t ) size_t consumerIndex { 0 };

public:
   /// Default destructor
   virtual ~LogConsumer() = default;

   /// Restart (or start) this LogConsumer thread
   [[maybe_unused]] virtual void restart();



};

} // namespace empire
