
CODEBLOCKS WIZARD FOR WINDOWS INSTALLATION INSTRUCTIONS
-----------------------------------------------------
Python is required to run the createProjects.py python script that is used by this wizard.

Install python:
http://www.python.org/getit/
Do not change the default installation location (i.e. c:\Python26)
This version was tested with Python 2.6.6

Install lxml (the MS Windows installer version)
http://pypi.python.org/pypi/lxml/2.3

Install argparse (Windows installer):
http://pypi.python.org/pypi/argparse/1.1

Copy the "openframeworks" folder in the same directory as this README to the following location:
"C:\Program Files\CodeBlocks\share\CodeBlocks\templates\wizard"

Then do the following:
    1. Open the global registration script for editing at C:\Program Files\CodeBlocks\share\CodeBlocks\templates\wizard\config.script     
    2. Add the following line to the main RegisterWizards() function:    
    RegisterWizard(wizCustom,     _T("openframeworks"),           _T("OpenFrameworks"),          _T("2D/3D Graphics"));
    3. Press CTRL+S (save the file)  
    4. Close and restart CodeBlocks 
    
 The Wizard is now installed. To use it, select File->New->Custom and select the OpenFrameworks template.
 Then fill in your project details (Project name and Project directory). 
 Note that you must create your project in the same directory where you installed openFrameworks.
 The projects should be created in a folder inside the apps directory, such as "myApps".
    
