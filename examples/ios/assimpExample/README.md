# About assimpExample

![Screenshot of assimpExample](assimpExample.png)

### Learning Objectives

This Example demonstrates using the ofxAssimpModelLoader addon to import an animating 3D model in an iOS application, and will demonstrate how to load in 3D animated models and control their animation.

After studying this example, you'll understand how to do simple imports, and control the rotation direction of your imported 3D asset.

In the code, pay attention to:

* the usage of the addon ofxAssimpModelLoader, which is used to load in 3D file types including 3ds, .obj, .dae etc. and provide access to their vertices, textures, materials, and animation.
* how most of the models being loaded in are collada files (.dae) and directX (.x), which can contain the model and animation.
* the difference in appearence between the .3ds model and the .dae and .X models.
* how to control the animation with ```model.playAllAnimations();```and ```model.setPausedForAllAnimations();``` and ```model.setPositionForAllAnimations();``` and
* setting up the lighting with ```light.enable();``` and ```ofEnableSeparateSpecularLight();```

### Expected Behavior

When launching this app, you should see

* A lit 3D model of astroboy.

Instructions for use:

* Tapping and dragging will rotate the model in the 3D space.
* Double-tapping will replace the model with another model in the queue.

### Other classes used in this file

This Example uses the following classes:

* [ofxiOS](http://openframeworks.cc/documentation/ofxiOS/) (for compiling openFrameworks to an Apple iOS device)
* ofxAssimpModelLoader (for importing and controlling the 3D model)
