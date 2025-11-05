The Design of Empire
====================

@brief Document the development philosophy and major design decisions

<img src="images/design.png" style="width:300px; float: right; margin: 20px 10px 10px 10px;" alt="Design"/>

## Design Philosophy

Design First.  Then code.

1. Embrace thoughtful, modern Software Engineering design practices
2. Is correct & efficient
3. Is fun

I'm writing Empire V because I want to learn more about Object-Oriented 
Programming.  Specifically, I want to become a practitioner of design patterns 
and [modern C++].  [Empire] is well understood and documented inasmuch as it's 
an existing program that I intend to replicate.  The requirements for Empire
are reasonably well understood, which a large body of work that I don't need to
worry about.  My focus is to implement the requirements using a solid design 
philosophy.  To that end, my intent is to consider and document alternatives, 
then select the best fit for the requirements.

I intend to use an [Agile] development methodology similar to [XP].  The good 
news is that I have an excellent "customer" -- the original design of [Empire].
My customer is clear, always available and takes criticism very well.

I don't intend to change the gameplay in any significant way.  I am rewriting it 
from the ground up.  It will be designed with modern programming
principles such that it'll be fully documented, tested, loosely coupled and 
extensible.

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



## The Choice of Language
Empire was originally written in [C].  [An examination of the source] will both
awe and terrify most programmers.  They really did an excellent job with the tools
they had.  The codebase implements primitive _objects_ and makes use of some 
design patterns.  The only criticism I'd level against the original is that 
some variable names were _very_ cryptic:  `int m_m_p_d = 1440;  /* max mins of play per day (per country) */`

