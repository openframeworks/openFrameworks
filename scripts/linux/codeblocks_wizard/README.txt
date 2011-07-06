
CODEBLOCKS WIZARD FOR LINUX INSTALLATION INSTRUCTIONS
-----------------------------------------------------
Copy the openframeworks folder in this directory to /usr/share/codeblocks/templates/wizard, or alternatively to /usr/local/share/codeblocks/templates/wizard if you have compiled codeblocks from source.
i.e. if you are in the same directory as this README do the following:

sudo cp -r ./openframeworks /usr/share/codeblocks/templates/wizard

Make sure the permissions are correct by running the following in the terminal:
sudo chmod -R 755 /usr/share/codeblocks/templates/wizard/openframeworks  


After starting CodeBlocks follow these steps:
	1. File > New > Project
    2. Click the right mouse button on one of the projects
    3. Select Edit global registration script     
    4. Add the following line to the main RegisterWizards() function:    
    RegisterWizard(wizProject,     _T("openframeworks"),           _T("OpenFrameworks"),          _T("2D/3D Graphics"));
    5. Press CTRL+S (save the file)
    6. Close and restart CodeBlocks 
    
 The Wizard is now installed. To use it, select File->New->Custom and select the OpenFrameworks template.
 Then fill in your project details (Project name and Project directory). 
 Note that you must create your project in the same directory where you installed openFrameworks.
 The projects should be created in a folder inside the apps directory, such as "myApps".
    
