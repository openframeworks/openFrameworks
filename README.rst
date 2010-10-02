OpenFrameworks with CMake build files included
==============================================


This repository contains OpenFrameworks project with 
cmake file included.


OpenFrameworks can be compiled as a library using CMake build
files.

Works under linux, Mac OSX support in progress

Instructions
------------

::

  cd libs/openFrameworks

If you want to change options, use cmake-gui

::
  cmake-gui . 
  # check options, modify

Or you can directly genrate Makefile, codeblocks project, Eclipse project, XCode Project:

::
  cmake -G<GeneratorName> .


By default under linux Makefile is generated::
  cmake .
  make 



