#include "ofApp.h"

GLfloat lightOnePosition[] = {40.0, 40, 100.0, 0.0};
GLfloat lightOneColor[] = {0.99, 0.99, 0.99, 1.0};

GLfloat lightTwoPosition[] = {-40.0, 40, 100.0, 0.0};
GLfloat lightTwoColor[] = {0.99, 0.99, 0.99, 1.0};

//--------------------------------------------------------------
void ofApp::setup(){	
	ofBackground(255,255,255);
		
	ofSetVerticalSync(true);

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
    glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
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
    glBegin(GL_QUADS);
        glVertex3f(0.0, ofGetHeight(), -600);
        glVertex3f(ofGetWidth(), ofGetHeight(), -600);
        glVertex3f(ofGetWidth(), 0, -600);
        glVertex3f(0, 0, -600);
    glEnd();

    //fake wall
    ofSetColor(50, 50, 50);
    glBegin(GL_QUADS);
        glVertex3f(0.0, ofGetHeight(), 0);
        glVertex3f(ofGetWidth(), ofGetHeight(), 0);
        glVertex3f(ofGetWidth(), ofGetHeight(), -600);
        glVertex3f(0, ofGetHeight(), -600);
    glEnd();

    //lets tumble the world with the mouse
    glPushMatrix();

        //draw in middle of the screen
        glTranslatef(ofGetWidth()/2,ofGetHeight()/2,0);
        //tumble according to mouse
        glRotatef(-mouseY,1,0,0);
        glRotatef(mouseX,0,1,0);
        glTranslatef(-ofGetWidth()/2,-ofGetHeight()/2,0);

        ofSetColor(255, 255, 255, 255);
        squirrelModel.drawFaces();

    glPopMatrix();

    ofSetHexColor(0x000000);
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ 

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
