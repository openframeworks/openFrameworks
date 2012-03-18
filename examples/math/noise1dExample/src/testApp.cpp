#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetWindowTitle("Noise 1D Example");
	ofBackground(215,215,215);
	ofSetVerticalSync(true);
	ofSetCircleResolution(256);
	
	setupSignedNoiseDemo();
}

//--------------------------------------------------------------
void testApp::setupSignedNoiseDemo(){
	// Setup and allocate resources used in the signed noise demo.
	
	nSignedNoiseData = 400; // we'll store a history of 400 numbers
	signedNoiseData = new float[nSignedNoiseData];
	for (int i=0; i<nSignedNoiseData; i++){
		signedNoiseData[i] = 0; 
	}
	
	// Some coordinates...
	radialNoiseDemoY = 200; 
	radialNoiseDemoR = 100;
	radialNoiseDemoX = ofGetWidth()/2 - radialNoiseDemoR; 
	
	// These 2 sliders control the noise waveform at the top. 
	radialNoiseStepSlider.setup(radialNoiseDemoX,   radialNoiseDemoY+150, 200,16, 0.010, 0.150, 0.05, false,true);
	radialNoiseAmountSlider.setup(radialNoiseDemoX, radialNoiseDemoY+170, 200,16, 0.000, 1.000, 0.40, false,true);
	
	radialNoiseStepSlider.setLabelString(  "Noise Step"); 
	radialNoiseAmountSlider.setLabelString("Noise Amount"); 
	radialNoiseStepSlider.setNumberDisplayPrecision(3); 
	radialNoiseCursor = 0.0;
}

//--------------------------------------------------------------
void testApp::update(){
	updateSignedNoiseDemo();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient( ofColor(255), ofColor(180), OF_GRADIENT_CIRCULAR);
	renderSignedNoiseDemo();
	renderNoisyRobotArmDemo();
}



//--------------------------------------------------------------
void testApp::updateSignedNoiseDemo (){
	
	// Shift all of the old data forward through the array
	for (int i=(nSignedNoiseData-1); i>0; i--){
		signedNoiseData[i] = signedNoiseData[i-1];
	}
	
	// Compute the latest data, and insert it at the head of the array.
	// Here is where ofSignedNoise is requested.
	float noiseStep    = radialNoiseStepSlider.getValue();
	float noiseAmount  = radialNoiseAmountSlider.getValue();
	
	signedNoiseData[0] = noiseAmount * ofSignedNoise( radialNoiseCursor ); 
	radialNoiseCursor += noiseStep;
}


//--------------------------------------------------------------
void testApp::renderNoisyRobotArmDemo(){
	
	float t = ofGetElapsedTimef(); 
	float shoulderNoiseAngleDegrees = 90 + 70.0 * ofSignedNoise(t * 1.00); 
	float elbowNoiseAngleDegrees    = 60 + 80.0 * ofSignedNoise(t * 0.87); 
	float wristNoiseAngleDegrees	= (2.5 * 72) + 45.0 * ofSignedNoise(t * 1.13); 
	
	float noisyR = ofNoise(t * 0.66); // different multiplicative step-factors 
	float noisyG = ofNoise(t * 0.73); // guarantee that our color channels are 
	float noisyB = ofNoise(t * 0.81); // not all (apparently) synchronized.
	
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetCircleResolution(12);
	ofSetLineWidth(1.0);
	
	ofPushMatrix();
	
	// Translate over to the shoulder location; draw it
	ofTranslate(ofGetWidth()/2, 540, 0);
	ofRotate(shoulderNoiseAngleDegrees, 0, 0, 1);
	drawNoisyArmRect(100,24); 
	
	// Translate over to the forearm location; draw it
	ofTranslate(76, 0, 0);
	ofRotate(elbowNoiseAngleDegrees, 0, 0, 1);
	drawNoisyArmRect(90,16); 
	
	// Translate over to the hand location; draw it. 
	// Note that the color of the 'hand' is controlled by noise. 
	ofTranslate(74, 0, 0);
	ofRotate(wristNoiseAngleDegrees, 0, 0, 1);
	ofSetCircleResolution(5); // a kludgy "pentagon"
	ofFill();
	ofSetColor (ofFloatColor(noisyR, noisyG, noisyB, 0.75)); 
	ofEllipse(-10,0, 60,60); 
	ofNoFill();
	ofSetColor(0);
	ofEllipse(-10,0, 60,60); 
	ofSetCircleResolution(12);
	ofSetColor(0); 
	ofFill();
	ofEllipse(0,0, 7,7);
	
	ofPopMatrix();
}


//--------------------------------------------------------------
void testApp::drawNoisyArmRect (float w, float h){
	// A little helper function to simplify the code above. 
	
	ofFill();
	ofSetColor(255,255,255, 128);
	ofRect(-h/2,-h/2, w,h);
	
	ofNoFill();
	ofSetColor(0);
	ofRect(-h/2,-h/2, w,h); 
	
	ofSetColor(0); 
	ofFill();
	ofEllipse(0,0, 7,7);
}



