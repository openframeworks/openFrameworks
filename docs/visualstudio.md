[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Visual Studio
=============

Installation
------------
To getting started with openFrameworks and Visual Studio, you need to have Visual Studio 2012 installed.
Any version should work, Express included.  
See [setup guide](http://openframeworks.cc/setup/vs) for more detailed instructions.

Creating a new openFrameworks project
-------------------------------------
See [projectGenerator page](projectgenerator.md), to understand the usage of this tool.

Open your new Solution
----------------------
Every openFrameworks application is represented by a **'Solution'** and a **'Project'**. A solution is a collection of (generally) co-dependent projects. 
The solution for your project contains your project, and the openFrameworks project, which is a library of all openFrameworks functions and classes 
which can be used by your project.  
Your solution filename will be `openFrameworks/apps/[your name]/[project name]/[project name].sln`. Open this file with Visual Studio either by double 
clicking on it in Explorer, or selecting File>Open>Project/Solution... inside Visual Studio.

Compile and Run 
---------------
Open your solution file `[project].sln`, and hit **F5**. This should compile and run your project. You should see a blank OpenGL window appear.  
_If Visual Studio complains that your project cannot be started, try right clicking on your project in the 'Solution Explorer' and select 'Set as startup project' and then try F5 again._

The difference between Debug and Release mode
---------------------------------------------
These are two build configurations, **"Debug"** and **"Release"**:

* **Debug** is useful when developing your project, as it will _provide the most information_ about where and why something crashed.
* **Release** is useful when you're done developing your project. Release will create a _smaller_, _faster_ app, but it _won't give you much information_ if it crashes.

Change the world
----------------
Now it's time to get to work coding with openFrameworks. Hit 'Esc' to exit the blank, boring application you just built, and start editing `testApp.cpp` 
and `testApp.h` to finally put to rest your critics and friends and make that generative/interactive/totally awesome piece of work that you've been wanting 
to get out.  
Just be careful not to forget your friends and critics in the process!

openFrameworks resources
------------------------
If you have questions or issues, the best place to look is the [openFrameworks forum](http://forum.openframeworks.cc/).


Thanks for reading this and enjoy!  
_The OF Team_
