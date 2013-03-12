#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	angle = 0;
	oneShot = false;
	pdfRendering = false;

	ofBackground(225,225,225);
	ofSetVerticalSync(true);
	
	font.loadFont("frabk.ttf", 24, true, false, true);
	
	dropZoneRects.assign(3, ofRectangle());

	images.assign(3, ofImage());
	images[0].loadImage("DSC09316.jpeg");
	
	for(unsigned int k = 0; k < dropZoneRects.size(); k++){
		dropZoneRects[k] = ofRectangle(32 + k * 310, 200, 300, 200);
	}
}

//--------------------------------------------------------------
void testApp::update(){
	angle++;
}

//--------------------------------------------------------------
void testApp::draw(){
	if( oneShot ){
		ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
	}
	
	ofSetColor(54);
	ofDrawBitmapString("PDF OUTPUT EXAMPLE", 32, 32);
	if( pdfRendering ){
		ofDrawBitmapString("press r to stop pdf multipage rendering", 32, 92);
	}else{	
		ofDrawBitmapString("press r to start pdf multipage rendering\npress s to save a single screenshot as pdf to disk", 32, 92);
	}
		
		
	ofFill();		
	ofSetColor(54,54,54);
	ofDrawBitmapString("TTF Font embdedded into pdf as vector shapes", 32, 460);
	
	if( oneShot || pdfRendering ){
		font.drawStringAsShapes("Current Frame: ",  32, 500);
		ofSetColor(245, 58, 135);
		font.drawStringAsShapes( ofToString(ofGetFrameNum()), 32 + font.getStringBoundingBox("Current Frame: ", 0, 0).width + 9, 500);
	}else{
		font.drawString("Current Frame: ",  32, 500);	
		ofSetColor(245, 58, 135);		
		font.drawString( ofToString(ofGetFrameNum()), 32 + font.getStringBoundingBox("Current Frame: ", 0, 0).width + 9, 500);		
	}
	
	
	ofSetColor(54,54,54);
	ofDrawBitmapString("Images can also be embedded into pdf", 32, dropZoneRects[0].y - 18);
	
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofNoFill();
	for(unsigned int k = 0; k < dropZoneRects.size(); k++){
		ofSetColor(54,54,54);
		ofRect(dropZoneRects[k]);
		ofSetColor(245, 58, 135);		
		ofDrawBitmapString("drop images here", dropZoneRects[k].getCenter().x - 70, dropZoneRects[k].getCenter().y);
	}

	ofSetColor(255);
	for(unsigned int j = 0; j < images.size(); j ++){
		if( images[j].width > 0 ){
			
			float tw = 300;
			float th = 200;
			
			if( images[j].getWidth() / images[j].getHeight() < tw / th ){
				tw = th * ( images[j].getWidth() / images[j].getHeight() );
			}else{
				th = tw * ( images[j].getHeight() / images[j].getWidth() );			
			}
			
			images[j].draw(dropZoneRects[j].x, dropZoneRects[j].y, tw, th);
			
		}
	}
	
	//lets draw a box with a trail
	ofSetColor(245, 58, 135);
	
	ofRectangle boxBounds(32, 500, ofGetWidth()-32, 250);
	
	//lets get a noise value based on the current frame
	float noiseX = ofNoise(float(ofGetFrameNum())/600.f, 200.0f);
	float noiseY = ofNoise(float(ofGetFrameNum())/800.f, -900.0f);

	ofNoFill();
	ofBeginShape();
	ofVertices(boxTrail);
	ofEndShape(false);
	
	ofFill();
	ofSetRectMode(OF_RECTMODE_CENTER);

	ofPushMatrix();
		float x = ofMap( noiseX, 0, 1, boxBounds.x, boxBounds.x + boxBounds.width, true);
		float y = ofMap( noiseY, 0, 1, boxBounds.y, boxBounds.y + boxBounds.height, true);

		ofTranslate(x, y, 0);
		ofRotate(angle);
		ofRect(0, 0, 30, 30);
	ofPopMatrix();	
	
	if( boxTrail.size() == 0 || ( boxTrail.back() - ofPoint(x, y) ).length() > 1.5 ){
		boxTrail.push_back(ofPoint(x, y));
	}
	
	if(boxTrail.size() > 800 ){
		boxTrail.erase(boxTrail.begin(), boxTrail.begin()+1);
	}
	
	if( oneShot ){
		ofEndSaveScreenAsPDF();
		oneShot = false;
	}	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if( key=='r'){
		pdfRendering = !pdfRendering;	
		if( pdfRendering ){
			ofSetFrameRate(12);  // so it doesn't generate tons of pages
			ofBeginSaveScreenAsPDF("recording-"+ofGetTimestampString()+".pdf", true);
		}else{
			ofSetFrameRate(60);
			ofEndSaveScreenAsPDF();		
		}
	}
	
	if( !pdfRendering && key == 's' ){
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
	for(unsigned int j = 0; j < dropZoneRects.size(); j++){
		if( dropZoneRects[j].inside( dragInfo.position ) ){
			images[j].loadImage( dragInfo.files[0] );
			break;
		}
	}
}


