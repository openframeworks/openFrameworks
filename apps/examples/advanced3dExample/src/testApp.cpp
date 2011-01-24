#include "testApp.h"

ofNode target;
ofRectangle rect;
bool oldMousePress = false;
ofVec3f mousePrev;

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetVerticalSync(true);
	ofBackground(70, 70, 70);
	glEnable(GL_DEPTH_TEST);
	
//	glEnable(GL_CULL_FACE);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_COLOR_MATERIAL);
	
	
	/////////////////////
	// SETUP CAMERAS
	/////////////////////	
	//
	iCurrentCamera = 1;
	
	// user camera
	camEasyCam.setTarget(nodeSwarm);
	cameras[0] = &camEasyCam;

	
	// front
	camFront.scale = 20;
	cameras[1] = &camFront;
	
	// top
	camTop.scale = 20;
	camTop.tilt(-90);
	cameras[2] = &camTop;
	
	// left
	camLeft.scale = 20;
	camLeft.pan(-90);
	cameras[3] = &camLeft;
	
	//
	/////////////////////
	
	/////////////////////
	// DEFINE VIEWPORTS
	/////////////////////	
	//
	setupViewports();
	//
	/////////////////////	
	
	/////////////////////
	// SETUP SWARM
	/////////////////////	
	//
	nodeSwarm.init(100, 50, 20);
	//
	/////////////////////	
	
}

//--------------------------------------------------------------

void testApp::setupViewports()
{
	//call here whenever we resize the window
	
	/////////////////////
	// DEFINE VIEWPORTS
	/////////////////////	
	//
	float xOffset = ofGetWidth()/3;
	float yOffset = ofGetHeight()/4;
	
	viewMain.x = xOffset;
	viewMain.y = 0;
	viewMain.width = xOffset * 2;
	viewMain.height = ofGetHeight();
	
	for (int i=0; i<4; i++) {
		
		viewGrid[i].x = 0;
		viewGrid[i].y = yOffset * i;
		viewGrid[i].width = xOffset;
		viewGrid[i].height = yOffset;
	}
	//
	/////////////////////	
}

//--------------------------------------------------------------
void testApp::update(){

}


//--------------------------------------------------------------
void testApp::draw(){
	
	//////////////////////////
	// BACKGROUND HIGHLIGHT
	//////////////////////////
	//
	glDisable(GL_DEPTH_TEST);
	ofPushStyle();
	ofSetColor(100, 100, 100);
	ofRect(viewGrid[iCurrentCamera]);
	ofPopStyle();
	glEnable(GL_DEPTH_TEST);
	//
	//////////////////////////
	
	
	
	//////////////////////////
	// DRAW ALL VIEWPORTS
	//////////////////////////
	//
	
	//draw main viewport
	cameras[iCurrentCamera]->begin(viewMain);
	drawScene(iCurrentCamera);
	cameras[iCurrentCamera]->end();
	
	for (int i=0; i<4; i++)
	{
		cameras[i]->begin(viewGrid[i]);
		drawScene(i);
		cameras[i]->end();
	}
	
	//
	//////////////////////////
	
	
	
	//////////////////////////
	// DRAW STUFF ON TOP
	//////////////////////////
	//
	ofPushStyle();
	glDepthFunc(GL_ALWAYS); // draw on top of everything
	
	//draw some labels
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Press keys 1-4 to select a camera for main view", viewMain.x + 20, 30);
	ofDrawBitmapString("Camera selected: " + ofToString(iCurrentCamera+1), viewMain.x + 20, 50);
	ofDrawBitmapString("Press 'f' to toggle fullscreen", viewMain.x + 20, 70);
	ofDrawBitmapString("EasyCam",	viewGrid[0].x + 20, viewGrid[0].y + 30);
	ofDrawBitmapString("Front",		viewGrid[1].x + 20, viewGrid[1].y + 30);
	ofDrawBitmapString("Top",		viewGrid[2].x + 20, viewGrid[2].y + 30);
	ofDrawBitmapString("Left",		viewGrid[3].x + 20, viewGrid[3].y + 30);

	//draw outlines on views
	ofSetLineWidth(5);
	ofNoFill();
	ofSetColor(255, 255, 255);
	//
	for (int i=0; i<4; i++)
		ofRect(viewGrid[i]);
	//
	ofRect(viewMain);
	
	glDepthFunc(GL_LESS);
	ofPopStyle();
	//
	//////////////////////////
}

void testApp::drawScene(int iCameraDraw){	
	
	nodeSwarm.draw();
	nodeGrid.draw();
	
	//////////////////////////////////
	// DRAW EASYCAM FRUSTUM
	//////////////////////////////////
	//
	//this section draws our camera in
	//the scene
	//
	//the pyramid-like shape which the
	//camera creates is called a 'frustum'
	//
	//check we're not looking through ofEasyCam
	if (iCameraDraw != 0)
	{
		
		//draw camera frustum.
		//
		//in 'camera space' this frustum
		//is defined by a box with bounds
		//-1->1 in each axis
		//
		//to convert from camera to world
		//space, we multiply by the inverse
		//matrix of the camera
		
		ofMatrix4x4 inverseCamera;
		
		inverseCamera.makeInvertOf(cameras[0]->getModelViewProjectionMatrix());
		
		ofPushMatrix();
		glMultMatrixf(inverseCamera.getPtr());
		
		
		ofPushStyle();
		ofSetColor(255, 100, 100);
		
		// xy plane at z=-1 in camera sapce
		// (small rectangle at camera position)
		//
		glBegin(GL_LINE_LOOP);
			glVertex3f(-1, -1, -1);
			glVertex3f(-1, 1, -1);
			glVertex3f(1, 1, -1);
			glVertex3f(1, -1, -1);
		glEnd();
		
		
		// xy plane at z=1 in camera space
		// (generally invisible because so far away)
		//
		glBegin(GL_LINE_LOOP);
			glVertex3f(-1, -1, 1);
			glVertex3f(-1, 1, 1);
			glVertex3f(1, 1, 1);
			glVertex3f(1, -1, 1);
		glEnd();
		
		// connecting lines between above 2 planes
		// (these are the long lines)
		//
		glBegin(GL_LINES);
			glVertex3f(-1, 1, -1);
			glVertex3f(-1, 1, 1);
			
			glVertex3f(1, 1, -1);
			glVertex3f(1, 1, 1);
		
			glVertex3f(-1, -1, -1);
			glVertex3f(-1, -1, 1);
			
			glVertex3f(1, -1, -1);
			glVertex3f(1, -1, 1);
		glEnd();

		ofPopStyle();
		ofPopMatrix();
	}
	
	//
	//////////////////////////////////
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (key >= '1' && key <= '4')
		iCurrentCamera = key - '1';
	
	if (key == 'f')
		ofToggleFullscreen();
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	setupViewports();
}

