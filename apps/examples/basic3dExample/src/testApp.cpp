#include "testApp.h"
#include "ofCamera.h"
#include "ofMeshNode.h"


#define kNumTestNodes	4
#define kMoveInc		10
#define kRotInc			2


ofMeshNode	testNodes[kNumTestNodes];
ofCamera	cam[2];
int			targetIndex[2];		// which test node to target (one for each camera)

int			camToView = 0;
int			camToConfigure = 1;

bool		doMouseOrbit = false;
float		orbitRadius = 100;

bool		doAutoOrbit = false;


void reset() {
	cam[0].resetTransform();
	cam[0].setFov(60);
	cam[0].setPosition(60, 40, 190);
	targetIndex[0]	= -1;	// don't look at any target
	
	cam[1].resetTransform();
	cam[1].setFov(60);
	cam[1].setPosition(130, 70, -30);
	targetIndex[1]	= kNumTestNodes-1;	// look at smallest target
}


//--------------------------------------------------------------
void testApp::setup(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHT0);
	
	ofEnableLighting();
	
	reset();
	
	for(int i=0; i<kNumTestNodes; i++) {
		if(i>0) testNodes[i].setParent(&testNodes[i-1]);
	}
}

//--------------------------------------------------------------
void testApp::update(){
	float freqMult = 1;
	float amp = 30;
	float scale = 1;
	
	for(int i=0; i<kNumTestNodes; i++) {
		testNodes[i].setPosition(ofVec3f(sin(ofGetElapsedTimef() * freqMult) * amp, cos(ofGetElapsedTimef() * freqMult) * amp, sin(ofGetElapsedTimef() * freqMult * 0.7) * amp));
		
		testNodes[i].setOrientation(ofVec3f(sin(ofGetElapsedTimef() * freqMult * 0.2) * amp * 5, cos(ofGetElapsedTimef() * freqMult * 0.2) * amp * 5, sin(ofGetElapsedTimef() * freqMult * 0.2 * 0.7) * amp * 5));
		testNodes[i].setScale(scale);
		
		freqMult *= 3;
		amp *= 0.8;
		scale *= 0.8;
	}
}


