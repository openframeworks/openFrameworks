#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(50, 0);
    
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    
	glEnable(GL_DEPTH_TEST);
    
    glShadeModel(GL_SMOOTH); //some model / light stuff
    light.enable();
    ofEnableSeparateSpecularLight();
    
    modelIndex = 0;
    modelsTotal = 5;
    loadModel(modelIndex);
}

void testApp::loadModel(int modelIndex) {
    ofPoint modelPosition(ofGetWidth() / 2, (float)ofGetHeight() * 0.75 , 0);
    
    switch(modelIndex){
        case 0:
            model.loadModel("astroBoy_walk.dae");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            ofEnableSeparateSpecularLight();
            break;
        case 1:
            model.loadModel("TurbochiFromXSI.dae");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            model.setRotation(0, -180, 1, 0, 0);
            model.setScale(1.2, 1.2, 1.2);
            ofEnableSeparateSpecularLight();
            break;
        case 2:
            model.loadModel("dwarf.x");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            model.setScale(1.2, 1.2, 1.2);
            ofDisableSeparateSpecularLight();
            break;
        case 3:
            model.loadModel("monster-animated-character-X.X");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            model.setRotation(0, -90, 0, 0, 1);
            ofDisableSeparateSpecularLight();
            break;
        case 4:
            model.loadModel("squirrel/NewSquirrel.3ds");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            ofDisableSeparateSpecularLight();
            break;
        default:
            break;
    }
    
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
}

//--------------------------------------------------------------
void testApp::update(){
    model.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255);
    ofEnableAlphaBlending();
    
	//note we have to enable depth buffer in main.mm
	//see: window->enableDepthBuffer(); in main.mm

	glEnable(GL_DEPTH_TEST);	
    ofPushMatrix();
    ofTranslate(model.getPosition().x, model.getPosition().y, 0);
    ofRotate(-mouseX, 0, 1, 0);
    ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    model.drawFaces();
    ofPopMatrix();

    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(), 2), 10, 15);
    ofDrawBitmapString("num animations for this model: " + ofToString(model.getAnimationCount()), 10, 30);
    ofDrawBitmapString("double tap to change model", 10, 60);
}

//--------------------------------------------------------------
void testApp::exit(){
    
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){
    if(++modelIndex > modelsTotal - 1){
        modelIndex = 0;
	}
    loadModel(modelIndex);
}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::lostFocus(){
    
}

//--------------------------------------------------------------
void testApp::gotFocus(){
    
}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
    
}

