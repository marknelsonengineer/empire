Empire Drawings
===============

# The Singletons of Empire

@startuml
!theme crt-amber
skinparam classAttributeIconSize 0
top to bottom direction

' Abstract parent: template-based Singleton
abstract class "Singleton<T>" as Singleton {
- static T* instance
  --
+ static T& getInstance()
# Singleton()
# ~Singleton()
}

' Concrete singletons
class Core
class Configuration
class Commands
class Nations
class Messages
class Loans
class Logger
class Metrics
' List of entities

' Relationships (inheritance)
Core          -up-|> Singleton
Configuration -up-|> Singleton
Commands      -up-|> Singleton
Nations       -up-|> Singleton
Messages      -up-|> Singleton
Loans         -up-|> Singleton
Logger        -up-|> Singleton
Metrics       -up-|> Singleton

' Notes for clarity
note right of Singleton
Template-based abstract class
that enforces the Singleton pattern
end note

@enduml

<br><br>

# Composition of Entities

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

@startuml
!theme crt-amber
skinparam classAttributeIconSize 0

class BaseMap
class WorldMap
class NationMap
class Nations { class Nation }

'class Nations { class Nation {} }

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
