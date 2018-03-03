# About allEventsExample

![Screenshot of allEventsExample](allEventsExample.png)


### Learning Objectives

This Example demonstrates the keyboard and mouse event of openFrameworks.

After studying this example, you'll understand how to trigger events and retrieve information from events such as mouse behavior, cursor position, which mouse button is pressed or what key is pressed.

In the code, pay attention to:

* Different commands to access the current time information: ```ofGetTimestampString()``` and ```ofGetElapsedTimeMillis()```
* Access individual key information: ```keyReleased(int key)```
*									 ```keyPressed(int key)```
* Access mouse information: ```mouseMoved(int x, int y )```
*							```mouseDragged(int x, int y, int button)```
*							```mousePressed(int x, int y, int button)```
*							```mouseReleased(int x, int y, int button)```
* Access information about the window size when resizing: ```windowResized(int w, int h)```


### Expected Behavior


When launching this app, you should see a screen with

* a text display of the current time, the elapsed time in milliseconds, the current event being triggered


Instructions for use:

* Press  keys to see how their corresponding labels are displayed. Manipulate the mouse to see how different mouse events are triggered.


### Other classes used in this file

This Example uses no other classes.