Before choosing [C++], I considered a number of programming languages.  Also, 
please refer to [High Level Design Requirements](DESIGN.md#high-level-design-requirements) 
for the criteria:

### Java (and C#)
#### Pros
  - In the last decade, [Java] has published some excellent, large, multi-platform, 
    internationalized, programs: [Ghidra], [IntelliJ], [Eclipse], et. al.
  - It has a huge library to draw from.
  - It has a good ecosystem of tools: [Javadoc], code signing, etc.

#### Cons
  - Performance.  Although I think it's very good, it's simply not optimal.
  - Fun factor.  I've written a lot of Java.  I want to dig into something new.

I envision the client being written in one of these languages someday

### Python (and Pearl and PHP)
#### Pros
  - [Python], [Pearl] and [PHP] have many of the same Pros as [Java], but... 

#### Cons
  - I continue to be concerned about the stability of [Python]
  - Performance
  - Lack of static analysis tools
  - They are a scripting languages
  - [Python] programs seem (to me, at least) to be fragile.  They break all the
    time and I have to sort out what to fix.

### Go
#### Pros
  - I'll be honest, [Go] has a lot going for it.  It was authored Bob Pike & Ken
    Thompson.  Giants in our community.
  - It's compiled to machine executables
  - Strong typing
  - Concurrent / native thread support

#### Cons
  - Lack of generics:  No templates.  No compile-time guarantees.
  - I'd miss assertions

### Rust
#### Pros
  - This is another tough call.  [Rust] is used in the Linux Kernel, which says a
    lot about its suitability as a systems language.
  - It's compiled to machine executables.  
  - Strong typing.  
  - Concurrent.
  - Functional.  
  - A decent standard library (not amazing).  
  - Generics.  
  - Ranges.
  - It's fast.  In the same league as [C].

#### Cons
  - Stability.  Partly, the language and partly the standards body.

Really, in hindsight, not going with [Rust] may be a mistake.  I'll leave this
for Empire VI.

### Swift (and Objective-C)
#### Pros
  - Messaging.  For Empire's core, this could be a game-changer.  Think about
    how the internals of Empire would benefit from this.

#### Cons
  - [Swift]: The paint's still wet... it's so new
  - Lack of libraries on non-Apple platforms
  - Objective-C: It's based on [C], not [C++]
  - Objective-C: I find the language to be ungainly

It may also be a mistake not going with [Swift]

### C++
#### Pros
  - It's fast.  
  - Strong typing (although it's getting weaker.  If I want to use `auto`, I'll
    write in JavaScript)
  - Generics/templates.  
  - Functional.  
  - Has mature libraries, toolset and a variety of compilers

#### Cons
  - I really feel like the current standards committee is distorting the language.
    - There's no fewer than 4 ways to make a `const` and one of them is still
      mutable (I'm looking at you, [constinit]).
  - It's not expressive.  I know they've added "expressive" features, but if
    you look at a declaration for a range-based iterator that uses concepts, 
    you'd be mystified (at least I am).  Also, the error messages can be byzantine.

#### Bottom Line

So, why did I go with [C++]?  Because I know it (well enough, that is) and I 
want to get into it a bit more.  I don't like the idea of learning a new 
language from scratch unless I'm getting paid for it.  It also takes a few 
_years_ to gain competency in a language.

I realize that this isn't a good basis for such a fundamental design decision, 
but it boils down to this:  **Get it done**.  If we want to get it done, then 
[C++] is the best choice because I can start now.  Otherwise, I'll need to wait 
a few years.  Maybe someone will come along and rewrite Empire in [Rust] or [Swift], 
incorporating some of the design patterns I'll be prototyping for them and 
improving on others.


## High Level Design Requirements

In no particular order, here are the things I'd like Empire V to do:

  - International UNICODE support
    - I'm on the fence about number & date formatting
    - I don't think I'll be supporting right-to-left languages
    - Make it easy to translate from one language to another
  - Uses a modern logger for debugging, telemetry, etc.
  - Uses a modern exception-handling mechanism
  - Is persistent
  - Uses modern Software Engineering practices like:
    - Continuous integration with [GitHub Actions]
    - Source-level documentation with [Doxygen]
    - Unit testing with [Boost Test]
    - Static analysis / linting with [clang-tidy]
    - Build orchestration with [CMake]
  - Is secure

### Low Level Design Requirements
  - All major data structures should be aligned
  - Works on several compilers:  [Visual Studio], [clang], [gcc], [icc]
  - Work on several 64-bit platforms:  Linux & Windows

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

Doxygen version x.x.x shipped a few days ago.  They made an enhancement I 
requested and I was impressed with how they migrate configuration files.  It 
occurred to me (while doing the dishes) that I really liked Doxygen's 
configuration system.  I should emulate, strike that, copy it.

Doxygen's "master" configuration is a large XML file.  Their configuration 
source is a mix of C and C++.  

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
- Improvement over classic Empire:  Consolidate all config in one place

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


## Singletons

What are we talking about?  The objects I'd like to make globally available
via Singleton are:
- Core:  The core services object
- Config: The server configuration state
- Nations:  The collection of nations
- Units:  The collection of all units
- World:  The collection of all sectors
- Logger:  The global logger

I've read [Singletons are Pathological Liars], [Where Have All the Singletons Gone?] 
and [Performant Singletons].

> The Singleton is probably one of the most controversial design patterns, 
> sparking perennial debate on forums and discussion boards, and prompting 
> analysis and dissection in many articles and papers.

> -- <cite>Performant Singletons</cite>

The Singleton pattern should be applied only when:
1. Singularity must exist
2. Global accessibility must exist

I think this applies to the objects listed above.

The arguments against Singletons...
  - Well-designed systems isolate responsibility.  Singletons encourage tight
    coupling by making it easy for components to depend on them.
    - Agreed:  I'd have to think on this, but my initial reaction is that, for 
      the things I intend to make Singletons, those dependencies are inherent.
  - Whether there needs to be a single instance of an object depends on the
    context that object is being used.
    - I will not be running multiple instances of Empire in the same process space.
    - I won't be testing multiple Empire programs concurrently.  One Empire at a time.
    - We should have the ability to shutdown and restart the core in the same process.
  - Have the self-control and awareness to create as many instances as you 
    need — one, if that’s all it takes.
    - I'd rather have the object guarantee this.  Isn't that the argument for 
      Encapsulation; The object hides its data to protect itself.
  - When you encounter a scenario where you need to depend on another object or 
    subsystem, stop and ask yourself why you need that dependency.
    - Yea, sometimes we really do.  Even if I have Coreable, Loggable, and Nationable 
      interfaces, sooner or later, I'll need an actual object to reference.
    - It's global state.  It is what it is.  The trick is to make is usable, 
      extendable **and** safe.
  - How do you create mock worlds for testing?
    - How will we test multiple world sizes?  How will we test creating and 
      destroying a world?
      - To achieve optimal performance, we'd hardcode configuration (for 
        example). But this sacrifices a lot of unit testing capabilities.  
        Therefore, we will make a testability-over-performance tradeoff and make
        configuration dynamic.
      - empire::Singleton will have an `erase()` method which will destroy it 
        and allow it to be recreated.
  - You loose control of initializations
    - Not true.  The Core will initialize all the Singletons.  That's what
      the core does.  Also, I will be using instances of Singletons (or the 
      contents therein) as parameters when I create other objects.
    - The test harness will manage the test Singletons.
  - Explicit reference passing makes garbage collection work.
    - All of these objects will begin with Empire and end with Empire.
    - The `erase()` method on Singleton will destroy.  There's nothing wrong 
      with that.  The Singleton guarantees only one object will
      exist after you ask for it (which is still true).  It also makes it 
      globally available, which will also continue to be true.
  - Singletons are nothing more than global state. Global state makes it so your
    objects can secretly get hold of things which are not declared in their APIs, 
    and, as a result, Singletons make your APIs into pathological liars.
    - It's true... I'm collaborating with more objects than the API claims.  I'll
      give this some thought.
  - As the original author of the code, I know the true dependencies.  Anyone 
    who comes after is baffled, since not all of the dependencies are 
    declared and information flows in some secret paths which are not clear from
    the API.  You live in a society full of liars.
    - That's a little dramatic.  Especially, if you tell everyone up-front what
      our Singletons are and they are logical.  Still, I think it's good point.
      When do we pass a parameter and when do I grab from a Singleton?
  - Globals provide invisible lines of influence across all the code
    - Let's distinguish between reading & writing globals.  Unexpected writing 
      to a global is bad.  We can all agree on that.  Reading from read-only 
      globals are not too different from constants, which is fine.
    - So, let's not make native datatypes a Singleton.
    - Agreed, in the context of raw variables...  but these globals are objects
      that have all of the protections an encapsulated class should have.
  - Globals lead to spaghetti code.
    - Agreed, when they are abused.  All global state will be maintained in the 
      object model.
    - I'm not creating `static int i, j, k` as global iterators.
  - When you go to include 3rd party libraries in your code, sometimes they use 
   the same names as yours.
    - All of our globals will be in the Empire namespace.

Detractors say that Singletons actively work against separation of duties by 
introducing implicit, hidden dependencies on external systems and allowing 
instant access from any location, even if that location is illogical.  True, but
I can't see why having access to any of the above would be illogical.

I also do _**not**_ want to pass these into every function, constructor, et. al. 
under the sun.  That's not happening.

I agree that Globals should be used with utmost care, and functions should 
clearly scope out the variables they use via an API, locals, what have you.

Another way to manage this is to carefully control the creation of Singletons.  We aren't
letting just any variable, say `auto i`, to be used as a global iterator.

The important thing is to remember the overall goal: clarity

The "no global variables" rule is there because most of the time, global 
variables make the meaning of code less clear.  However, like many rules, people 
remember the rule, and not what the rule was intended to do.

I've seen programs that seem to double the size of the code by passing an 
enormous number of parameters around simply to avoid the evil of global variables. 
In the end, using globals would have made the program clearer to those reading it. 
By mindlessly adhering to the word of the rule, the original programmer had 
failed the intent of the rule.

So, yes, globals are often bad. But if you feel that in the end, the intent of 
the programmer is made clearer by the use of global variables, then go ahead. 
However, remember the drop in clarity that automatically ensues when you force 
someone to access a second piece of code (the globals) to understand how the 
first piece works.

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
- [ ] Do a UMLet drawing of the current Empire codebase
- [ ] Expand on the UMLet drawing to include the design of the Business Domain / 
    data model.
- [ ] Set the project icon in CLion

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
