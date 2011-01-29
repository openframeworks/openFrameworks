#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);    
	
    // we need GL_TEXTURE_2D for our models coords.
    ofDisableArbTex();

    model.loadModel("astroBoy_walk.dae");
    model.setAnimation(0);
    model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
  
	glEnable(GL_DEPTH_TEST);
    
    //some model / light stuff
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	
	bAnimate		= false;
	animationTime	= 0.0;
}

//--------------------------------------------------------------
void testApp::update(){
    //model.setRotation(1, ofGetElapsedTimef() * 60, 0, 1, 0);
    
	//this is for animation if the model has it. 
	if( bAnimate ){
		animationTime += ofGetLastFrameTime();
		if( animationTime >= 1.0 ){
			animationTime = 0.0;
		}
	}
    model.setNormalizedTime(animationTime);
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(50, 50, 50, 0);
    ofSetColor(255, 255, 255, 255);
    
    ofPushMatrix();
		ofTranslate(model.getPosition().x, model.getPosition().y, 0);
		ofRotate(mouseY, 1, 0, 0);
		ofRotate(-mouseX, 0, 0, 1);		
		ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    
		model.draw();
            
    ofPopMatrix();
    
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
    ofDrawBitmapString("keys 1-4 load models, spacebar to trigger animation", 10, 30);	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case '1':
            model.loadModel("astroBoy_walk.dae");
            break;
        case '2':
            model.loadModel("TurbochiFromXSI.dae");
            break;
        case '3':
            model.loadModel("dwarf.x");
            break;
        case '4':
            model.loadModel("monster-animated-character-X.X");
            break;
		case '5':
			model.loadModel("squirrel/NewSquirrel.3ds");
			break;
		case ' ':
			bAnimate = !bAnimate;
			break;
        default:
            break;
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

