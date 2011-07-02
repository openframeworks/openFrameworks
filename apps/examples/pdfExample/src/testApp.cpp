#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	// get the ofPtr to the default gl renderer
	gl = ofGetGLRenderer();

	// create a cairo renderer and a collection renderer with both cairo and gl
	cairo = ofPtr<ofCairoRenderer>(new ofCairoRenderer);
	cairo->setup("flipbook.pdf",ofCairoRenderer::PDF,true);

	rendererCollection = ofPtr<ofRendererCollection>(new ofRendererCollection);

	rendererCollection->renderers.push_back(gl);
	rendererCollection->renderers.push_back(cairo);

	pdfRendering = false;
	angle = 0;

	ofBackground(255,255,255);
	ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::update(){
	angle++;
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255,0,0);
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofPushMatrix();

	float noise = ofNoise(float(ofGetFrameNum())/600.f);

	ofTranslate(noise*ofGetWidth(),noise*ofGetHeight());
	ofRotate(angle);
	ofTranslate(noise*-ofGetWidth(),noise*-ofGetHeight());

	ofRect(noise*ofGetWidth(),noise*ofGetHeight(),50,50);
	ofPopMatrix();

	ofSetColor(0,0,0);
	ofDrawBitmapString("press p to swap pdf/gl rendering",20,20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='p'){
		if(pdfRendering){
			ofSetCurrentRenderer(gl);
			ofSetFrameRate(60);
		}else{
			ofSetCurrentRenderer(rendererCollection);
			ofSetFrameRate(12);  // so it doesn't generate tons of pages
		}
		pdfRendering = !pdfRendering;
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
