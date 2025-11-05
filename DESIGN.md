Design Notes (Unsorted)
=======================

@brief Document the development philosophy and major design decisions

<img src="images/design.png" style="width:300px; float: right; margin: 20px 10px 10px 10px;" alt="Design"/>





Rpc
Supported
Efficient
Multi platform
Secure
Authentication




Original empire had a telnet like in her face to the program. Empire to will have a proper API, and provide a ton that like interface by default, moved it exactly what I chose not to do.

Create a convert that converts one commodity to another.  This can be used to convert civs to mil or gold bars to gold dust.





Fixup validations
Singleton:  plus up its documentation & tests
Clean all documentation warnings

Get into class serialization / marshaling

Convert all char and strings to wide. Everything everything everything

Singleton
Add one more test get

Get rid of the iterator class (fixup documentation)


—GC-sections

-z noexecstack
Mark output as not requiring executable stack
-z now
Mark object for immediate function binding


Every empire object has (from empobj.h):
Type (Sector, land, ship, plane, nuke)
Sequence number
Generation ???
UID
Timestamp
Nation owner
Coord x
Coord y
Efficiency
Mobility
Tech
Group
Linked list // Implement my own container


“gcc -fsanitize=address,undefined”

Whole program -fwhole-program

Get into cppcheck

C++ Singletons & Statics

Write a C program for preparing commands from a single source.

Figure out how to get environment into each Makefile.  I’m thinking a make.env or make include.

LATER
Build to another compiler
Can i install icc, the Intel compiler?
— Think about correctly scoping my enums (in a Class) — can’t do CommodityType, though


Profile the code… find hotspots and make ‘em inline

Make my getters inline


ON HOLD
- Make a custom set of asserts. We don’t want to fail every time.  [No longer necessary with BOOST_ASSERT??]

- [ ] Install a logger utility
- [ ] Need a way to manage persistence... Marshalling or a database?
- [ ] Wide character support
- [ ] For now, I'm going to write Empire V as a narrow-character program.  However, I think there will come a time when there should be a compiler switch for it to support wide characters.




Like any project, there's a lot of things you have to get right upfront:
  - Programming language:  [C++]
  - Development environment:  [CLion]
  - Version control:  [GitHub]
  - Testing: [Boost test]
  - Logging
  - Persistence: [Boost serialization]
  - Internationalization:  [UTF-8]
  - Static analysis / linting:  [clang-tidy]
  - Documentation: [Doxygen]
  - Compilers:  [gcc], [clang] and I'd love to get into [Visual Studio] ASAP

This document will discuss the thinking behind several of these decisions.

Here's an interesting question:  What would a Microkernel-based Empire look like?




### Persistence

Empire _could_ be a traditional [3-tiered application], but I don't think a 
database is the right way to go.  Here's why:
  1. Fundamentally, the data model will be an object-model in the C++ memory
     space.  This is notoriously difficult to synchronize with a database.
  2. Which database would be the best fit?  A SQL database would be challenging
     to maintain.  A bag-of-words database like [Mongo] or [Elastic] is not a good fit.

I think Empire is best served with a C++ object model (fast and object-oriented)
hosted by a multi-threaded process (see Threading) that can be marshalled in/out
of files for persistence.

Outstanding questions:
  - [ ] What Marshalling technology to use
  - [ ] What file format will the data be in
  - [ ] How often is the in-memory object model backed up
  - [ ] How do you Marshall the object model in a concurrent environment

Question:  What are we giving up if we don't use a database?  Answer:  [ACID] properties.
  - **A**: Atomicity
    - Transactions occur in a single unit.  If we break down the transactions
      we will likely encounter in Empire, they fall into 4 big buckets:
      - Updating individual properties.  Mitigation:  We can use C++'s
        concurrency library.
      - Taking from one Unit and giving to another.  Mitigation:  Not sure, we
        may end up having some kind of Unit-level lock.  Or, we may just accept
        the consequences.
      - Calculations based on changing variables.  This is similar to the above.
      - Updating data structures.  Mitigation:  Use lockfree or blockfree 
        algorithms for all of our primary data structures.
  - **C**: Consistency
    - Ensures that a transaction will check for invariants.
      - We will make heavy use of `validate()` methods to enforce the model's 
        consistency rules.
  - **I**: Isolation
    - Determines how transaction integrity is visible to other users and systems.
      - For simplicity, I don't think we will have transaction levels.  This 
        may bite us in the future.  It's a risk.
  - **D**: Durability
    - Guarantees that a committed transaction remains committed even if the 
      system crashes.
      - Mitigation:  This is not an iron-clad, database-level guarantee, but 
        we will periodically Marshall (save) the state of the object model.

