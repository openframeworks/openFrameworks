
// 1 - simple use, render to single texture in fbo, render that to screen with shader

#pragma once

#include "testApp.h"


void testApp::demo1_setup() {
	ofLog(OF_LOG_NOTICE, "demo1_setup | useMSAA: " + useMSAA);
	demoMode = 1;
	
	fbo.setup(kFBOWidth, kFBOHeight, GL_RGBA, useMSAA ? ofxFbo::maxSamples() : 0);
	shader.setup("", "shaders/blur_frag.glsl");	// omit vertex shader
}


void testApp::demo1_draw() {
	// draw scene into fbo
	fbo.begin();
	drawScene(0, 0, fbo.getWidth(), fbo.getHeight());
	fbo.end();

	
	// draw fbo to screen
	glColor3f(1, 1, 1);
	fbo.draw(kPreviewX(1), kPreviewY(0), kPreviewWidth, kPreviewHeight);
	
	glColor3f(0, 0, 0);
	ofDrawBitmapString("FBO contents", kLabelX(1), kLabelY(0));

	
	// draw fbo straight to screen with shader
	// single pass, simple use
	shader.begin();
	fbo.getTexture(0).bind();
	shader.setUniform1i("tex0", 0);
	shader.setUniform1f("sampleOffset", 10);
	fbo.draw(kPreviewX(0), kPreviewY(1));
	fbo.getTexture(0).unbind();
	shader.end();
	
	
	glColor3f(0, 0, 0);
	ofDrawBitmapString("FBO contents drawn straight to screen with blur shader (one pass)", kLabelX(0), kLabelY(1));
}