//--------------------------------------------------------------
void testApp::renderSignedNoiseDemo(){
	
	// Draw the explanatory text. 
	string signedNoiseText = "ofSignedNoise() generates values between -1 and 1.\n";
	signedNoiseText       += "It can be used to generate a terrain - whether as\n";
	signedNoiseText       += "displacements to a line, or to some other shape.\n\n";
	signedNoiseText       += "1D Noise can also be used to control other visual\n";
	signedNoiseText       += "properties, such as rotation angles or color values.\n";
	signedNoiseText       += "Noise is mapped to rotation -- there's no physics here!\n";
	
	ofSetColor(0,0,0); 
	float signedNoiseTextX = radialNoiseDemoX - radialNoiseDemoR;
	float signedNoiseTextY = radialNoiseDemoY + radialNoiseDemoR * 2.0 + 16;
	ofDrawBitmapString(signedNoiseText, signedNoiseTextX, signedNoiseTextY);
	ofDrawBitmapString("ofSignedNoise()", signedNoiseTextX+1, signedNoiseTextY); // 'bold' it
	
	// Now draw the linear and circle-wrapped waveforms. 
	renderRadialSignedNoiseDemo();
	renderLinearSignedNoiseDemo();
}



//--------------------------------------------------------------
void testApp::renderRadialSignedNoiseDemo (){
	
	float centerX = radialNoiseDemoX; 
	float centerY = radialNoiseDemoY; 
	
	// Render the Signed Noise demo, using 
	// the noise as radial displacements to a circle. 
	ofPushMatrix();
	ofTranslate(centerX + radialNoiseDemoR,centerY,0);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofNoFill();
	
	// Draw a faint plain circle, so that we can better understand  
	// the radial displacements caused by the signed noise later on. 
	ofSetColor(0,0,0, 64); 
	ofSetCircleResolution(256);
	ofEllipse(0,0, radialNoiseDemoR*2,radialNoiseDemoR*2);
	
	// Let's use the signed noise as a radial displacement to a circle. 
	// We render out the points stored in the X and Y arrays. 
	ofMesh wigglyMeshLine; // yes, technically, it's a "mesh"
	wigglyMeshLine.setMode(OF_PRIMITIVE_LINE_STRIP);
	float px, py; 
	for (int i=(nSignedNoiseData-1); i>=0; i--){
		
		// From the 'i' iterator, use ofMap to compute both 
		// an angle (around a circle) and an alpha value. 
		float angle   = ofMap(i, 0,nSignedNoiseData-1, 0,-TWO_PI) - HALF_PI;
		float alph    = ofMap(i, 0,nSignedNoiseData-1, 1,0     );
		wigglyMeshLine.addColor(ofFloatColor(0,0,255, alph)); 
		
		// Cpmpute the displaced radius
		float wigglyRadius = radialNoiseDemoR;
		wigglyRadius +=  radialNoiseDemoR * signedNoiseData[i];
		
		// Good old-fashioned trigonometry: y = cos(t), x = sin(t)
		px = wigglyRadius * cos( angle );
		py = wigglyRadius * sin( angle );
		wigglyMeshLine.addVertex(ofVec2f(px,py));
	}
	
	// draw the "mesh" (line)
	ofEnableSmoothing();
	wigglyMeshLine.draw();
	
	// draw a little ball at the end
	ofFill();
	ofSetColor(0,0,0, 160);
	ofEllipse(px,py, 7,7); 
	
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::renderLinearSignedNoiseDemo(){
	
	// Draw the stored noise history as a straightforward timeline. 
	ofPushMatrix();
	
	float drawWiggleWidth = radialNoiseDemoR*TWO_PI;
	ofTranslate (radialNoiseDemoX + radialNoiseDemoR - drawWiggleWidth, radialNoiseDemoY-radialNoiseDemoR,0);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofNoFill();
	
	float noiseAmount = radialNoiseAmountSlider.getValue();
	
	// draw a "baseline"
	ofSetColor(0,0,0, 64); 
	ofLine(0,0, drawWiggleWidth,0); 
	
	// draw a wiggly line
	ofSetColor(255,0,0, 192); 
	ofPolyline wigglyPolyLine; 
	for (int i=0; i<nSignedNoiseData; i++){
		
		// From the 'i' iterator, use ofMap to compute both 
		// an angle (around a circle) and an alpha value. 
		float px = ofMap(i, 0,nSignedNoiseData-1, drawWiggleWidth,0); 
		float py = 0 - radialNoiseDemoR * signedNoiseData[i];
		wigglyPolyLine.addVertex(ofVec2f(px,py));
	}
	
	// draw the line 
	wigglyPolyLine.draw();
	ofPopMatrix();
}

// In case you're wondering, the simpleSliders get their mouse info through event handlers. 
//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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
