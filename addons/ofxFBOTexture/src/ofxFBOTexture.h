#pragma once

/**********************************

	Zach Gage, Theo Watson, Memo Akten

	supports:
	- alpha
	- custom Field-of-view
	- float textures
	- half-float textures
	- multi-sampling
	- OpenGL and OpenGL ES (e.g. iPhone)
	- rendering to surfaces (e.g. iPhone)



	Usage:

	ofxFBOTexture myFBO;

	myFBO.allocate(640, 480);
	// or
	myFBO.allocate(640, 480, GL_RGBA32F_ARB); // float texture
	// or
	myFBO.allocate(640, 480, GL_RGBA16F_ARB); // half-float texture
	// or
	myFBO.allocate(640, 480, GL_RGBA, 4); // 4x multi-sampling (can also use float textures)


	// draw into FBO
	myFBO.begin();
	ofCircle(30, 30, 50);
	myFBO.end();


	// draw FBO to full screen
	myFBO.draw(0, 0, ofGetWidth(), ofGetHeight());


***********************************/


#include "ofMain.h"

class ofxFBOTexture : public ofTexture {
public:
	ofxFBOTexture();
	
	float screenFov;
	
	void allocate(int w, int h, int internalGlDataType = GL_RGBA, int numSamples = 0);
	void swapIn();
	void swapOut();
	
	void setupScreenForMe();
	void setupScreenForThem();
	
	void begin() {
		swapIn();
		setupScreenForMe();
	}
	
	void end() {
		swapOut();
		setupScreenForThem();
	}
	
	void clear();
	void clear(float r, float g, float b, float a);
	
	void bindAsTexture();
	
	
protected:
	bool		_isActive;
	GLuint      fbo;                                 // Our handle to the FBO
	GLuint		mfbo;
	GLuint      depthBuffer;                        // Our handle to the depth render buffer
	GLuint      colorBuffer;                        // Our handle to the color render buffer
	GLint		oldRenderbuffer, oldFramebuffer;	// old buffers, needed if rendering to surfaces
	
	bool        autoClear;
	void        clean();
	float       clearColor[4];
	
	int			numSamples;
};
