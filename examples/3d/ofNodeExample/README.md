# About ofNodeExample

![Screenshot of ofNodeExample](ofNodeExample.jpg)

### Learning Objectives

 This tutorial shows how to use the ofNode to build a hierarchy of objects, it was
 originally developed in a workshop held by Arturo Castro at Lacuna Lab.

 Most 3D classes in OF inherit from ofNode. ofNode is simply a class that defines
 a point in a 3D space and allows it to:
 - Chain a point with another point
 - Apply matrix transformations to it

 We will never define an ofNode directly in this example, that's why you will
 not find the word `ofNode` in the code but both ofLight and ofBoxPrimitive
 inherit from ofNode, allowing us to access all its powerful methods.

In this example we want to create a car with 2 headlamps, and we want to move the lights together with the car, without re-calculating the position of the 2 lights.

We define only how the car should move into the space, the lights just need to stay on the front of the car, and their movement is just a consequence of the movement of the car.

Have a look at the Car.cpp file to see how we attach the lights on the front of the car and how to move the object using the handy rotate, move and tilt methods - without needing to use ofPushMatrix and ofPopMatrix.

In the example, pay attention to:

* The 3D movement methods in Car.cpp that make use of ofNode parent/child relationships
* The application and use of 3D materials like `roadMaterial`

### Expected Behavior

When launching this app, you should see a screen with:

* A 3D scene with a grey plane/floor
* A green cube with 2 "headlights" attached to the front of it
* Should be able to rotate the scene with your mouse (click and drag to rotate or pan, or zoom by scrolling)

Instructions for using the app:

* Click and drag (or scroll) in the window to move the 3D Scene. Double click to reset the camera.
* Use your keyboard arrow keys to move your "car" around the scene


### Other classes used in this file

* ofEasycam
* ofLight
* Car.h (custom class for the example)
