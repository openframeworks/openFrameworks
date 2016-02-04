#include "ofApp.h"

//--------------------------------------------------------------
//
//
// ADVANCED 3D EXAMPLE
//		ofNode3d, ofCamera, ofEasyCam
//
//
//--------------------------------------------------------------

//
// SUGGESTED EXERCISES
//
// 0. Run and understand the example
//
// 1. Change number of particles in the swarm.
// 2. Change the dynamic properties of the swarm (speed, orbit radius)
// 3. Change the near and far clipping planes of camEasyCam
//
// 4. Add another camera to the existing 4.
//		Have all parts of the example working with all 5 cameras.
//
// 6. Create your own custom node class and add an instance of it
//		to the scene.
//
// 7. Understand how the 'frustrum preview' works
//



//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofBackground(70, 70, 70);
	ofEnableSmoothing();
	ofEnableDepthTest();


	//--
	// Setup cameras

	iMainCamera = 0;
	bCamParent = false;

	// user camera
	camEasyCam.setTarget(nodeSwarm);
	camEasyCam.setDistance(100);
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
	//--



	//--
	// Define viewports

	setupViewports();

	//
	//--




	//--
	// Setup swarm

	// swarm is a custom ofNode in this example (see Swarm.h / Swarm.cpp)
	nodeSwarm.init(100, 50, 10);

	//
	//--
}

//--------------------------------------------------------------
void ofApp::setupViewports(){
	//call here whenever we resize the window


	//--
	// Define viewports

	float xOffset = ofGetWidth() / 3;
	float yOffset = ofGetHeight() / N_CAMERAS;

	viewMain.x = xOffset;
	viewMain.y = 0;
	viewMain.width = xOffset * 2;
	viewMain.height = ofGetHeight();

	for(int i = 0; i < N_CAMERAS; i++){

		viewGrid[i].x = 0;
		viewGrid[i].y = yOffset * i;
		viewGrid[i].width = xOffset;
		viewGrid[i].height = yOffset;
	}

	//
	//--
}

//--------------------------------------------------------------
void ofApp::update(){

}


//--------------------------------------------------------------
void ofApp::draw(){
	//--
	// Highlight background of selected camera

	ofDisableDepthTest();
	ofPushStyle();
	ofSetColor(100, 100, 100);
	ofDrawRectangle(viewGrid[iMainCamera]);
	ofPopStyle();
	ofEnableDepthTest();

	//
	//--



	//--
	// Draw all viewports

	// draw main viewport
	cameras[iMainCamera]->begin(viewMain);
	drawScene(iMainCamera);

	// calculate mouse ray whilst this camera is active
	updateMouseRay();

	cameras[iMainCamera]->end();

	// draw side viewports
	for(int i = 0; i < N_CAMERAS; i++){
		cameras[i]->begin(viewGrid[i]);
		drawScene(i);
		cameras[i]->end();
	}

	//
	//--



	//--
	// Draw annotations (text, gui, etc)

	ofPushStyle();
	glDepthFunc(GL_ALWAYS); // draw on top of everything

	// draw some labels
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Press keys 1-4 to select a camera for main view", viewMain.x + 20, 30);
	ofDrawBitmapString("Camera selected: " + ofToString(iMainCamera + 1), viewMain.x + 20, 50);
	ofDrawBitmapString("Press 'f' to toggle fullscreen", viewMain.x + 20, 70);
	ofDrawBitmapString("Press 'p' to toggle parents on OrthoCamera's", viewMain.x + 20, 90);

	ofDrawBitmapString("EasyCam",   viewGrid[0].x + 20, viewGrid[0].y + 30);
	ofDrawBitmapString("Front",     viewGrid[1].x + 20, viewGrid[1].y + 30);
	ofDrawBitmapString("Top",       viewGrid[2].x + 20, viewGrid[2].y + 30);
	ofDrawBitmapString("Left",      viewGrid[3].x + 20, viewGrid[3].y + 30);

	// draw outlines on views
	ofSetLineWidth(5);
	ofNoFill();
	ofSetColor(255, 255, 255);
	//
	for(int i = 0; i < N_CAMERAS; i++){
		ofDrawRectangle(viewGrid[i]);
	}
	//
	ofDrawRectangle(viewMain);

	// restore the GL depth function
	glDepthFunc(GL_LESS);
	ofPopStyle();

	//
	//--
}

