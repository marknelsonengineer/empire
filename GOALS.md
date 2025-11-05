Goals & Assumptions
===================

Empire V is an exercise in modern software engineering and design.  It's based 
on the well documented behavior of [Empire 4].  I don't intend to change the 
gameplay in any significant way.  I am rewriting it from the ground up so it 
can be designed and implemented with modern programming practices.

Therefore, we have the ideal "customer": The original design of [Empire 4]...
who is always clear, available and takes criticism _very well_.


## Design Philosophy

Design.  Then code.

1. Embrace thoughtful, best-of-breed, modern Software Engineering design practices
     - Fully documented, tested, loosely coupled and extensible
   
2. Is correct & efficient

3. Major design decisions are documented:
      - Alternative approaches
      - Pros and cons
      - Mitigations
      - Triggers for reevaluation

4. Is fun

I'm writing Empire V because I want to learn more about Object-Oriented
Programming.  Specifically, I want to become a practitioner of [design patterns]
and [modern C++].  Empire 4 is well understood and documented inasmuch as it's
an existing program that I intend to replicate.  Detailed user requirements need 
a substantial amount of work and evolve during design and implementation.  We 
are in a unique situation where the [requirements for Empire] are very well 
documented and stable.  This allows us to focus on the design.


## Layers of Design
Separate the design into manageable layers:
      - **Goals & Assumptions**
      - **Conceptual:**  Entities and their relationships
      - **Specification:**  Expose the ADTs and their APIs
      - **Implementation:**  Detailed design

We are looking at using the Zachman Framework for identifying the layers of 
design.


## High Level Design Requirements

In no particular order, here are the things I'd like Empire V to do:

- International UNICODE support
      - I'm on the fence about number & date formatting
      - I don't think I'll be supporting right-to-left languages
      - Make it easy to translate from one language to another
- Uses modern tools for logging, debugging, telemetry, performance monitoring, etc.
- Uses modern exception-handling
- Is persistent
- Uses modern Software Engineering practices like:
      - Continuous integration with [GitHub Actions]
      - Source-level documentation with [Doxygen]
      - Unit testing with [Boost Test]
      - Static analysis / linting with [clang-tidy]
      - Build orchestration with [CMake]
- Is secure
- Continual validation / built-in health checks


## Assumptions
  - Lots of memory
  - Multicore CPU
  - Fast, reliable network


[Empire 4]:  http://www.wolfpackempire.com
[modern c++]: https://www.modernescpp.com
[requirements for Empire]: https://empserver.sourceforge.net/infopages/all.html
[design patterns]: https://refactoring.guru/design-patterns
[GitHub actions]:  https://docs.github.com/en/actions
[Doxygen]:  https://doxygen.nl
[Boost test]: https://www.boost.org/doc/libs/1_82_0/libs/test/doc/html/index.html
[CMake]:  https://cmake.org
[clang-tidy]:  https://clang.llvm.org/extra/clang-tidy/
