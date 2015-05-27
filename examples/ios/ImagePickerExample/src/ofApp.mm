#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
    ofBackground(170);
    ofSetOrientation(OF_ORIENTATION_DEFAULT);
    
    camImg.load("images/camera.png");
    libImg.load("images/library.png");
    
    camRect.set(20, 20, camImg.getWidth(), camImg.getHeight());
    libRect.set(camImg.getWidth() + 40, 20, libImg.getWidth(), libImg.getHeight());

}


//--------------------------------------------------------------
void ofApp::update(){
    if(camera.getImageUpdated()){
        
        photo.setFromPixels(camera.getPixelsRef());
        
        imgPos.x = 0;
        imgPos.y = camRect.getBottom() + 20;
        
        camera.close();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    if(photo.isAllocated()){
        photo.draw(imgPos);
    }
    
    camImg.draw(camRect);
    libImg.draw(libRect);
}

//--------------------------------------------------------------
void ofApp::exit() {
}


//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
	if( camRect.inside(touch.x, touch.y) ){
        camera.setMaxDimension(MAX(1024, ofGetHeight()));
        camera.openCamera();
    }else if( libRect.inside(touch.x, touch.y) ){
        camera.setMaxDimension(MAX(1024, ofGetHeight()));
        camera.openLibrary();
    }else{
        prePoint.x = touch.x;
        prePoint.y = touch.y;
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
	if(touch.id == 0){
		imgPos.x += touch.x-prePoint.x;
		imgPos.y += touch.y-prePoint.y;
        
        prePoint.x = touch.x;
        prePoint.y = touch.y;
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
