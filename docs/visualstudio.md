[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Visual Studio
=============

Installation
------------
To getting started with openFrameworks and Visual Studio, you need to have [Visual Studio 2015](https://www.visualstudio.com) installed.
Any version should work, Express included.  
There are many versions of Visual Studio. If you don't know which one to install, then we recommend [Visual Studio Community 2015](https://www.visualstudio.com/products/visual-studio-community-vs.aspx).
Each generation of Visual Studio (i.e. 2010, 2012, 2015) comes with a different C++ compiler (a compiler is what makes your c++ code into an executable that you can run).
Generally, projects created in different generations of Visual Studio are not compatible with each other, but you can upgrade files from old generations to new generations.  

**See [setup guide](http://openframeworks.cc/setup/vs) for more detailed instructions.**

_Visual Studio Community is free for individual developers, open source projects, academic research, education, and small professional teams.  
Community versions have a reduced feature set whilst sharing the same compiler and general interface of the more premium versions (e.g. Professional, Enterprise etc).
The premium versions are only really recommended for more commercial oF users, for students who can otherwise get these versions for free, or for users who know for definite that they need
the premium features which aren't available in the Community distributions._

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
