OpenFrameworks with CMake build files included
==============================================


This repository contains OpenFrameworks project with 
cmake file included.


OpenFrameworks can be compiled as a library using CMake build
files.

The install command will install static and shared libraries into configured
prefix.

A pkconfig file will be installed to ease further openFrameworks developments 
(pkgconfig openFrameworks --libs --cflags)

Works under linux, Mac OSX support in progress

Instructions
------------

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




By default under linux Makefile is generated::
  cmake .
  make 


Install openFrameworks library

::
  make install (as root depending where you want to install it)