I experimented with [Boost Serialization].  The demo worked well and I was able
to see text and binary formats.  Couldn't get the XML to work (bummer).  It feels
rusty (not the language).  I don't think it's currently maintained.  Right now,
I'm disinclined to use it.

I also looked at BitSery.  It's fast and I can see why.  I think it'll tightly
couple our dataset to byte sizes.  It's currently maintained.  It feels a little
too lightweight, but maybe that's good.

There's a good list of alternative serialization libraries here:
https://github.com/fraillt/bitsery

I think one of the main properties/functions of the data model needs to be
serialization and (probably) lock management.  


## Threading

Should Empire's core be a multi-threaded application?  What requirement is 
driving it to be multi-threaded?   In many ways, we could serialize the core 
by adding a front-end queue and dispatching work from the queue.  Easy.

On the other hand, Empire is fundamentally a multi-user realtime simulation.  We
also live in an era of copious cores and I think single-threading the core is
a disservice to our stated goal of efficiency.  Third, Operating System kernels
are almost always multi-threaded.  Empire's core is very much like an OS kernel.
If they can make it work, then I think we can make it work.


## API Model & Layers

The original [Empire] has a Telnet-like interface.  Users connected with a hostname+port
and used a username+password to login.  Sessions were not encrypted.  3rd
party Empire Clients like [WinACE] and [PTkII2] had to use the same Telent 
interface as a backend and then create a whole presentation layer on top of that.

Empire V will present an API to clients.  The default client will have an SSH
terminal interface that accepts text input and converts it to the API.

Empire V will have the following layers:
  - Core:  A multi-threaded C++ program that maintains the state of the application and processes transactions.
  - Rep:  The Representative.  This layer runs on the same server as the core, but in a different process space (Probably).  Its purpose is to mediate requests between the Client API.
  - Client API:  [gRPC] over SSH using XXX for authentication.
  - Default Client:  A multi-platform shell-like program that is the default Empire client.  The Client is considered untrusted code.

I'm thinking of using [gRPC] for the client-layer API.

## The Representative:

It is not my intention to create a RESTful API -- one where we re-create state 
for every request.  Rather, the Representative layer is meant to maintain state
for a session.  For example:
  - Ensure that only 1 Representative per country is logged in at any given time
  - It knows who it is...  It has its own Nation class built
  - It knows what it owns
  - It knows what it knows (in other words what it sees around it, regardless
    of how that may change over time)

[ ] We need to think about how we push new information about the real model to the users' model.


## Configuration

when I consider using the persistence model for holding configuration, here are
my thoughts:
  - Consideration:  Use it as the master for config.  Users would modify the persisted files and
    then Empire imports it.
    - Decision:  The persistence model does not store key-value pairs.
I considered the persistence model for holding configuration.  The problem is
that the model does not store key-value pairs.  Positional information in the
model is implied by source order.  Therefore, users have no idea what they
are configuring.

I'm going to explore the persistence model before looking into Boost 
Program Options.  Maybe we can get away with using persistence for config.

Options come in many flavors:
1. Compiled into the code (most of them - for efficiency)
2. Runtime options
3. Config file
4. Command line
5. Settable via unit tests

Config flyweight??
A configuration has:
- A type: Int, float, double, bool,
- A default value
- Min and max values
- Validator
- Group & order within the group
- Long description
- A config key
- Is settable/resettable from a test
- ?? Source


This is how a controller would use the config;

    int& theConfig.getSomeValue();

### Configuration Requirements

- getSomeConfig() should **NOT** return a reference.  The reference would be mutable.
- The config model (may) be persisted and restored with the game.
  - This will allow certain values to be updated during the game.
  - More importantly
- Everything should (probably) have a value.  For those that do, the order is:
  - Default
  - Config from a file
  - Config from saved state
  - Config from a setter (for example, from a unit test)
  - Config from command line??

- The following iterators:
  - Iterate over all config to read them in from a file
  - Iterate over all config to print out the current configuration
  - Iterate over all config to write a new file
  - Iterate over all config to update a file
  - Iterate over all config and validate the values

