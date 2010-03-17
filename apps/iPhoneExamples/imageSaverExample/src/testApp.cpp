#include "testApp.h"
#include "ofxiPhoneExtras.h"

//--------------------------------------------------------------
void testApp::setup(){	
	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	snapCounter = 0;
	bSnapshot = false;
	cooper.loadFont("cooperBlack.ttf", 50);
	phase = 0;
	memset(snapString, 0, 255);		// clear the string by setting all chars to 0
}


//--------------------------------------------------------------
void testApp::update(){
	ofBackground(255,255,255);	
}

//--------------------------------------------------------------
void testApp::draw(){

	ofScale(0.4, 0.4, 1.0);
	
	phase += 0.35f;

	ofSetColor(0xDDDDFF);
	ofRect(200,200,300,180);
	
	ofEnableAlphaBlending();
	ofSetColor(255,255,255,128);
	glPushMatrix();
		float width = cooper.stringWidth("catch me\nif you can!");
		glTranslatef(350,290,0);
		glRotatef(phase*3, 0,0,1);
		glScalef(1 + 0.5f * sin(phase/10.0f), 1 + 0.5f * sin(phase/10.0f), 1);
		cooper.drawString("catch me\nif you can!", -width/2,20);
	glPopMatrix();
	
	ofSetColor(255,150,140,128);
	glPushMatrix();
		glTranslatef(330,280,0);
		glRotatef(phase*5, 0,0,1);
		ofRect(-25,-25,50,50);
	glPopMatrix();
	
	ofDisableAlphaBlending();
	
	ofSetColor(0x000000);
	ofDrawBitmapString("press 'x' to capture screen \n", 200,460);
	
	ofSetColor(0xFFFFFF);
	
	if (bSnapshot == true){
		// grab a rectangle at 200,200, width and height of 300,180
		if( img.width == 0 ){
			img.allocate(300, 180, OF_IMAGE_COLOR_ALPHA);
		}
		img.grabScreen(100,100,300,180);
		char fileName[255];
		sprintf(fileName, "snapshot_%0.3i.png", snapCounter);
		//img.saveImage(ofxiPhoneGetDocumentsDirectory()+string(fileName));
		sprintf(snapString, "saved %s%s", ofxiPhoneGetDocumentsDirectory().c_str(), fileName);
		snapCounter++;
		bSnapshot = false;
	}
	
	ofDrawBitmapString(snapString, 600,460);
	
	if( snapCounter > 0){
		//ofEnableAlphaBlending();
		ofSetColor(0xFFFFFF);
		img.draw(600,200,300,180);
	}
}

//--------------------------------------------------------------
void testApp::exit() {

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	bSnapshot = true;	
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	bSnapshot = false;	
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

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}

