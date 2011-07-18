
CODEBLOCKS WIZARD FOR LINUX INSTALLATION INSTRUCTIONS
-----------------------------------------------------
Copy the openframeworks folder in this directory to /usr/share/codeblocks/templates/wizard, or alternatively to /usr/local/share/codeblocks/templates/wizard if you have compiled codeblocks from source.
i.e. if you are in the same directory as this README do the following:

cp -r ./openframeworks ~/.codeblocks/share/codeblocks/templates/wizard

then edit the file ~/.codeblocks/share/codeblocks/templates/wizard/config.script and add the following line:
RegisterWizard(wizCustom,     _T("openframeworks"),           _T("OpenFrameworks"),          _T("2D/3D Graphics"));

    
The Wizard is now installed. To use it, select File->New->Custom and select the OpenFrameworks template.
Then fill in your project details (Project name and Project directory). 
Note that you must create your project in the same directory where you installed openFrameworks.
The projects should be created in a folder inside the apps directory, such as "myApps".
    
