#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);

	ofBackground(255,255,255);

	//-----------
	//the string is printed at the top of the app
	//to give the user some feedback
	message = "loading mySettings.xml";

	//we load our settings file
	//initially we do this from the data/ folder
	//but when we save we are saving to the documents folder which is the only place we have write access to
	//thats why we check if the file exists in the documents folder. 
	
	if( XML.loadFile(ofxiPhoneGetDocumentsDirectory() + "mySettings.xml") ){
		message = "mySettings.xml loaded from documents folder!";
	}else if( XML.loadFile("mySettings.xml") ){
		message = "mySettings.xml loaded from data folder!";
	}else{
		message = "unable to load mySettings.xml check data/ folder";
	}
	
	//read the colors from XML
	//if the settings file doesn't exist we assigns default values (170, 190, 240)
	red		= XML.getValue("BACKGROUND:COLOR:RED", 170);
	green	= XML.getValue("BACKGROUND:COLOR:GREEN", 190);
	blue	= XML.getValue("BACKGROUND:COLOR:BLUE", 240);

	/*
		"BACKGROUND:COLOR:RED" referes to a structure like this:

		<BACKGROUND>
			<COLOR>
				<RED>101.103516</RED>
			</COLOR>
		</BACKGROUND>
	*/

	//we initalize some of our variables
	lastTagNumber	= 0;
	pointCount		= 0;
	lineCount		= 0;


	//-------
	//this is a more advanced use of ofXMLSettings
	//we are going to be reading multiple tags with the same name

	//lets see how many <STROKE> </STROKE> tags there are in the xml file
	int numDragTags = XML.getNumTags("STROKE:PT");

	//if there is at least one <STROKE> tag we can read the list of points
	//and then try and draw it as a line on the screen
	if(numDragTags > 0){

		//we push into the last STROKE tag
		//this temporarirly treats the tag as
		//the document root.
		XML.pushTag("STROKE", numDragTags-1);

			//we see how many points we have stored in <PT> tags
			int numPtTags = XML.getNumTags("PT");

			if(numPtTags > 0){

				//We then read those x y values into our
				//array - so that we can then draw the points as
				//a line on the screen

				//we have only allocated a certan amount of space for our array
				//so we don't want to read more than that amount of points
				int totalToRead = MIN(numPtTags, NUM_PTS);

				for(int i = 0; i < totalToRead; i++){
					//the last argument of getValue can be used to specify
					//which tag out of multiple tags you are refering to.
					int x = XML.getValue("PT:X", 0, i);
					int y = XML.getValue("PT:Y", 0, i);
					dragPts[i].set(x, y);
					pointCount++;
				}
			}

		//this pops us out of the STROKE tag
		//sets the root back to the xml document
		XML.popTag();
	}

	//load a monospaced font
	//which we will use to show part of the xml structure
	TTF.loadFont("mono.ttf", 7);
}

//--------------------------------------------------------------
void testApp::update(){
	//we change the background color here based on the values
	//affected by the mouse position
	ofBackground((int)red,(int)green,(int)blue);

}

//--------------------------------------------------------------
void testApp::draw(){

	string drawString = "How the data is stored:\n\n";
	if(xmlStructure.size() > 0){
		ofEnableAlphaBlending();
		ofSetColor(255-red, 255-green, 255-blue, 180);	
		drawString += xmlStructure+"</STROKE>";
		TTF.drawString(drawString, 5, 60);
	}
	
	//---------
	//Lets draw the stroke as a continous line
	ofSetHexColor(0x222222);
	ofNoFill();
	ofBeginShape();
	for(int i = 0; i < pointCount; i++){
		ofVertex(dragPts[i].x, dragPts[i].y);
	}
	ofEndShape(false);

    ofFill();

	//the message bars at the top and bottom of the app
	//ofSetHexColor(0xDDDDDD);
	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, 200);

	ofRect(0, 0, ofGetWidth(), 36);
	ofRect(0, ofGetHeight()-20, ofGetWidth(), 20);

	//we draw our status message at the top
	//ofSetColor(210, 90, 100);
	ofSetColor(240, 240, 240);
	TTF.drawString("Settings are automatically saved to app documents folder\nmouse drag changes background color and records stroke", 10, 15);

	//instructions at the bottom
	TTF.drawString("Status: "+message, 10, ofGetHeight() - 9);
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){

	if( touch.id == 0 ){
		//we can also add tags with the same name.
		//here we are just adding an empty tag
		//and when the user drags their mouse
		//we will store the pts in this tag
		lastTagNumber	= XML.addTag("STROKE");
		xmlStructure	= "<STROKE>\n";

		//We start a new stroke
		lineCount		= 0;
		pointCount		= 0;
	}

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){

	if( touch.id == 0 ){

		//-------
		//we change the background color based on
		//the two mouse coords coming in
		float xpct = (float)touch.x / ofGetWidth();
		float ypct = (float)touch.y / ofGetHeight();
		red			= xpct * 255.0;
		green		= ypct * 255.0;
		blue		= (int)(red - green) % 255;


		//-------------
		//we also want to record the stroke

		//lets store the drag of the user.
		//we will push into the most recent
		//<STROKE> tag - add the mouse points
		//then pop out
		if( XML.pushTag("STROKE", lastTagNumber) ){

			//now we will add a pt tag - with two
			//children - X and Y

			int tagNum = XML.addTag("PT");
			XML.setValue("PT:X", touch.x, tagNum);
			XML.setValue("PT:Y", touch.y, tagNum);

			XML.popTag();
		}

		//-------------
		//here we make a string of text that looks like how the xml data
		//is stored in the settings file - this is just so people can
		//visually see how the data is stored.

		//if the text is about to go off screen
		if(lineCount > 64){
			//we find the first <PT> tag with the
			//x and y data and we remove it from the begining
			//this way the displayed text always shows the newest data
			//without going offscreen.
			int pos = xmlStructure.find("</PT>");
			xmlStructure = xmlStructure.substr(pos+6);
		}

		//here we add the most recent point to our fake xml string
		xmlStructure += "    <PT>\n        <X>"+ofToString(touch.x)+"</X>\n        <Y>"+ofToString(touch.y)+"</Y>\n    </PT>\n";
		lineCount+=4; //we have added 4 lines so increment by 4

		//------------
		//we also record the x y points into an array - so we can draw it
		if(pointCount < NUM_PTS -1){
			dragPts[pointCount].set(touch.x, touch.y);
			pointCount++;
		}
	
	}

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	//update the colors to the XML structure when the mouse is released
	XML.setValue("BACKGROUND:COLOR:RED", red);
	XML.setValue("BACKGROUND:COLOR:GREEN", green);
	XML.setValue("BACKGROUND:COLOR:BLUE", blue);

	XML.saveFile( ofxiPhoneGetDocumentsDirectory() + "mySettings.xml" );
	message = "mySettings.xml saved to app documents folder";
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}



