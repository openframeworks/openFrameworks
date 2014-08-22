#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
	ofBackground(255,255,255);	
	ofSetFrameRate(60);
	
	nCurveVertices = 7;
	
	curveVertices[0].x = 326;
	curveVertices[0].y = 209;
	curveVertices[1].x = 306;
	curveVertices[1].y = 279;
	curveVertices[2].x = 265;
	curveVertices[2].y = 331;
	curveVertices[3].x = 304;
	curveVertices[3].y = 383;
	curveVertices[4].x = 374;
	curveVertices[4].y = 383;
	curveVertices[5].x = 418;
	curveVertices[5].y = 309;
	curveVertices[6].x = 345;
	curveVertices[6].y = 279;
	
	for (int i = 0; i < nCurveVertices; i++){
		curveVertices[i].bOver 			= false;
		curveVertices[i].bBeingDragged 	= false;
		curveVertices[i].radius = 4;
	}
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofFill();
	ofSetHexColor(0xe0be21);

	//------(a)--------------------------------------
	// 
	// 		draw a star
	//
	// 		use poly winding odd, the default rule
	//
	// 		info about the winding rules is here:
	//		http://glprogramming.com/red/images/Image128.gif
	// 
	ofSetPolyMode(OF_POLY_WINDING_ODD);	// this is the normal mode
	ofBeginShape();
		ofVertex(200,135);
		ofVertex(15,135);
		ofVertex(165,25);
		ofVertex(105,200);
		ofVertex(50,25);
	ofEndShape();
	
	
	//------(b)--------------------------------------
	// 
	// 		draw a star
	//
	// 		use poly winding nonzero
	//
	// 		info about the winding rules is here:
	//		http://glprogramming.com/red/images/Image128.gif
	// 
	ofSetHexColor(0xb5de10);
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
		ofVertex(400,135);
		ofVertex(215,135);
		ofVertex(365,25);
		ofVertex(305,200);
		ofVertex(250,25);
	ofEndShape();
	//-------------------------------------
	
	
	
	//------(c)--------------------------------------
	// 
	// 		draw a star dynamically
	//
	// 		use the mouse position as a pct
	//		to calc nPoints and internal point radius
	//
	float xPct = (float)(mouseX) / (float)(ofGetWidth());
	float yPct = (float)(mouseY) / (float)(ofGetHeight());
	int nTips = 5 + xPct * 60;
	int nStarPts = nTips * 2;
	float angleChangePerPt = TWO_PI / (float)nStarPts;
	float innerRadius = 0 + yPct*80;
	float outerRadius = 80;
	float origx = 525;
	float origy = 100;
	float angle = 0;
	
	ofSetHexColor(0xa16bca);
	ofBeginShape();
	for (int i = 0; i < nStarPts; i++){
		if (i % 2 == 0) {
			// inside point:
			float x = origx + innerRadius * cos(angle);
			float y = origy + innerRadius * sin(angle);
			ofVertex(x,y);
		} else {
			// outside point
			float x = origx + outerRadius * cos(angle);
			float y = origy + outerRadius * sin(angle);
			ofVertex(x,y);
		}
		angle += angleChangePerPt;
	}
	ofEndShape();
	//-------------------------------------
	
	//------(d)--------------------------------------
	// 
	// 		poylgon of random points
	//
	// 		lots of self intersection, 500 pts is a good stress test
	// 
	// 
	ofSetHexColor(0x0cb0b6);
	ofSetPolyMode(OF_POLY_WINDING_ODD);
	ofBeginShape();
	for (int i = 0; i < 10; i++){
		ofVertex(ofRandom(650,850), ofRandom(20,200));
	}
	ofEndShape();
	//-------------------------------------
	
	
	//------(e)--------------------------------------
	// 
	// 		use sin cos and time to make some spirally shape
	//
	ofPushMatrix();
		ofTranslate(100,300,0);
		ofSetHexColor(0xff2220);
		ofFill();
		ofSetPolyMode(OF_POLY_WINDING_ODD);
		ofBeginShape();
		float angleStep 	= TWO_PI/(100.0f + sin(ofGetElapsedTimef()/5.0f) * 60); 
		float radiusAdder 	= 0.5f;
		float radius 		= 0;
		for (int i = 0; i < 200; i++){
			float anglef = (i) * angleStep;
			float x = radius * cos(anglef);
			float y = radius * sin(anglef); 
			ofVertex(x,y);
			radius 	+= radiusAdder; 
		}
		ofEndShape(OF_CLOSE);
	ofPopMatrix();
	//-------------------------------------
	
	//------(f)--------------------------------------
	// 
	// 		ofCurveVertex
	// 
	// 		because it uses catmul rom splines, we need to repeat the first and last 
	// 		items so the curve actually goes through those points
	//

	ofSetHexColor(0x2bdbe6);
	ofBeginShape();
	
		for (int i = 0; i < nCurveVertices; i++){
			
			
			// sorry about all the if/states here, but to do catmull rom curves
			// we need to duplicate the start and end points so the curve acutally 
			// goes through them.
			
			// for i == 0, we just call the vertex twice
			// for i == nCurveVertices-1 (last point) we call vertex 0 twice
			// otherwise just normal ofCurveVertex call
			
			if (i == 0){
				ofCurveVertex(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
				ofCurveVertex(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
			} else if (i == nCurveVertices-1){
				ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
				ofCurveVertex(curveVertices[0].x, curveVertices[0].y);	// to draw a curve from pt 6 to pt 0
				ofCurveVertex(curveVertices[0].x, curveVertices[0].y);	// we duplicate the first point twice
			} else {
				ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
			}
		}
		
	ofEndShape();
	
	
	// show a faint the non-curve version of the same polygon:
	ofEnableAlphaBlending();
		ofNoFill();
		ofSetColor(0,0,0,40);
		ofBeginShape();
			for (int i = 0; i < nCurveVertices; i++){
				ofVertex(curveVertices[i].x, curveVertices[i].y);
			}
		ofEndShape(true);
		
		
		ofSetColor(0,0,0,80);
		for (int i = 0; i < nCurveVertices; i++){
			if (curveVertices[i].bOver == true) ofFill();
			else ofNoFill();
			ofCircle(curveVertices[i].x, curveVertices[i].y,4);
		}
	ofDisableAlphaBlending();
	//-------------------------------------
	
	
	//------(g)--------------------------------------
	// 
	// 		ofBezierVertex
	// 
	// 		with ofBezierVertex we can draw a curve from the current vertex
	//		through the the next three vertices we pass in.
	//		(two control points and the final bezier point)
	//		
	
	float x0 = 500;
	float y0 = 300;
	float x1 = 550+50*cos(ofGetElapsedTimef()*1.0f);
	float y1 = 300+100*sin(ofGetElapsedTimef()/3.5f);
	float x2 = 600+30*cos(ofGetElapsedTimef()*2.0f);
	float y2 = 300+100*sin(ofGetElapsedTimef());
	float x3 = 650;
	float y3 = 300;
	
	
	
	ofFill();
	ofSetHexColor(0xFF9933);
	ofBeginShape();
	ofVertex(x0,y0);
	ofBezierVertex(x1,y1,x2,y2,x3,y3);
	ofEndShape();
	
	
	ofEnableAlphaBlending();
		ofFill();
		ofSetColor(0,0,0,40);
		ofCircle(x0,y0,4);
		ofCircle(x1,y1,4);
		ofCircle(x2,y2,4);
		ofCircle(x3,y3,4);
	ofDisableAlphaBlending();
	
	
	
	//------(h)--------------------------------------
	// 
	// 		holes / ofNextContour
	// 
	// 		with ofNextContour we can create multi-contour shapes
	// 		this allows us to draw holes, for example... 
	//
	ofFill();
	ofSetHexColor(0xd3ffd3);
	ofRect(80,480,140,70);
	ofSetHexColor(0xff00ff);
	
	ofBeginShape();
		
		ofVertex(100,500);
		ofVertex(180,550);
		ofVertex(100,600);
		
		ofNextContour(true);
		
		ofVertex(120,520);
		ofVertex(160,550);
		ofVertex(120,580);
		
	ofEndShape(true);
	//-------------------------------------
	
	
	//------(i)--------------------------------------
	// 
	// 		CSG / ofNextContour
	// 
	// 		with different winding rules, you can even use ofNextContour to 
	// 		perform constructive solid geometry 
	// 		
	// 		be careful, the clockwiseness or counter clockwisenss of your multiple
	// 		contours matters with these winding rules.
	//
	// 		for csg ideas, see : http://glprogramming.com/red/chapter11.html
	// 
	// 		info about the winding rules is here:
	//		http://glprogramming.com/red/images/Image128.gif
	// 
	ofNoFill();
	
	
	ofPushMatrix();
	
	ofSetPolyMode(OF_POLY_WINDING_ODD);
	
	ofBeginShape();
		
		ofVertex(300,500);
		ofVertex(380,550);
		ofVertex(300,600);
		
		ofNextContour(true);
		
		for (int i = 0; i < 20; i++){
			float anglef = ((float)i / 19.0f) * TWO_PI;
			float x = 340 + 30 * cos(anglef);
			float y = 550 + 30 * sin(anglef); 
			ofVertex(x,y);
			radius 	+= radiusAdder; 
		}
		

	ofEndShape(true);
	
	ofTranslate(100,0,0);
	
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);	
	ofBeginShape();
		
		ofVertex(300,500);
		ofVertex(380,550);
		ofVertex(300,600);
		
		ofNextContour(true);
		
		for (int i = 0; i < 20; i++){
			float anglef = ((float)i / 19.0f) * TWO_PI;
			float x = 340 + 30 * cos(anglef);
			float y = 550 + 30 * sin(anglef); 
			ofVertex(x,y);
			radius 	+= radiusAdder; 
		}
		
	ofEndShape(true);
	
	ofTranslate(100,0,0);
	ofSetPolyMode(OF_POLY_WINDING_ABS_GEQ_TWO);
	ofBeginShape();
		ofVertex(300,500);
		ofVertex(380,550);
		ofVertex(300,600);
		ofNextContour(true);
		
		for (int i = 0; i < 20; i++){
			float anglef = ((float)i / 19.0f) * TWO_PI;
			float x = 340 + 30 * cos(anglef);
			float y = 550 + 30 * sin(anglef); 
			ofVertex(x,y);
			radius 	+= radiusAdder; 
		}
		
		
	ofEndShape(true);
	
	ofPopMatrix();

	//-------------------------------------
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(a) star\nwinding rule odd", 20,210);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(b) star\nwinding rule nonzero", 220,210);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(c) dynamically\ncreated shape", 420,210);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(d) random points\npoly", 670,210);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(e) fun with sin/cos", 20,410);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(f) ofCurveVertex\nuses catmull rom\nto make curved shapes", 220,410);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(g) ofBezierVertex\nuses bezier to draw curves", 460,410);
	
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(h) ofNextContour\nallows for holes", 20,610);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(i) ofNextContour\ncan even be used for CSG operations\nsuch as union and intersection", 260,620);
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//------------- -------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	for (int i = 0; i < nCurveVertices; i++){
		float diffx = x - curveVertices[i].x;
		float diffy = y - curveVertices[i].y;
		float dist = sqrt(diffx*diffx + diffy*diffy);
		if (dist < curveVertices[i].radius){
			curveVertices[i].bOver = true;
		} else {
			curveVertices[i].bOver = false;
		}	
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	for (int i = 0; i < nCurveVertices; i++){
		if (curveVertices[i].bBeingDragged == true){
			curveVertices[i].x = x;
			curveVertices[i].y = y;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	for (int i = 0; i < nCurveVertices; i++){
		float diffx = x - curveVertices[i].x;
		float diffy = y - curveVertices[i].y;
		float dist = sqrt(diffx*diffx + diffy*diffy);
		if (dist < curveVertices[i].radius){
			curveVertices[i].bBeingDragged = true;
		} else {
			curveVertices[i].bBeingDragged = false;
		}	
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	for (int i = 0; i < nCurveVertices; i++){
		curveVertices[i].bBeingDragged = false;	
	}
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
