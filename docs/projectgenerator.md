[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

projectGenerator
================

About
-----
* The tool can be used to generate project files for the platform you've downloaded (osx xcode, windows codeblocks etc).
* The project generator can add addons to your project, but not all addons (especially non core addons) are packaged in the right way for this.
* It assumes you will want to put your project in `root/apps/myApps`, but you can put your projects anywhere relative to OF, and it should work. Putting your apps in non standard paths (ie, not at a height of `../../../`) is not recommended. 

Usage
-----
It's pretty simple to make an new project, there are 3 things you can adjust: 

1. **Name of the project**  
This is where you set the name of the project. This creates both a folder with this name and a project file, so for example, if your name is "myCoolExample" it will generate: 
`myCoolExample/myCoolExample.xcodeproj` where the myCoolExample will have all the src / settings / etc of an exmpty example. 

2. **Path of the project**  
This defaults to `apps/myApps`, but it should allow you to put projects anywhere. We strongly recommend you to keep them inside this release of OF, so that if the OF release or your project get moved, or if some lower level folder gets renamed, the generated paths don't break. It's much safer to have paths that are `../../../` vs paths that have a name in them. Please note that because we also create a folder with this tool with the name of your project, the actuall full path to your project will look like: 
`chosenPath/projectName/projectName.project` (where chosenPath and projectName are based on your settings, and .project is the xcode, code blocks, visual studio file that's generated).

3. **Addons**  
To use, choose addons in the gui then hit the back button. 
Here you can select the addons you'd like included. Not all addons work, but all of the core addons should and if the addon doesn't work, you can tell the addon developer to take a look at it. The project generator is basically searching the file system recursively through the addons, and it's looking for things like folders that say "osx" on them, etc, so it can make decisions about what to include / exclude, etc. For most simple addons, this should work fairly well, but we haven't tested with many addons yet.
After this release is out, we'll spend some time to document exactly how our system parses, and some of the more intricate nuances that will help addon developers make PG friendly releases. 

4. **Generate**  
When you hit this button, hopefully magic will happen. Some addons take a very long time to parse (such as opencv) so please be patient. If you add alot of addons, it can sometimes take several seconds for a project to generate. 