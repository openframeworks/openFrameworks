
#include "testApp.h"

#define POS1_LATITUDE		51.538034
#define POS1_LONGITUDE		-0.021286

#define POS2_LATITUDE		43.530038
#define POS2_LONGITUDE		5.440958

//--------------------------------------------------------------
void testApp::setup(){	
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// touch events will be sent to testApp
	ofxMultiTouch.addListener(this);
	
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	
	// dump lots of info to console (useful for debugging)
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	ofEnableAlphaBlending();
	
	// load font for displaying info
	font.loadFont("verdana.ttf", 12);
	
	// open the mapview
	mapKit.open();
	
	// set initial position to specified coordinates, and cover 1000 Km x 1000 Km area
//	mapKit.setRegionWithMeters(POS2_LATITUDE, POS2_LONGITUDE, 1000 * 1000, 1000 * 1000);
	mapKit.setRegionWithMeters(POS2_LATITUDE, POS2_LONGITUDE, 1000, 1000);
	

	// optionally add testApp as a listener for map event (callbacks below)
	mapKit.addListener(this);
	
	// set other optional parameters
	mapKit.setShowUserLocation(true);
	mapKit.setType(OFXIPHONE_MAPKIT_HYRBID);
//	mapKit.setAllowZoom(false);
//	mapKit.setAllowScroll(false);
//	mapKit.setAllowUserInteraction(false);
	
	
	
	// do some hardcode OPTIONAL stuff to render the opengl view ONTOP of the map view
	// note this could affect performance quite seriously
	
	// send the OpenGL View to the front (on top of the mapview)
	iPhoneSendGLViewToFront();
	
	// OpenGL View is now in front of mapview, but it isn't transparent, so we can't see the map
	// Make the map transparent so we can see the mapview in undrawn areas
	iPhoneSetGLViewTransparent(true);
	
	// Now that the OpenGL view is in front of the mapview, it will receive all the touch events, so we cannot interact with the map
	// So disable interaction with the OpenGL view (testApp::touchXXXXX will not be called)
	iPhoneSetGLViewUserInteraction(false);

	// When user is interacting with the map, the OpenGL update & draw will freeze
	// To remedy this, run the update & draw in a separate thread
	iPhoneEnableLoopInThread();
}


//--------------------------------------------------------------
void testApp::update() {
	if(mapKit.isOpen()) {
		
//		printf("User is %son screen\n", mapKit.isUserOnScreen() ? "" : "not ");
	}
	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	// if mapKit is open....
	if(mapKit.isOpen()) {

		// draw a line between pos1 and pos2
		ofSetColor(255, 0, 0);
		ofSetLineWidth(5);
		ofPoint pos1 = mapKit.getScreenCoordinatesForLocation(POS1_LATITUDE, POS1_LONGITUDE);
		ofPoint pos2 = mapKit.getScreenCoordinatesForLocation(POS2_LATITUDE, POS2_LONGITUDE);
		ofLine(pos1.x, pos1.y, pos2.x, pos2.y);
		
		
		// draw black circle in middle of screen
		ofSetColor(0, 0, 0);
		ofNoFill();
		ofCircle(ofGetWidth()/2, ofGetHeight()/2, 10);
		ofFill();
		

		// get location (latitude, longitude) for center of map
		ofxMapKitLocation centerLocation = mapKit.getCenterLocation();
		string sLocation = ofToString(centerLocation.latitude)+"\n"+ofToString(centerLocation.longitude);
		float sHeight = font.stringHeight(sLocation);

		// draw semi-transparent rectangle in top part of screen
		ofSetColor(0, 0, 0, 200);
		ofRect(0, 0, ofGetWidth(), sHeight + 10);

		// display at top of screen
		ofSetColor(255, 255, 255);
		font.drawString(sLocation, 0, sHeight/2);
	}
	
}

void testApp::exit() {
}


void testApp::regionWillChange(bool animated) {
	printf("testApp::regionWillChange | animated: %i\n", animated);
}

void testApp::regionDidChange(bool animated) {
	printf("testApp::regionDidChange | animated: %i\n", animated);
}

void testApp::willStartLoadingMap() {
	printf("testApp::willStartLoadingMap\n");
}

void testApp::didFinishLoadingMap() {
	printf("testApp::didFinishLoadingMap\n");
}

void testApp::errorLoadingMap(string errorDescription) {
	printf("testApp::errorLoadingMap : %s\n", errorDescription.c_str());
}


//--------------------------------------------------------------
void testApp::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data){
	// to receive events in opengl view, set mapKit.setAllowUserInteraction(false) in setup
	printf("testApp::touchDown - if you can see me, the opengl view is receiving touch events!\n");
}

//--------------------------------------------------------------
void testApp::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data){
}

//--------------------------------------------------------------
void testApp::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data){
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data){
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

