#include "ofApp.h"

GLfloat lightOnePosition[] = {40.0, 40, 100.0, 0.0};
GLfloat lightOneColor[] = {0.99, 0.99, 0.99, 1.0};

GLfloat lightTwoPosition[] = {-40.0, 40, 100.0, 0.0};
GLfloat lightTwoColor[] = {0.99, 0.99, 0.99, 1.0};

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);

    //some model / light stuff
    ofEnableDepthTest();
    glShadeModel (GL_SMOOTH);

    /* initialize lighting */
    glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor);
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT1, GL_POSITION, lightTwoPosition);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
    //glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);

    glEnable (GL_COLOR_MATERIAL);

    //load the squirrel model - the 3ds and the texture file need to be in the same folder
    squirrelModel.loadModel("squirrel/NewSquirrel.3ds", 20);

    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
    squirrelModel.setRotation(0, 90, 1, 0, 0);
    squirrelModel.setRotation(1, 270, 0, 0, 1);
    squirrelModel.setScale(0.9, 0.9, 0.9);
    squirrelModel.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
}

//--------------------------------------------------------------
void ofApp::update(){
    squirrelModel.setRotation(1, 270 + ofGetElapsedTimef() * 60, 0, 0, 1);
}

//--------------------------------------------------------------
void ofApp::draw(){
	 //fake back wall
   ofSetColor(20, 20, 20);

   GLfloat vertices1[] = {0.0, ofGetHeight(), -600,
          ofGetWidth(), ofGetHeight(), -600,
          ofGetWidth(), 0, -600,
          0, 0, -600};

   GLubyte indices[] = {0, 1, 2, 0, 2, 3};

   glVertexPointer(3, GL_FLOAT, 0, vertices1);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

   //fake wall
   ofSetColor(50, 50, 50);
   GLfloat vertices2[] = {0, ofGetHeight(), 0,
		   ofGetWidth(), ofGetHeight(), 0,
		   ofGetWidth(), ofGetHeight(), -600,
		   0, ofGetHeight(), -600
   };

   glVertexPointer(3, GL_FLOAT, 0, vertices2);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

   //lets tumble the world with the mouse
   glPushMatrix();

       //draw in middle of the screen
       glTranslatef(ofGetWidth()/2,ofGetHeight()/2,0);
       //tumble according to mouse
       glRotatef(-mouseY,1,0,0);
       glRotatef(mouseX,0,1,0);
       glTranslatef(-ofGetWidth()/2,-ofGetHeight()/2,0);

       ofSetColor(255, 255, 255, 255);
       squirrelModel.draw();

   glPopMatrix();

   ofSetHexColor(0x000000);
   ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}
