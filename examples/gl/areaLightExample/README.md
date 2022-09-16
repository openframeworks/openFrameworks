# areaLightExample

![Screenshot of areaLightExample](areaLightExample.png)

### Learning Objectives

This example demonstrates how to create an area light source that is reflected from a plane surface.

You will learn how to..
* easily create an interactive 3D scene with `ofEasyCam`
* create, place and angle a light source
* load a mesh from file and apply a material
* create a reflecting plane and mesh by setting its ambient, diffuse and specular color


### Expected Behavior

When launching this app, you should see a screen showing a white glowing rectangle and a 3D OF logo. The reflection on an otherwise dark surface should react to changes in the camera and light placement.

Instructions for use:

* Use the mouse to adjust the interactive camera
    * Left click drag = orbit the target
    * Right click drag = dolly in and out
	* Double click = reset camera position
* Press the `m` key to change between the 3 modes of the application

### Other classes used in this file

This Example uses the following classes:

* [ofLight](http://openframeworks.cc/documentation/gl/ofLight/)
* [ofPlanePrimitive](http://openframeworks.cc/documentation/3d/ofPlanePrimitive/)
* [ofEasyCam](http://openframeworks.cc/documentation/3d/ofEasyCam/)
* [ofMaterial](http://openframeworks.cc/documentation/gl/ofMaterial/)
* [ofMesh](https://openframeworks.cc//documentation/3d/ofMesh/)
