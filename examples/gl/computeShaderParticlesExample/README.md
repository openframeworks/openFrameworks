# computeShaderParticlesExample

![Screenshot of computeShaderParticlesExample](computeShaderParticlesExample.png)

### Learning Objectives

This example demonstrates how to set up a point cloud with editable parameters like attraction, cohesion or repulsion. It uses a shader that computes the movements of each particle according to these parameters. The shader also includes two forces in order to separate the particles into two groups that can be animated differently.

You will learn how to..
* write a shader that animates particles according to their attraction, cohesion or repulsion
* write a shader that creates particle groups by applying different forces
* create lively animations using [`ofNoise()`](http://openframeworks.cc/documentation/math/ofMath/#show_ofNoise)
* use `ofParameter` objects and `ofxGui` to create editable variables
* connect parameters to a shader
* set a blend mode
* easily navigate in 3D with `ofEasyCam`


### Expected Behavior

When launching this app, you should see a cloud of particles floating in the center of a white cube. In the top left corner there is a panel to manipulate the particle cloud. The panel also includes a display of the current framerate.

Instructions for use:

* Use the mouse to adjust the interactive camera
    * Left click drag = orbit the target
    * Right click drag = dolly in and out
	* Double click = reset camera position
* Change the parameters in the panel to manipulate the particle cloud, for example..
	* check "dir as color" to show each particle in a color according to the direction it is heading
	* change `attr1_force` to adjust the force applied to all particles with an odd index
	* change `attr2_force` to adjust the force applied to all particles with an even index

### Other classes used in this file

This Example uses the following classes:

* [ofShader](http://openframeworks.cc/documentation/gl/ofShader/)
* [ofVbo](http://openframeworks.cc/documentation/gl/ofVbo/)
* [ofBufferObject](http://openframeworks.cc/documentation/gl/ofBufferObject/)
* [ofEasyCam](http://openframeworks.cc/documentation/3d/ofEasyCam/)
* [ofParameter](http://openframeworks.cc/documentation/types/ofParameter/)
* [ofParameterGroup](http://openframeworks.cc/documentation/types/ofParameterGroup/)
* [ofxPanel](http://openframeworks.cc/documentation/ofxGui/ofxPanel/)
