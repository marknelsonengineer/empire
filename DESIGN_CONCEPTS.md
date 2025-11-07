Conceptual Design
=================

## The Singletons of Empire

Singletons ensure controllers can find every object in the model from a
convenient set of starting points.

@startuml
!theme crt-amber
top to bottom direction

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

Core          -up-|> Singleton
Configuration -up-|> Singleton
Commands      -up-|> Singleton
Nations       -up-|> Singleton
WorldMap      -up-|> Singleton
Messages      -up-|> Singleton
Loans         -up-|> Singleton
Logger        -up-|> Singleton
Metrics       -up-|> Singleton

note right of Singleton: I am undecided on the need for a MobileUnits singleton

@enduml


## Core Services of the Empire V Server
As the `main()` for the Empire V server:
     - Initialize the system-level singletons
     - Either:
       - Create the Nations and then build the world's sectors
       - Unmartial saved game state
     - Test the system / assure it's healthy
     - Initiate timed activities such as updates
     - Start the network server
     - Shutdown and save the game state

The Core class will also maintain timekeeping services like the game clock.

@startuml
!theme crt-amber
top to bottom direction

class Core
class EmpireException
class Sessions
class Session
class Nation
class BaseThread
class SessionThread
class DeityThread

Sessions "n" *-- "1" Session
Session "1" *-- "1" Nation
BaseThread <|-down- SessionThread
BaseThread <|-down- DeityThread
SessionThread "1" -left- "1" Session

@enduml

@todo:  Create a Sessions Singleton or hold Sessions in Core

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
class Commands
class Accept
class Add
class Xdump
class Zdone

Commands      --|>     Singleton
Accept        -up-|>   BaseCommand
Add           -up-|>   BaseCommand
Xdump         -up-|>   BaseCommand
Zdone         -up-|>   BaseCommand
Accept "1"    --*      Commands
Add "2"       --*      Commands
Xdump "199"   --*      Commands
Zdone "200"   --*      Commands

@enduml


## Composition of Entities

`MobileUnit` is between `BaseEntity` and the unit classes because sectors can’t
be held by other sectors and we want to distinguish things that can be held 
(`MobileUnit`) from things that are inviolate once they’re created (`Sectors`).

@startuml
!theme crt-amber

class Sector {
   ResourceArray resources[ RESOURCE_COUNT ]
}
class BaseEntity {
   CommodityArray commodities[ COMMODITY_COUNT ]
}

class MobileUnit
class LandUnit
class SeaUnit
class AirUnit
class NukeUnit

Sector      -up-|> BaseEntity
MobileUnit  -up-|> BaseEntity
LandUnit    -up-|> MobileUnit
SeaUnit     -up-|> MobileUnit
AirUnit     -up-|> MobileUnit
NukeUnit    -up-|> MobileUnit

@enduml

Each `BaseEntity` holds an array of `commodities`... and

Each `Sector` holds an array of `resources`...

@startuml
!theme crt-amber

class CommodityEnum
class CommodityType
class CommodityArray

class Commodity {
   int maxValue
   int value
   CommodityType commodityType
}

CommodityArray   -up-|> CommodityType
Commodity        -->    CommodityType

class Resource {
   int value
   ResourceType resourceType
}

class ResourceEnum
class ResourceType
class ResourceArray

ResourceArray   -up-|> ResourceType
Resource        -->    ResourceType

@enduml


## Composition of Maps

Sectors are created in Genesis. And never ever re-created after that.  All sectors are held in the base map, array, as well as each nations national map array.

Consider maintaining a world map, and then each nation having their own map. The nations map objects will have pointers to the global map. When a nation needs to generate their own map, they follow those pointers to the master map. If the nation owns the sector, they get all the data. If the nation is an ally, and has agreed to share data, they’ll get an appropriate amount of data from that. Otherwise, the nation will get information that was stored the last time they flew an aircraft over or learned something from a ship or a satellite or a radar station.

@startuml
!theme crt-amber
skinparam classAttributeIconSize 0

class BaseMap
class WorldMap
class NationalView
class Nation

WorldMap         -up-|>   BaseMap
NationalView     -up-|>   BaseMap
NationalView     --       Nation

@enduml


## Messages

@startuml
!theme crt-amber

class Messages
class Message

Messages "1" -right-> "n" Message 

@enduml


## Loans

@startuml
!theme crt-amber

class Loans
class Loan

Loans "1" -right-> "n" Loan

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

Metrics "1" -right-> "n" Metric

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
