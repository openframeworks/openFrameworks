#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	capture = false;
	bFill	= true;
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(255, 255, 255);

}

//--------------------------------------------------------------
void ofApp::draw(){

	//we don't want to capture every frame
	//so we only capture one frame when capture
	//is set to true
	if(capture){
		output.beginEPS("test.ps");
	}

	//do we want filled shapes or outlines?
	if(bFill)output.fill();
	else output.noFill();


	//--------------------
	// some lines - lets make a grid!
	//

	int numX = ofGetWidth() / 10;
	int numY = ofGetHeight() / 10;

	output.setColor(0xEEEEEE);

	for(int y = 0; y < numY; y++){
		output.line(0, y * 10, ofGetWidth(), y * 10);
	}

	for(int x = 0; x < numX; x++){
		output.line(x * 10, 0, x * 10, ofGetHeight() );
	}


	//--------------------
	// basic shapes
	//

	//a - draw a triangle
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("a) triangle();", 65, 140);

	output.setColor(0x000000);
	output.triangle(80, 110, 110, 50, 140, 110);

	//b - rectangle from the top left corner
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("b & c) rect();\n", 220, 140);
	output.setColor(0x4d4d4d);
	output.disableCenterRect();
	output.rect(240, 50, 60, 60);

	//c - rectangle from the center
	output.setColor(0xededed);
	output.enableCenterRect();
	output.rect(270, 80, 20, 20);

	//d - circle
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("d) circle(); \n", 380, 140);
	output.setColor(0x828282);
	output.circle(425, 80, 30);

	//e - ellipse
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("e) ellipse(); \n", 520, 140);

	output.setColor(0x363636);
	output.ellipse(570, 80, 40, 30);

	//f - bezier curve - we define the start, the first control point, the second control point and the end
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("f) bezier(); \n", 70, 300);

	output.setColor(0x5c5c5c);
	output.bezier(70, 270,  100,  200, 120, 260, 180, 270);

	//lets also draw the handles
	output.setColor(0xCCCCCC);
	output.line(70, 270, 100, 200);
	output.line(180, 270, 120, 260);
	output.setColor(0xAAAAAA);
	output.enableCenterRect();
	output.rect(100, 200, 4, 4);
	output.rect(120, 260, 4, 4);


	//g - catmull curve - we give four points - the first and last are not drawn, but used to calculate the curve.
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("g) curve(); \n", 240, 300);

	output.setColor(0x7e7e7e);
	output.curve(160, 100,  240, 270,   330, 240,  360, 500);


	//h - an arc
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("h) arc(); \n", 390, 300);

	output.setColor(0x5c5c5c);
	output.arc(400, 270, 60, 0, -70);


	//i - just a line
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("i) line(); \n", 530, 300);

	output.setColor(0x5c5c5c);
	output.line(540, 220, 600, 270);




	//--------------------
	//--------------------
	//--------------------
	// advanced shapes
	// these shapes can have an unlmited number of vertices




	// -----------------------------------
	// j - polygon shape based on mouse position
	// -----------------------------------
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("j) shape w/ \npolyVertex \n", 70, 520);

	output.setColor(0x6b6b6b);
	output.beginShape();

		//set number of sides based on mouse position
		int numSteps = ( (float)mouseX / ofGetWidth() )* 12.0;

		//make sure we don't go bellow 3 sides
		numSteps = MAX(3, numSteps);


		float step		= TWO_PI / (numSteps);
		float angle		= 0.0;
		float cenX		= 110;
		float cenY		= 430;
		float radius	= 50;

		for(int i = 0; i < numSteps; i++){
			float rx = cenX + cos(angle) * radius;
			float ry = cenY + sin(angle) * radius;

			output.polyVertex(rx, ry);
			angle += step;
		}

	output.endShape(true);




	// -----------------------------------
	// k - curve shape based on mouse position
	// -----------------------------------
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("k) shape w/ \ncurveVertex \n", 290, 520);

	output.setColor(0xb8b8b8);
	output.beginShape();

		//set number of sides based on mouse position
		numSteps = ( (float)mouseX / ofGetWidth() )* 12.0;

		//make sure we don't go bellow 3 sides
		numSteps = MAX(3, numSteps);

		step		= TWO_PI / (numSteps);
		angle		= 0.0;
		cenX		= 340;
		cenY		= 430;
		radius		= 50.0;

		//we need 3 extra points to get a complete curve shape
		for(int i = 0; i < numSteps + 3; i++){
			float rx = cenX + cos(angle) * radius;
			float ry = cenY + sin(angle) * radius;

			output.curveVertex(rx, ry);
			angle += step;
		}

	output.endShape(true);



	// -----------------------------------
	// l - bezier shape based on mouse position
	// -----------------------------------
	ofSetHexColor(0xCC0000);
	ofDrawBitmapString("l) shape w/ \nbezierVertex \n", 510, 520);

	// we specify control points that shape each part of the shape
	output.setColor(0xd4d4d4);
	output.beginShape();

		//set number of sides based on mouse position
		numSteps = ( (float)mouseX / ofGetWidth() )* 12.0;

		//make sure we don't go bellow 3 sides
		numSteps = MAX(3, numSteps);

		step		= TWO_PI / (numSteps);
		angle		= 0.0;
		cenX		= 550;
		cenY		= 430.0;
		radius		= 40.0;
		float scale = 1.0 + 0.6 * sin(ofGetElapsedTimef());

		for(int i = 0; i < numSteps; i++){
			float rx = cenX + cos(angle) * radius;
			float ry = cenY + sin(angle) * radius;

			if(i == 0){
				output.polyVertex(rx, ry);
			}

			float rx2 = cenX + cos(angle+step) * radius;
			float ry2 = cenY + sin(angle+step) * radius;

			//lets make our control points in between each side and out a little way
			float cx = cenX + cos(angle + step*0.5) * radius * scale;
			float cy = cenY + sin(angle + step*0.5) * radius * scale;

			output.bezierVertex(cx, cy, cx, cy, rx2, ry2);

			angle += step;
		}

	output.endShape(true);




	//----------------------------
	//----------------------------
	//----------------------------
	// Bonuse :)
	// Mouse draw - with curved line
	//

	if( pts.size() > 0 ){

		int numPts = pts.size();

		output.setColor(0x0088EE);
		output.noFill();
		output.beginShape();

		//catmull curves create nice smooth curves between points
		//so actually a lower resolution creates a smoother line
		//and reduce the number of poiints in our eps file.
		int rescaleRes = 6;

		for(int i = 0; i < numPts; i++){

			//we need to draw the first and last point
			//twice for a catmull curve
			if(i == 0 || i == numPts -1){
				output.curveVertex(pts[i].x, pts[i].y);
			}

			if(i % rescaleRes == 0) output.curveVertex(pts[i].x, pts[i].y);
		}

		output.endShape();
	}



	//-------------------------
	//-------------------------
	// End capture!
	//

	//once we have done all our drawing
	//we end the ouput which saves the file
	//and then we stop capturing
	if(capture){
		output.endEPS();
		capture =false;
	}

	//
	// End capture!
	//-------------------------
	//-------------------------


	//we don't save this to eps - just info for the app

	ofFill();
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofSetHexColor(0x000000);
	ofRect(60, 630, 200,60);

	//some text to explain whats what
	ofSetHexColor(0xDDDDDD);
	ofDrawBitmapString("spacebar to capture\n'f' key toggles fill\nmouse to doodle", 75, 650);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == ' '){
		capture = true;
	}else if(key =='f'){
		bFill = !bFill;
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

	//we add a new point to our line
	pts.push_back(ofPoint());
	int last = pts.size()-1;

	pts[last].x = x;
	pts[last].y = y;

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	pts.clear();

	//lets store the first point of the line
	pts.push_back(ofPoint());
	pts[0].x = x;
	pts[0].y = y;

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
