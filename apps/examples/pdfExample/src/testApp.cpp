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
	oneShot = false;

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
	if( pdfRendering ){
		ofDrawBitmapString("press p to stop pdf multipage rendering",20,20);
	}else{	
		ofDrawBitmapString("press p to start pdf multipage rendering\npress s to save a single screenshot as pdf to disk",20,20);
	}
	
	if( oneShot ){
		cairoTmp->close();
		oneShot = false;
		ofSetCurrentRenderer(gl);
	}
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
	if( !pdfRendering && key == 's' ){
		cairoTmp.reset();
		
		cairoTmp = ofPtr<ofCairoRenderer>(new ofCairoRenderer);
		cairoTmp->setup(ofGetTimestampString() + ".pdf", ofCairoRenderer::PDF, false); 
		
		ofSetCurrentRenderer(cairoTmp);
		oneShot = true;	
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
