ofxKinect
=========

Copyright (c) 2010-2013 ofxKinect Team

MIT License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

See https://github.com/openframeworks/openFrameworks/tree/master/addons/ofxKinect for documentation as well as the [OF forums](http://forum.openframeworks.cc/index.php).

This project uses [libfreenect](https://github.com/OpenKinect/libfreenect), copyrighted by the Open Kinect Project using the Apache License v2. See the file "APACHE20" in libs/libfreenect.

Description
-----------

ofxKinect is an Open Frameworks addon for the Xbox Kinect that runs on Mac OSX, Linux, and Windows.
OpenFrameworks is a cross platform open source toolkit for creative coding in C++.

[http://www.openframeworks.cc/](http://www.openframeworks.cc/)

#### NOTE: Xbox Kinect models > 1414 (1473, etc) or Kinect4Windows devices that have been plugged into an XBox will not work with ofxKinect on  Mac OSX

If you have an Xbox Kinect model 1473+ or Kinect4Windows device and want to use it with ofxKinect on OSX, DO NOT plug it into an XBox! If you do, it will receive a firmware update which will cause it to freeze after about 20 secs with libfreenect/ofxKinect on OSX. Linux and Windows are fine. The [fix is in the works](https://github.com/OpenKinect/libfreenect/pull/325).

In the meantime, we suggest you get the *original* Xbox Kinect model 1414 and keep those new toys away from an XBox ...


Running the Example Project
---------------------------

An example project is provided in the `examples/addons/kinectExample` in your OpenFrameworks distribution. If you've downloaded/cloned OF from Github, use the OpenFrameworks ProjectGenerator in `apps/projectGenerator` to generate the Xcode, VS2012, CodeBlocks projects and/or Makefiles by pointing it to the `kinectExample` folder and making sure to include the following addons:

* ofxKinect (duh)
* ofxOpenCv (for blob tracking in the example, not required by ofxKinect itself.)

### OSX

Open the Xcode project, select the "kinectExample Debug" scheme, and hit "Run".

### Linux

Install the libusb-1.0 library. On Ubuntu, you can do this with:
<pre>
sudo apt-get install libusb-1.0-0-dev
</pre>

Note: as of OF 0.8.2 ofxKinect requires libusb 1.0.12 or newer. 
This can be installed by updating or re-installing the libusb-1.0-0-dev package.  

Open the Code::Blocks .cbp and hit F9 to build. Optionally, you can build the example with the Makefile.

To run it, use the terminal:
<pre>
make
cd bin
./example_debug
</pre>

Also, you can add a set of udev rules which allow you to run a Kinect app without root privileges:
<pre>
   sudo cp libs/libfreenect/platform/linux/udev/51-kinect.rules /etc/udev/rules.d
</pre>

### Windows

Precompiled libfreenect drivers and libusb-win32 libs are included for Windows.

New install instructions: 
However on some new Windows OS it might be a struggle to install 

The easiest way to add the drivers is to use the Free USB Driver tool: http://zadig.akeo.ie/
Select your Xbox Camera, Xbox Audio and Xbox Motor in the drop down and then select the libusb-win32( v1.2.6.0 ) driver from the driver menu and click install. ( you will need to do this three times, once for the camera, then motor, then audio ).  This advice comes from the https://github.com/OpenKinect/libfreenect#windows ReadMe. 

Old install instructions: 
Make sure to install or update the libfreenect Kinect camera, motor, and audio drivers through Windows Device Manager by pointing it to the driver folder:
<pre>
ofxKinect/libs/libfreenect/platform/windows/inf
</pre>

You may need to manually update each driver individually if you've plugged it in before. ofxKinect will not work if the drivers are not installed.

**NOTE**: You cannot use the OpenNI drivers and the libfreenect drivers included with ofxKinect at the same time. You must manually uninstall one and reinstall the other in the Device Manager. Sorry, that's just how it is. :P 

How to Create a New ofxKinect Project
-------------------------------------

To develop your own project based on ofxKinect, simply copy the example project and rename it. You probably want to put it in your apps folder, for example, after copying:

`openFrameworks/examples/addons/kinectExample/ => openFrameworks/apps/myApps/kinectProject/`

Then after renaming:

`openFrameworks/apps/myApps/myKinectProject/`

As of OF 0.8.0, you can also create a new ofxKinect project using the ProjectGenerator, found in `openFrameworks/apps/projectGenerator`.

### Mac (Xcode):

Rename the project in Xcode (do not rename the .xcodeproj file in Finder!): Slow double click the project name and rename

### Codeblocks (Win & Linux):

Close Codeblocks and rename the *.cbp and *.workspace files to the same name as the project folder. Open the workspace and readd the renamed project file.

### Windows (Visual Studio):

Close Visual Studio and rename the *.sln file. Open the solution, right click on the project in the project browser and select "Rename ...".

Adding ofxKinect to an Existing Project
---------------------------------------

**As of OF 0.8.0, you can use the OF ProjectGenerator to create ofxKinect projects. The following instructions are for setting up the project files manually.**

If you want to add ofxKinect to another project, you need to make sure you include the src folder and the :
<pre>
openFrameworks/addons/ofxKinect/src
</pre>
as well as search paths for the addons (ofxOpenCv) and libs (freenect, libusb) required by ofxKinect.

Don't forget to include the ofxOpenCv addon sources and opencv libraries.

### Mac (Xcode):

In the Xcode project browser:

* add the ofxKinect sources to the project:
  * create a new group "ofxKinect"
  * drag these directories from ofxKinect into this new group: ofxKinect/src & ofxKinect/libs
* add a search path to the libusb headers: `../../../addons/ofxKinect/libs/libusb/include/libusb-1.0` to your project Target build settings
  * add the lib path to your Project.xconfig, see the example
* add the path to the libusb precompiled library: `../../../addons/ofxKinect/libs/libusb-1.0/lib/osx/usb-1.0.a`
  * add the lib path to your Project.xconfig, see the example
  
### Linux (Makefiles & Codeblocks):

Edit the Makefile config files:
 
* edit addons.make in your project folder and add the following line to the end of the file: 
	<pre>ofxKinect</pre>
* edit config.make in your project folder and change the lines for USER_LIBS to:
	<pre>
	USER_LIBS = -lusb-1.0
	</pre>

#### Windows (Visual Studio):

* add the ofxPd sources to the project:
	* right click on the addons folder in your project tree and create a new filter named "ofxKinect"
	* drag the src directory from the ofxKinect addon folder in Explorer into the ofxKinect folder in your project tree
* add header and library search paths:
	* right lick on your project and select "Properties"
	* choose "Debug" from the "Configurations" drop down box
	* under C/C++->General, add the following to the "Additional Include Directories":
	<pre>
	..\\..\\..\addons\ofxKinect\src
	..\\..\\..\addons\ofxKinect\libs\libfreenect\src
	..\\..\\..\addons\ofxKinect\libs\libfreenect\include
	..\\..\\..\addons\ofxKinect\libs\libfreenect\platform\windows
	..\\..\\..\addons\ofxKinect\libs\libfreenect\platform\windows\libusb10emu\libusb-1.0
	..\\..\\..\addons\ofxKinect\libs\libusb-win32\include
	</pre>
	* under Linker->General, add the following to the "Additional Library Directories":
	<pre>
	..\..\..\addons\ofxKinect\libs\libusb-win32\lib\vs
	</pre>
	* repeat for the "Release" configuration
	
#### Windows (Msys2):

Install libusb using

    pacman --needed -Sy mingw-w64-i686-libusb

	
Notes
-----

### Using multiple Kinects

ofxKinect supports multiple Kinects, however stability is based on the bandwidth of your usb hardware. If you only require the depth image, it is recommended to disable the video grabbing of the rgb/ir images from your devices:
<pre>
kinect.init(false, false);  // disable video image (faster fps)
</pre>

For Kinect4Windows, Microsoft states that only 2 Kinects can be supported on the same USB bus. In practice on OSX, this proves to be the case as, even with the RGB images disabled, there are transfer errors using ofxKinect and 3 Kinects simultaneously. If you need to support many Kinects, you will probably need to add extra USB controllers to your machine …

Developing ofxKinect
--------------------

Feel free to log bug reports and issues to the openFrameworks Github page: https://github.com/openframeworks/openFrameworks