- There should be one single class that contains all config getters.

## Object Model

?? Should we create an object for a version?  It might be used in configuration.

## Other Notes - for now

We are going to work very hard to keep the object model separate from the 
business rules.  We will call the object model the "Business Domain", or a set
of classes that represent objects in the business model being implemented.

I need to define a small set of fundamental behaviors, that can be composed
into more complex ones.

We will likely use the GoF's Builder pattern to create the world (Sectors).

I'll explore GoF's Structural Patterns to build the object model...

Then, explore their Behavioral Patterns to implement the business rules.

I need to get smarter on delegates and composites




## SOLID
The SOLID ideas are
The Single-responsibility principle: "There should never be more than one reason for a class to change."[5] In other words, every class should have only one responsibility.[6]
The Open–closed principle: "Software entities ... should be open for extension, but closed for modification."[7]
The Liskov substitution principle: "Functions that use pointers or references to base classes must be able to use objects of derived classes without knowing it."[8] See also design by contract.[8]
The Interface segregation principle: "Clients should not be forced to depend upon interfaces that they do not use."[9][4]
The Dependency inversion principle: "Depend upon abstractions, [not] concretions."[10][4]

## Latency Study
I put some effort into benchmarking how to efficiently copy strings on x86 systems.
Some ways to do it are:
MOV m64, r64  & MOV r54, m64:  1, 2, p237 p4, 2, 1 + 1, 1, p23, 2, 0.5
MOVS



## Things to do

- Here's a good question:  Can I create every sector at compile time?  Should I --
  How does that reconcile with marshalling?



[Empire]:  http://www.wolfpackempire.com
[3-tiered application]:  https://en.wikipedia.org/wiki/Multitier_architecture
[ACID]: https://en.wikipedia.org/wiki/ACID
[WinACE]: https://sourceforge.net/projects/winace/files/
[PTkII2]:  https://sourceforge.net/projects/ptkei2/
[gPRC]:  https://grpc.io
[Singletons are Pathological Liars]: https://testing.googleblog.com/2008/08/by-miko-hevery-so-you-join-new-project.html
[Where Have All the Singletons Gone?]: https://testing.googleblog.com/2008/08/where-have-all-singletons-gone.html
[Performant Singletons]: https://web.archive.org/web/20090303174418/http://scientificninja.com/advice/performant-singletons
[modern c++]: https://www.modernescpp.com
[Agile]: https://agilemanifesto.org
[XP]: http://www.extremeprogramming.org
[clang]: https://clang.llvm.org
[C++]: https://en.cppreference.com
[CLion]:  https://jetbrains.com/clion
[GitHub]:  https://github.com
[Boost test]: https://www.boost.org/doc/libs/1_82_0/libs/test/doc/html/index.html
[clang-tidy]:  https://clang.llvm.org/extra/clang-tidy/
[Doxygen]:  https://doxygen.nl
[gcc]:  https://gcc.gnu.org
[Visual Studio]:  https://visualstudio.microsoft.com
[Boost serialization]:  https://www.boost.org/doc/libs/1_82_0/libs/serialization/doc/index.html
[C]:  https://www.bell-labs.com/usr/dmr/www/chist.html
[An examination of the source]: http://git.pond.sub.org/?p=empserver
[UTF-8]: http://utf8everywhere.org
[Java]:  https://java.com
[C-Sharp]:  https://learn.microsoft.com/en-us/dotnet/csharp/
[Ghidra]:  http://Ghidra-sre.org
[IntelliJ]:  https://www.jetbrains.com/idea/
[Eclipse]:  https://www.eclipse.org/ide/
[Javadoc]:  https://en.wikipedia.org/wiki/Javadoc
[Python]:  https://www.python.org
[Pearl]:  https://www.perl.org
[PHP]:  https://www.php.net
[Go]:  https://go.dev
[Rust]:  https://www.rust-lang.org
[Swift]:  https://www.swift.org
[constinit]:  https://en.cppreference.com/w/cpp/language/constinit
[icc]:  https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html
[CMake]:  https://cmake.org
[GitHub actions]:  https://docs.github.com/en/actions
[Mongo]:  https://www.mongodb.com
[Elastic]:  https://www.elastic.co
[gRPC]:  https://grpc.io
