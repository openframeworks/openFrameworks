[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](TOC.md)

Visual Studio
=============

Getting started
---------------

### 1. Select a version of Visual Studio

There are many versions of Visual Studio. If you don't know which one to install, then we recommend Visual C++ 2010 Express, currently available from http://www.microsoft.com/visualstudio/eng/downloads#d-2010-express .

Each generation of Visual Studio (i.e. 2008, 2010, 2012) comes with a different C++ compiler (a compiler is what makes your c++ code into an executable that you can run). Generally, projects created in different generations of Visual Studio are not compatible with each other, but you can upgrade files from old generations to new generations.

Visual Studio Express is a special version of Visual Studio which is free to download and use indefinitely. There appears to be no restriction of commercial use for Visual Studio Express versions, but Express versions have a reduced feature set whilst sharing the same compiler and general interface of the more premium versions (e.g. Professsional, Premium, Ultimate, etc). The premium versions are only really recommended for more commercial oF users, for students who can otherwise get these versions for free, or for users who know for definite that they need the premium features which aren't available in the Express distributions.

Trial versions of the expensive editions of Visual Studio can be downloaded for free trials and registered online. Express versions can be registered online for free (which is necessary for continued usage).

### 2. Install Visual Studio

After selecting a version of Visual Studio that you want to install, then run the installer! If you only want Visual Studio for openFrameworks, then you can de-select languages other than C++ when installing (e.g. deselect Visual Basic, Visual C#, Visual F#, Visual Java if selected) as these will just occupy space on your hard drive.

### 3. Use Project Generator to create your first solution

[where will project generator be? instructions on how to run PG]

At this point, you can select any dependant addons for your project in the left hand list (e.g. if your project needs OpenCV, then select ofxOpenCv). Then select 'create project' in the right hand list. Generally at this point you will select a name for your project and save the project inside openFrameworks/apps/[your name]/[project name].

### 4. Open your new Solution

Every openFrameworks application is represented by a 'Solution' and a 'Project'. A solution is a collection of (generally) co-dependant 'Project's. The solution for your project contains your project, and the openFrameworks project, which is a library of all openFrameworks functions and classes which can be used by your project.

Your solution filename will be openFrameworks/apps/[your name]/[project name]/[project name].sln. Open this file with Visual Studio either by double clicking on it in Explorer, or selecting File>Open>Project/Solution... inside Visual Studio.

### 5. Compile and run 

Open your solution file, and hit F5. This should compile and run your project. You should see a blank OpenGL window appear. 

(If Visual Studio complains that your project cannot be started, try right clicking on your project in the 'Solution Explorer' and select 'Set as startup project' and then try F5 again)

### 6. Change the world

Now it's time to get to work coding with openFrameworks. Hit 'Esc' to exit the blank, boring application you just built, and start editing 'testApp.cpp' and 'testApp.h' to finally put to rest your critics and friends and make that generative/interactive/totally awesome piece of work that you've been wanting to get out. Just be careful not to forget your friends and critics in the process!


Usage notes
-----------
(todo)
* Static projects
* Visual Studio C++ runtime
* Adding headers, libraries
