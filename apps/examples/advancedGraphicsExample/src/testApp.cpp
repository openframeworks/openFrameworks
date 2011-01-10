#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	counter = 0.0;
	spin	= 0.0;
	spinPct	= 0.0;
	mouseX  = 263;
	mouseY  = 267;
	bFirstMouseMove = true;

	//set background to black
	ofBackground(0, 0, 0);

	//lets make our circles look a little nicer!
	ofSetCircleResolution(40);

	//for smooth animation, set vertical sync if we can
	ofSetVerticalSync(true);
	// also, frame rate:
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void testApp::update(){

	//add 0.029 to our counter
	counter = counter + 0.029f;

	//the quick way to write
	//the same thing would be:
	//counter += 0.029f;

	//reduce the spinPct by a small amount
	//so that the spinning eventually stops
	//this is the same as writing:
	//spinPct = spinPct * 0.99;
	spinPct	*= 0.99f;

	//update the spin -which is storing the
	//total rotation- by spinPct
	spin	+= spinPct;
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetupScreen();


	//Lets enable blending!
	//We are going to use a blend mode that adds
	//all the colors to white.
	ofEnableAlphaBlending();
	glBlendFunc(GL_SRC_COLOR, GL_ONE);


	//---------------------------------
	//RGB CICRLES

	//We only want to rotate the circles
	//So we use push and pop matrix to
	//make sure the rotation is contained
	glPushMatrix();
		//we position the rotation point
		//at the location we want it to
		//spin around .
		glTranslatef(750, 320, 0);
		glRotatef(spin, 0, 0, 1);

		//draw a red circle
		ofSetColor(255,0, 0);
		ofCircle(-50, -25, 100);

		//draw a green circle
		ofSetColor(0,255, 0);
		ofCircle(50, -25, 100);

		//draw a blue circle
		ofSetColor(0, 0, 255);
		ofCircle(0, 57, 100);
	glPopMatrix();


	//---------------------------------
	//PIXEL OCEAN

	//Lets make some 8bit looking waves
	//out of rectangles
	ofSetColor(0, 90, 170);
	float k = 0.0;
	for(int i = 0; i < ofGetWidth(); i+= 50){

		//Lets get a unique height for our 'wave'
		//using sine
		float height = sin( counter + k);

		//sine produces -1 to 1 values
		//lets add 1 to make sure the height
		//is always positive
		height += 1.0;

		//Now it is going from 0 to 2
		//but we want it to be taller than that.
		//Lets make it go from 0 - 100 by
		//multiplying 50
		height *= 50;

		//lets draw it!
		ofRect(i, ofGetHeight(), 50, -height);

		//this variable makes sure that each
		//rect has a unique height otherwise
		//they would all be moving up and down
		//at the same time
		k+=0.7;
	}

	//This is doing it again but for a different color
	ofSetColor(0, 120, 190);
	k = 0;
	for(int i = 0; i < ofGetWidth(); i+= 50)
	{
		ofRect(i+5, ofGetHeight(), 50, -50 * (sin(1.4*counter-k)+1.0));
		k+=0.4;
	}

	//Lets stop the blending!
	ofDisableAlphaBlending();


	//---------------------------------
	//LISSAJOUS EXAMPLE
	//http://en.wikipedia.org/wiki/Lissajous_curve


	float x = 0;
	float y = 0;

	ofSetColor(255, 255, 255);
	//Lets make the curves out of a series
	//of points
	for(int i = 0; i < 800; i++){

		//Lets use the mouse x and y position
		//to affect the x and y paramters of
		//the curve. These values are quite
		//large, so we scale them down by 0.0001
		float xPct = (float)(i * mouseX) * 0.0001;
		float yPct = (float)(i * mouseY) * 0.0001;

		//lets also use the spin from the
		//rgb circles to transform the curve
		//over time
		xPct += spin * 0.002;
		yPct += spin * 0.003;

		//Lets feed these two values to
		//sin and cos functions and multiply
		//by how large we want it to be.
		//Because the sin function is producing
		//-1 to 1 results the total width in
		//this case will be double what we
		//specify. So 400 by 300.
		x =  200.0  * sin(xPct);
		y =  150.0  * cos(yPct);

		//We add the position we want them to be osicalting around
		//and draw the rects as small 2 pixel by 2 pixel squares
		ofRect(300 + x, 320 + y, 2, 2);
	}

	//---------------------------
	// use the bitMap type
	// note, this can be slow on some graphics cards
	// because it is using glDrawPixels which varies in
	// speed from system to system.  try using ofTrueTypeFont
	// if this bitMap type slows you down.
	ofSetHexColor(0xFFFFFF);
	ofDrawBitmapString("this is a Lissajous curve", 190, 500);
	ofDrawBitmapString("move mouse to spin me", 670, 500);

	//---------------------------
	// a bunch of lines
	for (int i = 0; i < 20; i++){

		//Lets make a cheesy sunset
		ofSetColor(255-i*10, 255-i*20, 0);

		ofLine(0,  i*4 , ofGetWidth(), i*4);
	}

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	//update spinPct by the distance the mouse
	//moved in x and y. We use abs so it always
	//spins in the same direction

	// we use the "bFirstMouseMove" flag so that we calculate only
	// after we have the first prevMY and prevMX stored;

	if (bFirstMouseMove == false){
		spinPct += (float)abs( y - prevMY ) * 0.03;
		spinPct += (float)abs( x - prevMX ) * 0.03;
	} else {
		bFirstMouseMove = false;
		//turn the flag off
	}

	//store the x and y
	//so we can get the prev value
	//next time the mouse is moved
	prevMY = y;
	prevMX = x;


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

