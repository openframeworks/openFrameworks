#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofSetLineWidth(3.0);
	
	vboMode = 0;
	
	// ------------------------- Particles 2d Vbo
	particleCount = 0;
	for(int i=0; i<NUM_PARTICLES; i++) {
		particlePos[i] = 0;
		particleVel[i] = 0;
		particleColor[i].set(1, 0);
		particleTime[i] = ofRandom(1.0);
	}
	particleVbo.setColorData(particleColor, NUM_PARTICLES, GL_STATIC_DRAW);
	particleVbo.setVertexData(particlePos, NUM_PARTICLES, GL_DYNAMIC_DRAW);
	
	
	
	
	// ------------------------- Random fading start 3d Vbo
	for(int i=0; i<NUM_PTS; i++) {
		
		// randomly place the pts
		pts[i].x = ofRandom(-500, 500);	
		pts[i].y = ofRandom(-500, 500);	
		pts[i].z = ofRandom(-500, 500);	
		
		// random give the stars a color
		color[i].r = 1.0;
		color[i].g = 1.0;
		color[i].b = ofRandom(1.0);
		color[i].a = ofRandom(1.0);
		
		fadeSpeed[i] = ofRandom(0.02, 0.03);
		fadeFlip[i]  = false;
	}
	ptsVbo.setVertexData(pts, NUM_PTS, GL_STATIC_DRAW);
	ptsVbo.setColorData(color, NUM_PTS, GL_DYNAMIC_DRAW);
	
	
	
	
	
	// ------------------------- wiggle worm 3d Vbo
	wormDrawMode  = GL_LINE_STRIP;
	int colorFlip = 0;
	for (int i=0; i<WORM_LEN; i++) {
		if(i%33==0)colorFlip = !colorFlip;
		colorFlip ? wormColor[i].setHex(0xffffff) : wormColor[i].setHex(0xff00ff);
		wormColor[i].normalize();
		worm[i].set(ofRandom(10), ofRandom(10), ofRandom(10));
		
		worm[i].x = ofRandom(-1500, 1500);	
		worm[i].y = ofRandom(-1500, 1500);	
		worm[i].z = ofRandom(-1500, 1500);	
	}
	
	wormVbo.setVertexData(worm, WORM_LEN, GL_DYNAMIC_DRAW);
	wormVbo.setColorData(wormColor, WORM_LEN, GL_STATIC_DRAW);
	
	
}

