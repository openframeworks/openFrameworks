#include "testApp.h"

#include "ofCairoRenderer.h"
#include "ofGLRenderer.h"
#include "ofRendererCollection.h"
ofPtr<ofCairoRenderer> cairo(new ofCairoRenderer);
ofPtr<ofGLRenderer> gl(new ofGLRenderer);
ofPtr<ofRendererCollection> renderer(new ofRendererCollection);

#define RENDER_TO_PDF

//--------------------------------------------------------------
void testApp::setup(){
#ifdef RENDER_TO_PDF
	cairo->setup("paths.pdf",ofCairoRenderer::PDF,true);
	renderer->renderers.push_back(gl);
	renderer->renderers.push_back(cairo);
	ofSetCurrentRenderer(renderer);
	ofSetFrameRate( 12 ); //each frame generates a page so let's keep it low
#endif
	
	nCurveVertexes = 7;
	
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
	
	for (int i = 0; i < nCurveVertexes; i++){
		curveVertices[i].bOver 			= false;
		curveVertices[i].bBeingDragged 	= false;
		curveVertices[i].radius = 4;
	}
	
	
	

	//------(a)--------------------------------------
	// 
	// 		draw a star
	//
	// 		use poly winding odd, the default rule
	//
	// 		info about the winding rules is here:
	//		http://glprogramming.com/red/images/Image128.gif
	// 
	shapeA.setHexColor(0xe0be21);
	shapeA.setFilled(true);
	shapeA.setPolyWindingMode(OF_POLY_WINDING_ODD);	// this is the normal mode
	shapeA.moveTo(200,135);
	shapeA.lineTo(15,135);
	shapeA.lineTo(165,25);
	shapeA.lineTo(105,200);
	shapeA.lineTo(50,25);
	
	

	//------(b)--------------------------------------
	// 
	// 		draw a star
	//
	// 		use poly winding nonzero
	//
	// 		info about the winding rules is here:
	//		http://glprogramming.com/red/images/Image128.gif
	// 
	shapeB.setHexColor(0xb5de10);
	shapeB.setFilled(true);
	shapeB.setPolyWindingMode(OF_POLY_WINDING_NONZERO);
	shapeB.moveTo(400,135);
	shapeB.lineTo(215,135);
	shapeB.lineTo(365,25);
	shapeB.lineTo(305,200);
	shapeB.lineTo(250,25);
	shapeB.close();
	//-------------------------------------
	
	
	
	//------(c)--------------------------------------
	// 
	// 		draw a star dynamically
	//
	// 		use the mouse position as a pct
	//		to calc nPoints and internal point radius
	//
	float xPct = (float)(200) / (float)(ofGetWidth());
	float yPct = (float)(400) / (float)(ofGetHeight());
	int nTips = 5 + xPct * 60;
	int nStarPts = nTips * 2;
	float angleChangePerPt = TWO_PI / (float)nStarPts;
	float innerRadius = 0 + yPct*80;
	float outerRadius = 80;
	float origx = 525;
	float origy = 100;
	float angle = 0;
	
	shapeC.setHexColor(0xa16bca);
	shapeC.setFilled(true);
	for (int i = 0; i < nStarPts; i++){
		if (i % 2 == 0) {
			// inside point:
			float x = origx + innerRadius * cos(angle);
			float y = origy + innerRadius * sin(angle);
			shapeC.lineTo(x,y);
		} else {
			// outside point
			float x = origx + outerRadius * cos(angle);
			float y = origy + outerRadius * sin(angle);
			shapeC.lineTo(x,y);
		}
		angle += angleChangePerPt;
	}
	//-------------------------------------
	
	//------(d)--------------------------------------
	// 
	// 		poylgon of random points
	//
	// 		lots of self intersection, 500 pts is a good stress test
	// 
	// 
	shapeD.setHexColor(0x0cb0b6);
	shapeD.setFilled(true);
	shapeD.setPolyWindingMode(OF_POLY_WINDING_ODD);
	for (int i = 0; i < 10; i++){
		shapeD.lineTo(ofRandom(650,850), ofRandom(20,200));
	}
	//-------------------------------------
	
	
	//------(e)--------------------------------------
	// 
	// 		use sin cos and time to make some spirally shape
	//
	shapeE.setHexColor(0xff2220);
	shapeE.setFilled( true );
	shapeE.setPolyWindingMode(OF_POLY_WINDING_ODD);
	float angleStep 	= TWO_PI/(100.0f + sin(35/5.0f) * 60);
	float radiusAdder 	= 0.5f;
	float radius 		= 0;
	for (int i = 0; i < 200; i++){
		float anglef = (i) * angleStep;
		float x = radius * cos(anglef) + 100;
		float y = radius * sin(anglef) + 300;
		shapeE.lineTo(x,y);
		radius 	+= radiusAdder; 
	}
	shapeE.close();
	//-------------------------------------
	
	//------(f)--------------------------------------
	// 
	// 		addCurveVertex
	// 
	// 		because it uses catmul rom splines, we need to repeat the first and last 
	// 		items so the curve actually goes through those points
	//
	
	shapeF.setHexColor(0x2bdbe6);
	shapeF.setFilled(true);
	
	for (int i = 0; i < nCurveVertexes; i++){
		
		
		// sorry about all the if/states here, but to do catmull rom curves
		// we need to duplicate the start and end points so the curve acutally 
		// goes through them.
		
		// for i == 0, we just call the vertex twice
		// for i == nCurveVertexes-1 (last point) we call vertex 0 twice
		// otherwise just normal ofCurveVertex call
		
		if (i == 0){
			shapeF.curveTo(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
			shapeF.curveTo(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
		} else if (i == nCurveVertexes-1){
			shapeF.curveTo(curveVertices[i].x, curveVertices[i].y);
			shapeF.curveTo(curveVertices[0].x, curveVertices[0].y);	// to draw a curve from pt 6 to pt 0
			shapeF.curveTo(curveVertices[0].x, curveVertices[0].y);	// we duplicate the first point twice
		} else {
			shapeF.curveTo(curveVertices[i].x, curveVertices[i].y);
		}
	}
	shapeF.close();
	
	// show a faint the non-curve version of the same polygon:
	shapeFNonCurve.setFilled( false );
	shapeFNonCurve.setColor( ofColor(0,0,0,40) );
	for (int i = 0; i < nCurveVertexes; i++){
		shapeFNonCurve.lineTo(curveVertices[i].x, curveVertices[i].y);
	}
	shapeFNonCurve.close();
	//-------------------------------------
	
	
	//------(g)--------------------------------------
	// 
	// 		addBezierVertex
	// 
	// 		with addBezierVertex we can draw a curve from the current vertex
	//		through the the next three vertexes we pass in.
	//		(two control points and the final bezier point)
	//		
	
	float x0 = 500;
	float y0 = 300;
	float x1 = 550+50*cos(5.8*1.0f);
	float y1 = 300+100*sin(5.8/3.5f);
	float x2 = 600+30*cos(5.8*2.0f);
	float y2 = 300+100*sin(5.8);
	float x3 = 650;
	float y3 = 300;
		
	
	shapeG.setFilled(true);
	shapeG.setHexColor(0xFF9933);
	shapeG.lineTo(x0,y0);
	shapeG.bezierTo(x1,y1,x2,y2,x3,y3);

	
	//------(h)--------------------------------------
	// 
	// 		holes / nextContour
	// 
	// 		with nextContour we can create multi-contour shapes
	// 		this allows us to draw holes, for example... 
	//
	shapeH.setFilled(true);
	shapeH.setHexColor( 0xff00ff );
	shapeH.lineTo(100,500);
	shapeH.lineTo(180,550);
	shapeH.lineTo(100,600);
	shapeH.close();
	shapeH.lineTo(120,520);
	shapeH.lineTo(160,550);
	shapeH.lineTo(120,580);
	shapeH.close();

	//-------------------------------------

	
	//------(i)--------------------------------------
	// 
	// 		CSG / nextContour
	// 
	// 		with different winding rules, you can even use nextContour to 
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
	shapeIa.setFilled(false);
	shapeIa.setPolyWindingMode(OF_POLY_WINDING_ODD);
	shapeIa.setHexColor(0xff00ff);
	shapeIa.lineTo(300,500);
	shapeIa.lineTo(380,550);
	shapeIa.lineTo(300,600);
	shapeIa.close();
	shapeIa.arc(340,550,30,30,0,360);


	shapeIb.setPolyWindingMode(OF_POLY_WINDING_NONZERO);
	shapeIb.setHexColor(0xff00ff);
	shapeIb.setFilled(false);
	shapeIb.lineTo(400,500);
	shapeIb.lineTo(480,550);
	shapeIb.lineTo(400,600);
	shapeIb.close();
	shapeIb.arc(440,550,30,60,0,360);

	shapeIc.setPolyWindingMode(OF_POLY_WINDING_ABS_GEQ_TWO);
	shapeIc.setHexColor(0xff00ff);
	shapeIc.setFilled(false);
	shapeIc.lineTo(500,500);
	shapeIc.lineTo(580,550);
	shapeIc.lineTo(500,600);
	shapeIc.close();
	shapeIc.arc(540,550,30,20,0,360);
	
	selectedDraggableVertex.arc(0,0,4,4,0,360);
	selectedDraggableVertex.close();
	selectedDraggableVertex.setFilled(true);
	selectedDraggableVertex.setColor(ofColor(0,0,0,80));

	unselectedDraggableVertex.arc(0,0,4,4,0,360);
	unselectedDraggableVertex.close();
	unselectedDraggableVertex.setFilled(false);
	unselectedDraggableVertex.setColor(ofColor(0,0,0,80));
	

	ofBackground( 255, 255, 255 );

}

//--------------------------------------------------------------
void testApp::update(){

	//------(d)--------------------------------------
	// 
	// 		poylgon of random points
	//
	// 		lots of self intersection, 500 pts is a good stress test
	// 
	// 
	shapeD.clear();
	for (int i = 0; i < 10; i++){
		shapeD.lineTo(ofRandom(650,850), ofRandom(20,200));
	}
	shapeD.tessellate();
	//-------------------------------------

	
	//------(e)--------------------------------------
	// 
	// 		use sin cos and time to make some spirally shape
	//
	shapeE.clear();
	float angleStep 	= TWO_PI/(100.0f + sin(ofGetElapsedTimef()/5.0f) * 60); 
	float radiusAdder 	= 0.5f;
	float radius 		= 0;
	for (int i = 0; i < 200; i++){
		float anglef = (i) * angleStep;
		float x = radius * cos(anglef);
		float y = radius * sin(anglef); 
		shapeE.lineTo(x,y);
		radius 	+= radiusAdder; 
	}
	shapeE.close();
	shapeE.tessellate();
	
	
	//------(g)--------------------------------------
	// 
	// 		addBezierVertex
	// 
	// 		with addBezierVertex we can draw a curve from the current vertex
	//		through the the next three vertexes we pass in.
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
	
	
	shapeG.clear();
	shapeG.lineTo(x0,y0);
	shapeG.bezierTo(x1,y1,x2,y2,x3,y3);
	shapeG.tessellate();
}


//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255,255,255);

	//------(a)--------------------------------------
	// 
	// 		draw a star
	// 
	// 		use poly winding odd
	// 
	shapeA.draw();

	//------(b)--------------------------------------
	// 
	// 		draw a star
	//
	// 		use poly winding nonzero	
	// 
	shapeB.draw();
	
	//------(c)--------------------------------------
	// 
	// 		draw a star dynamically	
	// 
	shapeC.draw();
	
	
	//------(d)--------------------------------------
	// 
	// 		polygon of random points	
	// 
	shapeD.draw();
	
	
	//------(e)--------------------------------------
	// 
	// 		use sin cos and time to make some spirally shape
	// 
	ofPushMatrix();
	ofTranslate(100,300);
	shapeE.draw();
	ofPopMatrix();
	

	
	//------(f)--------------------------------------
	// 
	// 		addCurveVertex
	// 
	shapeF.draw();
	
	ofEnableAlphaBlending();
	shapeFNonCurve.draw();

	ofSetColor(255,255,255);
	for (int i = 0; i < nCurveVertexes; i++){
		if (curveVertices[i].bOver == true) selectedDraggableVertex.draw(curveVertices[i].x, curveVertices[i].y);
		else unselectedDraggableVertex.draw(curveVertices[i].x, curveVertices[i].y);
	}
	ofDisableAlphaBlending();
	
	
	//------(g)--------------------------------------
	// 
	// 		addBezierVertex	
	// 
	shapeG.draw();
	float x0 = 500;
	float y0 = 300;
	float x1 = 550+50*cos(ofGetElapsedTimef()*1.0f);
	float y1 = 300+100*sin(ofGetElapsedTimef()/3.5f);
	float x2 = 600+30*cos(ofGetElapsedTimef()*2.0f);
	float y2 = 300+100*sin(ofGetElapsedTimef());
	float x3 = 650;
	float y3 = 300;
	ofEnableAlphaBlending();
	ofFill();

	selectedDraggableVertex.draw(x0,y0);
	selectedDraggableVertex.draw(x1,y1);
	selectedDraggableVertex.draw(x2,y2);
	selectedDraggableVertex.draw(x3,y3);
	ofDisableAlphaBlending();
	
	

	//------(h)--------------------------------------
	// 
	// 		holes / ofNextContour
	//
	ofSetHexColor(0xd3ffd3);
	ofRect(80,480,140,70);
	ofSetColor(255,255,255);
	shapeH.draw();
	
	
	//------(i)--------------------------------------
	// 
	// 		CSG / ofNextContour
	// 
	ofPushMatrix();
	
	shapeIa.draw();
	
	ofTranslate(100,0,0);

	shapeIb.draw();
	
	ofTranslate(100,0,0);

	shapeIc.draw();
	
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
	ofDrawBitmapString("(f) addCurveVertex\nuses catmull rom\nto make curved shapes", 220,410);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(g) addBezierVertex\nuses bezier to draw curves", 460,410);
	
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(h) nextContour\nallows for holes", 20,610);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(i) nextContour\ncan even be used for CSG operations\nsuch as union and intersection", 260,620);

	ofDrawBitmapString(ofToString((int)ofGetFrameRate()),20,20);
		
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//------------- -------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	for (int i = 0; i < nCurveVertexes; i++){
		float diffx = x - curveVertices[i].x;
		float diffy = y - curveVertices[i].y;
		float dist = sqrt(diffx*diffx + diffy*diffy);
		if (dist < curveVertices[i].radius){
			curveVertices[i].bOver = true;
		} else {
			curveVertices[i].bOver = false;
		}	
	}
	
	
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
	
	shapeC.clear();
	for (int i = 0; i < nStarPts; i++){
		if (i % 2 == 0) {
			// inside point:
			float x = origx + innerRadius * cos(angle);
			float y = origy + innerRadius * sin(angle);
			shapeC.lineTo(x,y);
		} else {
			// outside point
			float x = origx + outerRadius * cos(angle);
			float y = origy + outerRadius * sin(angle);
			shapeC.lineTo(x,y);
		}
		angle += angleChangePerPt;
	}
	shapeC.tessellate();
	//-------------------------------------
	
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	for (int i = 0; i < nCurveVertexes; i++){
		if (curveVertices[i].bBeingDragged == true){
			curveVertices[i].x = x;
			curveVertices[i].y = y;
		}
	}

	//------(f)--------------------------------------
	// 
	// 		addCurveVertex
	// 
	// 		because it uses catmul rom splines, we need to repeat the first and last 
	// 		items so the curve actually goes through those points
	//
	
	shapeF.clear();
	for (int i = 0; i < nCurveVertexes; i++){
		
		// sorry about all the if/states here, but to do catmull rom curves
		// we need to duplicate the start and end points so the curve acutally 
		// goes through them.
		
		// for i == 0, we just call the vertex twice
		// for i == nCurveVertexes-1 (last point) we call vertex 0 twice
		// otherwise just normal ofCurveVertex call
		
		if (i == 0){
			shapeF.curveTo(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
			shapeF.curveTo(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
		} else if (i == nCurveVertexes-1){
			shapeF.curveTo(curveVertices[i].x, curveVertices[i].y);
			shapeF.curveTo(curveVertices[0].x, curveVertices[0].y);	// to draw a curve from pt 6 to pt 0
			shapeF.curveTo(curveVertices[0].x, curveVertices[0].y);	// we duplicate the first point twice
		} else {
			shapeF.curveTo(curveVertices[i].x, curveVertices[i].y);
		}
	}

	
	// show a faint the non-curve version of the same polygon:
	shapeFNonCurve.clear();
	for (int i = 0; i < nCurveVertexes; i++){
		shapeFNonCurve.lineTo(curveVertices[i].x, curveVertices[i].y);
	}
	shapeFNonCurve.close();
	shapeF.tessellate();
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	for (int i = 0; i < nCurveVertexes; i++){
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
void testApp::mouseReleased(int x, int y, int button){
	
	for (int i = 0; i < nCurveVertexes; i++){
		curveVertices[i].bBeingDragged = false;	
	}
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

