#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
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

    glEnable(GL_NORMALIZE);

    ofPushMatrix();
    ofTranslate(model.getPosition().x-300, model.getPosition().y, 0);
    ofRotate(-mouseX, 0, 1, 0);
    ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);

    ofxAssimpMeshHelper & meshHelper = model.getMeshHelper(0);

    ofMultMatrix(model.getModelMatrix());
    ofMultMatrix(meshHelper.matrix);

    ofMaterial & material = meshHelper.material;
    if(meshHelper.hasTexture()){
        meshHelper.getTexturePtr()->bind();
    }
    material.begin();
    mesh.drawWireframe();
    material.end();
    if(meshHelper.hasTexture()){
        meshHelper.getTexturePtr()->unbind();
    }
    ofPopMatrix();


    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
    ofDrawBitmapString("keys 1-5 load models, spacebar to trigger animation", 10, 30);
    ofDrawBitmapString("drag to control animation with mouseY", 10, 45);
    ofDrawBitmapString("num animations for this model: " + ofToString(model.getAnimationCount()), 10, 60);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    //
}

//--------------------------------------------------------------
void testApp::touchMoved(int x, int y, int id){
    // scrub through aninations manually.
	animationPosition = y / (float)ofGetHeight();

}

//--------------------------------------------------------------
void testApp::touchDown(int x, int y, int id){
    // pause all animations, so we can scrub through them manually.
    model.setPausedForAllAnimations(true);
	animationPosition = y / (float)ofGetHeight();
    bAnimateMouse = true;
}

//--------------------------------------------------------------
void testApp::touchUp(int x, int y, int id){
    // unpause animations when finished scrubbing.
    if(bAnimate) {
        model.setPausedForAllAnimations(false);
    }
    bAnimateMouse = false;
}

void testApp::touchDoubleTap(int x, int y, int id){

}

void testApp::touchCancelled(int x, int y, int id){

}

void testApp::swipe(ofxAndroidSwipeDir swipeDir, int id){
}

void testApp::pause(){

}

void testApp::stop(){

}

void testApp::resume(){

}

void testApp::reloadTextures(){

}


bool testApp::backPressed(){
	return false;
}

void testApp::okPressed(){

}

void testApp::cancelPressed(){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

