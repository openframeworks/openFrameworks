# Slitscan Radial Clock
 dan@buzzo.com
 http://buzzo.com
 https://github.com/danbz
 https://www.youtube.com/danbuzzo

 Description: In this example we use the oF video grabber to sample camera input. Then the code accesses the pixels in each frame from the camera to create a
 slitscan video. The image is created in two styles, progressively across the screen or as a ribbon, pushing pixels sampled from the center of the screen.
 rendered to the screen as a flat, horizontal image or using oftransform to display as a radial image.
 

code in C++ using openFrameworks.cc for timeLapse radial slitscan clock

#Controls

* 1: slitscan ribbon horizontal
* 2: slitscan clock horizontal
* c: toggle debug text and camera preview
* f: toggle fullscreen
* r: toggle drawing anystyle scan as radial clock

 ![screenshot](horizontal-push-ribbon.png)
 Horizontal horizontal-push-ribbon
 ![screenshot](horizontal-clock.png)
 horizontal-clock.png
 ![screenshot](radial-push-ribbon.png)
 radial-push-ribbon
 ![screenshot](radial-clock.png)
 radial-clock.png

### Learning Objectives

This openFrameworks extends the videograbber example is designed to illustrate accessing and manipulating image data from a  webcam.

Studying this example will help you understand:

* how to print a list of available webcam devices (``ofVideoDevice``)
* how to initiate ``ofVideoGrabber``
* how to allocate memory on your graphics card for an ``ofTexture`` or for ``ofPixels``
* how to iterate through a webcam frame's pixels (by accessing its ``ofPixels``)
* how to manipulate the webcam image and read data from an image into another image object
* how to use ofTransform to display image data in various ways
* how to load and use a truetype font

### Expected Behavior

If your computer has a webcam, you should be able to see the webcam image when launching this app on the left and an inverted version of the webcam stream on the right.

### Classes used in this example

This example uses the following classes: 

* ``ofVideoGrabber``
* ``ofPixels``
* ``ofTexture``
* ``ofTrueTypeFont``

