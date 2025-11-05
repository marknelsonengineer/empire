Conceptual Design
=================

## The Singletons of Empire

@startuml
!theme crt-amber
skinparam classAttributeIconSize 0
top to bottom direction

' Template-based, abstract Singleton
abstract class "Singleton< T >" as Singleton {
}

' Concrete singletons
class Core
class Configuration
class Commands
class Nations
class WorldMap
class Messages
class Loans
class Logger
class Metrics
' List of entities


' Relationships
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

<br><br>

## The Core services of the Empire V server
As the `main()` for the Empire V server:
     - Initialize the system-level singletons
     - Either:
       - Create the Nations and then build the world's sectors
       - Unmartial saved game state
     - Test the system / assure it's healthy
     - Initiate timed activities such as updates
     - Start the network server
     - Shutdown and save the game state

<br><br>

## Configuring the Empire
The configuration has two broad types:

- **Static Config:**  Unchanging parameters
  - The number of nations
  - The size of the world
- **Dynamic Data:**  Uhich can be marshaled with the entire state
  - Empire Time Units
  - Time of next update

### Deign Goals
  - Ensure each type of configuration is consolidated in one place
  - There should be a single class that contains all config getters (regardless
    of type) and setters (as appropriate)
  - There should be a method to validate the configuration and ensure it follows
    the business rules

### Implementation Goals
  - For performance reasons, try to use compile-time expressions for the static 
    configuration in the source.  Use C++ features like `constexpr` and `consteval`.
  - Doxygen has a good configuration mechanism... emulate it.

By design, and for optimization purposes, we are configuring the configuration at pre-compilation time.  Also, the nations and sectors data structures will be arrays who sizes are fixed at compilation time.


<br><br>

## Commands in Empire V

I think the network layer API maps very closely to the commands. This way, all clients are created equal.

The command handlers on the server side become controllers for orchestrating what the command does. That makes it very easy to add new commands.

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

' Relationships (inheritance)
'Singleton -[hidden]left-> BaseCommand
Commands          --|> Singleton
Accept            -up-|> BaseCommand
Add            -up-|> BaseCommand
Xdump            -up-|> BaseCommand
Zdone            -up-|> BaseCommand
'BaseCommand -[hidden]down-> Accept
Accept "1"              --* Commands
'Accept -[hidden]down-> Add
Add "2"              --* Commands
'Add -[hidden]down-> Xdump
Xdump "199"              --* Commands
'Xdump -[hidden]down-> Zdone
Zdone "200"              --* Commands

@enduml


# Composition of Entities

We need to put `BaseUnit` between `BaseEntity` and the three unit classes.  This is because sectors can’t be held by other sectors and we wanted to distinguish things that can be help from things that are inviolate once they’re created.

@startuml
!theme crt-amber
skinparam classAttributeIconSize 0

class BaseEntity
class Sector
class BaseUnit
class LandUnit
class SeaUnit
class AirUnit
class NukeUnit

' Relationships (inheritance)
Sector          -up-|> BaseEntity
BaseUnit        -up-|> BaseEntity
LandUnit        -up-|> BaseUnit
SeaUnit         -up-|> BaseUnit
AirUnit         -up-|> BaseUnit
NukeUnit        -up-|> BaseUnit

@enduml

<br><br>


# Composition of Maps

Sectors are created in Genesis. And never ever re-created after that.  All sectors are held in the base map, array, as well as each nations national map array.

Consider maintaining a world map, and then each nation having their own map. The nations map objects will have pointers to the global map. When a nation needs to generate their own map, they follow those pointers to the master map. If the nation owns the sector, they get all the data. If the nation is an ally, and has agreed to share data, they’ll get an appropriate amount of data from that. Otherwise, the nation will get information that was stored the last time they flew an aircraft over or learned something from a ship or a satellite or a radar station.

@startuml
!theme crt-amber
skinparam classAttributeIconSize 0

class BaseMap
class WorldMap
class NationMap
class Nations
class Nation

' Relationships (inheritance)
WorldMap        -up-|> BaseMap
NationMap       -up-|> BaseMap
'NationMap       -right-> Nation
'Nations "1" -- "1..*" Nation : contains

@enduml

<br><br>

# Other Important Classes

@startuml
!theme crt-amber
skinparam classAttributeIconSize 0

class Core
class EmpireException

' Relationships (inheritance)

@enduml



