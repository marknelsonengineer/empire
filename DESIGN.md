The Design of Empire ][
=======================

@brief Document the development philosophy and major design decisions

## Design Philosophy

  1) Embrace modern Software Engineering design practices
  2) Is correct & efficient
  3) Is fun

To a large degree, I'm writing Empire because I want to learn more about
Object-Oriented Programming.  Specifically, I want to become a practitioner of
design patterns.  [Empire] is well understood and documented inasmuch as it's an
existing program that I intend to replicate.  To that end, my intent is to 
consider as many patterns as possible and select the best fit for the 
application.

## High Level Design

Options come in 2 flavors:
1) Compiled into the code (most of them - for efficiency)
2) Runtime options


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




Empire:  http://www.wolfpackempire.com
3-tiered application:  https://en.wikipedia.org/wiki/Multitier_architecture
ACID: https://en.wikipedia.org/wiki/ACID
