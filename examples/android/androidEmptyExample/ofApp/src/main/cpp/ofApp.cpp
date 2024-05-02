#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(255,255,255);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

    bool loadok = font.load("verdana.ttf", 200, true, false, true, 0.4, 72);
	shader.load("shaders/noise.vert", "shaders/noise.frag");

	auto textShapes = font.getStringAsPoints("openFrameworks");
	for (auto glyph : textShapes) {
		text.append(glyph);
	}

	ofRectangle boundingBox;
	for (auto outline : text.getOutline()) {
		boundingBox = boundingBox.getUnion(outline.getBoundingBox());
	}

	boundingBox.alignTo(ofGetCurrentViewport(), OF_ALIGN_HORZ_CENTER, OF_ALIGN_VERT_CENTER);
	glm::vec2 textPos(boundingBox.getX(), boundingBox.getMaxY());

	text.translate(textPos);
	text.setFillColor(ofColor(245, 58, 135));

	doShader = false;
}

void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    int r = 128 + 50 * cosf(ofGetElapsedTimef());
    int g = 0;
    int b = 128 + 50 * sinf(ofGetElapsedTimef());

    ofBackground(r,g,b);

    //ofSetColor(245, 58, 135);
    //ofFill();
    //font.drawStringAsShapes("meditate", 300, 400);
	
	if( doShader ){
		shader.begin();
		//we want to pass in some varrying values to animate our type / color
		shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
		shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );

		//we also pass in the mouse position
		//we have to transform the coords to what the shader is expecting which is 0,0 in the center and y axis flipped.
		shader.setUniform2f("mouse", ofGetMouseX() - ofGetWidth()/2, ofGetHeight()/2-ofGetMouseY() );

	}

	//finally draw our text
	text.draw();

	if( doShader ){
		shader.end();
	}



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
	doShader = true;
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
	doShader = false;

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

void ofApp::deviceRefreshRateChanged(int refreshRate) {

}

void ofApp::deviceHighestRefreshRateChanged(int refreshRate) {

}

void ofApp::deviceRefreshRateChangedEvent(int &refreshRate) {

}

void ofApp::deviceHighestRefreshRateChangedEvent(int &refreshRate) {

}
