#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);    
	
    // we need GL_TEXTURE_2D for our models coords.
    ofDisableArbTex();

    loaded = false;
    if(model.loadModel("astroboy_walk.dae",true)){
    	model.setAnimation(0);
    	model.setPosition(ofGetWidth() * 0.5, ofGetHeight() * 0.5 , 0);
    }

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	glEnable(GL_DEPTH_TEST);

    //some model / light stuff
    glShadeModel(GL_SMOOTH);
    light.enable();
    ofEnableSeparateSpecularLight();

	
	bAnimate		= false;
	bAnimateMouse 	= false;
	animationTime	= 0.0;
}

//--------------------------------------------------------------
void testApp::update(){
    
	//this is for animation if the model has it. 
	if( bAnimate ){
		animationTime += ofGetLastFrameTime();
		if( animationTime >= 1.0 ){
			animationTime = 0.0;
		}
	    model.setNormalizedTime(animationTime);
	}


	if( bAnimateMouse ){
	    model.setNormalizedTime(animationTime);
	}

	model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(50, 50, 50, 0);
    ofSetColor(255, 255, 255, 255);
    

    ofPushMatrix();
		ofTranslate(model.getPosition().x, model.getPosition().y, 0);
		ofRotate(-mouseX, 0, 1, 0);
		ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);

		model.drawFaces();
            
    ofPopMatrix();


    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
    ofDrawBitmapString("drag to control animation + rotation", 10, 45);
    ofDrawBitmapString("num animations for this model: " + ofToString(model.getAnimationCount()), 10, 60);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	bAnimateMouse = true;
	animationTime = float(y)/float(ofGetHeight());
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	bAnimateMouse = false;

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	model.calculateDimensions();
	model.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.5 , 0);

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