void ofApp::drawScene(int iCameraDraw){

	nodeSwarm.draw();
	nodeGrid.draw();

	//--
	// Draw frustum preview for ofEasyCam camera

	// This code draws our camera in
	//	the scene (reddy/pink lines)
	//
	// The pyramid-like shape defined
	//	by the cameras view is called
	//	a 'frustum'.
	//
	// Often we refer to the volume
	//	which can be seen by the
	//	camera as 'the view frustum'.


	// First check if we're already drawing the view through the easycam
	// If so, don't draw the frustum preview.
	if(iCameraDraw != 0){

		ofPushStyle();
		ofPushMatrix();

		//--
		// Create transform for box->frustum

		// In 'camera space' the bounds of
		//  the view are defined by a box
		//  with bounds -1->1 in each axis
		//
		// To convert from camera to world
		//  space, we multiply by the inverse
		//  camera matrix of the camera, i.e
		//  inverse of the ViewProjection
		//  matrix.
		//
		// By applying this transformation
		//  our box in camera space is
		//  transformed into a frustum in
		//  world space.
		//

		// The camera's matricies are dependant on
		//  the aspect ratio of the viewport.
		//  (Which is why we use the viewport as
		//  an argument when we begin the camera.
		//
		// If this camera is fullscreen we'll use
		//   viewMain, else we'll use viewGrid[0]
		ofRectangle boundsToUse;
		if(iMainCamera == 0){
			boundsToUse = viewMain;
		}
		else{
			boundsToUse = viewGrid[0];
		}

		// Now lets get the inverse ViewProjection
		//  for the camera
		ofMatrix4x4 inverseCameraMatrix;
		inverseCameraMatrix.makeInvertOf(camEasyCam.getModelViewProjectionMatrix(boundsToUse));

		// By default, we can say
		//	'we are drawing in world space'
		//
		// The camera matrix performs
		//	world->camera
		//
		// The inverse camera matrix performs
		//	camera->world
		//
		// Our box is in camera space, if we
		//	want to draw that into world space
		//	we have to apply the camera->world
		//	transformation.
		//

		// This syntax is a little messy.
		// What it's saying is, send the data
		//  from the inverseCameraMatrix object
		//  to OpenGL, and apply that matrix to
		//  the current OpenGL transform
		ofMultMatrix( inverseCameraMatrix );

		//
		//--



		//--
		// Draw box in camera space
		// (i.e. frustum in world space)

		ofNoFill();
		// i.e. a box -1, -1, -1 to +1, +1, +1
		ofDrawBox(0, 0, 0, 2.0f);
		//
		//--

		ofPopStyle();
		ofPopMatrix();
	}

	//
	//--



	//--
	// Draw mouse ray

	// Draw the ray if ofEasyCam is in main view,
	//  and we're not currently drawing in that view
	if(iMainCamera == 0 && iCameraDraw != 0){
		ofPushStyle();
		ofSetColor(100, 100, 255);
		ofDrawLine(ray[0], ray[1]);
		ofPopStyle();
	}

	//
	//--
}

//--------------------------------------------------------------
void ofApp::updateMouseRay(){
	// Define ray in screen space
	ray[0] = ofVec3f(ofGetMouseX(), ofGetMouseY(), -1);
	ray[1] = ofVec3f(ofGetMouseX(), ofGetMouseY(), 1);

	// Transform ray into world space
	ray[0] = cameras[iMainCamera]->screenToWorld(ray[0], viewMain);
	ray[1] = cameras[iMainCamera]->screenToWorld(ray[1], viewMain);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if(key >= '1' && key <= '4'){
		iMainCamera = key - '1';
	}

	if(key == 'f'){
		ofToggleFullscreen();
	}

	if(key == 'p'){
		if(bCamParent){
			camFront.clearParent();
			camTop.clearParent();
			camLeft.clearParent();

			bCamParent = false;
		}
		else{
			camFront.setParent(nodeSwarm.light);
			camTop.setParent(nodeSwarm.light);
			camLeft.setParent(nodeSwarm.light);

			bCamParent = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	setupViewports();
}

