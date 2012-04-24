#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	cameraPixels = NULL; 
	camera = new ofxiPhoneImagePicker();
	camera->setMaxDimension(480); //otherwise we will have enormous images
	imgPos.x=ofGetWidth()/2;
	imgPos.y=ofGetHeight()/2;
	
	photo.loadImage("images/instructions.png");
}


//--------------------------------------------------------------
void testApp::update()
{
	if(camera->imageUpdated){
		
		// the pixels seem to be flipped, so let's unflip them: 
		if (cameraPixels == NULL){
			// first time, let's get memory based on how big the image is: 
			cameraPixels = new unsigned char [camera->width * camera->height*4];
		}
		// now, lets flip the image vertically:
		for (int i = 0; i < camera->height; i++){
			memcpy(cameraPixels+(camera->height-i-1)*camera->width*4, camera->pixels+i*camera->width*4, camera->width*4);
		}
		
		// finally, set the image from pixels...
		photo.setFromPixels(cameraPixels,camera->width, camera->height, OF_IMAGE_COLOR_ALPHA);
		
		imgPos.x=ofGetWidth()/2;
		imgPos.y=ofGetHeight()/2;
		
		camera->imageUpdated=false;
	}
	
}

//--------------------------------------------------------------
void testApp::draw()
{
	photo.draw(imgPos.x-photo.width/2,imgPos.y-photo.height/2);
}

//--------------------------------------------------------------
void testApp::exit() {
}


//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){
	
	if(touch.id == 1){
		camera->openCamera();
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


