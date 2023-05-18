Empire V
========

<img src=".doxygen/images/logo_memscan_660x495.png" style="width:300px; float: left; margin: 0 10px 10px 0;" alt="Tinfoil Hat Cat"/>

Empire is a real time, multiplayer, Internet-based game, featuring military, 
diplomatic, and economic goals.

Empire is the pinnacle of strategy games in today's world. There is none better.

#### Links
The project's home page:  https://github.com/marknelsonengineer-sp23/sre_lab4_memscan  (hosted by GitHub)

The source code is documented at:  https://www2.hawaii.edu/~marknels/sre/memscan  (hosted by The University of Hawaiʻi at Mānoa)

### Empire
Memscan 1 ...

## Usage Notes
- Lorem ipsum



# CMake {#MakeTargets}
Empire uses the following `CMake` targets:

| Command       | Purpose                                                       |
|---------------|---------------------------------------------------------------|
| `make`        | Compile memscan                                               |
| `make test`   | Compile memscan and run it.  Run as `root` to pass all tests. |
| `make debug`  | Compile memscan with debug mode ( `DEBUG` is defined)         |
| `make static` | Statically compile memscan                                    |
| `make clean`  | Remove all compiler-generated files                           |
| `make doc`    | Make a Doxygen website and push it to UH UNIX                 |
| `make lint`   | Use `clang-tidy` to do static analysis on the source code     |

To statically compile memscan, you need to manually build and install libcap.
ArchLinux does not normally ship static libraries.
