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


