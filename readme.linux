OF linux CB: 

a) installation
---------------

inside the scripts folder you'll find under ubuntu, debian or fedora some
scripts to install everything you need to use openframeworks. inside that
folders there's a file called `install_codeblocks.sh` that will install
Code::Blocks svn version, please use this script to install codeblocks as
some libraries in the new project format used in 006 won't work with
codeblocks 8.02, the last stable version present in most linux distributions.
there's also a script called `install_dependencies.sh` that will install
all the libraries needed to use openFrameworks under linux.

you will need to run it from a console:

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

that's all, now go to the apps folder where you will find the examples
and have fun!


b) manual install
-----------------

if you are using a different distribution than ubuntu, debian or fedora,
please take a look at one of the `install_dependencies.sh` scripts from
one of the supported distributions to know what libraries you need to install.
Also if you have problems or manage to install oF in any other distribution
please post in the forums so we can add it to the next release.


c) projectGenerator
--------------------

this is an openFrameworks application that will make your life easier when 
creating projects or adding addons to existing ones.

you'll need to compile it before being able to use it. in a terminal:

    cd OF_ROOT/scripts/projectGenerator
    make
    
then copy the application to /usr/bin so you can use it from anywhere:

    sudo cp bin/projectGenerator /usr/bin

to create a new openFrameworks project, or update an already existing from a 
console do:

    cd OF_ROOT/apps    (wherever you have uncompressed OF)
    projectGenerator myApps/projectName

to add addons to a project just add the name of the addon to the addons.make 
file in the root folder of the project and run the projectGenerator again passing
the path to update the codeblocks project


d) wizard
---------- 	  	
you can also install a codeblocks wizard to create new projects, check the	

instructions in scripts/linux/codeblocks_wizard


e) codeblocks
-------------

codeblocks projects are now using Makefiles to make the configuration easier
take a look at the next step: d  to know how it works, the only difference is that
now to add search paths, or libraries you need to edit config.make, if you want 
to add addons use addons.make

you can also run the createProjects.py script with a -n parameter to generate old 
style codeblocks projects

f) Makefile
-----------

every example has a Makefile you can configure it using the files: config.make
and addons.make

config.make: has options to add search paths, libraries... the syntax is the 
usual syntax in makefiles, there's help comments inside the file

addons.make: if you want to use an addon inside the addons folder, just add its
name in a new line in this file.


g) post build steps:
--------------------
	
some folks have mentioned trouble with post-build step which copies the export
directory into the bin folder. if you have issues, please let us know,
and esp what linux you are running, what version of CB, etc...
(please read the readme in export to understand the post build step).

We've modified in this release the script to use `$(PROJECT_DIR)bin` instead of
`$(TARGET_OUTPUT_DIR)bin/` since this lead to `bin/bin` on some versions of CB.
It should work now on all versions (fingers crossed).

one problem occurs if you try to compile OF from a folder you got to in nautilus
via a symbolic link:

error will look like this:

    cp -r ../../../export/libs /home/zach/Desktop/Link to openframeworks/0.05/0.05b_fat/apps/addonsExamples/allTestExample/bin/
    cp: target `openframeworks/0.05/0.05b_fat/apps/addonsExamples/allTestExample/bin/' is not a directory: No such file or directory
    Process terminated with status 1 (0 minutes, 1 seconds)
    0 errors, 0 warnings

so don't open up the project via a symbolic link.
we will report this to CB forum. 


h) scripts
----------

we've created a scripts directory to make everyone life easier, and to get a
hang of building / cleaning multiple projects at once.
also, you can enable or diable running app from within CB via our hacked out
build = build + run system.
we are hoping that crafty folk will get into scripting, esp. as it relates to
addons, etc.  please share any helpful scripts on the forum.

	
i) linux info, advice, etc
--------------------------

linux version of openFrameworks is in some way different to windows and mac,
mainly the video support provided by quicktime doesn-t exist here. in 0.06
this is provided with gStreamer for video files and unicap for cameras.
although it has been tested, some cameras and video formats can pose some
issues, if you have some problem please post in the forum as much info
as posible.



many thanks!! OFteam

