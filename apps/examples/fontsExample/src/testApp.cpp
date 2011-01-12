#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	 
	
	// this load font loads the non-full character set
	// (ie ASCII 33-128), at size "32"
	
	franklinBook.loadFont("frabk.ttf", 32);
	
	// now load another font, but with extended parameters:
	// font name, size, anti-aliased, full character set
	verdana.loadFont("verdana.ttf",8, false, true);
	verdana.setLineHeight(20.0f);
	
	counter = 0;
}


//--------------------------------------------------------------
void testApp::update(){
	ofBackground(255,255,255);	
	counter += 1.0f;
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetHexColor(0x00FF00);
	franklinBook.drawString("hello, this is franklin book calling\nanyone home?", 100,100);
	
	ofSetHexColor(0x000000);
	verdana.drawString("hello, I am aliased verdana -- full character set, see: � ! ", 100,210);
	
	ofSetHexColor(0x00FF00);
	franklinBook.drawString("I can't make an (�) like you", 100,310);
	
	ofSetHexColor(0x000000);
	verdana.drawString("yeah, but I'm not exactly pretty\nthe problem is with freeType library...\napple has a patent on TTF font hints\nso our aliased type via freeType isn't super looking", 100,380);
	
	ofSetHexColor(0x00FF00);
	franklinBook.drawString("you look ok ! don't worry", 100,520);
	
	//------------------- bounding rectangle : 
	char tempString[255];
	sprintf(tempString,"%i", (int)counter);
	// ok first job to rotate around the center, is to get the bounding box:
	ofRectangle rect = franklinBook.getStringBoundingBox(tempString, 0,0);
	// this is the actual midpt (x + w/2, y + h/2);
	float centerx = rect.x + rect.width / 2;
	float centery = rect.y + rect.height / 2;

	ofPushMatrix();
		ofTranslate(100,650,0);
		ofRotate(counter, 0,0,1);
		// draw type & rect centered around 0,0 (subtract midpt from both):
		ofSetHexColor(0xcccccc);
		ofRect(rect.x - centerx, rect.y - centery, rect.width, rect.height);
		ofSetHexColor(0xff3399);
		franklinBook.drawString(tempString, -centerx,-centery);
	ofPopMatrix();

	// -------------------------------------

	ofPushMatrix();
		ofTranslate(225,675,0);
		ofScale(5,5,1);
		ofSetHexColor(0x333333);
		verdana.drawString("scale 5x!",0, 0);
	ofPopMatrix();
	
	float size = 2 + 2*sin(counter/300.0f);
	ofPushMatrix();
		ofTranslate(520,675,0);
		ofScale(size,size,1);
		ofSetHexColor(0x00FF00);
		franklinBook.drawString("$k@!%",0, 0);
	ofPopMatrix();
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
