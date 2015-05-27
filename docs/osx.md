[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

OS X
====
To use openFrameworks with OS X, you need to have Xcode installed.  Then you can run the examples in apps/ by opening the .xcodeproj and clicking the "Build and Run" button.


Installing Xcode
----------------
openFrameworks is primarily developed with Xcode 4+, but may work with other versions. 

To install Xcode, you need to register as an Apple developer and download the Xcode disk image from: https://developer.apple.com/xcode/downloads/

More detailed instructions are available at:
http://openframeworks.cc/setup/xcode


Creating a new openFrameworks project
-------------------------------------
The easiest way to create a new openFrameworks project is to use the project generator: 

See [projectGenerator page](projectgenerator.md), to understand the usage of this tool.

The second easiest way is to copy one that's similar to what you want to do. 

OF Examples follow the app directory structure pattern (e.g. apps/categoryName/appName) and are grouped by topic/addon.

For example:

- Copy an example folder like: examples/empty/emptyExample/ and rename the copy to apps/examples/myApp/

- Inside the myApp/ folder rename the .xcodeproj file to myApp.xcodeproj

- Open myApp.xcodeproj

- In the sidebar, under "Targets", right click on "emptyExample" and rename it to "myApp"


The difference between Debug and Release mode
---------------------------------------------
These are two build configurations, "Debug" and "Release".

Debug is useful when developing your project, as it will provide the most information about where and why something crashed.

Release is useful when you're done developing your project. Release will create a smaller, faster app -- but it won't give you much information if it crashes.


openFrameworks resources
------------------------
If you have questions or issues, the best place to look is the openFrameworks forum: 
http://forum.openframeworks.cc/


Thanks for reading this and enjoy!
- The OF Team
