[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](TOC.md)

Code::Blocks - Windows
======================

We like Code::Blocks for Windows development since it's light weight, and avoids some of the quirkiness of windows visual studio. here's a step by step tutorial to get up and running.  
**Note**: [Additions](#additions) are required to run openFrameworks. Please do not skip this step. 

Installation
------------ 
Follow the steps [here][1].

Additions
---------
Similar to devcpp, we will have to add a few libraries to devcpp. You can download the files:

> [Additions for Code::Blocks to work with openFrameworks][2]


In the zip there are two folders, you need to put the **contents** of them into the contents of folders in MinGW.

*   Add the _contents_ of the folder "**add\_to\_codeblocks\_mingw\_include**" into "**C:\\Program Files\\CodeBlocks\\MinGW\\include**" (or wherever your app\\mingw\\include is)
*   Add the _contents_ of the folder "**add\_to\_codeblocks\_mingw\_lib**" into "**C:\\Program Files\\CodeBlocks\\MinGW\\lib**" (or wherever your app\\mingw\\lib is)
    
These are additional libs and header files that need to be added to the MinGW distribution that comes with Code::Blocks. 

Here's a quick video which shows what this should look like: [copy steps for codeblocks][3].

Workspaces
----------

In 0.061 we've moved to a "compiled" openframeworks.  This means, you should open up the workspace
that comes with each project: 

imageSaverExample.cbp  
imageSaverExample.workspace 	<---------- load this

The workspaces load both the imageSaverExample project as well as the openframeworksLib project, and 
will recompile the openframeworks library as needed.    

### A few points:

1. It's easy to open up multiple projects into the workspace.  remember to close the workspace before working 
with a new project.  If you accidently open up other projects into the workspace, just don't save it. 

2. Within each workspace, it's easy to highlight which project you want to build by clicking it.  
choose the "project", ie, imageSaverExample as opposed to the openframeworks lib.

New project
-----------

1. Copy the folder inside of apps and paste in the same directory

ie (copy "emptyExample" and paste "copy of emptyExample")

2. Rename the folder, and inside the folder, rename the .cbp and .workspace

3. Open the .cbp file with a text editor.  
    * Change the occurance of the old name to the new name:  
    line 5: `<Option title="emptyExample" />`

    * For example, if I copied emptyexample and wanted to call it "coolExample" would look like:  
    line 5: `<Option title="coolExample" />`

    * In the workspace file, change the name of .cbp file:  
    line 4: `<Project filename="emptyExample.cbp" active="1" />`

to whatever you've renamed the cbp to.

Wizard
------

From 007 there's a codeblocks wizard to create projects, you can find instructions
on how to install it in `scripts/win_cb/codeblocks_wizard`.

**As always have fun!**

[1]: http://www.openframeworks.cc/setup/setup/codeblocks
[2]: http://www.openframeworks.cc/content/files/codeblocks_additions.zip
[3]: http://vimeo.com/33985058
