[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Linux
=====

a) Installation
---------------

Inside the scripts folder you'll find some scripts in the ubuntu, debian, fedora 
and archlinux folders which will install everything you need to use openFrameworks.
Inside those folders there's a file called `install_codeblocks.sh` that will install 
a current Code::Blocks version. Please use this script to install Code::Blocks, 
it will avoid problems with some old Code::Blocks versions, but typically your 
distribution's current version will be installed.
There's also a script called `install_dependencies.sh` that will install
all the libraries needed to use openFrameworks under Linux.

You will need to run from a console:

for ubuntu:

    cd your_oF_directory/scripts/linux/ubuntu
    sudo ./install_codeblocks.sh
    sudo ./install_dependencies.sh
    if you want to have support for mp3 and some video codecs:
    sudo ./install_codecs.sh

for debian:

    su -
    cd your_oF_directory/scripts/linux/debian
    ./install_codeblocks.sh
    ./install_dependencies.sh
    if you want to have support for mp3 and some video codecs:
    sudo ./install_codecs.sh
    
for fedora

    su -
    cd your_oF_directory/scripts/linux/fedora
    ./install_codeblocks.sh
    ./install_dependencies.sh
    if you want to have support for mp3 and some video codecs:
    sudo ./install_codecs.sh

for archlinux

    su -
    cd your_oF_directory/scripts/linux/archlinux
    ./install_codeblocks.sh
    ./install_dependencies.sh
    if you want to have support for mp3 and some video codecs:
    sudo ./install_codecs.sh

That's all, now go to the your_oF_directory/examples folder, where you will find 
the examples, and have fun!


b) Other distributions
----------------------

If you are using a different distribution than ubuntu, debian, fedora or arch,
please take a look at one of the `install_dependencies.sh` scripts from
one of the supported distributions to know what libraries you need to install.
Also, if you have problems or manage to install OF in any other distribution
please post in the forums so we can add it to the next release.


c) projectGenerator
-------------------

This is an openFrameworks application that will make your life easier when 
creating projects or adding addons to existing ones. It will be compiled when
you run the install_dependencies.sh script.

To use it open the executable in OF_ROOT/projectGenerator.  You can find more
instructions in that folder too.


d) Code::Blocks Wizard
---------------------- 	  	
You can also install a Code::Blocks wizard to create new projects, check the	
instructions in scripts/linux/codeblocks_wizard.


e) Code::Blocks projects
------------------------

Code::Blocks projects are now using Makefiles to make the configuration easier.
Take a look at the next section to see how it works, to add search paths or 
libraries you need to edit config.make, if you want to add addons, edit addons.make.


f) Makefile
-----------

Every example has a Makefile you can configure using the files config.make
and addons.make.

config.make: This file has options to add search paths, libraries, etc., the 
syntax is the usual syntax in makefiles, there's help comments inside the file.

addons.make: if you want to use an addon which is inside the addons folder, just 
add its name in a new line in this file.


g) Post build steps:
--------------------
    
some folks have mentioned trouble with post-build step which copies the export
directory into the bin folder. if you have issues, please let us know,
and especially what Linux you are running, what version of Code::Blocks, etc...
(please read the readme in export to understand the post build step).

In a previous release, we've modified  the script to use `$(PROJECT_DIR)bin` instead of
`$(TARGET_OUTPUT_DIR)bin/`, since this lead to `bin/bin` on some versions of CB.
It should work now on all versions (fingers crossed).

One problem occurs if you try to compile OF from a folder you reached in nautilus
via a symbolic link:

The error will look like this:

    cp -r ../../../export/libs /home/zach/Desktop/Link to openframeworks/0.05/0.05b_fat/apps/addonsExamples/allTestExample/bin/
    cp: target `openframeworks/0.05/0.05b_fat/apps/addonsExamples/allTestExample/bin/' is not a directory: No such file or directory
    Process terminated with status 1 (0 minutes, 1 seconds)
    0 errors, 0 warnings

So, don't open up the project via a symbolic link. We will report this to CB forum. 


h) Other scripts
----------------

We've created a /scripts directory to make everyone life easier, and to get a
hang of building / cleaning multiple projects at once.
Also, you can enable or diable running app from within CB via our hacked out
build = build + run system.
We are hoping that crafty folk will get into scripting, esp. as it relates to
addons, etc. Please share any helpful scripts on the forum.

    
i) Linux info, advice, etc
--------------------------

The linux version of openFrameworks is in some way different to Windows and MacOS,
mainly the video support provided by quicktime doesn't exist here. From OF 0.06,
this is provided with gStreamer for video files and unicap for cameras.
Although it has been tested, some cameras and video formats can pose some
issues, if you have some problem please post in the forum as much info
as possible.


Libs 
----
The `export/` folder contains necessary components that need to be exported with the application:
contains .so (shared objects) that the app needs to run against this allows OF linux apps to be released in a very portable way  
users of the apps shouldn't need to install anything besides glut (which should be installed in most cases).


many thanks!! OFteam

