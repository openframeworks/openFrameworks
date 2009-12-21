
OF linux CB: 



1) -------------------------------------------------------- installation

        inside the scripts folder you'll find under ubuntu, debian or fedora some scripts to install everything
        you need to use openframeworks. inside that folders there's a file called install_codeblocks.sh that will 
        install Code::Blocks svn version, please use this script to install codeblocks as some libraries in the 
        new project format used in 006 won't work with codeblocks 8.02, the last stable version present in most
        linux distributions. there's also a script called install_dependencies.sh that will install all the libraries 
        needed to use openFrameworks under linux.

        you will need to run it from a console:

	for ubuntu:
        cd your_oF_directory/scripts/ubuntu
        sudo ./install_codeblocks.sh
        sudo ./install_dependencies.sh

	for debian:
	    su -
	    cd your_oF_directory/scripts/debian
	    ./install_codeblocks.sh
	    ./install_dependencies.sh

	for fedora
	    su -
	    cd your_oF_directory/scripts/fedora
	    ./install_codeblocks_svn.sh
	    ./install_dependencies.sh

        that's all, now go to the apps folder where you will find the examples and have fun!



2) -------------------------------------------------------- ofProjectManager

        this is a set of python scripts that will make your life easier when creating projects 
	or adding addons to existing ones.

	in the scripts folder you will find an install_project_manager.sh file. double click on it 
	to install the project manager.

	this will install python and copy the project manager to /usr/local/ofProjectManager. 
	at the end of then of the process you will be presented with a text file were you need
	to change the directory where your openFrameworks install is. 

    once it's installed, to create a new openFrameworks project, from a console do:

	ofProject create project_name

	or to add an addon to an existing project:

	ofProject add project_name ofxAddonName


a) -------------------------------------------------------- manual install
        
	if you are using a different distribution than ubuntu, debian or fedora, please take a look at one of the 
	install_dependencies.sh scripts from one of the supported distributions to know what libraries you need to 
	install. Also if you have problems or manage to install oF in any other distribution please post in the 
	forums so we can add it to the next release.



b) -------------------------------------------------------- post build steps:
	
	some folks have mentioned trouble with post-build step which copies the export directory into the bin folder.
	if you have issues, please let us know, and esp what linux you are running, what version of CB, etc...
	(please read the readme in export to understand the post build step).
	
	We've modified in this release the script to use "$(PROJECT_DIR)bin" instead of "$(TARGET_OUTPUT_DIR)bin/"
	since this lead to bin/bin on some versions of CB.  It should work now on all versions (fingers crossed).

	one problem occurs if you try to compile OF from a folder you got to in nautilus via a symbolic link:

	error will look like this:

	"cp -r ../../../export/libs /home/zach/Desktop/Link to openframeworks/0.05/0.05b_fat/apps/addonsExamples/allTestExample/bin/
	cp: target `openframeworks/0.05/0.05b_fat/apps/addonsExamples/allTestExample/bin/' is not a directory: No such file or directory
	Process terminated with status 1 (0 minutes, 1 seconds)
	0 errors, 0 warnings"

	so don't open up the project via a symbolic link.  we will report this to CB forum. 


c) -------------------------------------------------------- running app from within CB: 

	from 0.06 this only applies to codeblocks 8.02. if you used the install script or installed the svn version
        manually you shouldn-t need to do this

	
	because of the issue w/ codeblock not being able to run a shell script when you click run, we have a hack :)

	in the post build step, we call a shell script (of_build_runner.sh) and tell it to either
	CD into the directory of the exe and run it, or to launch nautilus file browser to that 
	directory so you can click on the shell scripts.  essentially, we are making build do one of three things:
	
	build = build
	build = build + launch nautilus to the folder where the script/exe is
	build = build + run the exe
	
	this feature is turned off by default.  if you want to turn it on, just try either: 

	a) changing the custom variables in project->build options->custom variables  (make sure you have selected both targets,
	not just debug or release). set either (not both, I think it wont be worth it) to true.

	b) opening up the .cbp file and altering the xml: 
			
			<Environment>
				<Variable name="nautilusOnBuild" value="false" />
				<Variable name="runOnBuild" value="false" />
			</Environment> 

	setting to true.
	
	nautilusOnBuild - pops up the file explorer to the folder where the app is
	runOnBuild - runs the binary with the correct LD_LIBRARY_PATH set, so the app will run

	no more searching around to run apps we hope !!

	in "scripts" we have scripts that can turn these features on or off in all .cbp files in the folder structure. word

d) -------------------------------------------------------- unicap

	from 0.05 we have unicap up and running 

	try ucview to test your camera's capability.  and please report any bugs on the forum!

	if you have any issues with cameras (we've noticed a couple) you can go back to V4L by commenting out a line in ofConstants.h:

	#define OF_SWITCH_TO_UNICAP_FOR_LINUX_VIDCAP

        #ifdef OF_SWITCH_TO_UNICAP_FOR_LINUX_VIDCAP
                #define OF_VIDEO_CAPTURE_UNICAP
         #else
                #define OF_VIDEO_CAPTURE_V4L
        #endif

	just make the first line commented out, like:
	//#define OF_SWITCH_TO_UNICAP_FOR_LINUX_VIDCA


e) -------------------------------------------------------- scripts

	we've created a scripts directory to make everyone life easier, and to get a hang of building / cleaning multiple projects at once.
	also, you can enable or diable running app from within CB via our hacked out build = build + run system.  
	we are hoping that crafty folk will get into scripting, esp. as it relates to addons, etc.  please share any helpful scripts on the forum.

e) -------------------------------------------------------- Makefile

    as so many linux guys have asked for, we've included a Makefile for each example, the makefiles are autogenerated with a script
    to convert codeblocks projects to Makefiles, done by pix, you can find more info and download the latest version of that script 
    here: http://pix.test.at/wiki/cbp2makefile
	
f) -------------------------------------------------------- linux info, advice, etc

	linux version of openFrameworks is in some way different to windows and mac, mainly the video support provided by quicktime doesn-t exist
        here. in 0.06 this is provided with gStreamer for video files and unicap for cameras. although it has been tested, some cameras and video
        formats can pose some issues, if you have some problem please post in the forum as much info as posible.



many thanks!! OFteam