//--------------------------------------------------------------
void testApp::draw(){
	
	// look at target if has target
	if(targetIndex[0] >= 0) cam[0].lookAt(testNodes[targetIndex[0]]);
	if(targetIndex[1] >= 0) cam[1].lookAt(testNodes[targetIndex[1]]);
	
	// mouse orbit camera
	if(doMouseOrbit) {
		if(targetIndex[1] < 0) {
			cam[1].orbit(ofMap(mouseX, 0, ofGetWidth(), 180, -180), ofMap(mouseY, 0, ofGetHeight(), 90, -90), orbitRadius);
		} else {
			cam[1].orbit(ofMap(mouseX, 0, ofGetWidth(), 180, -180), ofMap(mouseY, 0, ofGetHeight(), 90, -90), orbitRadius, testNodes[targetIndex[1]]);
		}
	}
	
	// auto orbit camera
	if(doAutoOrbit) {
		if(targetIndex[1] < 0) {
			cam[1].orbit(ofGetElapsedTimef(), 50, orbitRadius);
		} else {
			cam[1].orbit(ofGetElapsedTimef(), 50, orbitRadius,  testNodes[targetIndex[1]]);
		}
	}
	
	ofEnableLighting();
	
	// activate camera
	cam[camToView].begin(ofGetWindowRect());
	
	
	// draw world axis
	float axisLength = 100;
	// draw world x axis
	ofSetColor(255, 0, 0);
	glPushMatrix();
	glTranslatef(axisLength/2, 0, 0);
	glScalef(axisLength, 2, 2);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
	
	// draw world y axis
	ofSetColor(0, 255, 0);
	glPushMatrix();
	glTranslatef(0, axisLength/2, 0);
	glScalef(2, axisLength, 2);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
	
	// draw world z axis
	ofSetColor(0, 0, 2550);
	glPushMatrix();
	glTranslatef(0, 0, axisLength/2);
	glScalef(2, 2, axisLength);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
	
	for(int i=0; i<kNumTestNodes; i++) {
		testNodes[i].draw();
	}
	
	
	ofSetColor(255, 255, 0);
	cam[0].debugDraw();
	cam[1].debugDraw();
	
	// draw line from cam2 to its target
	if(targetIndex[1] >= 0) {
		ofSetColor(255, 255, 255);
		glBegin(GL_LINES);
		ofVec3f v1 = cam[1].getGlobalPosition();
		ofVec3f v2 = testNodes[targetIndex[1]].getGlobalPosition();
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glEnd();
	}
	
	cam[camToView].end();
	
	ofSetColor(0, 0, 0);
	string s = string("") + 
	"\n" + 
	"Purple boxes (4 of them) are generic nodes with simple circular motion, linked in a hierarchy (with ofNode::setParent).\n" + 
	"Yellow boxes (2 of them) are cameras. You are looking through one of them so can only see one box on screen.\n" + 
	"\n" + 
	"KEYS:\n" + 
	"\n" + 
	"z      reset transforms\n" + 
	"\n" + 
	"v      switch camera to view: " + ofToString(camToView) + "\n" +
	"\n" + 
	
	"o      toggle mouse orbit for cam1\n" + 
	"O      toggle auto orbit for cam1\n" + 

	"\n" + 
	"c      switch camera to configure: " + ofToString(camToConfigure) + "\n" +
	" t      cycle target\n" + 
	" LEFT   pan left\n" + 
	" RIGHT  pan right\n" + 
	" UP     tilt up\n" + 
	" DOWN   tilt down\n" + 
	" ,      roll left\n" + 
	" .      roll right\n" + 
	" a      truck left\n" + 
	" d      truck right\n" + 
	" w      dolly forward\n" + 
	" s      dolly backward\n" + 
	" r      boom up\n" + 
	" f      boom down\n";
	
	ofDisableLighting();
	ofDrawBitmapString(s, ofPoint(20, 20));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	ofNode *n = &cam[camToConfigure];
	
	switch(key) {
		case 'c':
			camToConfigure = 1 - camToConfigure;
			printf("\n\n** MOVING CAMERA %i **\n", camToConfigure);
			break;
			
		case 'v':
			camToView = 1 - camToView;
			printf("\n\n** SHOWING CAMERA %i **\n", camToView);
			break;
			
			
		case OF_KEY_LEFT: n->pan(kRotInc); break;
		case OF_KEY_RIGHT: n->pan(-kRotInc); break;
			
		case OF_KEY_UP: n->tilt(-kRotInc); break;
		case OF_KEY_DOWN: n->tilt(kRotInc); break;
			
		case ',': n->roll(kRotInc); break;
		case '.': n->roll(-kRotInc); break;
			
		case 'a': n->truck(-kMoveInc); break;
		case 'd': n->truck(kMoveInc); break;
			
		case 'w': n->dolly(-kMoveInc); break;
		case 's': n->dolly(kMoveInc); break;
			
		case 'r': n->boom(kMoveInc); break;
		case 'f': n->boom(-kMoveInc); break;
			
		case 'o':
			doMouseOrbit ^= true;
			doAutoOrbit = false;
			break;
			
		case 'O':
			doMouseOrbit = false;
			doAutoOrbit ^= true;
			break;
			
//			if(cam[camToView].getOrtho()) {
//				cam[camToView].disableOrtho();
//			} else {
//				cam[camToView].enableOrtho();
//			}
			break;
			
		case 'z':
			reset();
			break;
			
		case 't':
			targetIndex[camToConfigure]++ ; 
			if(targetIndex[camToConfigure]>=kNumTestNodes) targetIndex[camToConfigure] = -1;
			break;
	}
	
	printf("\n** MOVING CAMERA %i **\n", camToConfigure);
	printf("POSITION: %f %f %f\n", n->getX(), n->getY(), n->getZ());
	printf("X-AXIS: %f %f %f\n", n->getXAxis().x, n->getXAxis().y, n->getXAxis().z);
	printf("Y-AXIS: %f %f %f\n", n->getYAxis().x, n->getYAxis().y, n->getYAxis().z);
	printf("Z-AXIS: %f %f %f\n", n->getZAxis().x, n->getZAxis().y, n->getZAxis().z);
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	static float px = -1;
	if(doMouseOrbit) {
		if(px>=0) orbitRadius += x - px;
		px = x;
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

