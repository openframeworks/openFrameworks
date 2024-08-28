#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0,0,0);

	bPause	   = false;
	restLength = 3.0;
	space      = 4;

	zoom	   = -500;
	zoomTarget = 350;

	center.x   = ((GRID_WIDTH-1)  * space) / 2;
	center.y   = ((GRID_HEIGHT-1) * space) / 2;

	for (int i=0; i<GRID_WIDTH; i++) {
		for (int j=0; j<GRID_HEIGHT; j++) {

			int x	  = (i * space) - center.x;
			int y     = 0;
			int z     = (j * space) - center.y;
			int index = (j*GRID_WIDTH+i) * LENGTH;

			// now we are at each line
			for (int k=0; k<LENGTH; k++) {

				y = ofMap(k, 0, LENGTH, 0, 50);

				pos[index + k].x = x;
				pos[index + k].y = y;
				pos[index + k].z = z;
			}


		}
	}

	total = GRID_WIDTH*GRID_HEIGHT*LENGTH;
	vbo.setVertexData(pos, total, GL_DYNAMIC_DRAW);

}

//--------------------------------------------------------------
void ofApp::update(){

	if(!bPause) {
		glm::vec3 vec;
		float   r = 0.3;

		for (int i=0; i<GRID_WIDTH; i++) {
			for (int j=0; j<GRID_HEIGHT; j++) {

				int x	  = (i * space) - center.x;
				int y     = 0;
				int z     = (j * space) - center.y;
				int index = (j*GRID_WIDTH+i) * LENGTH;

				pos[index].set(x, y, z);
				pos[index+1].set(x+ofRandom(-r,r), y+ofRandom(-r,r), z+ofRandom(-r,r));
				pos[index+1].y -= 1.0;

				for (int k=2; k<LENGTH; k++) {
					vec		 =  pos[index + k] - pos[index + (k-2)];
					float d  = glm::length(vec);
					if(d > 0.0) {
						pos[index + k] = pos[index + k-1] + (vec * restLength) /d;
					}
				}
			}
		}
	}

	zoom += (zoomTarget-zoom) * 0.04;

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, zoom);

	ofRotateDeg(mouseX, 1, 0, 0);
	ofRotateDeg(mouseY, 0, 1, 1);


	// the lines
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255);
	//vbo.bind();
	vbo.updateVertexData(pos, total);

	for (int i=0; i<GRID_WIDTH; i++) {
		for (int j=0; j<GRID_HEIGHT; j++) {
			int index = (j*GRID_WIDTH+i) * LENGTH;
			vbo.draw(GL_LINE_STRIP, index, LENGTH);
		}
	}

	//vbo.unbind();

	ofPopMatrix();


	ofSetColor(90, 90, 90);
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 1), 5, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){
	if(id == 2) bPause = !bPause;
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
	if(id == 1) {
		zoomTarget = ofMap(x, 0.0, ofGetWidth(), 100, 500);
	}
}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){
	// android destroys the gl context when it pauses apps
	// most OF textures are automatically regenerated
	// but you'll need to regenerate the vbo yourself
	vbo.setVertexData(pos, total, GL_DYNAMIC_DRAW);
}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}
