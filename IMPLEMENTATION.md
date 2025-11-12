Implementation Design
=====================

## Goals
Make sure that all variables are naturally aligned with the architecture.

Use lock free algorithms wherever possible.

Use atomic operations wherever possible

Align LogIndez and make it CAS

Do and acquire, or release, when setting the ready flag. By setting a half memory variable barrier.

Look into opportunities for static assertions

Things you have to get right upfront. Testing, logging, persistence, Character width, linting, documentation, compiler’s,

- Rpc
- Supported
- Efficient
- Multi platform
- Secure
- Authentication


# Tool Choices

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


## To Do

[ ] Do some UTF-8 tests
[ ] Do parallel builds
[ ] Watch cppcon text talk
[ ] Look at ccmake TUI
[ ] Seriously consider removing Boost Log and replacing it with a macro-fronted, printf-based logger.  This can be removed in production code.
- Not in released code
- Vardac style format
- Use std::format
- Dynamically switch level and category
- Log to console and/or files
- Log file rotation

[ ] Bring stringify over
[ ] Get visual studio going
[ ] Add 2 more CLion targets:  Release (static) and Release Min Size (not static)
[ ] Do a UMLet drawing of the current Empire codebase
[ ] Expand on the UMLet drawing to include the design of the Business Domain /
  data model.

?? UTF8 or UTF16?
I experimented with u8string and it was a disaster

Consider using a publish-subscribe pattern to keep various lists up to date.


# On Hold
[ ] Make a custom set of asserts. We don’t want to fail every time.  [No longer necessary with BOOST_ASSERT??]

[ ] Install a logger utility
[ ] Need a way to manage persistence... Marshalling or a database?
[ ] Wide character support
[ ] For now, I'm going to write Empire V as a narrow-character program.  However, I think there will come a time when there should be a compiler switch for it to support wide characters.



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



### Low Level Design Requirements
- All major data structures should be aligned
- Works on several compilers:  [Visual Studio], [clang], [gcc], [icc]
- Work on several 64-bit platforms:  Linux & Windows


## Latency Study
I put some effort into benchmarking how to efficiently copy strings on x86 systems.
Some ways to do it are:
MOV m64, r64  & MOV r54, m64:  1, 2, p237 p4, 2, 1 + 1, 1, p23, 2, 0.5
MOVS


## Compiler Options
—GC-sections

-z noexecstack
Mark output as not requiring executable stack
-z now
Mark object for immediate function binding

“gcc -fsanitize=address,undefined”

Whole program -fwhole-program

Get into cppcheck



Profile the code… find hotspots and make ‘em inline

Make my getters inline


Build to another compiler
Can i install icc, the Intel compiler?
— Think about correctly scoping my enums (in a Class) — can’t do CommodityType, though

@brief The lowest level of design:  Detailed design & implementation decisions

- Use modern Software Engineering practices like:
  - Continuous integration with [GitHub Actions]
  - Source-level documentation with [Doxygen]
  - Unit testing with [Boost Test]
  - Integration testing
  - Static analysis / linting with [clang-tidy]
  - Build orchestration with [CMake]


## Negative Compilation Testing

Let’s explore some negative compilation testing (ensure things that should not compile, really don't compile).  
It may be part of a existing framework, we may be able to use CMake.


## Uncategorized Notes

We also need a set of rules for making sure that our copy and move constructors are either implemented or suppressed for every major object class

Add an object checklist to the implementation content

In the implementation, write a small program that extracts the commands and concepts into HTML and add it to the content… or render them is marked down files

`misc.h` has some great macros like `LIMIT_TO()`

If there's an exception (other than a `FatalEmpireException`), catch it and keep
the server up.  When there is an exception, do a stack trace and re-validate
the entire model.

Find a way to create a hybrid Strings table of consteval and defines

With modern CPUs what's more efficient `float` or `double`?

Power is held as a float (at the nation level?)

Place patterns before and after each set of methods and validate them.  We are
looking for buffer overruns.

Static constants and the use of getters.  I’m going to allow consteval static constant to be used directly and not require getters.  This is primarily in support of, optimization.  I believe that direct use of constants makes the code more expressive.

When you login, there should be a login event where the configuration class sends the entire set of configuration to the client

Let’s layer a template within a template.  For example, will use my Singleton, 
and then have it incorporate a standard list or map or array.  

I'm starting to reconsider my `validate()` and `validateThing()` strategy.
Right now, I'm thinking they return strings or lists of strings.  If the 
list (or string) is empty, there's no issue.  This gives the caller more choice
for an exception.  We could also throw a FatalEmpireException if we ever find
one of those..., otherwise return a helpful string.

When a client logs out, the Session object should zero itself out and be
destroyed.  There is no socket attached to it.  All stateful information should
be in Nation.

Have a way of destroying and re-creating a Singleton.  This is for testing purposes only.

I think our namespace strategy is lacking

Hang class UID_Generator off of Core.   It can generate one or contiguous blocks of ids.
Consider creating a serialized number class, a Singleton actually. And then 
assigning a unique ID number to every single object ever created. And that would 
be the primary key for the serialization or marshaling mechanism.
