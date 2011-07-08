#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);

    // we need GL_TEXTURE_2D for our models coords.
    ofDisableArbTex();

    if(model.loadModel("astroBoy_walk.dae",true)){
    	model.setAnimation(0);
    	model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
    	//model.createLightsFromAiModel();
    	//model.disableTextures();
    	//model.disableMaterials();

    	mesh = model.getMesh(0);
    	position = model.getPosition();
    	normScale = model.getNormalizedScale();
    	scale = model.getScale();
    	sceneCenter = model.getSceneCenter();
    	material = model.getMaterialForMesh(0);
        tex = model.getTextureForMesh(0);
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
    //model.setRotation(1, ofGetElapsedTimef() * 60, 0, 1, 0);

	//this is for animation if the model has it.
	if( bAnimate ){
		animationTime += ofGetLastFrameTime();
		if( animationTime >= 1.0 ){
			animationTime = 0.0;
		}
	    model.setNormalizedTime(animationTime);
		mesh = model.getCurrentAnimatedMesh(0);
	}


	if( bAnimateMouse ){
	    model.setNormalizedTime(animationTime);
		mesh = model.getCurrentAnimatedMesh(0);
	}


}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(50, 50, 50, 0);
    ofSetColor(255, 255, 255, 255);

    ofPushMatrix();
		ofTranslate(model.getPosition().x+100, model.getPosition().y, 0);
		ofRotate(-mouseX, 0, 1, 0);
		ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);

		model.drawFaces();

    ofPopMatrix();


    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_CULL_FACE);
    ofPushMatrix();

		ofTranslate(model.getPosition().x-300, model.getPosition().y, 0);
		ofRotate(-mouseX, 0, 1, 0);
		ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);

		ofTranslate(position);
		ofRotate(180, 0, 0, 1);
		ofTranslate(-sceneCenter.x, -sceneCenter.y, sceneCenter.z);


		ofScale(normScale , normScale, normScale);


		ofScale(scale.x,scale.y,scale.z);

		tex.bind();
		material.begin();
		mesh.drawWireframe();
		material.end();
		tex.unbind();

	ofPopMatrix();

	glPopAttrib();


    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
    ofDrawBitmapString("keys 1-5 load models, spacebar to trigger animation", 10, 30);
    ofDrawBitmapString("drag to control animation with mouseY", 10, 45);
    ofDrawBitmapString("num animations for this model: " + ofToString(model.getAnimationCount()), 10, 60);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case '1':
            model.loadModel("astroBoy_walk.dae");
            model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
            ofEnableSeparateSpecularLight();
            break;
        case '2':
            model.loadModel("TurbochiFromXSI.dae");
            model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
            model.setRotation(0,90,1,0,0);
            ofEnableSeparateSpecularLight();
            break;
        case '3':
            model.loadModel("dwarf.x");
            model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
            ofDisableSeparateSpecularLight();
            break;
        case '4':
            model.loadModel("monster-animated-character-X.X");
            model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
            ofDisableSeparateSpecularLight();
            break;
		case '5':
			model.loadModel("squirrel/NewSquirrel.3ds");
		    model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
            model.setRotation(0,-90,1,0,0);
            ofDisableSeparateSpecularLight();
			break;
		case ' ':
			bAnimate = !bAnimate;
			break;
        default:
            break;
    }


	mesh = model.getMesh(0);
	position = model.getPosition();
	normScale = model.getNormalizedScale();
	scale = model.getScale();
	sceneCenter = model.getSceneCenter();
	material = model.getMaterialForMesh(0);
    tex = model.getTextureForMesh(0);

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
	animationTime = float(y)/float(ofGetWidth());
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

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

