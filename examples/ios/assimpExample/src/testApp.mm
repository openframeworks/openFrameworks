#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(50, 0);
    
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    
    model.loadModel("astroBoy_walk.dae", true);
    model.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0);
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    
	glEnable(GL_DEPTH_TEST);
    
    glShadeModel(GL_SMOOTH); //some model / light stuff
    light.enable();
    ofEnableSeparateSpecularLight();
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
    ofDrawBitmapString("fingers 2-5 load models", 10, 30);
    ofDrawBitmapString("num animations for this model: " + ofToString(model.getAnimationCount()), 10, 45);
}

//--------------------------------------------------------------
void testApp::exit(){
    
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){
	if(touch.id >= 1){
		switch(touch.id){
			case 1:
				model.loadModel("dwarf.x");
				model.setPosition(ofGetWidth() / 2, (float)ofGetHeight() * 0.75 , 0);
				model.setScale(1.2, 1.2, 1.2);	
				ofDisableSeparateSpecularLight();
				break;
			case 2:
				model.loadModel("TurbochiFromXSI.dae");
				model.setPosition(ofGetWidth() / 2, (float)ofGetHeight() * 0.75 , 0);
				model.setRotation(0,-180,1,0,0);
				model.setScale(1.2, 1.2, 1.2);
				ofEnableSeparateSpecularLight();
				break;				
			case 3:
				model.loadModel("squirrel/NewSquirrel.3ds");
				model.setPosition(ofGetWidth() / 2, (float)ofGetHeight() * 0.75 , 0);
				ofDisableSeparateSpecularLight();
				break;
			case 4:
				model.loadModel("astroBoy_walk.dae");
				model.setPosition(ofGetWidth() / 2, (float)ofGetHeight() * 0.75 , 0);
				ofEnableSeparateSpecularLight();
				break;
			default:
				break;
		}
	}
    
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){

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

