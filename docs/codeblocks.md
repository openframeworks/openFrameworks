[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Code::Blocks - Windows
======================

We like Code::Blocks for Windows development since it's light weight, and avoids some of the quirkiness of windows visual studio. here's a step by step tutorial to get up and running.  
**Note**: [Additions](#additions) are required to run openFrameworks. Please do not skip this step. 

Installation
------------ 
Follow the steps [here][1].

Additions
---------
1. Download the files  
> [Additions for Code::Blocks to work with openFrameworks][2]

2. In the zip there are two folders, you need to put the **contents** of them into the contents of folders in MinGW.

 * Add the _contents_ of the folder "**add\_to\_codeblocks\_mingw\_include**" into "**C:\\Program Files\\CodeBlocks\\MinGW\\include**" (or wherever your app\\mingw\\include is)
 * Add the _contents_ of the folder "**add\_to\_codeblocks\_mingw\_lib**" into "**C:\\Program Files\\CodeBlocks\\MinGW\\lib**" (or wherever your app\\mingw\\lib is)
    
These are additional libs and header files that need to be added to the MinGW distribution that comes with Code::Blocks. 

Here's a quick video which shows what this should look like: [copy steps for codeblocks][3].

Workspaces
----------

You should open up the workspace that comes with each project: 

`imageSaverExample.cbp`  
`imageSaverExample.workspace` 	**<------ load this**

The workspaces load both the imageSaverExample project as well as the openframeworksLib project, and 
will recompile the openframeworks library as needed.    

### A few points:

1. It's easy to open up multiple projects into the workspace.  remember to close the workspace before working 
with a new project.  If you accidently open up other projects into the workspace, just don't save it. 

2. Within each workspace, it's easy to highlight which project you want to build by clicking it.  
choose the "project", ie, imageSaverExample as opposed to the openframeworks lib.

New project
-----------

To create a new project, you could simply use the projectGenerator. [See here how to](projectgenerator.md).

**As always have fun!**

[1]: http://www.openframeworks.cc/setup/codeblocks
[2]: http://www.openframeworks.cc/content/files/codeblocks_additions.zip
[3]: http://vimeo.com/33985058
