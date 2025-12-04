[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)


MSYS2
=====

Installing msys2
----------------

First, install MSYS2 using the [one-click installer](https://msys2.github.io/) or
directly unzipping the archive from their [repository](http://sourceforge.net/projects/msys2/files/Base/x86_64/)

If you have an old install of MSYS2 (before 2018), it's recommended to do a fresh install.

If you are going to use QtCreator you should install msys2 in the default install folder, c:\msys64



Now, let's update the MSYS2 installation.
From an MSYS2 shell (it can be MSYS, MINGW32 or MINGW64), run :

```sh
pacman -Syu --noconfirm --needed
```

If some system files are updated, you may be requested to close the shell.
If that happens, close the shell as instructed and open a new one to update the remaining packages using the same command :

```sh
pacman -Syu --noconfirm --needed
```

You are now ready to install openFrameworks.


Installing openFrameworks
-------------------------

**IMPORTANT**
MSYS2 comes in 3 flavors : MSYS (msys2.exe), MINGW32 (mingw32.exe), MINGW64 (mingw64.exe).
This is really important to remember as many problems with running OF with MSYS2 come from using the wrong flavor.
**MINGW32** is the historically supported flavor.
**MINGW64** is still experimental.
So, to run oF, you must use the shell that matches your flavor - mingw32.exe for **MINGW32**, mingw64.exe for **MINGW64**.

For the following instructions, it assumed that MSYS2 is installed in `C:\msys64`.
If it has been installed elsewhere, adapt the instructions to reflect your MSYS2 installation path
It is also assumed that we are working on the 32 bits version.
If you are working with the 64 bits version, adapt the instructions to use a **MING64** shell (`C:\msys64\mingw64.exe`) and, if needed,  use `C:\msys64\mingw64\bin` in your PATH instead of `C:\msys64\mingw32\bin`.

Download and unzip the **qt creator / msys2** version of oF. 
**DO NOT INSTALL** oF in a folder having space or other special character.

Open an **MINGW32** shell (`C:\msys64\mingw32.exe` ) and install OF dependencies:

```sh
cd your_oF_directory/scripts/msys2
./install_dependencies.sh
```

Open an **MINGW32** shell (run `C:\msys64\mingw32.exe`) and compile oF libraries:

    cd your_oF_directory/libs/openFrameworksCompiled/project
    make

You can speed-up compilation using parallel build `make -j4` or the number of cores you want it to use


Setting the PATH variable
-------------------------

Setting the PATH variable is an optional step but is also the cause of many trouble.

~~As of v0.10.1, the install_dependencies.sh script does it in an wrong way!~~

### Why would you need to set the PATH variable ?

__To be able to run my oF application by double clicking on it.__

To run, the application needs to have the dll it was compiled with.
If the required dll is not found at the location of your application, Windows will traverse the folders in your PATH to find it.
If `C:\msys64\mingw32\bin` is included in your PATH, it will hopefully find the right dll.
However, it may find a dll with a matching name in a different folder that is not compatible...  
It may also happen that, after an MSYS2 update, it finds a newer version in `C:\msys64\mingw32\bin` that is also incompatible...

The solution is to copy all the needed dlls in the application folder.
This can be easily done with the command : 

```sh
make copy_dlls
```
This will also ease the installation of the application on a different computer....


__To compile oF in IDE (Qt Creator or VS Code )__

These softwares will try to detect compiler programs (gcc, make) by scanning the PATH variable.
So it's an easy way to setup up your IDE.
There may also be some settings in the IDE to configure where to find the programs.
That gives you better control.
As in the previous point, relying on the PATH variable to find the programs may result in unexpected behaviours (for example, using Windows C:\Windows\System32\find.exe instead of MSYS2 C:\msys64\usr\bin\find.exe)

It may be interesting to write a wrapper batch file to lauch your IDE where you set the PATH to use.
This way you do not pollute your PATH system-wide.

### I've decided to use the PATH variable. How do I set it ?

You can find how to set the PATH in windows here: http://www.computerhope.com/issues/ch000549.htm

You'll need to add `c:\msys64\mingw32\bin` and `c:\msys64\usr\bin` to your PATH in **that order**.
There are two ways:

1. Either add them via 'Environment Variables' from the Control Panel > System > Advanced System Settings.
2. Or you can also set the PATH from the command line: open a Windows cmd prompt and set you user PATH.
```
setx PATH "c:\msys64\mingw32\bin;c:\msys64\usr\bin;%PATH%"
```

Don't forget to logoff/logon as PATH is updated at logon.

That's all, now go to the your_oF_directory/examples folder, where you will find
the examples, and have fun!


Running examples
----------------
Compile the example (for example the 3DPrimitivesExample)

    cd your_oF_directory/examples/3d/3DPrimitivesExample
    make

At this point, `make run` to launch. 

To be able to double-click on the exe file to run it, run `make copy_dlls` (if you haven't set the PATH!)

Makefile
--------

Every example has a Makefile you can configure using the files config.make
and addons.make.

config.make: This file has options to add search paths, libraries, etc., the
syntax is the usual syntax in makefiles, there's help comments inside the file.

addons.make: if you want to use an addon which is inside the addons folder, just
add its name in a new line in this file.


Dlls
----

Windows applications compiled with msys2 need some system dlls to run. If you are going to run the application in a different machine where you don't wont to install msys2 you can run:

    make copy_dlls
	
from the msys2 terminal and it'll copy the necessary dlls to the project bin folder. Now you can copy that bin folder to any computer and it will run right away without having to install msys2.

This is still experimental so if you find any problem please [report it on github](https://github.com/openframeworks/openFrameworks/new?title=Problem%20with%20msys2%20dlls)


QtCreator
---------

With msys2 you can also use QtCreator as an IDE, you can find more information in the corresponding setup guide:

http://openframeworks.cc/setup/qtcreator

FAQ / Common problems
--------------------- 
- "I have a TLSv1_1_client_method missing error" when I double-click the exe ?"

The executable looks for ssleay32.dll and libeay32.dll and it first finds a version that doesn't support TLS v1.1. Often it happens with Intel iCls software. The solution is to move the your_msys2_directory\mingw32\bin path before the conflicting path. If the conflicting path is in the system PATH and you do not have administrative privileges, copy/link ssleay32.dll and libeay32.dll from your_msys2_directory\mingw32\bin to the executable folder.

- "I'm on a corporate network with a proxy. I cannot download packages with pacman."

You may need to set HTTP_PROXY and HTTPS_PROXY environment variables.

    From a DOS/CMD prompt :    
    set http_proxy=http://your_proxy:your_port
    set http_proxy=http://username:password@your_proxy:your_port
    set https_proxy=https://your_proxy:your_port
    set https_proxy=https://username:password@your_proxy:your_port
Don't forget to escape special characters in your password...



 

many thanks!! OFteam

