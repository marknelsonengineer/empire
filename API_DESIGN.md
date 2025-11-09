API & ADT Design
================

## Lists & Collections

| Type    | Element            | Container    | Default, min, max          | Estimated Count | Size (per element) | Total Size |
|---------|--------------------|--------------|----------------------------|-----------------|--------------------|------------|
| Array   | Nation             | Nations      | 10, 8, 86                  |                 |                    |            |
| Array   | Sectors            | WorldMap     | (64x32) (64x32) (184x88)   |                 |                    |            |
| Array   | View               | NationalView | Same as WorldMap x Nations |                 |                    |            |
| Array   | Comodity           | BaseEntity   | 14 Commodities             |                 |                    |            |
| List    | MobileUnit         | Nation       |                            |                 |                    |            |
| List    | MobileUnit         | BaseEntity   |                            |                 |                    |            |
| List    | BaseEntity + Fleet | Nation       |                            |                 |                    |            |
| List    | Metric             | Metrics      |                            |                 |                    |            |
| List    | Message            | Messages     |                            |                 |                    |            |
| Array   | Session            | Sessions     |                            |                 |                    |            |
| List    | Loan               | Loans        |                            |                 |                    |            |
| HashMap | Command            | Commands     |                            |                 |                    |            |

The default starting sectors/country = 30 for 10 countries with a world size of 52x32

For example, a games starts with "30 sanctuaries" giving each player 30 sectors.

Genesis creates a specified number of start islands (continents), and spaces 
them evenly accross the map.  It makes all of them the same size, and makes sure
that they don't meet.  It gives each start island exactly the same distribution 
of resources and the same number of mountains.

### Collections under Consideration
  - All entities
  - Entities by nation plus fleet
  - Threads


## API & ADT Designs

In the Abstract Data Type (ADT) and Application Programming Interface (API)
design, we will document the defining characteristics... the soul of the class... 
and document *relevant* members and methods, but not *all* of them as we would 
in the implementation.

The Singleton highlights the differences between an ADT design and an 
implementation.

### Singleton

@startuml
!theme crt-amber
top to bottom direction

abstract class "Singleton< T >" as ADT {
  # T instance
  + T get()
}

note right
An ADT
end note

abstract class "Singleton< T >" as Implementation {
# {static} std::unique_ptr<T>  s_instance
- {static} boost::uuids::uuid uuid
- {static} singleton_counter_t constructCounter
- {static} singleton_counter_t destructCounter

+ Singleton(Singleton &)=delete
+ Singleton(const Singleton &)=delete
+ Singleton(const Singleton &&)=delete
+ Singleton & operator=(const Singleton)=delete
+ Singleton & operator=(const Singleton &)=delete
+ Singleton & operator=(const Singleton &&)=delete
+ T & operator=(const T)=delete
+ T & operator=(const T &)=delete
+ T & operator=(const T &&)=delete
+ virtual ~Singleton()
+ boost::uuids::uuid getUUID() const
+ {static} T & get()
+ {static} std::string info()
+ {static} singleton_counter_t getConstructedCount()
+ {static} singleton_counter_t getDestroyedCount()
+ {static} bool isInstantiated()
+ {static} void validate()
+ {static} void erase()
# Singleton()
}

note left
An implementation contains:
  - Copy & move constructors & assignments
  - Constructors, Destructor
  - Operators:  Equality, ordering, printing and hashing operators
  - UUID methods
  - validate() and info() methods
  - Support for testing
  - Const correctness & typedefs and strict typing
  - Exceptions, serializing & threading
  - Alignment & padding
end note

@enduml


### Configuration

Configuration options come in many forms:
1. Compiled into the code as `constexpr`.  For efficiency, this design's 
   primary option set should be compiled in.
2. Runtime options.  These are dynamic options that could be set/changed while
   the program is running.
3. Config file.  We will maintain state in files, but I don't intend to use
   a configuration file in Empire V
4. Command line.  The server will have a minimal set of command line options.
5. Settable via unit tests
6. Information about the platform


#### Command line options

    -h --help          Print this help and exit 
    -v --version       Print the version and exit
    --random_seed SEED Use for testing or reproducing certain configurations

#### Configuration Object Model

@startuml
!theme crt-amber

class typedefs {
  const uint8_t  const_version_number_t
  const uint32_t const_build_number_t
  const uint16_t singleton_counter_t
}

class Config {
  uint PORT 6565
  uint WORLD_X 64
  uint WORLD_Y 32
  uint ETUs_PER_UPDATE 60
  ...
  bool RUNNING_TEST_SUITE false

  void processCommandLineOptions()
  void setRandomSeed()
}

class Version {
  MAJOR_VERSION
  ...
  BUILD_NUMBER
  string VERSION

  getVersion()
}

class Platform {
  getOS()
  getAddressSize()
  getCompiler()
  isDebug()
}

@enduml

When serializing, store the fixed, compiled-in configuration.  
When deserializing, compare the current configuration with the stored state and
report any changes.


### Commodities & Resources

@startuml
!theme crt-amber

enum CommodityEnum {
  CIV   =0
  MIL   =1
  ...
  UCW  =12
  RAD  =13
}

class CommodityType {
  name1 
  name3
  name8
  name32
  power
  isSellable
  price
  weight
  packingInefficient
  packingNormal
  packingWarehouse
  packingUrban
  packingBank
}

