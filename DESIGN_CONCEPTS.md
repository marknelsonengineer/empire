Conceptual Design
=================

## The Singletons of Empire

Singletons ensure controllers can find every object in the model from a
convenient set of starting points.

@startuml
!theme crt-amber

' Template-based, abstract Singleton
abstract class "Singleton< T >" as Singleton {
}

class Core
class Configuration
class Commands
class Nations
class WorldMap
class Messages
class Loans
class Logger
class Metrics
class Market

class Sessions
class Nation
class Command
class Sector
class Message
class Loan
class Metric
class Listing

Singleton <|-- Core         
Singleton <|-- Configuration
Singleton <|-- Commands     
Singleton <|-- Nations      
Singleton <|-- WorldMap     
Singleton <|-- Messages     
Singleton <|-- Loans        
Singleton <|-- Logger       
Singleton <|-- Metrics     
Singleton <|-- Market

Core         -->  Sessions
Nations  "n" *-- Nation   
Commands "n" *-- Command  
WorldMap "n" *-- Sector   
Messages "*" *-- Message  
Loans    "*" *-- Loan     
Metrics  "n" *-- Metric
Market   "*" *-- Listing

@enduml

Singleton's I'm considering are:
  - `MobileUnits` (or hold them as lists in `Nation` and `BaseUnit`)


## Core Services of the Empire V Server
As the `main()` for the Empire V server:
     - Initialize the system-level singletons
     - Either:
       - Create the Nations and then build the world's sectors
       - Unmartial saved game state
     - Test the system / assure it's healthy
     - Initiate timed activities such as updates
     - Listen for network connections
     - Shutdown and save the game state

The Core class will also maintain timekeeping services like the game clock.

@startuml
!theme crt-amber

class Core
class EmpireException
class Sessions
class Session
class Nation
class BaseThread
class SessionThread
class DeityThread

Sessions "n"  *--     Session
Session "1"   *-- "1" Nation
BaseThread    <|--    SessionThread
BaseThread    <|--    DeityThread
SessionThread --      Session

@enduml

The compute, application, and storage architecture of Empire is:

@startuml
!theme crt-amber
top to bottom direction

package Server {
  rectangle "=Empire Server" as EmpireServer
  rectangle "Security Oracle" as LSASS

  file State
  file Credentials

  State <-up-> EmpireServer : Marshalling
  Credentials <-up-> LSASS 
  EmpireServer <--> LSASS : Authorizations
}

cloud "Internet" {
collections Clients
}

Clients --> EmpireServer : API modeled after commands

@enduml

**Note:**  Empire does not use a database.  For efficiency, the model is in the
  Empire Server and periodically marshalled to/from files to maintain state.

Sessions will be held in the core and not as a Singleton.  Sessions don’t need 
to be accessed by absolutely everything, and I feel better having a little bit 
of control around them via a getter from the core.


## Configuring the Empire
Configuration data falls into two broad families:

- **Static Config:**  Unchanging, compiled-in parameters
  - The number of nations
  - The size of the world
- **Dynamic Data:**  Uhich can be marshaled with the entire state
  - Empire Time Units
  - Time of next update

### Deign Goals
  - Consolidation:  Ensure each type of configuration is in one place
  - Have a single class with all config getters (regardless of type) and 
    setters (as appropriate)
  - Have methods to validate the configuration and ensure they follow the
    business rules

### Implementation Goals
  - For performance reasons, try to use compile-time expressions for the static 
    configuration in the source (C++ features like `constexpr` and `consteval`)
  - Doxygen has a good configuration mechanism... emulate it

By design and for optimization purposes, we set the static configuration at
pre-compilation time.  
  - Implication:  Several key collections like Nations, WorldMap and Sessions 
    will be arrays that are fixed at compilation time


## Commands in Empire

The network layer API will map closely to the commands.  This way, all clients 
are created equal.

The command handlers on the server side become controllers for orchestrating 
what the command does.  This makes it very easy to add new commands.

@startuml
!theme crt-amber
skinparam classAttributeIconSize 0

class BaseCommand
class Singleton
class Commands <<Map>>
class Xdump
class Zdone
class Accept
class Add

Singleton <|-- Commands

BaseCommand <|-- Zdone
BaseCommand <|-- Xdump
BaseCommand <|-- Add   
BaseCommand <|-- Accept

Commands *-- "200" Zdone
Commands *-- "199" Xdump
Commands *-- "2" Add
Commands *-- "1" Accept

@enduml


## Composition of Entities

