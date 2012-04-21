#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
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

	phase += 0.35f;

	ofSetHexColor(0xDDDDFF);
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

	ofSetHexColor(0x000000);
	ofDrawBitmapString("press 'x' to capture screen \n", 200,460);

	if (bSnapshot == true){
		// grab a rectangle at 200,200, width and height of 300,180
		img.grabScreen(200,200,300,180);

		string fileName = "snapshot_"+ofToString(10000+snapCounter)+".png";
		img.saveImage(fileName);
		sprintf(snapString, "saved %s", fileName.c_str());
		snapCounter++;
		bSnapshot = false;
	}

	ofDrawBitmapString(snapString, 600,460);


	ofSetHexColor(0xFFFFFF);
	if(snapCounter > 0) {
        img.draw(600,200,300,180);
	}


}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	if (key == 'x'){
		bSnapshot = true;
	}
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

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