class Commodity {

commodityType
maxValue
value

Commodity( CommodityEnum, MaxCommodityValue );
  getMaxValue()
  getValue()
  operator +=()
  operator -=()
}

Commodity::commodityType -right-* CommodityType
Commodity::commodityType -right-* CommodityEnum

@enduml


@startuml
!theme crt-amber

enum ResourceEnum {
MINERAL = 0
GOLD    = 1
FERTILE = 2
OIL     = 3
URANIUM = 4
}

class ResourceType {
name1
name3
name8
name32
power
}

class Resource {
Resource( ResourceEnum, MaxResourceValue );
resourceType
value

getMaxValue()
getValue()
operator +=()
operator -=()
}

Resource::resourceType -right-* ResourceType
Resource::resourceType -right-* ResourceEnum


@enduml


### Entities

@startuml
!theme crt-amber

class BaseEntity {
id
uid
owner

efficiency
mobility

created_timestamp
last_modified_timestamp
}

class Sector {
  geography
  designation
  elevation
  isCostal
  loyalty
  che
  oldOwner
}

class MobileUnit {
}

BaseEntity <|-- Sector
BaseEntity <|-- MobileUnit
@enduml



## Use of Design Patterns and Containers

Make a table of design patterns and where they’re being used:
- Use flyweight for commodities, sectors, and the various mobile units
- Use composition, mostly for source code management
- Use the command patterns for updates and combat
- We have a number of Singletons



## Composite Pattern

Behaviors to carve out as Composite Behaviors include:
  - Spy
  - Plague
  - Combat
  - Diplomacy
  - Update

For our composites...  
  - Data used by more than one obejct will be held in the parent class
  - Data used only by the composite will be held in the composite

In the command pattern, each command should engage the model to update it, and
then engage a view on the model to report back the current/new state.  It's
on the client to identify changes & convert fields into a visualization.

## Message

  - Dates
  - Message
  - UID
  - Sequence
  - Type
  - Source
  - DestArray[ NATIONS ] -- A non-null value means that country is an intended recipient


## Maps
The world map will be in array, as well, the nation map.  Map entries will have pointers to the sectors that are to the left right above and below.

Each sector probably needs an observed method that generates data for a non-friendly nation that they would be able to see. That generates with a non-friendly nation would be able to see.


## Instrumentation

In addition to a really good logger, we should also build in a metrics and 
instrumentation capability to monitor the performance of the system.  It should 
have periodic performance counters that measure the number of operations as well 
as timers that measure the duration of operations. That information should be 
made visible and could provide benchmarks and indications for health management.


## Design Notes to be Organized

Every unit gets a nation, serialized ID, and a random UID that will not change.

Have a way of destroying and re-creating a Singleton.  This is for testing purposes only.

I think our namespace strategy is lacking

Hang class UID_Generator off of Core.   It can generate one or contiguous blocks of ids.

Consider creating a serialized number class, a Singleton actually. And then assigning a unique ID number to every single object ever created. And that would be the primary key for the serialization or marshaling mechanism.

We definitely need something like a model view controller architecture model is the colonels internal database. The view is each users capital centric view of the world. And everything the user interacts with needs to interact based on that map. In other words, users should never be exposed directly to the model itself.

Put most of the commodity counters in the base class, or at least a large fraction of them, so they’re always there and in a predictable cashed place


## Singletons

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
        - To achieve optimal performance, we are hardcoding the configuration. 
          But this sacrifices a lot of unit testing capabilities.  
          Therefore, we will need to make a set of configuration files for testing.
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


## Object Model




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

Original empire had a telnet like interface to the program. Empire to will have a proper API, and provide a ton that like interface by default, moved it exactly what I chose not to do.



## SOLID Design Principles
The SOLID ideas are
The Single-responsibility principle: "There should never be more than one reason for a class to change."[5] In other words, every class should have only one responsibility.[6]
The Open–closed principle: "Software entities ... should be open for extension, but closed for modification."[7]
The Liskov substitution principle: "Functions that use pointers or references to base classes must be able to use objects of derived classes without knowing it."[8] See also design by contract.[8]
The Interface segregation principle: "Clients should not be forced to depend upon interfaces that they do not use."[9][4]
The Dependency inversion principle: "Depend upon abstractions, [not] concretions."[10][4]



## Entities
Every empire object has (from empobj.h):
- Type (Sector, land, ship, plane, nuke)
- Sequence number
- Generation ???
- UID
- Timestamp
- Nation owner
- Coord x
- Coord y
- Efficiency
- Mobility
- Tech
- Group
- Linked list // Implement my own container

Create a convert that converts one commodity to another.  This can be used to convert civs to mil or gold bars to gold dust.


## To Do
[ ] Schedule a design review with some of the original authors.  Maybe advertise
    on LinkedIn or fiverr


## Uncategorized Notes

As we’re building out our objects, the constructor is going to be responsible 
for either creating the objects for a new game or on marshaling them.  If we 
marshall them, it’s gonna have to be done in two steps.  One question:   can we 
construct objects in an uninitialized state, and then have methods for Genesis 
or on marshaling?

Consider a standard set of methods, like dump, validate, reset, sterilize, deserialize,

Each country has a random GUID. That is the one and only mechanism for representing yourself to the server after initial login and credential exchange.

