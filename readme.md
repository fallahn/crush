CRUSH
-----

Crush or be Crushed!

A simple 2D game where the main objective is to crush all of the bad guys either by jumping on them
or by squishing them between two blocks, without getting crushed yourself. Special bonuses are 
awarded for collecting all of the time limited items which appear in the map.


CRUSH is written in C++1y and requires a modern compiler, such as Visual Studio 2013 or clang/llvm

CRUSH depends on SFML 2.x, and is developed using the latest snapshots as they are available. The
CRUSH repository contains a Visual Studio solution, and a CMake file which should provide a good
starting point for various Linuces. CRUSH is released under the zlib license, as it appears in the
source code. Contributions are welcome, particularly when adding platform support to the CMake file.


------Building------

LINUX:
------

CRUSH has been tested with clang/llvm 3.4 and g++ 4.9. To build you first need to install SFML 2.x,
preferably building from the latest source. Then, from the commandline:

git clone path/to/crush.git

cd crush && mkdir build && cd build && cmake ..

make

the CRUSH binary will be compiled and output to the build subdirectory.


WINDOWS:
--------

The repository contains a Visual Studio 2013 Community Edition solution, along with a set of SFML
binaries compiled on Windows 7 64 bit. This is probably the easiest route, although it is only 
included for my personal convenience, and may or may not work for you. The Visual Studio solution
also includes a project for building a level editor, written in C#. The level editor requires the
nuget package for JSON.net to be installed via the nuget manager.

You can also use Cmake-gui 2.8 or newer to create project files for other IDEs such as Code::Blocks,
or unix style make files for mingw. The same limitations apply as the linux build process: you will
need to have either clang 3.4 or g++ 4.9 or higher installed, and a set of SFML binaries for your 
chosen compiler. Clicking Configure in CMake-gui will probably lead to path errors when it cannot 
find the SFML libraries, simply open each entry highlighted in red and browse to the corresponding 
lirary file. Click Configure again, and then Generate if there are no furhter errors to output a 
project file which can be used to build CRUSH.