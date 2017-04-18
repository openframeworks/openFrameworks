# About dragDropExample

![Screenshot of Example, stored as exampleName/screenshot.png (or .gif or .jpg)](dragDropExample.png)


### Learning Objectives

This example demonstrates how to enabled and capture the dragging and dropping of files into the application window.  

In the code, pay attention to: 

* Use of capturing the ```dragEvent``` to handle drag related events
* Inspecting information about the drag event using ```ofDragInfo``` to determine what file(s) were dragged. 
* Use of ```position``` on ```ofDragInfo``` to determine where on the window the file was dropped.



### Expected Behavior

When launching this application you will see:

* A gray background with text instructing you to drag an image file. 

When an image file is dragged onto the application and then dropped the application: 

* captures that event with the dragEvent 
* inspects the information about the drag event to find the images that were dropped along with the position of the drop
* load in the image in the position it was dropped. 


Instructions for use:

* Drag and drop an image file ( png or jpg ) onto the application window. 


### Other classes used in this file

This Example uses no other classes.