Empire V
========

<img src="images/logo_empire_500x500.png" style="width:80px; float: left; margin: 0 10px 10px 0;" alt="Empire Logo"/>

Empire is a real time, strategic, multiplayer, Internet-based game, featuring military, 
diplomatic, and economic goals.

This design is based on the classic game from the 1970s here:  http://www.wolfpackempire.com

The project's Git repository is at:  https://github.com/marknelsonengineer/empire

The source code is documented at:  https://mark.nelson.engineer/srcdoc/empire/

Empire V is an exercise in modern software engineering and design.  It's based
on the well documented behavior of [Empire 4].  I don't intend to change the
gameplay in any significant way.  I am redesigning it from the ground up so it
can be implemented with modern programming practices.

The design has been divided into manageable layers:
- [Goals & Assumptions:](GOALS.md)      The highest level of design
- [Conceptual:](DESIGN_CONCEPTS.md)     Entities and their relationships
- [Specification:](API_DESIGN.md)       Expose the Abstract Data Types and their Application Programming Interfaces
- [Implementation:](IMPLEMENTATION.md)  Detailed design & implementation decisions

[Empire 4]:  http://www.wolfpackempire.com
