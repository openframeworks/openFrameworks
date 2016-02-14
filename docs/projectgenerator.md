[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

projectGenerator
================

About
-----
* The tool can be used to generate project files for the platform you've downloaded (osx Xcode, Windows, etc).
* The project generator can add addons to your project, but not all addons (especially non core addons) are packaged in the right way for this.
* It assumes you will want to put your project in `OF_ROOT/apps/myApps`, but you can put your projects anywhere relative to OF, and it should work. Putting your apps in non standard paths (ie, not at a height of `../../../`) is not recommended.  Also putting your apps outside of the OF folder itself is not recommended, since that makes the relative path system more fragile. 

*NOTE: If you are on linux, be sure to run the install_dependencies.sh script, which will install necessary components for this.*


Changes
-----
This project generator is new since 0.9 -- the original one in earlier OF releases only allowed you to make new projects, this one allows for updating existing projects as well as recursive updating.  


Usage
-----
It's pretty simple to make an new project, or update an exisiting project, in the simple mode. there are 3 things you can adjust: 

1. **Name of the project**  
This is where you set the name of the project. This creates both a folder with this name and a project file, so for example, if your name is "myCoolExample" it will generate: 
`myCoolExample/myCoolExample.xcodeproj` where the myCoolExample will have all the src / settings / etc of an empty example. 

2. **Path of the project**  
This defaults to `apps/myApps`, but it should allow you to put projects anywhere. We strongly recommend you to keep them inside this release of OF, so that if the OF release or your project get moved, or if some lower level folder gets renamed, the generated paths don't break. It's much safer to have paths that are `../../../` vs paths that have a name in them. Please note that because we also create a folder with this tool with the name of your project, the actual full path to your project will look like: 
`chosenPath/projectName/projectName.project` (where chosenPath and projectName are based on your settings, and .project is the xcode, code blocks, visual studio file that's generated).

3. **Addons**  
To use, choose addons you want to add, and/or remove ones you don't want.  Not all addons work with the PG, but all of the core addons should and if the addon doesn't work, you can tell the addon developer to take a look at it. The project generator is basically searching the file system recursively through the addons, and it's looking for things like folders that say "osx" on them, etc, so it can make decisions about what to include / exclude, etc. For more complex addons, you may need to read the documentation or reach out to the addon author for more info. Also note that not all addons are designed for every platform.

4. **Generate**  
When you hit this button, hopefully magic will happen. Some addons take a very long time to parse (such as opencv) so please be patient. If you add a lot of addons, it can sometimes take several seconds for a project to generate.  You should see a success message with the option of opening the folder or opening the project in an IDE.

Settings
-----

In the settings panel, you can update the path to the root of openframeworks.  This is the folder that is the base level, which contains "libs", "addons", etc.  There are also options for advanced mode (see below) and verbose output.  Verbose output runs the project generator w/ verbose flag, which gives a ton of details about how its adding an addon and what it's doing.


Advanced
-----

Since 0.9 the project generator features an advanced mode, which gives you additional functionalities.  You can choose other platforms for cross platform development (Note: this is useful only if you are working from the master branch or have those other templates available).  Also, it can recursively update projects, useful if you are working with a folder full of project files that you want to test in a newer version of OF. 

Technical considerations
-----

This project generator uses two tools -- a command line tool and an gui front end.  The command line tool is very powerful, and in later releases we will expose this more for the end users to install locally on their machines, etc since it is really helpful for advanced users.  For now, if you poke around the PG folders, you can find this tool (it's in slightly different locations on osx, windows and linux).  On linux this is compiled and installed for you.   This command line tool is wrapped in a gui front end which uses electron (http://electron.atom.io/).   