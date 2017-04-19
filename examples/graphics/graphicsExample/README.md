# About graphicsExample

![Screenshot of graphicsExample](graphicsExample.png)

### Learning Objectives

This example demonstrates a few of the most basic graphics capabilities of openFrameworks. After studying this example, you'll understand how to render some simple colored lines and elementary shapes, such as ellipses and rectangles.

In the code, pay attention to: 

* different commands for setting the active color, such as ```ofSetColor()``` and ```ofSetHexColor()```. These commands set the color for subsequent drawing operations, including lines and fills.
* toggling transparency, with ```ofEnableAlphaBlending()``` and ```ofDisableAlphaBlending()```
* toggling of shape fills, with ```ofFill()``` and ```ofNoFill()```
* toggling the anti-aliasing of lines, with ```ofEnableAntiAliasing()``` and ```ofDisableAntiAliasing()```
* rendering bitmap text to the screen, with ```ofDrawBitmapString()```.

### Expected Behavior

When launching this app, you should see a screen with:

* a filled and unfilled (outline) circle
* a group of 200 randomly-colored, randomly-positioned rectangles
* some transparent and opaque rectangles
* some diagonal red lines
* some bitmap text. 

Instructions for using the app:

* Press ```s``` ("smooth") to toggle whether or not the red lines are anti-aliased. 

### Other classes used in this file

This example uses no other classes.