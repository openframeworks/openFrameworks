#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(0,0,0);
	ofSetFrameRate(60);
	int maxSize = 0;
	// get the max texture size available for openGL
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	
	// take the smaller size
	int fboWidth = MIN(8192,maxSize);
	int fboHeight = MIN(8192,maxSize);
	//allocate our fbo with 3 color channels and 4x multisample
	#ifdef TARGET_EMSCRIPTEN
		fboOutput.allocate(fboWidth, fboHeight, GL_RGB);
	#else
		fboOutput.allocate(fboWidth, fboHeight, GL_RGB, 4);
	#endif
	// another way to allocate an fbo is by using ofFboSettings
//	ofFboSettings fboSettings;
//	fboSettings.numSamples = 4;
//	fboSettings.width = fboWidth;
//	fboSettings.height = fboHeight;
//	fboSettings.internalformat = GL_RGB;
//	fboOutput.allocate(fboSettings);
	
	// set higher resolution circle output
	ofSetCircleResolution(128);

}

//--------------------------------------------------------------
void ofApp::update(){
	
	// create a rectangle that represents the window size
	ofRectangle screenRect(0,0,ofGetWidth(), ofGetHeight());
	// create a rectangle that represents the fbo size
	fboRect = ofRectangle(0,0,fboOutput.getWidth(), fboOutput.getHeight());
	// assign the scaledFboRect the value of fboRect
	scaledFboRect = fboRect;
	// scale the scaledFboRect to fit within the screenRect
	scaledFboRect.scaleTo(screenRect, OF_SCALEMODE_FIT);
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	float renderMouseX = ofGetMouseX();
	float renderMouseY = ofGetMouseY();
	
	float centerX = ofGetWidth() / 2;
	float centerY = ofGetHeight() / 2;
	
	float renderWidth = ofGetWidth();
	float renderHeight = ofGetHeight();
	
	float renderScale = (float)ofGetWidth() / fboOutput.getWidth();
	
	if( bShowHighRes ){
		// lets map the coordinates of the mouse in window space to that of the FBO
		renderMouseX = ofMap(ofGetMouseX(), scaledFboRect.getLeft(), scaledFboRect.getRight(), fboRect.getLeft(), fboRect.getRight(), false );
		renderMouseY = ofMap(ofGetMouseY(), scaledFboRect.getTop(), scaledFboRect.getBottom(), fboRect.getTop(), fboRect.getBottom(), false );
		
		centerX = fboRect.getCenter().x;
		centerY = fboRect.getCenter().y;
		
		renderWidth = fboOutput.getWidth();
		renderHeight = fboOutput.getHeight();
		
		renderScale = 1.;
	}
	
	
	
	ofFloatColor centerColor;
	centerColor.setHex(0xEDDE45);
	ofFloatColor outterColor;
	outterColor.setHex(0x9BF0E9);
	
	if(bShowHighRes){
		ofBackground(0,0,0);
		fboOutput.begin();
		// clear the contents of the fbo
		ofClear(237,229,226, 255);
	}else{
		ofBackground(237,229,226);
	}
	float elapsedTimef = ofGetElapsedTimef();
	ofSetColor( centerColor );
	
	ofNoFill();
	for( int i = 0; i < 100; i++ ){
		float ipct = (float)i / 100.0f;
		ofSetColor( centerColor.getLerped(outterColor, 1.f-ipct));
		float circleX = ofLerp( centerX, renderMouseX, ipct );
		float circleY = ofLerp( centerY, renderMouseY, ipct );
		
		ofSetLineWidth(20 * renderScale * (1.0f-ipct) + 3 * renderScale);
		float radius = (1.f-ipct)*renderWidth * 0.4+renderWidth * 0.05;
		float offsetX = cosf((float)i*0.1-elapsedTimef)*renderWidth*0.075;
		float offsetY = sinf(elapsedTimef*0.65+(float)i*0.05)*renderHeight*0.05;
		ofDrawCircle(circleX+offsetX, circleY+offsetY, radius );
	}
	ofFill();
	if( ofGetMousePressed() ) {
		ofDrawCircle(renderMouseX, renderMouseY, 100 * renderScale );
	}
	if(bShowHighRes){
		fboOutput.end();
	}
	
	if( bSaveOutput ){
		bSaveOutput=false;
		
		fboOutput.readToPixels(pixels);
		ofSaveImage( pixels, ofGetTimestampString()+".jpg", OF_IMAGE_QUALITY_BEST );
	}
	
	
	if(bShowHighRes){
		ofSetColor( 255 );
		fboOutput.draw( scaledFboRect );
		ofNoFill();
		ofSetColor( 200 );
		ofDrawRectangle( scaledFboRect );
		ofFill();
	}
	
	string message = "Show high res output(spacebar): "+ofToString(bShowHighRes);
	if(bShowHighRes) {
		message += "\nSave output(s) ";
	}
	ofDrawBitmapStringHighlight(message, 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == 's' ) {
		if(bShowHighRes) {
			bSaveOutput = true;
		}
	}
	if( key == ' ' ) {
		bShowHighRes = !bShowHighRes;
		if( !bShowHighRes ) {
			ofSetCircleResolution(22);
		} else {
			ofSetCircleResolution(128);
		}
	}
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
