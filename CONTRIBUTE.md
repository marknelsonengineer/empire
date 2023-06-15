Contribute to Empire
====================

@brief Document the development environment, code conventions, et al.

Empire V is a [CMake]-based C++, POSIX usermode console program.

## Directories
| Directory                                   | Description                                                                                          |
|---------------------------------------------|------------------------------------------------------------------------------------------------------|
| `./src`                                     | All Empire V source code.                                                                            |
| `./src/lib`                                 | There's a need for common functionality between all of the Empire programs, so let's make a library. |
| `./test`                                    | For our Boost Test code.                                                                             |
| `./.doxygen`                                | The configuration files and scripts for auto generating documentation with [Doxygen].                |
| `./.doxygen/generated_docs/html/index.html` | The root of the [Doxygen]-generated documentation.                                                   |
| `./images`                                  | All images for the project.                                                                          |


## C++ Naming Conventions

https://lefticus.gitbooks.io/cpp-best-practices/content/03-Style.html

- C++ files are suffixed with `.cpp` and header files with `.hpp`
- Types start with upper case: `MyClass`
- Functions and variables start with lower case: `myMethod`
- Constants are all upper case: const double `PI=3.14159265358979323;`
- Macro names use upper case with underscores: `INT_MAX`
- Template parameter names use camel case: `InputIterator`
- All other names use snake case: `unordered_map`

Name private data with a `m_` prefix to distinguish it from public data. `m_` stands for "member" data.

Prefixes:
`m_` for members
`c_` for constants/readonlys
`p_` for pointer (and pp for pointer to pointer)
`s_` for static

Variable names:
`i`, `j` & `k` for indexes and iterators


## CMake
Empire uses [CMake] for its build system.  It's thoroughly modern, 
multi-platform, multi-compiler aware.  It also works well with [CLion], my IDE.

[CMake] has a steep learning curve.  It's taken me years to get the hang of it.
In fact, it's taken several years of on-and-off development to get it to do all
the things I'd like it to do: Compile, generate Doxygen documentation, Lint 
code, publish documentation, etc.  Our CMakeLists.txt file is definitely
non-trivial.

Our build environment is complex, so I'll try to document it here.  The first
concept is the idea of a `CMAKE_BUILD_TYPE`.  Think of a Build Type as a specific
set of tools.  For example:
  - A default compiler with debug flags enabled
  - A linting tool that uses `clang-tidy`

The second concept is targets.  Targets are created by processing a set of source 
files through a Build Type.

Empire uses the following `CMake` Build Types:

| Build Type   | Description                                                                                           |
|--------------|-------------------------------------------------------------------------------------------------------|
| `Debug`      | A default compiler with debug enabled and optimization disabled.                                      |
| `Release`    | A default compiler with no debugging.  The target is optimized for speed and uses static executables. |
| `MinSizeRel` | A default compiler with non non-static libraries.  The target is optimized for size.                  |
| `Clang`      | Compile with the `clang++` compiler.                                                                  |
| `Doxygen`    | Configure the clang compiler.  Use [Doxygen] to build the documentation using the clang parser.       |
| `Lint`       | Use `clang-tidy` to do static analysis on the source code.                                            |

### Design Decisions
ArchLinux doesn't ship static libraries with its packages.  Therefore, we have
to build our Boost package from scratch.  It's in `/opt/boost`.  This allows
us to compile Empire as a static executable.


### CMake Targets
Empire uses the following `CMake` targets:

| Target                | Description                                      |
|-----------------------|--------------------------------------------------|
| `All_Boost_Tests`     | The [Boost Test] target                          |
| `doxygen_local_build` | Make a local [Doxygen] website                   |
| `doxygen_publish`     | Push the [Doxygen] website to a public webserver |
| `empire`              | The common Empire V library                      |
| `empire_client`       | A command-line client for Empire V               |
| `empire_server`       | The Empire V server                              |


#### Issues
Not sure how to wire in `Valgrind`  Run All_Boost_Tests using [valgrind] and report any leaks 

There's actually a 3rd type of target: A CLion configuration.  I've created 
a configuration called `valgrind` that runs against All_Boost_Tests.


