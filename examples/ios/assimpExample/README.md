#About assimpExample

![Screenshot of assimpExample](assimpExample.png)

###Learning Objectives

This Example demonstrates using the ofxAssimpModelLoader addon to import an animating 3D model in an iOS application.

After studying this example, you'll understand how to do simple imports, and control the rotation direction of your imported 3D asset.

In the code, pay attention to:

* A double-tap will load a different model.
* A drag will rotate the model along the Y axis.
* ```model.setLoopStateForAllAnimations(OF_LOOP_NORMAL)``` placed in ```setup()``` allows the model to play indefinitely.
* ```model.playAllAnimations()``` in ```setup()``` will start that infinite looping.
* ```model.drawFaces()``` in the ```draw()``` will draw the model and its textures at the current animation frame of the loop.

### Expected Behavior

When launching this app, you should see a screen with

* A model walking through the space, centered on screen.

Instructions for use:

* Tapping and dragging will rotate the model in the 3D space.
* Double-tapping will replace the model with another model in the queue.

###Other classes used in this file

This Example uses the following classes for event-based interaction:

* ofxiOS (for compiling openFrameworks to an Apple iOS device)
* ofxAssimpModelLoader (for importing and controlling the 3D model)
