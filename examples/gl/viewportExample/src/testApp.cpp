#include "testApp.h"

// Viewports are useful for when you want
//	to display different items of content
//	within their own 'window'.
//
// Viewports are similar to 'ofTranslate(x,y)'
//	in that they move your drawing to happen
//	in a different location. But they also
//	constrain the drawing so that it is masked
//	to the rectangle of the viewport.
//
//
// When working with viewports you should
//	also be careful about your transform matrices.
// ofSetupScreen() is your friend.
// Also camera.begin() will setup relevant transform
//	matrices.

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(90);
	randomizeViewports();
	ofEnableSmoothing();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	//--
	// 2d view

	drawViewportOutline(viewport2D);

	// keep a copy of your viewport and transform matrices for later
	ofPushView();

	// tell OpenGL to change your viewport. note that your transform matrices will now need setting up
	ofViewport(viewport2D);

	// setup transform matrices for normal oF-style usage, i.e.
	//  0,0=left,top
	//  ofGetViewportWidth(),ofGetViewportHeight()=right,bottom
	ofSetupScreen();

	ofFill();
	ofSetColor(220);
	for(int x = 0; x < 1000; x += 20){
		for(int y = 0; y < 1000; y += 20){
			ofCircle(x, y, sin((x + y) / 100.0f + ofGetElapsedTimef()) * 5.0f);
		}
	}

	// restore the old viewport (now full view and oF coords)
	ofPopView();
	//--


	//--
	// 3d view

	drawViewportOutline(viewport3D);

	// note the camera accepts the viewport as an argument
	// this is so that the camera can be aware of which viewport
	// it is acting on
	//
	// ofPushView() / ofPopView() are automatic
	camera.begin(viewport3D);
	ofDrawGrid(100);
	camera.end();
	//--


	ofDrawBitmapString("Press [space] to randomize viewports", 20, 20);
}


//--------------------------------------------------------------
void testApp::randomize(ofRectangle & viewport){
	// utlitly function to randomise a rectangle
	viewport.x = ofRandom(ofGetWidth() * 2.0f / 3.0f);
	viewport.y = ofRandom(ofGetHeight() * 2.0f / 3.0f);
	viewport.width = 100 + ofRandom(ofGetWidth() - viewport.x - 100);
	viewport.height = 100 + ofRandom(ofGetHeight() - viewport.y - 100);
}

//--------------------------------------------------------------
void testApp::randomizeViewports(){
	randomize(viewport2D);
	randomize(viewport3D);
}

//--------------------------------------------------------------
void testApp::drawViewportOutline(const ofRectangle & viewport){
	ofPushStyle();
	ofFill();
	ofSetColor(50);
	ofSetLineWidth(0);
	ofRect(viewport);
	ofNoFill();
	ofSetColor(25);
	ofSetLineWidth(1.0f);
	ofRect(viewport);
	ofPopStyle();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == ' '){
		randomizeViewports();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
