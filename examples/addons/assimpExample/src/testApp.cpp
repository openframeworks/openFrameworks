#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(50, 0);

    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.

    bAnimate = false;
    bAnimateMouse = false;
    animationPosition = 0;
    
    model.loadModel("astroBoy_walk.dae", true);
    model.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0);
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    }
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH); //some model / light stuff
    light.enable();
    ofEnableSeparateSpecularLight();
}

//--------------------------------------------------------------
void testApp::update(){
    model.update();
    
    if(bAnimateMouse) {
        model.setPositionForAllAnimations(animationPosition);
    }

    mesh = model.getCurrentAnimatedMesh(0);
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255);

    ofPushMatrix();
    ofTranslate(model.getPosition().x+100, model.getPosition().y, 0);
    ofRotate(-mouseX, 0, 1, 0);
    ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    model.drawFaces();
    ofPopMatrix();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glEnable(GL_NORMALIZE);

    ofPushMatrix();
    ofTranslate(model.getPosition().x-300, model.getPosition().y, 0);
    ofRotate(-mouseX, 0, 1, 0);
    ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    
    ofxAssimpMeshHelper & meshHelper = model.getMeshHelper(0);
    
    ofMultMatrix(model.getModelMatrix());
    ofMultMatrix(meshHelper.matrix);
    
    ofMaterial & material = meshHelper.material;
    ofTexture & texture = meshHelper.texture;
    
    texture.bind();
    material.begin();
    mesh.drawWireframe();
    material.end();
    texture.unbind();
	ofPopMatrix();

	glPopAttrib();

    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
    ofDrawBitmapString("keys 1-5 load models, spacebar to trigger animation", 10, 30);
    ofDrawBitmapString("drag to control animation with mouseY", 10, 45);
    ofDrawBitmapString("num animations for this model: " + ofToString(model.getAnimationCount()), 10, 60);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    ofPoint modelPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75);
    switch (key) {
        case '1':
            model.loadModel("astroBoy_walk.dae");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            ofEnableSeparateSpecularLight();
            break;
        case '2':
            model.loadModel("TurbochiFromXSI.dae");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            model.setRotation(0, -180, 1, 0, 0);
            ofEnableSeparateSpecularLight();
            break;
        case '3':
            model.loadModel("dwarf.x");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            ofDisableSeparateSpecularLight();
            break;
        case '4':
            model.loadModel("monster-animated-character-X.X");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            model.setRotation(0, -90, 0, 0, 1);
            ofDisableSeparateSpecularLight();
            break;
		case '5':
			model.loadModel("squirrel/NewSquirrel.3ds");
		    model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            ofDisableSeparateSpecularLight();
			break;
		case ' ':
			bAnimate = !bAnimate;
			break;
        default:
            break;
    }

	mesh = model.getMesh(0);

    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    //
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    // scrub through aninations manually.
	animationPosition = y / (float)ofGetHeight();
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    // pause all animations, so we can scrub through them manually.
    model.setPausedForAllAnimations(true);
	animationPosition = y / (float)ofGetHeight();
    bAnimateMouse = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    // unpause animations when finished scrubbing.
    if(bAnimate) {
        model.setPausedForAllAnimations(false);
    }
    bAnimateMouse = false;
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

