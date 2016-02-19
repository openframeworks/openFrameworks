#About firmataExample

![Screenshot of firmataExample](firmataExample.png)


###Learning Objectives

This Example demonstrates communication with Arduino using the Firmata protocol.

Running this example requires uploading Firmata to your Arduino. With your Arduino connected, open Arduino and navigate to File -> Examples -> Firmata and open StandardFirmata. Compile and upload StandardFirmata for your board before running this application.

This example covers the following functions:

* Connecting to an Arduino and setting port and baud rate
* Sending and receiving digital and analog signals
* Adding and removing event listeners on the Arduino port

In the code, pay attention to: 

* Different arguments for setting the poly winding type, such as ```ofSetPolyMode(OF_POLY_WINDING_NONZERO)``` and ```ofSetPolyMode(OF_POLY_WINDING_ODD)```.
* Methods for making closed and open polygons, using ```ofEndShape()``` and ```ofEndShape(OF_CLOSE)```
* Use of the struct ```draggableVertex``` in the ofApp.h
* Changing colors with ```ofSetHexColor()```
* Creating holes and constructive geometries with ```ofNextContour()```
* Toggling of shape fills, with ```ofFill()``` and ```ofNoFill();```.
* Rendering bitmap text to the screen, with ```ofDrawBitmapString()```.

### Expected Behavior

When launching this app, you should see a screen with an image of an Arduino and a breadboard. 

While the Arduino is not connected, 

###Instructions for use:

* Drag the vertex points of the ofCurveVertex polygon (f) to adjust its size.

###Other classes used in this file

This Example uses no other classes.