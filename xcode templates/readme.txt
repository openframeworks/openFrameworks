

== SIMPLE INSTALL == 
1. Make sure XCode is not running (quit if it is)
2. Drag the "Project Templates" folder on the alias named "Drag here to install"


== MANUAL INSTALLATION ==
1. Make sure XCode is not running (quit if it is)
2. Copy the "ofxiPhone Template" folder (inside Project Templates/openFrameworks) to any ONE of the following folders:
just unzip and place the openFrameworks folder in ONE of the following folders creating the folders if nessecary:
/Library/Application Support/Developer/3.0/Xcode/Project Templates/openFrameworks
/Library/Application Support/Developer/Shared/Xcode/Project Templates/openFrameworks
~/Library/Application Support/Developer/3.0/Xcode/Project Templates/openFrameworks
~/Library/Application Support/Developer/Shared/Xcode/Project Templates/openFrameworks

The "SIMPLE INSTALL" steps above simplifies this process



== USAGE ==

1. Launch XCode, and from file menu select New Project (SHIFT + CMD + N).
From the sidebar on the left select the openFrameworks group (under USER TEMPLATES), and then select "ofxiPhone Template".

2. Choose a name and location for the project. 
A folder will be created with the name you choose, also the executable will have the same name and so will the xcode project.


*** The location HAS to be a subfolder of the apps in the related openframeworks working directory. ***

I.e. You need to choose the location:
<folder_where_openframeworks_iphone_is> / apps / <some_folder>

This will create the folder below and place all files in there:
<folder_where_openframeworks_iphone_is> / apps / <some_folder> / <project_name>
e.g.
/Users/Memo/Documents/openFrameworks_iphone/apps/Memo Stuff/Killer App 4
