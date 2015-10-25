[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

MSYS2
=====

a) MSYS2 Installation
---------------------

First, install MSYS2 using the [one-click installer](https://msys2.github.io/) or 
directly unzipping the archive from their [repository](http://sourceforge.net/projects/msys2/files/Base/x86_64/)

Open a MSYS2 shell and update the system packages :

    pacman --noconfirm  --needed -Sy bash pacman pacman-mirrors msys2-runtime
	
Close the shell and open a new one to update the remaining packages :

    pacman --noconfirm -Su

You are now ready to install openFrameworks.	


b) openFrameworks Installation
------------------------------

Download and unzip the win_cb version of oF.

Open an MSYS shell and install OF dependencies:

    cd your_oF_directory/scripts/win_cb/msys2
    ./install_dependencies.sh`
	
Open an **MINGW32** shell and compile oF libraries:

	cd your_oF_directory/libs/openFrameworksCompiled/project
    make

You can speed-up compilation using parallel build `make -j${NUMBER_OF_PROCESSORS}`

Compile the videoinput lib
	pacman --noconfirm  --needed -Sy git
	cd your_oF_directory/scripts/apothecary
	./apothecary update videoinput


That's all, now go to the your_oF_directory/examples folder, where you will find 
the examples, and have fun!

c) Running examples
-------------------
Compile the example (for example the 3DPrimitivesExample)

    cd your_oF_directory/examples/3d/3DPrimitivesExample
    make

At this point, `make run` or  double-clicking on the exe file will fail because of missing Dlls. 
To take care of that, you need to modify your Windows PATH.

- If you have administrative rights

add `your_msys2_directory\mingw32\bin;your_oF_directory\export\win_cb` at the beginning of your system PATH.


- If you don't have admistrative rights

open a Windows cmd prompt and set you user PATH :

    setx PATH "your_msys2_directory\mingw32\bin;your_oF_directory\export\win_cb"
The user PATH will be appended after the system PATH. Then, copy `libeay32.dll` and `ssleay32.dll` to your `example/bin` directory. 

- Create a batch file 
 
Create `3DPrimitivesExample.bat`in the bin directory with the following content
 
    set PATH=your_msys2_directory\mingw32\bin;your_oF_directory\export\win_cb;%PATH%
	3DPrimitivesExample.exe
 



d) projectGenerator
-------------------

TODO


e) Makefile
-----------

Every example has a Makefile you can configure using the files config.make
and addons.make.

config.make: This file has options to add search paths, libraries, etc., the 
syntax is the usual syntax in makefiles, there's help comments inside the file.

addons.make: if you want to use an addon which is inside the addons folder, just 
add its name in a new line in this file.




many thanks!! OFteam

