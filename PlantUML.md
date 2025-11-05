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