## Setup the development environment
To set up your development environment on a fresh system, you'll likely need 
the following:
- This section is incomplete, but...
  - ...but it worked on my ArchLinux rig!
- After [Git] is installed, you can use `git clone https://github.com/marknelsonengineer/empire` 
  to download memscan

| Package            | Debian / apt-get                     | ArchLinux / pacman       | AWS ARM-64                        | RedHat / rpm |
|--------------------|--------------------------------------|--------------------------|-----------------------------------|--------------|
| [Git]              |                                      |                          | `# dnf install git`               |              |
| [C++]              |                                      |                          | `# dnf install g++`               |              |
| [Boost]            | `# apt-get install libboost-all-dev` |                          | `# dnf install boost-devel`       |              |
| [clang-tidy]       | `# apt-get install clang-tidy`       |                          | `# dnf install clang-tools-extra` |              |
| [Graphviz] for DOT | `# apt-get install graphviz`         |                          | `# dnf install graphviz`          |              |
| [Python pip]       | `# apt-get install python3-pip`      | `# pacman -S python-pip` | `# dnf install python3-pip`       |              |
| [GitPython]        | `$ pip install GitPython`            |                          | `$ pip install GitPython`         |              |
| [Doxygen]          | `# apt-get install doxygen`          |                          | `# dnf install doxygen`           |              |
| [Valgrind]         | `# apt-get install valgrind`         |                          | `# dnf install valgrind`          |              |


If you plan to contribute, don't forget to set your name and eMail in [Git]:

    git config --global user.name "Mark Nelson"
    git config --global user.email "marknels@hawaii.edu"


ArchLinux does not normally ship `.a` files.  To statically link memscan, we
need to manually install `libcap`.  

    git clone https://git.kernel.org/pub/scm/libs/libcap/libcap.git
    cd libcap
    make
    make test
    make sudotest
    sudo make install


## Declaration order

Google favors this order: "Typedefs and Enums, Constants, Constructors, Destructor, Methods, including static methods, Data Members, including static data members."

Matthew Wilson (Safari subscription required) recommends the following order: "Construction, Operations, Attributes, Iteration, State, Implementation, Members, and my favorite, Not to be implemented."

They offer good reasons, and this kind of approach seems to be fairly standard, but whatever you do, be consistent about it.


## Handling errors & warnings in a testing framework
- Empire has a dedicated logger.
- Messages and exceptions should be pre-pended with getProgramName and sent 
  to `stderr`.
- Instrument programs to make them easily to test.  This is easier with C++ 
  programs that have `try`/`catch` blocks and exceptions.  As a C program, 
  we have created some tools for test instrumentation:
  - `PRINT( outStream, ... )`:  Print a message
  - `WARNING( msg, ... )`:  Print a formatted warning message
  - `ASSERT( condition )`:  Assert a condition.  Call `exit()` when the 
     condition is not `true`
  - `FATAL_ERROR( msg, ... )`:  Print a message and call `exit()`
    
- Use ASSERT() for things that should never happen.
- Use FATAL_ERROR() for things that may happen and we need to tell the user
  why the program is ending.
- Functions may return `bool` if the local logic needs to check the results.  In
  most cases, however, functions should call FATAL_ERROR().
- Successful functions should operate silently.

Memscan uses FATAL_ERROR() and ASSERT() for error checking.  Normally,
these functions will call `exit( EXIT_FAILURE )`, however when `TESTING` is 
defined, they will call throwException() so [BOOST_CHECK_THROW] can catch it.

Furthermore, when an unexpected call to FATAL_ERROR() or ASSERT() happens,
the [Boost Test] framework can catch the exception rather than immediately
`exit()`.

This utility function is a unique hybrid of C and C++ -- as C does not support
exceptions.

Using this technique gives me a works/doesn't work test, but we loose
fidelity in that we don't get a reason code or message.  I'm willing to
accept that as I'd like to focus on writing a traditional command-line
program in C.  Bare-bones tests will tell us if something works or doesn't
but not _why_.


