[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

MSYS2
=====

Installing MSYS2 
----------------

First, install MSYS2 using the [one-click installer](https://msys2.github.io/) or 
directly unzipping the archive from their [repository](http://sourceforge.net/projects/msys2/files/Base/x86_64/)

Open a MSYS2 shell and update the system packages :

    pacman --noconfirm  --needed -Sy bash pacman pacman-mirrors msys2-runtime
	
Close the shell and open a new one to update the remaining packages :

    pacman --noconfirm -Su

You are now ready to install openFrameworks.	


Installing openFrameworks
-------------------------

Download and unzip the win_cb version of oF.

Open an MSYS shell and install OF dependencies:

    cd your_oF_directory/scripts/win_cb/msys2
    ./install_dependencies.sh`
	
Open an **MINGW32** shell and compile oF libraries:

	cd your_oF_directory/libs/openFrameworksCompiled/project
    make

You can speed-up compilation using parallel build `make -j${NUMBER_OF_PROCESSORS}`

Compile the videoinput lib (This step shouldn't be required for the official 0.9.0 release !)

	pacman --noconfirm  --needed -Sy git
	cd your_oF_directory/scripts/apothecary
	./apothecary update videoinput




Setting the PATH variable
-------------------------
On MSYS2, openFrameworks needs the dlls that are provided by MSYS2 package manager `pacman`. The PATH variable tells the system where to look for these dlls. On Windows, the system starts to look into the executable folder, then into the folders defined in system PATH and finally into the folders defined in user PATH.

Open a Windows cmd prompt and set you user PATH (**WARNING** This will override your current user PATH! Adapt to include other needed paths.)

    setx PATH "your_msys2_directory\mingw32\bin;your_oF_directory\export\win_cb;your_other_paths"

If you have administrative privileges, you can directly set the system PATH. All users will benefit of it...

That's all, now go to the your_oF_directory/examples folder, where you will find 
the examples, and have fun!  

Running examples
----------------
Compile the example (for example the 3DPrimitivesExample)

    cd your_oF_directory/examples/3d/3DPrimitivesExample
    make

At this point, `make run` or  double-click on the exe file to launch. 


Makefile
--------

Every example has a Makefile you can configure using the files config.make
and addons.make.

config.make: This file has options to add search paths, libraries, etc., the 
syntax is the usual syntax in makefiles, there's help comments inside the file.

addons.make: if you want to use an addon which is inside the addons folder, just 
add its name in a new line in this file.

FAQ / Common problems
--------------------- 
- "I have a TLSv1_1_client_method missing error" when I double-click the exe ?"

The executable looks for ssleay32.dll and libeay32.dll and it first finds a version that doesn't support TLS v1.1. Often it happens with Intel iCls software. The solution is to move the your_msys2_directory\mingw32\bin path before the conflicting path. If the conflicting path is in the system PATH and you do not have administrative privileges, copy/link ssleay32.dll and libeay32.dll from your_msys2_directory\mingw32\bin to the executable folder.




 

many thanks!! OFteam