//--------------------------------------------------------------
void ofApp::update() {
	
	// ------------------------- 
	// move the mouse around a toss particles
	// that move around with noise
	if(vboMode == 0) {
		// update the particle pos
		ofVec2f vel(ofGetMouseX()-ofGetPreviousMouseX(), ofGetMouseY()-ofGetPreviousMouseY());
		vel.limit(20.0);
		
		if(vel.length() > 3.0) {
			for (int i=0; i<200; i++) {
				if (particleColor[particleCount].a > 0.2) continue;
				vel.x += ofRandomf();
				vel.y += ofRandomf();
				
				particlePos[particleCount].set(ofGetMouseX(), ofGetMouseY());
				particleVel[particleCount]     = vel;
				particleColor[particleCount].a = 1;
				particleTime[particleCount]    = ofRandom(0.02, 0.4);
				
				particleCount ++;
				particleCount %= NUM_PARTICLES;
			}
		}
		
		
		// now loop through and update the particles
		float div = 250.0;
		for(int i=0; i<NUM_PARTICLES; i++) {
			
			if(particleColor[i].a <= 0.0) continue;
			
			float t = ofGetElapsedTimef() * particleTime[i];
			particleColor[i].a -= 0.008;
			particlePos[i]     += particleVel[i];
			particleVel[i].x   += ofSignedNoise(ofGetElapsedTimef() * 0.4, t, particlePos[i].y/div);
			particleVel[i].y   += ofSignedNoise(particlePos[i].x/div, t, ofGetElapsedTimef() * 0.4) + 0.9f;
			particleVel[i].x   *= 0.89f;
			particleVel[i].y   *= 0.89f;
			
			// wrap the borders
			if(particlePos[i].x > ofGetWidth()) particlePos[i].x = 0;
			else if(particlePos[i].x < 0) particlePos[i].x = ofGetWidth();
			else if(particlePos[i].y > ofGetHeight()) particlePos[i].y = 0;
			else if(particlePos[i].y < 0) particlePos[i].y = ofGetHeight();
		}
	}
	
	
	// ------------------------- 
	// stars fade in and out. a worm
	// moves around randomly
	if(vboMode == 1) {
		// fade the stars out then fade them
		// back in, also give a new random color
		for(int i=0; i<NUM_PTS; i++) {
			
			if(fadeFlip[i]) {
				color[i].a -= fadeSpeed[i];
				if(color[i].a <= 0.0) {
					color[i].r   = ofRandom(0.1, 0.5);
					color[i].g   = 1;
					color[i].b   = ofRandom(0.8, 0.9);
					fadeFlip[i]  = !fadeFlip[i];
					fadeSpeed[i] = ofRandom(0.02, 0.03);
				}
			}
			else {
				color[i].a += fadeSpeed[i];
				if(color[i].a >= 1.0) {
					fadeFlip[i] = !fadeFlip[i];
					fadeSpeed[i] = ofRandom(0.02, 0.03);
				}
			}
		}	
		
		
		float t = (ofGetElapsedTimef()) * 0.1f;
		ofVec3f wormTarget(ofSignedNoise(t, 0, 0),
						   ofSignedNoise(0, t, 0),
						   ofSignedNoise(0, 0, t));
		wormTarget *= 200.0;
		
		// interpolate the second point
		// so that we get feedback through 
		// the rest of the worm
		worm[0] = wormTarget;
		worm[1] = worm[0];
		worm[1].x += ofRandomf();
		worm[1].y += ofRandomf();
		worm[1].z += ofRandomf();
		
		// simple IK for the worm 
		for (int i=2; i<WORM_LEN; i++) {
			ofVec3f vec = worm[i] - worm[i-2];
			float   len = vec.length();
			if(len > 0.0) {
				worm[i] = worm[i-1] + (vec * 2.0) / len;
			}
		}
	}

	
	
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableAlphaBlending();
	
	// ------------------------- 
	// particles
	if(vboMode == 0) {
		ofBackground(0);
		//update the vbo
		particleVbo.setColorData(particleColor, NUM_PARTICLES, GL_STATIC_DRAW);
		particleVbo.setVertexData(particlePos, NUM_PARTICLES, GL_DYNAMIC_DRAW);
		// and draw
		particleVbo.draw(GL_POINTS, 0, NUM_PARTICLES);
	}
	
	// ------------------------- 
	// stars and worm
	if(vboMode == 1) {
		
		ofBackground(0);
		
		// move the camera around
		ofVec2f des(ofGetMouseX(), ofGetMouseY());
		cameraRotation += (des-cameraRotation) * 0.03;
		
		ofPushMatrix();
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 200);
		ofRotate(cameraRotation.x, 1, 0, 0);
		ofRotate(cameraRotation.y, 0, 1, 0);
		ofRotate(cameraRotation.y, 0, 0, 1);
		
		// draw the star
		ptsVbo.setColorData(color,NUM_PTS,GL_DYNAMIC_DRAW);
		ptsVbo.draw(GL_POINTS, 0, NUM_PTS);
		
		
		// the worm
		wormVbo.setVertexData(worm,WORM_LEN,GL_DYNAMIC_DRAW);
		wormVbo.draw(wormDrawMode, 0, WORM_LEN);
		
		ofPopMatrix();
	}
	
	
	// ------------------------- 
	// debug info
	ofSetColor(255, 255, 255);
	string info = ofToString(ofGetFrameRate(), 2)+"\n";
	info += "Particle Size: "+ofToString(NUM_PARTICLES)+"\n";
	info += "Total Stars: "+ofToString(NUM_PTS)+"\n";
	info += "Worm Length: "+ofToString(WORM_LEN)+"\n";
	info += "Press 3-4 for worm Mode\n";
	info += "Press 1-3 for vbo Modes";
	ofDrawBitmapString(info, 30, 30);
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	if(key == 'f') ofToggleFullscreen();
	if(key == '1') vboMode = 0;
	if(key == '2') vboMode = 1;
	if(key == '3') vboMode = 2;
	
	if(key == '3') wormDrawMode = GL_LINE_STRIP;
	if(key == '4') wormDrawMode = GL_POINTS;
		
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

