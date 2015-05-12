# games
A collection of games written in C++

## Building from source
1. Make sure you have installed the following dependencies:
    * A compiler compliant with the C++11 standard (e.g. `g++` 4.8 or `clang++` 3.x)
    * `cmake`
    * `git`
    
  The only other major dependency is [SFML](http://www.sfml-dev.org/), which is included in the `libs/` directory.
  Users running Windows and Mac can build all the games right out of the box. Users running Linux will need to install
  SFML's dependencies as described [here](http://www.sfml-dev.org/tutorials/2.3/compile-with-cmake.php).

2. Download and build games:

    First, clone the repository (Depending on your network settings, you may have to use the `https` protocol instead of the `git` protocol):
    
        $ git clone git://github.com/fmenozzi/games.git
        $ cd games
    
    Then, create a build directory and run `cmake` to build the build files (if you want to do additional configuration with CMake, run `ccmake ..` instead):
    
        $ mkdir build && cd build
        $ cmake ..
        
    Now, use whatever build system you prefer to actually build the games. Users on Mac and Linux can simply use `make`. 
    Users on Windows can do...whatever the hell Windows users do to build C++. A Visual Studio project? I don't know. (I write
    my C++ on Linux. Sue me.)
    
3. There should now be a slew of executables in your `build` directory, one for each game. Have fun!
