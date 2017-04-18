# About simpleTimer

![Screenshot of simpleTimer](simpleTimer.png)


### Learning Objectives

This example demonstrates how to use the built-in OF event ```gotMessage```



In the code, pay attention to:

* ```ofSendMessage``` To send a message that will get caught by ```gotMessage```
* ```void gotMessage``` OF event



### Expected Behavior


* At the center of the window, a progress bar with progress percentage value displayed on it's right side.
* At the top left corner information about the timer linked to the progress bar updated in real time: FPS, Start Time, End Time, Timer, Percentage. A message: "Press ' ' to get a new random end time".
* When the progress bar / timer ends, a message is sent and intercepted by ```gotMessage```.


Instructions for use:

* Press spacebar or click on mouse button to start progress bar with a random timing.


### Other classes used in this file

This Example uses no other classes.
