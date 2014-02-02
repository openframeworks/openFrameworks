#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
    ofBackground(0);
    
    ofSetOrientation(OF_ORIENTATION_90_RIGHT);
	
    camera = NULL;
	
    imgPos.x=ofGetWidth()/2;
	imgPos.y=ofGetHeight()/2;
	
	photo.loadImage("images/instructions.png");
}


//--------------------------------------------------------------
void ofApp::update(){
    if(camera) {
        if(camera->imageUpdated){
            
            int cameraW = camera->width;
            int cameraH = camera->height;
            unsigned const char * cameraPixels = camera->pixels;
            
            photo.setFromPixels(cameraPixels, cameraW, cameraH, OF_IMAGE_COLOR_ALPHA);
            
            imgPos.x = ofGetWidth()/2;
            imgPos.y = ofGetHeight()/2;
            
            camera->close();
            delete camera;
            camera = NULL;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    int x = imgPos.x - photo.width / 2;
    int y = imgPos.y - photo.height / 2;
	photo.draw(x, y);
}

//--------------------------------------------------------------
void ofApp::exit() {
}


//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	if(touch.id == 1){
		
        if(!camera) {
            camera = new ofxiOSImagePicker();
            camera->setMaxDimension(MAX(ofGetWidth(), ofGetHeight())); // max the photo taken at the size of the screen.
            camera->openCamera();
//            camera->showCameraOverlay();
        }
        
		imgPos.x=ofGetWidth()/2;
		imgPos.y=ofGetHeight()/2;
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
	if(touch.id == 0){
		imgPos.x=touch.x;
		imgPos.y=touch.y;
	}		
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){	
    
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus() {
    
}

//--------------------------------------------------------------
void ofApp::gotFocus() {
    
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning() {
    
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    
}
