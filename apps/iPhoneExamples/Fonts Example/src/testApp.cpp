
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
	
	ofBackground(50, 50, 50);
	// this load font loads the non-full character set
	// (ie ASCII 33-128), at size "32"
	
	char buf[1024];
	printf("cwd: %s\n", getcwd( buf, 1024 ) );
	
	printf("loading Franklin Book..\n");
	
	franklinBook.loadFont(ofToDataPath("frabk.ttf"),32);
	
	// now load another font, but with extended parameters:
	// font name, size, anti-aliased, full character set
	printf("loading Verdana..\n");
	verdana.loadFont(ofToDataPath("verdana.ttf"),8, false, true);
	verdana.setLineHeight(20.0f);
	
	counter = 0;
}


//--------------------------------------------------------------
void testApp::update()
{
	counter += 1.0f;
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor(0x00FF00);
	franklinBook.drawString("hello, this is franklin book calling\nanyone home?", 10,40);
	
	ofSetColor(0x000000);
	verdana.drawString("hello, I am aliased verdana -- full character set,\nsee: \344 ! ", 10,100);
	
	ofSetColor(0x00FF00);
	franklinBook.drawString("I can't make an\n (\344) like you", 10,150);
	
	ofSetColor(0x000000);
	verdana.drawString("yeah, but I'm not exactly pretty\nthe problem is with freeType library...\napple has a patent on TTF font hints\nso our aliased type via freeType isn't super looking", 10,210);
	
	ofSetColor(0x00FF00);
	franklinBook.drawString("you look ok !\n don't worry", 10,300);
	
	
	//------------------- bounding rectangle : 
	char tempString[255];
	sprintf(tempString,"%f", ofGetElapsedTimef());
	// ok first job to rotate around the center, is to get the bounding box:
	ofRectangle rect = franklinBook.getStringBoundingBox(tempString, 0,0);
	// this is the actual midpt (x + w/2, y + h/2);
	float centerx = rect.x + rect.width / 2;
	float centery = rect.y + rect.height / 2;
	
	glPushMatrix();
	glTranslatef(50,400,0);
	glRotatef(counter, 0,0,1);
	// draw type & rect centered around 0,0 (subtract midpt from both):
	ofSetColor(0xcccccc);
	ofRect(rect.x - centerx, rect.y - centery, rect.width, rect.height);
	ofSetColor(0xff3399);
	franklinBook.drawString(tempString, -centerx,-centery);
	glPopMatrix();
	
	// -------------------------------------
	
	glPushMatrix();
	
	glTranslatef(50,400,0);
	glScalef(5,5,1);
	ofSetColor(0x333333);
	verdana.drawString("scale 5x!",0, 0);
	glPopMatrix();
	
	float size = 2 + 1.5*sin(ofGetElapsedTimef());
	glPushMatrix();
	glTranslatef(100,400,0);
	glScalef(size,size,1);
	ofSetColor(0x00FF00);
	franklinBook.drawString("$k@!%",0, 0);
	glPopMatrix();
}

//--------------------------------------------------------------
void testApp::exit() {
}


//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
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


