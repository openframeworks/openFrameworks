
OF linux: 

this folder contains necessary components that need to be exported with the application:

libs 
------ contains .so (shared objects) that the app needs to run against
------ this allows OF linux apps to be released in a very portable way
------ users of the apps shouldn't need to install anything besides glut
------ (which should be installed in most cases)

clickToLaunchApp.sh
------ a shell script which runs the exe and set this variable:
------ LD_LIBRARY_PATH, so that the app looks locally for the .so 
------ (shared objects) to run


