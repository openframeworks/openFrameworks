
#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	// touch events will be sent to myTouchListener
	ofxMultiTouch.addListener(this);
	
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

void testApp::exit() {
}


//--------------------------------------------------------------
void testApp::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data){
	if(touchId==1)
	{
		camera->openCamera();
		imgPos.x=ofGetWidth()/2;
		imgPos.y=ofGetHeight()/2;
	}
	printf("touch %i down at (%i,%i)\n", touchId, x,y);
}
//--------------------------------------------------------------
void testApp::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data){
	printf("touch %i moved at (%i,%i)\n", touchId, x,y);
	if(touchId==0)
	{
		imgPos.x=x;
		imgPos.y=y;
	}	
	
}

//--------------------------------------------------------------
void testApp::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data){
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data){
}

//--------------------------------------------------------------
void testApp::lostFocus()
{
}
//--------------------------------------------------------------
void testApp::gotFocus()
{
}
//--------------------------------------------------------------
void testApp::gotMemoryWarning()
{
}

