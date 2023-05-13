The Design of Empire ][
=======================

@brief Document the development philosophy and major design decisions

## Design Philosophy

1. Embrace modern Software Engineering design practices
2. Is correct & efficient
3. Is fun

To a large degree, I'm writing Empire because I want to learn more about
Object-Oriented Programming.  Specifically, I want to become a practitioner of
design patterns.  [Empire] is well understood and documented inasmuch as it's an
existing program that I intend to replicate.  To that end, my intent is to 
consider as many patterns as possible and select the best fit for the 
application.

## High Level Design

Options come in 2 flavors:
1. Compiled into the code (most of them - for efficiency)
2. Runtime options


## High Level Design Requirements
  - UNICODE / Wide-Character support
  - Uses a modern logger for debugging, telemetry, etc.
  - Uses a modern exception-handling mechanism
  - Is persistent
  - Uses modern Software Engineering practices like:
    - Continuous integration (GitHub Actions)
    - Source-level documentation (Doxygen)
    - Unit testing (Boost Tests)
    - Linting (clang-tidy)
    - Build (CMake)
  - Is secure

## Low Level Design Requirements
  - All major data structures should be aligned
  - Works on several compilers (Visual Studio, clang, gcc, icc)



We chose C++ as the programming language because:
  1.  It's efficient & modern
  2. It supports object-oriented design

## Persistence

Empire _could_ be a traditional [3-tiered application], but I don't think a 
database is the right way to go.  Here's why:
  1. Fundamentally, the data model will be an object-model in the C++ memory
     space.  This is notoriously difficult to synchronize with a database.
  2. Which database would be the best fit?  A SQL database would be challenging
     to maintain.  A bag-of-words database like Mongo or Elastic is not a good fit.

I think Empire is best served with a C++ object model (fast and object-oriented)
hosted by a multi-threaded process (see Threading) that can be marshalled in/out
of files for persistence.

Outstanding questions:
  [ ] What Marshalling technology to use
  [ ] What file format will the data be in
  [ ] How often is the in-memory object model backed up
  [ ] How do you Marshall the object model in a concurrent environment

Q:  What are we giving up if we don't use a database?  A:  [ACID] properties.
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

The original [Empire] had a Telnet-like interface.  Users connected with a hostname+port
and used a username+password to login.  Sessions were not encrypted.  3rd
party Empire Clients like [WinACE] and [PTkII2] had to use the same Telent 
interface as a backend and then create a whole presentation layer on top of that.

Empire ][ will present an API to clients.  The default client will have an SSH
terminal interface that accepts text input and converts it to the API.

Empire ][ will have the following layers:
  - Core:  A multi-threaded C++ program that maintains the state of the application and processes transactions.
  - Rep:  The Representative.  This layer runs on the same server as the core, but in a different process space (Probably).  It's purpose is to mediate requests between the Client API.
  - Client API:  [gRPC] over SSH using XXX for authentication.
  - Default Client:  A multi-platform shell-like program that can login to XXX.  The Client is considered untrusted code.

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

Classic Empire had a number of text-based configuration files that were pretty
easy to override and configure.  For the sake of efficiency and because I think
we can structure our source code to be pretty user-friendly, I'm going to make
all of those configurable elements part of the C++ source.  Specifically, they
will be built in as `inline constexpr`.

For review:
`const var` Means the variable won't change after it's initialized. Variables may be initialized at runtime, though.
`method() const` Means the method won't change the state of the object
`constexpr` Means "to be evaluated at compile time".  Applies to variables.
`consteval` Declares a function or template to produce a compile time constant expression.  It forces calles to happen at compile-time.
`constinit` Initializes a static variable at compile time.  It does not imply `const` nor `constexpr`.



## Other Notes (for now)

We are going to work very hard to keep the object model separate from the 
business rules.  We will call the object model the "Business Domain", or a set
of classes that represent objects in the business model being implemented.

I need to define a small set of fundamental behaviors, that can be composed
into more complex ones.

We will likely use the GoF's Builder pattern to create the world (Sectors).

I'll explore GoF's Structural Patterns to build the object model...

Then, explore their Behavioral Patterns to implement the business rules.

I need to get smarter on delegates and composites


## TODO
[ ] Do a UMLet drawing of the current Empire codebase
[ ] Expand on the UMLet drawing to include the design of the Business Domain / 
    data model.
[ ] Set the project icon in CLion


[Empire]:  http://www.wolfpackempire.com
[3-tiered application]:  https://en.wikipedia.org/wiki/Multitier_architecture
[ACID]: https://en.wikipedia.org/wiki/ACID
[WinACE]: https://sourceforge.net/projects/winace/files/
[PTkII2]:  https://sourceforge.net/projects/ptkei2/
[gPRC]:  https://grpc.io
