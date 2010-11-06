
// 2 - simple MRT, render same thing simultaneously to multiple textures in fbo

#pragma once

#include "testApp.h"


void testApp::demo2_setup() {
	ofLog(OF_LOG_NOTICE, "demo2_setup | useMSAA: " + useMSAA);
	demoMode = 2;
	
	ofxFbo::Settings s;
	s.width				= kFBOWidth;
	s.height			= kFBOHeight;
	s.numColorbuffers	= 4;
	s.numSamples		= useMSAA ? ofxFbo::maxSamples() : 0;
	fbo.setup(s);
	
	
}


void testApp::demo2_draw() {
	// draw scene into fbo
	fbo.begin();

	// set target buffer to draw into
	vector<GLenum>targetBuffers;
	targetBuffers.push_back(GL_COLOR_ATTACHMENT0_EXT);
	targetBuffers.push_back(GL_COLOR_ATTACHMENT1_EXT);
	targetBuffers.push_back(GL_COLOR_ATTACHMENT2_EXT);
	targetBuffers.push_back(GL_COLOR_ATTACHMENT3_EXT);
	glDrawBuffers(targetBuffers.size(), &targetBuffers[0]);	
	
	drawScene(0, 0, fbo.getWidth(), fbo.getHeight());
	fbo.end();
	
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	
	
	
	// draw fbo textures to screen
	glColor3f(1, 1, 1);
	for(int i=0; i<fbo.getNumTextures(); i++) {
		fbo.getTexture(i).draw(kPreviewX(i), kPreviewY(1), kPreviewWidth, kPreviewHeight);
	}
	
	glColor3f(0, 0, 0);
	ofDrawBitmapString("4 textures attached to FBO, contents are identical, all rendered in single pass", kLabelX(0), kLabelY(1));
}

