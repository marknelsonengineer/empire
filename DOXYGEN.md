Doxygen
=======

<img src="images/logo_doxygen_398x77.png" 
     style="height:40px; float: right; margin: 10px 50px 50px 50px;" 
     alt="Doxygen Logo"
/>

@brief Document how [Doxygen] is configured and used

[Doxygen] is the de facto standard for generating documentation from annotated
C++ sources.  Doxygen's website is:  https://www.doxygen.nl

## Notes
- On Linux, I prefer compile my own version of Doxygen.
- On Windows, I install it from a Setup file.
- I've done a lot of experimentation with Doxygen, and I've developed a
  baseline configuration that I really like.
    - I have it as both a Doxyfile and a CMake configuration.
- I need to use [DOT].  We are not using [DIA] which is very old and no longer 
  maintained.
- Here's a great link on [Doxygen]'s graphics capabilities:  Dig 
  into it:  https://alesnosek.com/blog/2015/06/28/diagrams-and-images-in-doxygen


### Installing Graphviz from source
[DOT] is a tool from [Graphviz] that [Doxygen] uses for its drawings.

As a mortal user:
````
    wget https://gitlab.com/api/v4/projects/4207231/packages/generic/graphviz-releases/8.0.1/graphviz-8.0.1.tar.gz
    gunzip graphviz-8.0.1.tar.gz
    tar -xvf graphviz-8.0.1.tar
    cd graphviz-8.0.1
    ./configure
    make
    sudo make install
````

### Installing Clang
As root, run one of the following:
````
    pacman -S clang
    dnf install clang

    wget https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-16.0.0.tar.gz
    gunzip -c llvmorg-16.0.0.tar.gz | tar -xvf -
    cd llvm-project-llvmorg-16.0.0
    cmake -S llvm -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
````


### Compile Doxygen from source on Linux
You may need some of these tools...  As root:
````
    pacman -S llvm    # This is 368MB, but it's only needed to compile DOxygen.  We will remove it after.
    pacman -S xapian-core

    dnf install cmake
````

As a mortal user:
````
     git clone -b Release_1_9_7 https://github.com/doxygen/doxygen
     cd doxygen
     
     mkdir build
     cd build

     cmake -L "Unix Makefiles" -Dbuild_search=ON -Duse_libclang=ON -Dbuild_parse=ON ..
     # ... and look over the build.  If you're OK with it, build it with -G
     
     cmake -G "Unix Makefiles" -Dbuild_search=ON -Duse_libclang=ON -Dbuild_parse=ON ..

     time make -j 3   # Watch your memory and CPU utilization... This needs at least 4G of memory
     time make -j 2   # Worked and took 10 minutes to build with 2G of memory

     make test   # One test failed, but it's OK

     sudo make install
````
As root:
````
     sudo pacman -R xapian-core
     sudo pacman -R llvm    # This is 368MB, but it's only needed to compile DOxygen.  We will remove it after
     sudo pacman -Scc       # Clean the cache
     sudo pacman -Qtd       # List orphan packages
     sudo rm -rf ~/.cache/*   
     sudo fstrim -av 
````

## To update a Doxyfile
Use `doxygen -u Doxyfile` to update an old Doxyfile


## To integrate (and run) Doxygen with Visual Studio
- TODO... I wire it into Tools
- Click Tools > Doxygen

## To integrate (and run) Doxygen with CLion
- It's automatically integrated into [CLion] - [CMake] does all the work
- Select the Doxygen target and build the project.  Don't run it as there's no 
  run target.

## To Run Doxygen from Bash
- `cd` to the directory and simply run `doxygen`

## To Configure Doxygen
- I clone the configuration from an existing app, then pick through the config
  line-by-line until I'm happy with it.  I have a lot of customizations, but
  it looks good the way I publish it.

[Doxygen]:  https://www.doxygen.nl
[Graphviz]:  https://graphviz.org
[DOT]: https://graphviz.org
[DIA]: http://dia-installer.de
[CLion]: https://www.jetbrains.com/clion
[CMake]:  https://cmake.org

## Doxygen Markdown Requirements
- All Doxygen links must work in GitHub
- All Doxygen links must work in public website
- All images must work
- Clang assisted parsing works
  - Clang knows how to find Boost
  - We are using the right version of Clang
