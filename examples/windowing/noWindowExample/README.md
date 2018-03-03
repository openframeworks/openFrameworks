# About noWindowExample


### Learning Objectives

This example demonstrates how to use openframeworks to run processes without a window. 

In the code, pay attention to: 

* Use of ````	ofAppNoWindow window```` in the main.cpp.
* The fact that no draw commands will be displayed. See ````draw()````


### Expected Behavior

When launching this application you will see the following:

* Openframeworks has no window.
* CommandLine is running as expected.



When the key ````r```` is pressed:

* Attention keystrokes will not have effect!

* the counter is NOT set to 0.
See ````keyPressed()````


Instructions for use:

* Running the application will result in a commandline displaying a constantly increasing number.
* Close the program by pressing ````ESC````


### Other classes used in this file

ofAppNoWindow.