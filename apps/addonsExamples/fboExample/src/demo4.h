
// 4 - pingpong use, render to single texture in fbo, then pingpong between two textures to blur

#pragma once

#include "testApp.h"


void testApp::demo4_setup() {
	ofLog(OF_LOG_NOTICE, "demo4_setup | useMSAA: " + useMSAA);
	demoMode = 4;
	
	ofxFbo::Settings s;
	s.width				= kFBOWidth;
	s.height			= kFBOHeight;
	s.numColorbuffers	= 2;
	s.numSamples		= useMSAA ? ofxFbo::maxSamples() : 0;
	fbo.setup(s);
	
	shader.setup("", "shaders/blur_frag.glsl");	// omit vertex shader
}


void testApp::demo4_draw() {
	// draw scene into fbo
	fbo.begin();
	drawScene(0, 0, fbo.getWidth(), fbo.getHeight());
	fbo.end();
	
	// draw fbo to screen
	glColor3f(1, 1, 1);
	fbo.draw(kPreviewX(1), kPreviewY(0), kPreviewWidth, kPreviewHeight);
	
	glColor3f(0, 0, 0);
	ofDrawBitmapString("FBO contents", kLabelX(1), kLabelY(0));
	
	
	// ping pong between two attachments using shader
	fbo.begin();
	shader.begin();
	glColor3f(1, 1, 1);

	for(int i=0; i<8; i++) {
		int srcPos = i % 2;				// attachment to write to
		int dstPos = 1 - srcPos;		// attachment to read from
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + dstPos);	// write to this texture
		ofClear();

		shader.setUniform1i("tex0", 0);
		shader.setUniform1f("sampleOffset", i*2+1);
		fbo.getTexture(srcPos).draw(0, 0);
	}
	
	shader.end();
	fbo.end();
	
	
	// draw blurred fbo to screen
	// draw fbo textures to screen
	glColor3f(1, 1, 1);
	for(int i=0; i<fbo.getNumTextures(); i++) {
		fbo.getTexture(i).draw(kPreviewX(i), kPreviewY(1), kPreviewWidth, kPreviewHeight);
	}
	
	
	glColor3f(0, 0, 0);
	ofDrawBitmapString("2 textures attached to FBO, used for ping pong with shader (blur)", kLabelX(0), kLabelY(1));
}

