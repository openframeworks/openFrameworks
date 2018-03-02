#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(50, 0);

    bAnimate = false;
    bAnimateMouse = false;
    animationPosition = 0;
    currentModel = 0;

    model.loadModel("astroBoy_walk.dae", true);
    model.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0);
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    }

    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofDisableAlphaBlending();

	ofEnableDepthTest();

    glShadeModel(GL_SMOOTH); //some model / light stuff
    light.enable();
    ofEnableSeparateSpecularLight();
}

//--------------------------------------------------------------
void ofApp::update(){
    model.update();

    if(bAnimateMouse) {
        model.setPositionForAllAnimations(animationPosition);
    }

    mesh = model.getCurrentAnimatedMesh(0);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);

    ofPushMatrix();
    ofTranslate(model.getPosition().x+100, model.getPosition().y, 0);
    ofRotateDeg(-mouseX, 0, 1, 0);
    ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    model.drawFaces();
    ofPopMatrix();

    glEnable(GL_NORMALIZE);

    ofPushMatrix();
    ofTranslate(model.getPosition().x-300, model.getPosition().y, 0);
    ofRotateDeg(-mouseX, 0, 1, 0);
    ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);

    ofxAssimpMeshHelper & meshHelper = model.getMeshHelper(0);

    ofMultMatrix(model.getModelMatrix());
    ofMultMatrix(meshHelper.matrix);

    ofMaterial & material = meshHelper.material;
    if(meshHelper.hasTexture()){
        meshHelper.getTextureRef().bind();
    }
    material.begin();
    mesh.drawWireframe();
    material.end();
    if(meshHelper.hasTexture()){
        meshHelper.getTextureRef().unbind();
    }
    ofPopMatrix();


    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
    ofDrawBitmapString("keys 1-5 load models, spacebar to trigger animation", 10, 30);
    ofDrawBitmapString("drag to control animation with mouseY", 10, 45);
    ofDrawBitmapString("num animations for this model: " + ofToString(model.getAnimationCount()), 10, 60);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    //
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
    // scrub through aninations manually.
	animationPosition = y / (float)ofGetHeight();

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){
    // pause all animations, so we can scrub through them manually.
    model.setPausedForAllAnimations(true);
	animationPosition = y / (float)ofGetHeight();
    bAnimateMouse = true;
}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
    // unpause animations when finished scrubbing.
    if(bAnimate) {
        model.setPausedForAllAnimations(false);
    }
    bAnimateMouse = false;
}

void ofApp::touchDoubleTap(int x, int y, int id){

}

void ofApp::touchCancelled(int x, int y, int id){

}

void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){
}

void ofApp::pause(){

}

void ofApp::stop(){

}

void ofApp::resume(){

}

void ofApp::reloadTextures(){
    model.loadModel("astroBoy_walk.dae", true);
    model.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0);
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    }
}


bool ofApp::backPressed(){
	return false;
}

void ofApp::okPressed(){

}

void ofApp::cancelPressed(){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

