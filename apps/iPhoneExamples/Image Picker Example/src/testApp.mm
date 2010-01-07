
#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// register touch events
	ofAddListener(ofEvents.touchDown, this, &testApp::touchDown);
	ofAddListener(ofEvents.touchUp, this, &testApp::touchUp);
	ofAddListener(ofEvents.touchMoved, this, &testApp::touchMoved);
	ofAddListener(ofEvents.touchDoubleTap, this, &testApp::touchDoubleTap);
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	
	camera = new ofxiPhoneImagePicker();
	camera->setMaxDimension(480); //otherwise we will have enormous images
	imgPos.x=ofGetWidth()/2;
	imgPos.y=ofGetHeight()/2;
	
	photo.loadImage("images/instructions.png");
}


//--------------------------------------------------------------
void testApp::update()
{
	if(camera->imageUpdated)
	{
		photo.setFromPixels(camera->pixels,camera->width, camera->height, OF_IMAGE_COLOR_ALPHA);
		//photo.tex.texData.bFlipTexture=false;
		
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
void testApp::touchDown(ofTouchEventArgs &touch){
	if(touch.id==1)
	{
		camera->openCamera();
		imgPos.x=ofGetWidth()/2;
		imgPos.y=ofGetHeight()/2;
	}
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	if(touch.id==0)
	{
		imgPos.x=touch.x;
		imgPos.y=touch.y;
	}		
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){
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


