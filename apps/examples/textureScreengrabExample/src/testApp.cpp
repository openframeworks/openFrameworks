#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	counter = 0;
	ofSetCircleResolution(100);
	texScreen.allocate(300,300,GL_RGB);
	ofBackground(230,230,240);
	
}
 

//--------------------------------------------------------------
void testApp::update(){
	counter += 1;
}


//--------------------------------------------------------------
void testApp::draw(){

	// 1st, draw on screen:
	ofSetHexColor(0x66CC33);	
	ofRect(100,100,300,300);
	
	ofSetHexColor(0xffffff);
	glPushMatrix();
		glTranslatef(200,200,0);
		glRotatef(counter,0,0,1);
		ofCircle(0,0,80);
		ofCircle(100,0,10);	// a small one
	glPopMatrix();
	ofSetHexColor(0x333333);
	ofDrawBitmapString("(a) on screen", 150,200);

	ofSetHexColor(0xFFCC33);	
	ofCircle(mouseX, mouseY,20);
	

	// 2nd, grab a portion of the screen into a texture
	// this is quicker then grabbing into an ofImage
	// because the transfer is done in the graphics card
	// as opposed to bringing pixels back to memory
	// note: you need to allocate the texture to the right size
	texScreen.loadScreenData(100,100,300,300);
	
	

	// finally, draw that texture on screen, how ever you want
	// (note: you can even draw the texture before you call loadScreenData, 
	// in order to make some trails or feedback type effects)
	glPushMatrix();
		ofSetHexColor(0xffffff);
		glTranslatef(550,300,0);
		//glRotatef(counter, 0.1f, 0.03f, 0);
		float width = 200 + 100 * sin(counter/200.0f);
		float height = 200 + 100 * sin(counter/200.0f);;
		texScreen.draw(-width/2,-height/2,width,height);
	glPopMatrix();

	glPushMatrix();
		ofSetHexColor(0xffffff);
		glTranslatef(700,210,0);
		glRotatef(counter, 0.1f, 0.03f, 0);
		texScreen.draw(-50,-50,100,100);
	glPopMatrix();

	ofSetHexColor(0x333333);
	ofDrawBitmapString("(b) in a texture, very meta!", 500,200);
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