`MobileUnit` is between `BaseEntity` and the unit classes because sectors can’t
be held by other sectors and we want to distinguish things that can be held 
(`MobileUnit`) from things that are inviolate once they’re created (`Sectors`).

@startuml
!theme crt-amber

class Sector {
  Resource mineral
  Resource gold
  Resource fertile
  Resource oil
  Resource uranium
}

class Resource {
  value
}

class BaseEntity {
  Commodity civ
  Commodity mil
  Commodity shell
  Commodity gun
  ...
  Commodity rad
}

class Commodity {
  maxValue
  value
}

class MobileUnit
class LandUnit
class SeaUnit
class AirUnit
class NukeUnit

Sector      -up-|> BaseEntity
Resource    -left-* Sector::resources
MobileUnit  -up-|> BaseEntity
Commodity   -left-*  BaseEntity::commodities
LandUnit    -up-|> MobileUnit
SeaUnit     -up-|> MobileUnit
AirUnit     -up-|> MobileUnit
NukeUnit    -up-|> MobileUnit

@enduml

Each `BaseEntity` holds a set of `commodities`, and each `Sector` holds a set of `resources`.

@startuml
!theme crt-amber

enum CommodityEnum {
  CIV   =0
  MIL   =1
  ...
  RAD  =13
}
class CommodityProfile
class CommodityProfiles {
  CommodityProfile[ n ]
}

class Commodity {
   maxValue
   value
   CommodityProfile
}

CommodityProfiles           *-- CommodityProfile
Commodity::CommodityProfile o-- CommodityProfile
CommodityEnum               --  Commodity        
CommodityEnum               --  CommodityProfile 
CommodityEnum               --  CommodityProfiles

class Resource {
   int value
   ResourceProfile
}

enum ResourceEnum {
  MINERAL = 0
  ...
  URANIUM = 4
}
class ResourceProfile
class ResourceProfiles {
  ResourceProfile[ n ]
}

ResourceProfiles          *-- ResourceProfile
Resource::ResourceProfile o-- ResourceProfile
ResourceEnum              --  Resource        
ResourceEnum              --  ResourceProfile 
ResourceEnum              --  ResourceProfiles

@enduml


## Composition of Maps

Sectors are created in Genesis, and never ever re-created after that.  All 
sectors are held in the BaseMap, array, as well as each Nations' array of sectors
owned.

There is a `WorldMap`, and then each `Nation` has their own `NationalView`, with 
pointers to `Sectors` in `WorldMap`.  When a nation view a map, they follow those 
pointers to the master map.  If the nation owns the sector, they get all the data. 
If the nation is an ally, and has agreed to share data, they’ll get an appropriate 
amount of data from that. Otherwise, the nation will get information that was 
stored the last time they flew an aircraft over or learned something from a 
ship or a satellite or a radar station.

@startuml
!theme crt-amber

class BaseMap <<array>>
class WorldMap
class NationalView
class Nation
class Sector

BaseMap      <|-- WorldMap
BaseMap      <|-- NationalView
NationalView --   Nation
WorldMap     *-- "n" Sector
NationalView o.. "n" Sector

@enduml


## Messages

@startuml
!theme crt-amber

class Messages
class Message

Messages *-- "*" Message 

@enduml


## Loans

@startuml
!theme crt-amber

class Loans
class Loan

Loans *-- "*" Loan

@enduml


## Logger & Metrics

@startuml
!theme crt-amber

class Logger
class Metrics
class Metric
class Counter
class Timer
class Measurement

Metrics *-- "n" Metric

Metric <|-- Counter
Metric <|-- Timer
Metric <|-- Measurement

@enduml


## Secure By Design
Bottom line:  Should the listener threads be in the same process space as the model?
If they are in the same process, then we’re must accept a certain amount of risk.

The opposite of security is usability.  I want the system to be both usable and 
secure. 

### Threat Vectors
  - Buffer overflow attacks
  - Memory overwrites - Where an authorization or security token gets overwritten
  - Denial of service
    - Memory corruption
    - Inducing crashes

The primary data model will not manage credentials.  Instead, we use a Security
Oracle that runs in a separate process.  This is similar to Windows' security 
process LSASS. ...and if it’s good enough for Microsoft...

The Security Oracle will:
  - Decrypt and manage a `shadow` password file
  - Change passwords
  - Cryptographically validate each session's nation ID to ensure it hasn't
    been overwritten

My guess is that attempts at remote code execution or memory overwrites will
tend to crash the thread and very likely corrupt the data model.
