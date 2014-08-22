#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(10, 10, 10);
	ofEnableDepthTest();
    
    // turn on smooth lighting //
    bSmoothLighting     = true;
    ofSetSmoothLighting(true);
    
    // lets make a high-res sphere //
    // default is 20 //
    ofSetSphereResolution(128);
	
    // radius of the sphere //
	radius		= 180.f;
	center.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);
    
    // Point lights emit light in all directions //
    // set the diffuse color, color reflected from the light source //
    pointLight.setDiffuseColor( ofColor(0.f, 255.f, 0.f));
    
    // specular color, the highlight/shininess color //
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 0.f));
	pointLight.setPointLight();
    
    
	
    spotLight.setDiffuseColor( ofColor(255.f, 0.f, 0.f));
	spotLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    
    // turn the light into spotLight, emit a cone of light //
    spotLight.setSpotlight();
    
    // size of the cone of emitted light, angle between light axis and side of cone //
    // angle range between 0 - 90 in degrees //
    spotLight.setSpotlightCutOff( 50 );
    
    // rate of falloff, illumitation decreases as the angle from the cone axis increases //
    // range 0 - 128, zero is even illumination, 128 is max falloff //
    spotLight.setSpotConcentration( 45 );
    
	
    // Directional Lights emit light based on their orientation, regardless of their position //
	directionalLight.setDiffuseColor(ofColor(0.f, 0.f, 255.f));
	directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	directionalLight.setDirectional();
    
    // set the direction of the light
    // set it pointing from left to right -> //
	directionalLight.setOrientation( ofVec3f(0, 90, 0) );
    
	
	bShiny = true;
    // shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess( 120 );
    // the light highlight of the material //
	material.setSpecularColor(ofColor(255, 255, 255, 255));
	
	bPointLight = bSpotLight = bDirLight = true;
    
    // tex coords for 3D objects in OF are from 0 -> 1, not 0 -> image.width
    // so we must disable the arb rectangle call to allow 0 -> 1
    ofDisableArbTex();
    // load an image to use as the texture //
    ofLogoImage.loadImage("of.png");
    bUseTexture = true;
}

//--------------------------------------------------------------
void ofApp::update() {
    pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x, 
						   sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y, 
						   -cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);
	
    spotLight.setOrientation( ofVec3f( 0, cos(ofGetElapsedTimef()) * RAD_TO_DEG, 0) );
	spotLight.setPosition( mouseX, mouseY, 200);
}

//--------------------------------------------------------------
void ofApp::draw(){
    // enable lighting //
    ofEnableLighting();
    // enable the material, so that it applies to all 3D objects before material.end() call //
	material.begin();
    // activate the lights //
	if (bPointLight) pointLight.enable();
	if (bSpotLight) spotLight.enable();
	if (bDirLight) directionalLight.enable();
    
    // grab the texture reference and bind it //
    // this will apply the texture to all drawing (vertex) calls before unbind() //
    if(bUseTexture) ofLogoImage.getTextureReference().bind();
    
	ofSetColor(255, 255, 255, 255);
    ofPushMatrix();
    ofTranslate(center.x, center.y, center.z-300);
    ofRotate(ofGetElapsedTimef() * .8 * RAD_TO_DEG, 0, 1, 0);
	ofDrawSphere( 0,0,0, radius);
    ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(300, 300, cos(ofGetElapsedTimef()*1.4) * 300.f);
	ofRotate(ofGetElapsedTimef()*.6 * RAD_TO_DEG, 1, 0, 0);
	ofRotate(ofGetElapsedTimef()*.8 * RAD_TO_DEG, 0, 1, 0);
	ofDrawBox(0, 0, 0, 60);
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(center.x, center.y, -900);
	ofRotate(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
	ofDrawBox( 0, 0, 0, 850);
	ofPopMatrix();
    
    if(bUseTexture) ofLogoImage.getTextureReference().unbind();
	
	if (!bPointLight) pointLight.disable();
	if (!bSpotLight) spotLight.disable();
	if (!bDirLight) directionalLight.disable();
	
    material.end();
	// turn off lighting //
    ofDisableLighting();
    
	ofSetColor( pointLight.getDiffuseColor() );
	if(bPointLight) pointLight.draw();
    
    ofSetColor(255, 255, 255);
	ofSetColor( spotLight.getDiffuseColor() );
	if(bSpotLight) spotLight.draw();
	
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Point Light On (1) : "+ofToString(bPointLight) +"\n"+
					   "Spot Light On (2) : "+ofToString(bSpotLight) +"\n"+
					   "Directional Light On (3) : "+ofToString(bDirLight)+"\n"+
					   "Shiny Objects On (s) : "+ofToString(bShiny)+"\n"+
                       "Spot Light Cutoff (up/down) : "+ofToString(spotLight.getSpotlightCutOff(),0)+"\n"+
                       "Spot Light Concentration (right/left) : " + ofToString(spotLight.getSpotConcentration(),0)+"\n"+
                       "Smooth Lighting enabled (x) : "+ofToString(bSmoothLighting,0)+"\n"+
                       "Textured (t) : "+ofToString(bUseTexture,0),
					   20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
		case '1':
			bPointLight = !bPointLight;
			break;
		case '2':
			bSpotLight = !bSpotLight;
			break;
		case '3':
			bDirLight = !bDirLight;
			break;
		case 's':
			bShiny	= !bShiny;
			if (bShiny) material.setShininess( 120 );
			else material.setShininess( 30 );
			break;
        case 'x':
            bSmoothLighting = !bSmoothLighting;
            ofSetSmoothLighting(bSmoothLighting);
            break;
        case 't':
            bUseTexture = !bUseTexture;
            break;
        case OF_KEY_UP:
            // setSpotlightCutOff is clamped between 0 - 90 degrees //
            spotLight.setSpotlightCutOff(spotLight.getSpotlightCutOff()+1);
            break;
        case OF_KEY_DOWN:
            // setSpotlightCutOff is clamped between 0 - 90 degrees //
            spotLight.setSpotlightCutOff(spotLight.getSpotlightCutOff()-1);
            break;
        case OF_KEY_RIGHT:
            // setSpotConcentration is clamped between 0 - 128 //
            spotLight.setSpotConcentration(spotLight.getSpotConcentration()+1);
            break;
        case OF_KEY_LEFT:
            // setSpotConcentration is clamped between 0 - 128 //
            spotLight.setSpotConcentration(spotLight.getSpotConcentration()-1);
            break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}