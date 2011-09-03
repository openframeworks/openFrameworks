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

    //some model / light stuff
    glShadeModel(GL_SMOOTH);
    light.enable();
    ofEnableSeparateSpecularLight();

	bAnimate		= true;
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
    
	//note we have to enable depth buffer in main.mm
	//see: window->enableDepthBuffer(); in main.mm

	glEnable(GL_DEPTH_TEST);	
    ofPushMatrix();
		ofTranslate(model.getPosition().x, model.getPosition().y, 0);
		ofRotate(-mouseX, 0, 1, 0);
		ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    
		model.drawFaces();
    ofPopMatrix();

    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
    ofDrawBitmapString("fingers 2-5 load models", 10, 30);
    ofDrawBitmapString("num animations for this model: " + ofToString(model.getAnimationCount()), 10, 45);

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	
	if( touch.id >= 1 ){
		
		switch (touch.id) {
		
			case 1:
				model.loadModel("dwarf.x");
				model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
				model.setScale(1.2, 1.2, 1.2);	
				ofDisableSeparateSpecularLight();
				break;
			case 2:
				model.loadModel("TurbochiFromXSI.dae");
				model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
				model.setRotation(0,90,1,0,0);
				model.setScale(1.2, 1.2, 1.2);
				ofEnableSeparateSpecularLight();
				break;				
			case 3:
				model.loadModel("squirrel/NewSquirrel.3ds");
				model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
				model.setRotation(0,-90,1,0,0);
				ofDisableSeparateSpecularLight();
				break;
			case 4:
				model.loadModel("astroBoy_walk.dae");
				model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
				ofEnableSeparateSpecularLight();
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
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}



