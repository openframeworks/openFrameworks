#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	pdfGrabOneFrame = false;
	pdfRecordMultipleFrames = false;

	ofBackground(255,255,255); // white background
	ofEnableAlphaBlending(); // needs to be enabled to also see the transparency of the rectangle which is set to 20 in this example
	
	ofSetFrameRate(60);
	
	// load the font from the data folder
	// PARAMETERS ===> myFont: frabk.ttf, size: 24 pixels, antialiasing: true, fullCharacterSet: false, makeContours: true);
	// we need makeContours to be true so we can save vectors for the text in the pdf rather than myImages of the characters of the text
	myFont.loadFont("frabk.ttf", 24, true, false, true); 
	
	// loads the image from the data folder
	myImage.loadImage("DSC09316.jpeg"); 
	
	// the coordinates of the rectangle are set to the center of the window at the beginning
	x = ofGetWidth()/2.;
	y = ofGetHeight()/2.;
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	// move the coordinates of the rectangle by a random number between -5 and +5 pixels.
	x += ofRandom(-5, 5); 
	y += ofRandom(-5, 5); 
	
	
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	if( pdfGrabOneFrame ){
		ofBeginSaveScreenAsPDF("outputSinglePage-"+ofGetTimestampString()+".pdf", false);
	}

	
	ofSetColor(255, 0, 0); // red
	ofFill(); 
		
	
	// -------------------------------------------------- Title "PDF OUTPUT EXAMPLE"
	ofDrawBitmapString("PDF OUTPUT EXAMPLE", 20, 20); // 20px from the left, 20px from the top
	
	
	// -------------------------------------------------- draw fonts
	ofSetColor(0, 0, 255); // blue
	ofFill(); 
	// Font as shapes is always not antialiased on the screen, but will be fine in the pdf"
	myFont.drawStringAsShapes("Font as shapes: looks good in pdf, looks bad on screen (always)",  20, 100); // TTF myFont embdedded into pdf as vector shapes
	myFont.drawString("Font as images: looks good on screen, looks bad in pdf",  20, 150); // TTF myFont embdedded into pdf as bitmap image
	
	// -------------------------------------------------- Title "images can also be embedded into pdf"
	ofSetColor(0, 0, 0); // black
	ofDrawBitmapString("images can also be embedded into pdf", 20, 200);
	
	// -------------------------------------------------- draw the image
	ofSetColor(255, 255, 255); // a color needs to be set for the image as well, white will draw the image as it is.
	myImage.draw(20, 250, myImage.getWidth(), myImage.getHeight());
	
	
	
	// -------------------------------------------------- draw the rectangle
	ofSetColor(0, 0, 255, 20); // blue with an alpha value of 20
	ofFill();
	ofRect(x, y, 10, 10);
	
	
	// -------------------------------------------------- instructions on the bottom on which key to press
	ofSetColor(255, 0, 0); // red
	ofFill();
	
	if( pdfRecordMultipleFrames ){
		ofDrawBitmapString("press r or a to stop pdf recording", 20, ofGetHeight()-100); // 20px from the left, -100 from the bottom
	}else{	
		ofDrawBitmapString("press r to start pdf recording: outputMultiFramesToMultiPages \n"
						   "press a to start pdf recording: outputMultiFramesToSinglePage \n"
						   "press s to save a single screenshot as pdf to disk"
						   , 20, ofGetHeight()-100); // 20px from the left, -100 from the bottom
	}
	
	
	if( pdfGrabOneFrame ){
		ofEndSaveScreenAsPDF();
		pdfGrabOneFrame = false;
	}	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if( key=='r'){
		if(pdfRecordMultipleFrames){ // if we are already in pdfRecordMultipleFrames mode
			// we END a recording
		    ofEndSaveScreenAsPDF();	// stop the current recording
			pdfRecordMultipleFrames = false;
			ofSetFrameRate(60);
			ofSetBackgroundAuto(true); // just in case ofSetBackgroundAuto was set false with the 'a' key
		}else{ // we are NOT in pdfRecordMultipleFrames mode
			// we START a recording
			ofSetFrameRate(12);  // so it doesn't generate tons of pages
			ofBeginSaveScreenAsPDF("outputMultiFramesToMultiPages-"+ofGetTimestampString()+".pdf", true); // Multipage: true
			pdfRecordMultipleFrames = true;
		}
	}
	
	
		
	if( key=='a'){ // a for ALLinOne ;)
		if(pdfRecordMultipleFrames){ // if we are already in pdfRecordMultipleFrames mode
			// we END a recording
		    ofEndSaveScreenAsPDF();	// stop the current recording
			pdfRecordMultipleFrames = false;
			ofSetFrameRate(60);
			ofSetBackgroundAuto(true); // just in case ofSetBackgroundAuto was set false with the 'a' key
		}else{ // we are NOT in pdfRecordMultipleFrames mode
			// we START a recording
			ofSetFrameRate(12);  // so it doesn't generate tons of pages
			
			// if ofSetBackgroundAuto is set to true, the background is cleared automatically every frame so that we only see the very last frame,
			// but we want to see all of the frames: the rectangle will leave trails on the screen this way
			// IMPORTANT: the font in the pdf will just _look_ BAD in a preview programme, because it is drawn in the same location on top of itself multiple times (many many times!)
			ofSetBackgroundAuto(false);

			
			ofBeginSaveScreenAsPDF("outputMultiFramesToSinglePage-"+ofGetTimestampString()+".pdf", false); // Multipage: false ===> everything goes into one page
			pdfRecordMultipleFrames = true;
		}
	}
	
	
	if( !pdfRecordMultipleFrames && key == 's' ){
		pdfGrabOneFrame = true;
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


