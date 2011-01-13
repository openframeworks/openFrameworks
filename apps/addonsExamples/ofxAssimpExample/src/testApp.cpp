#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // we need GL_TEXTURE_2D for our models coords.
    ofDisableArbTex();
    //ofSetVerticalSync(true);

    //model.loadModel("astroBoy_walk.dae");

    model.loadModel("TurbochiFromXSI.dae");
    //model.loadModel("astroBoy_walk.dae");
    //model.loadModel("rally.obj");
    //model.loadModel("CARGT.3DS");
    //model.loadModel("Poseidon_compiled.OBJ");

    model.setAnimation(0);
    
    model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    glEnable(GL_DEPTH_TEST);
    
    //some model / light stuff
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

//--------------------------------------------------------------
void testApp::update(){

    // calling model.update() is important to allow animations to update their keyframe.
    model.setRotation(1, ofGetElapsedTimef() * 60, 0, 1, 0);
    
    float normalizedTime = ofMap(mouseX, 0,ofGetWidth(), 0, 1, true);

    model.setNormalizedTime(normalizedTime);
}



//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(50, 50, 50, 0);
        
    ofSetColor(255, 255, 255, 255);
    
    glPushMatrix();
    glTranslated(ofGetWidth()/2.0, ofGetHeight()/2, 0);
    glRotatef(mouseY, 1, 0, 0);
    glTranslated(-ofGetWidth()/2.0, -ofGetHeight()/2, 0);
    
    model.draw();
            
    glPopMatrix();
    
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch (key) {
        case 49:
            model.loadModel("astroBoy_walk.dae");
            //model.setScale(1, 1, 1);
            break;
        case 50:
            model.loadModel("TurbochiFromXSI.dae");
            //model.setScale(2, 2, 2);
            break;
        case 51:
            model.loadModel("dwarf.x");
            //model.setScale(3, 3, 3);
            break;
        case 52:
            model.loadModel("monster-animated-character-X.X");
            //model.setScale(4, 4, 4);
            break;
        case 53:
            //model.setScale(5, 5, 5);
            break;
        case 54:
            //model.setScale(6, 6, 6);
            break;
        case 55:
            //model.setScale(0.1, 0.1, 0.1);
            break;
            
        default:
            break;
    }
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

