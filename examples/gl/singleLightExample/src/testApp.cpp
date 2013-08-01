#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(10, 10, 10);
	ofEnableDepthTest();
    
    // turn on smooth lighting //
    ofSetSmoothLighting(true);
    
    // lets make a sphere with more resolution than the default //
    // default is 20 //
    ofSetSphereResolution(32);
	
	radius		= 300.f;
	center.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);
    
    // Point lights emit light in all directions //
    // set the diffuse color, color reflected from the light source //
    pointLight.setDiffuseColor( ofColor(0.f, 255.f, 0.f));
    
    // specular color, the highlight/shininess color //
	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    pointLight.setPosition(center.x, center.y, 0);
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
	material.setShininess( 64 );
    
    
    sphereRadius    = 140;
    numSpheres      = 8;
    rotation        = 0.f;
    bDrawWireframe  = false;
    
    colorHue = ofRandom(0, 250);
    
    lightColor.setBrightness( 180.f );
    lightColor.setSaturation( 150.f );
    
    materialColor.setBrightness(250.f);
    materialColor.setSaturation(200);
}

//--------------------------------------------------------------
void testApp::update() {
	colorHue += .1f;
    if(colorHue >= 255) colorHue = 0.f;
    lightColor.setHue(colorHue);
    
    rotation += 1;
    if(rotation >=360) rotation = 0;
    
    radius = cos(ofGetElapsedTimef()) * 200.f + 200.f;
    
    pointLight.setDiffuseColor(lightColor);
    
    
    materialColor.setHue(colorHue);
    // the light highlight of the material //
	material.setSpecularColor(materialColor);
}

//--------------------------------------------------------------
void testApp::draw() {
    
    ofSetColor(pointLight.getDiffuseColor());
    ofDrawSphere(pointLight.getPosition(), 20.f);
    
    // enable lighting //
    ofEnableLighting();
    // the position of the light must be updated every frame, 
    // call enable() so that it can update itself //
    pointLight.enable();
    material.begin();
    
    if(bDrawWireframe) ofNoFill();
    else ofFill();
    
    ofPushMatrix();
    ofTranslate(center.x, center.y, 0);
    ofRotate(rotation, 0, 0, 1);
	for(int i = 0; i < numSpheres; i++) {
        float angle = TWO_PI / (float)numSpheres * i;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;
        ofDrawSphere(x, y, -200, sphereRadius);
    }
    ofPopMatrix();
	material.end();
	// turn off lighting //
    ofDisableLighting();
    
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("Draw Wireframe (w) : "+ofToString(bDrawWireframe, 0), 20, 20);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 'w':
            bDrawWireframe = !bDrawWireframe;
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
    pointLight.setPosition(x, y, 0);
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