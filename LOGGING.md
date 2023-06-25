Logging
=======

Empire will have a dedicated logger.

## Requirements
The requirements for the logger are:
  - Must support log levels (`TRACE` through `FATAL`)
  - Must be able to identify what module generated the log
  - Should be extensible (log to file, console, deity session, et. al.)
  - Should be fast & efficient:
    - If the log is used/available, it should be inlined
    - If the log is not used/available, it should be excluded from the compilation
  - Must be modern
    - Should use [C++20's new formatting library]
    - It should adopt the new [C++23 print functionality] when it's available
    - It should be in its own namespace
    - It should use `const` wherever possible
    - That said, for performance reasons, some things are:
      - Going to be fixed at compile time
      - Not going to adhere to a strict Object-Oriented discipline
    - Logs should be Unicode-friendly and use UTF-8 
      - However, logs will not be multilingual (no lookup tables)
  - Must be thread-safe
    - Let's steal from the Linux Kernel's logger and...
      - Log to a fixed-length, circular queue using a lockfree design
  - Must use easy and familiar idioms in the codebase
  - The queue does not need to be persistent, the handlers will manage that

Question:  Should each of the handlers run in its own thread?
Pros: They can manage wakeups on their own (for example for log rotation)
Cons: Just a bit more m_overhead
Discussion:
I'm thinking they run in their own threads.  We also start a global table of
handlers where we have clean/dirty flags.  At the end of each handler, we
look to see if any other handlers are dirty and trigger them.

### Example Header Usage
    
````
#include "Prelog.hpp" 
#define LOG_MODULE Example
#define LOG_MIN_LEVEL LOG_INFO
#include "log.hpp"
````
      
### Example Log Usage
    
````
LOG_INFO( "Unable to open file [{}]", filename ); 
````
      
Furthermore, a Log user shouldn't have to instantiate an object before using 
it.  Not everything has to be object-oriented.

## Major Design Decisions
### Should we "buy" or "build" our logger?
#### Discussion
Ideally, I'd love to "buy" a logger (find a library and wire it in).  I looked
at a few C++ loggers out there, and prototyped [Boost log].  The problem with 
[Boost log] is it couldn't be compiled out if it was not being used.  I also felt
it was too heavyweight of a tool.

I've built a lot of loggers over the years.  I never like building them, but
they are fundamental to a project and I try to learn from each iteration.  

For the sake of performance, I think Empire needs a custom logger.

### Extensibility:  Should the logger hold parameterized fields?
#### Discussion
Gawd, I'd really love to have this functionality.  I found a few C# loggers that
did this and I thought it was really cool.  Also, the C++ exception handling
mechanism as well as Microsoft Windows application report API supports this.

The downside is performance:  How do we build the queue at compile-time?  I 
think we could do a bit of subclass magic if we had to, but that's about it.

### Extensibility:  What fields should we log?
#### Discussion
The usual:  Timestamp, module, level and text.  I'm not inclined to log an ID
(a monotonic counter) unless we find a need for it.


## Overall Design Concept
Each user (source file) of the logger will set some default values (like 
`LOG_MODULE`) and then inline the logger.

The Log object is a singleton that instantiates the LogQueue and starts the
handler thread.  The handler thread gets called... _Semaphore?_ and it uses a 
Chain of Responsibility (?) to give each handler the opportunity to process
the log and update their relative position vis a vi the queue.

The Logger subscribes to the [RAII] philosophy.  Therefore, once the Log is 
instantiated, the handler thread is running.  You can restart it, but you can't
stop-and-start it.  Essentially, the handlers are always running.

Compile-time configuration:
  - The handlers
  - The size of the queue
  - The fields logged
  - The length of the fields
  - A minimum set of log methods available to each source module

Run-time configuration:
  - There should be a way to modify the log verbosity at runtime
    - I think the handlers will deal with this

Outstanding design decisions:
  - How will the log query mechanism work?
    - Well, if it's querying the queue, we can write our own API
    - However, if it's querying a handler's output, then maybe the handler has 
      its own API.

## Implementation Notes
We need to downshift from [C++20's new formatting library] to [snprintf].
This is driven mainly due to performance.  I did some benchmarking and compared
the runtimes of the following:

      std::string xString = std::format( "{} am {}\n", 1, "Sam" );
      snprintf( aString, 128, "%d am %s\n", 1, "Sam" );

| Function   | CPU Ticks | Relative speed |
|------------|-----------|----------------|
| `format`   | 208       | 45% increase   |
| `snprintf` | 143       | Baseline       |

The second note is the size of the log.  It's fixed at 128 bytes.  This is 
a compromise.  First, most packets of information should fit in 128 bytes.  If
you have more to say, then break up the log.  The other thing is related to 
performance (of course).  On modern systems, copying one byte takes about the
same amount of time as 512 bytes.  We will make sure our strings are on aligned
boundaries.

Lastly, the benchmarking really opened my eyes to the speed of `memcpy()`.  It's
very efficient.

[Boost log]:  https://www.boost.org/doc/libs/1_82_0/libs/log/doc/html/index.html
[C++20's new formatting library]: https://en.cppreference.com/w/cpp/utility/format
[C++23 print functionality]: https://en.cppreference.com/w/cpp/header/print
[RAII]: https://en.cppreference.com/w/cpp/language/raii
[snprintf]: https://en.cppreference.com/w/cpp/io/c/fprintf
