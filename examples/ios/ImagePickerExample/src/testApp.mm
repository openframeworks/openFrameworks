#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
    ofBackground(0);
    
    ofSetOrientation(OF_ORIENTATION_90_RIGHT);
	
    camera = NULL;
	
    imgPos.x=ofGetWidth()/2;
	imgPos.y=ofGetHeight()/2;
	
	photo.loadImage("images/instructions.png");
}


//--------------------------------------------------------------
void testApp::update(){
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
void testApp::draw(){
    int x = imgPos.x - photo.width / 2;
    int y = imgPos.y - photo.height / 2;
	photo.draw(x, y);
}

//--------------------------------------------------------------
void testApp::exit() {
}


//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){
	if(touch.id == 1){
		
        if(!camera) {
            camera = new ofxiPhoneImagePicker();
            camera->setMaxDimension(MAX(ofGetWidth(), ofGetHeight())); // max the photo taken at the size of the screen.
            camera->openCamera();
//            camera->showCameraOverlay();
        }
        
		imgPos.x=ofGetWidth()/2;
		imgPos.y=ofGetHeight()/2;
	}
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){
	if(touch.id == 0){
		imgPos.x=touch.x;
		imgPos.y=touch.y;
	}		
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){	
    
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void testApp::lostFocus() {
    
}

//--------------------------------------------------------------
void testApp::gotFocus() {
    
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning() {
    
}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
    
}
