#include "ofApp.h"

#define kMoveInc 10
#define kRotInc 5

// reset all transformations and options to defaults
//--------------------------------------------------------------
void ofApp::reset() {
	camToView = 0;
	camToConfigure = 1;
	
	orbitRadius = 200;
	
	for(int i=0; i<kNumCameras; i++) {
		cam[i].resetTransform();
		cam[i].setFov(60);
		cam[i].clearParent();
		lookatIndex[i] = -1; // don't lookat at any node
		parentIndex[i] = -1; // don't parent to any node
		doMouseOrbit[i] = false;
	}
	
	cam[0].setPosition(40, 40, 190);
	doMouseOrbit[0] = true;
	
	cam[1].setPosition(80, 40, 30);
	lookatIndex[1] = kNumTestNodes-1; // look at smallest node
}


//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableDepthTest();
	ofSetVerticalSync(true);
	ofEnableLighting();
	
	for(int i = 0; i < kNumLights; i++) {
		light[i].enable();
	}
	
	reset();
	
	// link all testNodes (parent to each other)
	for(int i=0; i<kNumTestNodes; i++) {
		if(i>0) testNodes[i].setParent(testNodes[i-1]);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	float freqMult = 1;
	float amp = 30;
	float scale = 1;
	
	// all testNodes move in simple circles
	// but because they are parented to each other, complex paths emerge
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
void ofApp::draw(){
	ofBackground(0);
	
	string s = string("") + 
	"\n" + 
	"Purple boxes (4 of them) are generic nodes with simple circular motion, linked in a hierarchy (with ofNode::setParent).\n" + 
	"Yellow boxes (2 of them) are cameras. You are looking through one of them so can only see one box on screen.\n" + 
	"\n" + 
	"KEYS:\n" + 
	"\n" + 
	"z reset transforms\n" + 
	"\n" + 
	"v switch camera to view: " + ofToString(camToView) + "\n" +
	"\n" + 
	
	"o toggle mouse orbit for cam\n" + 
	
	"\n" + 
	"c switch camera to configure: " + ofToString(camToConfigure) + "\n" +
	" t cycle lookat\n" + 
	" p cycle parent\n" +
	" LEFT pan left\n" + 
	" RIGHT pan right\n" + 
	" UP tilt up\n" + 
	" DOWN tilt down\n" + 
	" , roll left\n" + 
	" . roll right\n" + 
	" a truck left\n" + 
	" d truck right\n" + 
	" w dolly forward\n" + 
	" s dolly backward\n" + 
	" r boom up\n" + 
	" f boom down\n";
	glDisable(GL_CULL_FACE);
	ofSetColor(255);
	ofDisableLighting();
	ofDrawBitmapString(s, ofPoint(20, 20));
	
	glEnable(GL_CULL_FACE);
	ofEnableLighting();
	// update camera transforms
	for(int i=0; i<kNumCameras; i++) {
		
		// lookat node if it has one
		if(lookatIndex[i] >= 0) cam[i].lookAt(testNodes[lookatIndex[i]]);
		
		// mouse orbit camera
		if(doMouseOrbit[i] && ofGetMousePressed(0)) {
			static float lon = 0;
			static float lat = 0;
			
			lon = ofClamp(lon + mouseX - ofGetPreviousMouseX(), -180, 180);
			lat = ofClamp(lat + mouseY - ofGetPreviousMouseY(), -90, 90);
			
			if(lookatIndex[i] < 0) {
				cam[i].orbit(lon, lat, orbitRadius);
			} else {
				cam[i].orbit(lon, lat, orbitRadius, testNodes[lookatIndex[1]]);
			}
		}
		
	} 
	
	// activate camera
	cam[camToView].begin();
	
	
	// draw world axis
	ofDrawAxis(100);
	
	// draw testNodes
	for(int i=0; i<kNumTestNodes; i++) {
		ofSetColor(255, 128, 255);
		testNodes[i].draw();
	}
	
	// draw cameras
	for(int i=0; i<kNumCameras; i++) {
		ofSetColor(255, 255, 0);
		cam[i].draw();
		
		// draw line from cam to its lookat
		if(lookatIndex[i] >= 0) {
			ofSetColor(0, 255, 255);
			ofVec3f v1 = cam[i].getGlobalPosition();
			ofVec3f v2 = testNodes[lookatIndex[i]].getGlobalPosition();
            ofLine(v1,v2);
		}
		
		// draw line from cam to its parent
		if(parentIndex[i] >= 0) {
			ofSetColor(255, 255, 0);
			ofVec3f v1 = cam[i].getGlobalPosition();
			ofVec3f v2 = testNodes[parentIndex[i]].getGlobalPosition();
            ofLine(v1,v2);
		}
	}
	
	// restore view to previous state (default openFrameworks view)
	cam[camToView].end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
			doMouseOrbit[camToConfigure] ^= true;
			break;
			
			// case 'O':
			// doMouseOrbit = false;
			// doAutoOrbit ^= true;
			// break;
			// 
			// // if(cam[camToView].getOrtho()) {
			// // cam[camToView].disableOrtho();
			// // } else {
			// // cam[camToView].enableOrtho();
			// // }
			// break;
			// 
		case 'z':
			reset();
			break;
			
		case 't':
			lookatIndex[camToConfigure]++ ; 
			if(lookatIndex[camToConfigure]>=kNumTestNodes) {
				lookatIndex[camToConfigure] = -1;
				// cam[camToConfigure].disableTarget();
				// } else {
				// cam[camToConfigure].setTarget(testNodes[parentIndex[camToConfigure]]);
			}
			break;
			
		case 'p':
			parentIndex[camToConfigure]++ ; 
			ofVec3f oldP = cam[camToConfigure].getGlobalPosition();
			ofQuaternion oldQ = cam[camToConfigure].getGlobalOrientation();
			if(parentIndex[camToConfigure]>=kNumTestNodes) {
				parentIndex[camToConfigure] = -1;
				cam[camToConfigure].clearParent();
			} else {
				cam[camToConfigure].setParent(testNodes[parentIndex[camToConfigure]]);
			}
			cam[camToConfigure].setGlobalPosition(oldP);
			cam[camToConfigure].setGlobalOrientation(oldQ);
			
			break;
			
	}
	
	printf("\n** MOVING CAMERA %i **\n", camToConfigure);
	printf("POSITION: %f %f %f\n", n->getX(), n->getY(), n->getZ());
	printf("X-AXIS: %f %f %f\n", n->getXAxis().x, n->getXAxis().y, n->getXAxis().z);
	printf("Y-AXIS: %f %f %f\n", n->getYAxis().x, n->getYAxis().y, n->getYAxis().z);
	printf("Z-AXIS: %f %f %f\n", n->getZAxis().x, n->getZAxis().y, n->getZAxis().z);
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}
