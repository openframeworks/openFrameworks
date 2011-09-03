

1) installing Codeblocks:
--------------------------------------------------------------------------------

follow the steps here:
http://www.openframeworks.cc/setup/setup/codeblocks

** be sure to follow step e ** 

download this zip:

http://www.openframeworks.cc/content/files/codeblocks_additions.zip
and add the includes / libs to the corresponding includes / libs folder in mingw.


2) workspaces
--------------------------------------------------------------------------------

In 0.061 we've moved to a "compiled" openframeworks.  This means, you should open up the workspace
that comes with each project: 

imageSaverExample.cbp
imageSaverExample.workspace 	<---------- load this

the workspaces load both the imageSaverExample project as well as the openframeworksLib project, and 
will recompile the openframeworks library as needed.    

a few points:

a) it's easy to open up multiple projects into the workspace.  remember to close the workspace before working 
with a new project.  If you accidently open up other projects into the workspace, just don't save it. 

b) within each workspace, it's easy to highlight which project you want to build by clicking it.  
choose the "project", ie, imageSaverExample as opposed to the openframeworks lib.



3) if you want to make a new project:
--------------------------------------------------------------------------------

a) copy the folder inside of apps and paste in the same directory

ie (copy "emptyExample" and paste "copy of emptyExample")

b) rename the folder, and inside the folder, rename the .cbp and .workspace

c) open the .cbp file with a text editor.  Change the occurance of the old name to the new name:

line 5: <Option title="emptyExample" />

for example, if I copied emptyexample and wanted to call it "coolExample" would look like:

line 5: <Option title="coolExample" />

in the workspace file, change the name of .cbp file:

line 4: <Project filename="emptyExample.cbp" active="1" />

to whatever you've renamed the cbp to.



4) codeblocks wizard
--------------------------------------------------------------------------------

From 007 there's a codeblocks wizard to create projects, you can find instructions
on how to install it in scripts/win_cb/codeblocks_wizard.
	
	


as always if you have issues, check or post on the forum:  openframeworks.cc/forum

have fun! 