## Toolchain
- Memscan is written in C.
  - It's compiled with [gcc].
  - Use `make test` to test the program.  [Boost Test] is the test framework.
  - Use `make valgrind` to test for memory leaks using [valgrind].
  - Use `make lint` to lint the program.  [clang-tidy] is the linter.
  - Use `make doc` to build documentation:  [Doxygen] is used to produce it.
  - Use `make publish` to publish the documentation to [UHUnix].
  - It uses [Makefile] for its build.
  - It uses [GitHub] for version control.
  - All of the above should run clean and without warnings.
- Memscan uses [Boost Test] written in [C++].  Use `make clean` and 
  `make test` to test the application.  The tests have special flags,
  so always `make clean` first.
- Memscan is a Linux-based, user-mode command-line program that needs to run
  with root-level privileges.
- Memscan was written in CLion, using `Makefile` based build.  Note:  CLion 
  usually uses a CMake based build.

  
## Coding Conventions
- I intend to let [CLion] reflow the source code
- Put 2 blank lines between functions
- Put a space before the `;`.  Ex. `return true ;`
- Memscan uses `printf` but we may convert to something that can be tested 
  with the [Boost test] framework
- For enums, the last element should be called COUNT and has the number of elements in the enum.


## Naming Conventions
- I use [CaMel case] and [snake_case] interchangeably, depending on my mood.
- File names and the functions that are in them represent the names of the
  files they are processing.  Ie. `iomem`, `maps`, `pagemap`, et. al..
- Function names start with a lowercase letter.
- Structures, Enums, et al. start with an uppercase letter.
- Constants and macros are in all UPPERCASE.
- I'm not consistently using [Hungarian notation], but you might see it from
  time to time.

[CaMel case]: https://en.wikipedia.org/wiki/Camel_case
[snake_case]: https://en.wikipedia.org/wiki/Snake_case


## Documentation Conventions
- Fully document "published" functions in `.h` files.  Leave internal 
  documentation in the `.c` files.
- If there's a code-block that's longer than a page, I'll usually comment the 
  `}` with an indication of what it's closing.  Ex. `} // validate()`
- Markdown files should use [references] and avoid embedding URLs in the
  narrative.


## Testing Notes
I need to work on this section as well.  Memscan is severely test deficient.  I'd
like to sweep through the code and make a bunch of unit and system tests
for it.

`make test` needs to run as `root`

