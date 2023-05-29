Boost
=====

@brief Notes on installing Boost from source

Make sure that the Boost package is NOT installed.  It's OK that package `boost-libs` is installed as it XXXX

The problems we solve by using our own copy of Boost:
  - Arch is 1 rev behind:  `boost-1.81.0-6` 
  - Arch does not ship static libraries
 
The cost is:
  - It burns 226M of space!

````
$ wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz
$ gunzip -c boost_1_82_0.tar.gz | tar -xvf -
$ cd boost_1_82_0
$ ./bootstrap.sh --with-icu --prefix=/opt/boost
$ ./b2
$ ./b2 install
````
This goes in either booststrap or b2: `--without-mpi`

Be sure to remove the build directory as it burns over a gig of space

## Tools

The following Boost libraries have portions that require a separate build
and installation step. Any library not listed here can be used by including
the headers only.

The Boost libraries requiring their own build & installation are:
    - atomic
    - chrono
    - container
    - context
    - contract
    - coroutine
    - date_time
    - exception
    - fiber
    - filesystem
    - graph
    - graph_parallel
    - headers
    - iostreams
    - json
    - locale
    - log
    - math
    - mpi
    - nowide
    - program_options
    - python
    - random
    - regex
    - serialization
    - stacktrace
    - system
    - test
    - thread
    - timer
    - type_erasure
    - url
    - wave
