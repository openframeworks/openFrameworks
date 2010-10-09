OpenFrameworks with CMake build files included
==============================================


This repository contains OpenFrameworks project with cmake file included.


OpenFrameworks can be compiled as a library using CMake build files.

The install command will install static and shared libraries into configured
prefix.

A pkconfig file will be installed to ease further openFrameworks developments 
(pkgconfig openFrameworks --libs --cflags)

Note: PKG_CONFIG_PATH may be updated to the path you choose to install openFrameworks.pc file.
      By default openFrameworks will be installed in '/usr/local' prefix and openFrameworks.pc 
      will be installed in '/usr/local/lib/pkgconfig/' directory. Then you may add the following
      line in your '~/.profile' or '~/.bashrc'

::
    export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH


Works under linux, Mac OSX.


Building and installing OpenFrameworks library
----------------------------------------------

Edit cmake/OpenFrameworksConfig.cmake to adjust search path for includes and libraries

::
  cd libs/openFrameworks


If you want to change options, use cmake-gui or define options directly with -D options
with cmake command.

::
  cmake-gui . 
  # check options, modify



Or you can directly genrate Makefile, codeblocks project, Eclipse project, XCode Project:

::
  cmake -G<GeneratorName> .

By default under linux Makefile is generated

::
  cmake .
  make 


Install openFrameworks library

::
  make install

Building examples
-----------------

For example, to build advancedGraphicExample

::
  cd apps/example/advancedGraphicExample
  mkdir build
  cd build
  cmake ../
  make
  ./advancedGraphicExample


Building addons examples
------------------------

All addons include a cmake include file witch ease compiling apps using addons
for example ofxOpenCv addon includes a 'ofxOpenCv.cmake' witch may be included
in your project CMakeLists.txt.

::
    cd apps/addonsExamples/opencvExample
    mkdir build
    cd build
    cmake ../
    make
    ./openCvExample


Extending
---------

All addons should include a <addonname>.cmake witch can be included in application 
CMakeLists.txt.

For example, if you develop an addon named 'MyCustomAddon', you may follow the followind
directory architecture:

::
  - MyCustomAddon
  - src/
  - MyCustomAddon.cmake

To test your addon you develop a 'MyCustomAddonApplication' with the following direcory layout:


::
 - MyCustomAddonApplication
 - src/testApp.cpp
 - src/main.cpp
 - src/...
 - CMakeLists.txt  <- your cmake build file

And your CMakeLists.txt includes the 'MyCustomAddon.cmake' file like this

::
 ...
 include(<path>/<to>/<addon>/<directory>/MyCustomAddon.cmake

Then your addon may set the following cmake variables:
 - ADDONS_SOURCES: addon source files to compile with you application
 - ADDONS_LIBRARIES: addon external libraries (for example opencv for ofxOpenCvAddon)
 - ADDONS_INCLUDE_DIRS: all required include directories to build your addon an application (for example opencv include dirs)

For more detailed instruction check provided addon cmake files and addonsExample CMakeLists.txt files.