One cool thing about memscan is that it's a C program that is tested by a C++
unit test framework [Boost Test](https://www.boost.org/doc/libs/1_81_0/libs/test/doc/html/index.html).  
It's cool to have this working.


### Test Results
|                   | Archlinux | Debian on WSL                                | AWS ARM-64  |
|-------------------|-----------|----------------------------------------------|-------------|
| Architecture      | x86-64    | x86-64                                       | ARM-64      |
| Date tested       | Ongoing   | 27 Mar 2023                                  | 7 Apr 2023  |
| Build tested      | Ongoing   | 2.1.0+10332                                  | 2.3.0+12146 |
| `make memscan`    | Clean     | Clean                                        | Clean       |
| `make doc`        | Clean     | A few warnings, as Doxygen is out of date    |             |
| `make test`       | Clean     | Clean                                        | Clean       |
| `make valgrind`   | Clean     |                                              |             |
| `make lint`       | Clean     | Some warnings as `clang-tidy` is out of date | Clean       |
| `make static`     | Clean     |                                              |             |
| local allocations |           |                                              | Not working |
| iomem             |           |                                              | Not working |

#### AWS ARM-64
- The physical page maps are all reporting `Unallocated memory`.  It's like iomem or PFNs 
  aren't lining up.
- Local variables are relative to `sp` (the stack pointer) not the base pointer, so 
  local allocations are segfaulting badly.


## Release Procedures
- For each source file:
  - Cleanup `#include` files
    - Create `scratch.c`, then `#include` each `.h` file and ensure they are
      self-sufficient:
      - Ensure they have the `#include`s to stand alone.
      - Remove any unnecessary `#include`s.
      - Ensure the functions `#include`d are documented.
  - `const` correctness
    - Look for `const`-able parameters
  - Look for any API calls and document them with `@API{}`
    - Man pages come from:  https://man.archlinux.org
    - Some calls have multiple man pages; reference system calls (2) first, 
      then the Standard C API (3) and lastly the POSIX API (3p)
  - Read the Doxygen content for each source file and Markdown file
  - Check for a space before `;`
    - Use `clear ; grep --color -n "[[:graph:]];" *.c *.h tests/*.h tests/*.cpp`
    - ... and Find Regular Expression in CLion:  `\p{Graph};`
  - Ensure all parameters are validated
  - `assert( validateThing() )` before-and-after anything that modifies it
  - Look for CLion problems
  - Ensure each `NOLINT` is still relevant and valid
- Run `make clean` and then `make lint`
- Run `make clean` and then `make test`
- Run `make clean` and then `make valgrind`
- Run `make doc` and then `make publish`
- Scrub GitHub issues
- Scrub all `@todo`s
- Create a tagged release in Git
- Use https://tinypng.com to shrink the size of our .PNG files

# Constants

For review:
`const var` Means the variable won't change after it's initialized. Variables may be initialized at runtime, though.
`method() const` Means the method won't change the state of the object
`constexpr` Means "to be evaluated at compile time".  Applies to variables.
`consteval` Declares a function or template to produce a compile time constant expression.  It forces calls to happen at compile-time.
`constinit` Initializes a static variable at compile time.  It does not imply `const` nor `constexpr`.



## Source Code Status

| Module                           | const correctness | include correctness | Review Doxygen | 32-bit   | Boost tests   | API documented |
|----------------------------------|-------------------|---------------------|----------------|----------|---------------|:---------------|
| `Singleton`                      |                   |                     |                |          |               |                |


- (1):  getPageInfo() has comprehensive unit tests.  comparePages() needs some tests, but I don't want to do them right now.  The other 
        routines print things to the screen and don't lend themselves to unit testing.  I intend to implement system tests to exercise this code.
- (2):  See the following notes for the iomem module:
  - release_iomem() and release_iomem_summary() are validated through [valgrind]. 
  - getEnd() is pretty fundamental... it's also simple.  If this did not work, then lots of other stuff break.  I don’t want to expose 
    (Whitebox) regions, so I'm not going to directly test this.
  - print_iomem_regions() is intended for debugging.  It's not output by any of memscan's options.  It's also used to visualize tests.
  - The heart of the iomem module, get_iomem_region_description() and add_iomem_region() are thoroughly tested.
  - read_iomem() gets most of its testing through bulk tests.  It does file I/O and parsing and that's pretty well tested.
  - print_iomem_summary() and summarize_iomem() are not systematically tested.  Everything underneath them are.
  - compose_iomem_summary() and sort_iomem_summary() are tested with validate_summary() which is called frequently in all the tests and 
    at key places in the iomem module.

[Hungarian notation]: https://en.wikipedia.org/wiki/Hungarian_notation
[clang-tidy]: https://releases.llvm.org/13.0.0/tools/clang/tools/extra/docs/clang-tidy/
[Doxygen]: https://www.doxygen.nl
[UHUnix]: https://www2.hawaii.edu/~marknels/sre/memscan/index.html
[Makefile]: https://www.gnu.org/software/make/manual/make.html
[GitHub]: https://github.com/marknelsonengineer-sp23/sre_lab4_memscan
[Boost Test]: https://www.boost.org/doc/libs/1_81_0/libs/test/doc/html/index.html
[Boost]: https://www.boost.org/
[BOOST_CHECK_THROW]: https://www.boost.org/doc/libs/1_81_0/libs/test/doc/html/boost_test/utf_reference/testing_tool_ref/assertion_boost_level_throw.html
[references]: https://www.doxygen.nl/manual/markdown.html#md_reflinks
[Capabilities]: https://man7.org/linux/man-pages/man7/capabilities.7.html
[Python pip]:  https://pypi.org/project/pip/
[GitPython]:  https://gitpython.readthedocs.io/en/stable/
[Graphviz]:  https://graphviz.org
[Git]:  https://git-scm.com
[C++]:  https://gcc.gnu.org
[gcc]:  https://gcc.gnu.org
[valgrind]:  https://valgrind.org
[CLion]:  https://www.jetbrains.com/clion
[CMake]:  https://cmake.org
