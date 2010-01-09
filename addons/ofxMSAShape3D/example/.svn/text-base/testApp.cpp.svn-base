/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 
#include "testApp.h"
#include "ofxMSAShape3D.h"

ofxMSAShape3D myObj;
ofPoint pos;

void testApp::setup(){	 
	ofBackground(0, 0, 0);
	ofSetBackgroundAuto(false);
	
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	
	// select normal blend mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// preallocate space for 5000 vertices.
	myObj.reserve(5000);	
	
	// this is not nessecary but will speed up the addition of the first 5000 glVertex() commands between glBegin / glEnd
	// Even after the first 5000 glVertex() calls, you can still carry on calling glVertex and add new vertices...
	// but the arrays will be resized with every call to glVertex so will be a bit slow. 
	// Best to reserve as much space as you think you'll need up front
	// Its ok to over-reserve thousands cos memory usage is quite low (reserve(1000) allocates 48KB)
	// By default space for 1000 vertices is allocated...
}



void testApp::draw() {
	// clear depth buffer (but not color buffer)
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// enable blending
	glEnable(GL_BLEND);
	
	// choose semi-transparent black color
	myObj.setColor(0, 0, 0, 0.05f);
	
	// draw a black semi-transparent rectangle across whole screen to fade it out a bit
	myObj.drawRect(0, 0, ofGetWidth(), ofGetHeight());
	
	// disable blending
	glDisable(GL_BLEND);
	
	
	// get current time (could use ofGetElapsedTimef() but I DONT want it to be fps independant
	// cos when I'm saving frames and it's running slow I want it to behave the same
	float curTime = ofGetFrameNum() * 1.0f/60.0f;
	
	// choose start rotation based on curTime
	float theta = sin(curTime * 0.17f) * TWO_PI;
	
	// set start position offset from center of screen
	float startOffset = -50.0f * sin(curTime * 0.09f);
	pos.set(ofGetWidth()/2 + cos(theta) * startOffset, ofGetHeight()/2 + sin(theta) * startOffset, 0);
	
	// begin a triangle strip
	myObj.begin(GL_TRIANGLE_STRIP);
	for(int i=0; i<5000; i++) {
		// calculate and set colors
		// RGB components modulate sinusoidally, frequency increases with iteration count
		myObj.setColor(sin(curTime * 0.8f + i * 0.0011f) * 0.5f + 0.5f, sin(curTime * 0.7f + i * 0.0013f) * 0.5f + 0.5f, sin(curTime * 0.3f + i * 0.0017f) * 0.5f + 0.5f);
		
		// do some maths to calculate vertex positions
		// modulate theta (offset rotation) sinusoidally, frequency increases with iteration count
		// use two modulations with different frequencies to create complex harmonic motion 
		theta += sin(curTime * 0.1f + i * 0.00062f) * 2.0f * DEG_TO_RAD * i * 0.0004 + sin(curTime*0.2f + i * 0.0009f) * 3.0f * DEG_TO_RAD;
		float cos_t = cos(theta);
		float sin_t = sin(theta);
		
		// x, y position cumulatively rotates
		// z position modulates sinusoidally, frequency increases with iteration count
		pos += ofPoint(cos_t, sin_t, sin(curTime * 0.5f + i*0.002f) * 0.5);
		
		// send vertex data to myObj
		myObj.addVertex(pos.x, pos.y, pos.z);
		
		// do some math to calculate another vertex position
		// perpendicular to rotation
		// thickness modulates sinusoidally, frequency increases with iteration count
		// also modulation frequency sinusoidally
		ofPoint pos2 = pos + ofPoint(sin_t, -cos_t) * ofMap(sin(curTime * 0.4f * ( 1.0f + i * 0.001f) + i * 0.06f + sin(i*0.001f) * 0.2f), -1, 1, 5, 10 +  i * 0.01f);
		
		// place second vertex (can also pass pointer (array) to coordinates)
		myObj.addVertex2v(pos2.v);
	}
	
	// end vertices and draw to screen
	myObj.end();
}