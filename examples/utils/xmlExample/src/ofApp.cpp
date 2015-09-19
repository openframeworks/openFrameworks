#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
    ofDisableAntiAliasing();
	//-----------
	//the string is printed at the top of the app
	//to give the user some feedback
	message = "loading mySettings.xml";
    
	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( XML.load("mySettings.xml") ){
		message = "mySettings.xml loaded!";
	}else{
    
        // Ok, we didn't get a file, but that's ok, we're going
        // to go ahead and start making some stuff anyways! First, to
        // make a correct XML document, we need a root element, so
        // let's do that:
        
        XML.addChild("DRAWING");
        
        // now we set our current element so we're on the right
        // element, so when we add new nodes, they're still inside
        // the root element
        XML.setTo("DRAWING");
        
		message = "unable to load mySettings.xml check data/ folder";
	}
    
	//read the colors from XML or, if they don't exist, because we've
    // loaded them from elsewhere, let's just make some stuff up

    //the double forward slash //RED means: search from the root of the xml for a tag could RED
    //otherwise the search would be relative to where you are in the xml structure. // = absolute
    if(XML.exists("//RED")) {
        red	= XML.getValue<int>("//RED");
    } else {
        red = 170;
    }
       
    if(XML.exists("//GREEN")) {
        green	= XML.getValue<int>("//GREEN");
    } else {
        green = 190;
    }
    
    if(XML.exists("//BLUE")) {
        blue	= XML.getValue<int>("//BLUE");
    } else {
        blue = 240;
    }
    
    // If we have STROKE nodes that we've already created, then we can go ahead and
    // load them into the dragPts so they're drawn to the screen
    if(XML.exists("STROKE"))
    {
        // This gets the first stroke (notice the [0], it's just like an array)
        XML.setTo("STROKE[0]");
        
        
        do {
            // set our "current" PT to the first one
            if(XML.getName() == "STROKE" && XML.setTo("PT[0]"))
            {
                // get each individual x,y for each point
                do {
                    int x = XML.getValue<int>("X");
                    int y = XML.getValue<int>("Y");
                    ofVec2f v(x, y);

                    dragPts.push_back(v);
                }
                while(XML.setToSibling()); // go the next PT
                
                // go back up
                XML.setToParent();
            }
            
        }
        while( XML.setToSibling() ); // go to the next STROKE
    }
    
	//load a monospaced font
	//which we will use to show part of the xml structure
	TTF.load("mono.ttf", 7);
}

//--------------------------------------------------------------
void ofApp::update(){
	//we change the background color here based on the values
	//affected by the mouse position
	ofBackground((int)red,(int)green,(int)blue);

}

//--------------------------------------------------------------
void ofApp::draw(){

	//---------
	//Lets draw the stroke as a continous line
	ofSetHexColor(0x222222);
	ofNoFill();
	ofBeginShape();
	for(int i = 0; i < dragPts.size(); i++){
		ofVertex(dragPts[i].x, dragPts[i].y);
	}
	ofEndShape(false);
    
    ofFill();
    
	//--------
	//we make a black area on the left
	//which we can print the xml text on
	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, 200);
	ofDrawRectangle(0, 0, 160, ofGetHeight());
	ofDisableAlphaBlending();
    
	//our text that shows how the <STROKE> data looks in the xml file
	ofSetColor(240, 240, 240);
    
	string drawString = "How the data is stored:\n\n";
	if(xmlStructure.size() > 0) drawString += xmlStructure+"</STROKE>";
	TTF.drawString(drawString, 5, 40);
    
	//the message bars at the top and bottom of the app
	//ofSetHexColor(0xDDDDDD);
	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, 200);
    
	ofDrawRectangle(160, 0, ofGetWidth()-160, 20);
	ofDrawRectangle(160, ofGetHeight()-20, ofGetWidth()-160, 20);
    
	//we draw our status message at the top
	//ofSetColor(210, 90, 100);
	ofSetColor(240, 240, 240);
	TTF.drawString("Save settings to XML hit 's' key     status: "+message, 170, 12);
    
	//instructions at the bottom
	TTF.drawString("mouse drag changes background color and records stroke", 168, ofGetHeight() - 9);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

    //no data gets saved unless you hit the s key
    if(key == 's')
    {
        XML.save("mySettings.xml");

        message = "settings saved to xml!";
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

	//-------
	//we change the background color based on
	//the two mouse coords coming in
	float xpct = (float)x / ofGetWidth();
	float ypct = (float)y / ofGetHeight();
	red			= xpct * 255.0;
	green		= ypct * 255.0;
	blue		= (int)(red - green) % 255;
    
    
	//-------------
	//we also want to record the stroke
    
	//lets store the drag of the user.
	//we just make a new XML object and then add it to the element
    // that we're already pointing at, which as you'll note in update()
    // will be the newest <STROKE> tag

    
    ofXml point;
    point.addChild("PT");
    point.setTo("PT");
    
    point.addValue("X", x);
    point.addValue("Y", y);
    
    XML.addXml(point);

    
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
	xmlStructure += "    <PT>\n        <X>"+ofToString(x)+"</X>\n        <Y>"+ofToString(y)+"</Y>\n    </PT>\n";
	lineCount+=4; //we have added 4 lines so increment by 4
    
	//------------
	//we also record the x y points into an array - so we can draw it
    ofVec2f v(x,y);
    dragPts.push_back(v);


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    //lets clear everything on mouse pressed so we save just one stroke. 
    dragPts.clear();
    XML.clear();
    XML.addChild("DRAWING");

	// let's go back to the root (this is the same thing as reset() btw)
    XML.setTo("//DRAWING");
    
    // add a new stroke and then figure out how many strokes there are
	XML.addChild("STROKE");
    int strokes = XML.getNumChildren("STROKE");
    
    // use a compound path: STROKE[2] for example to set the current element
    // to the 3rd stroke tag
    XML.setTo("STROKE[" + ofToString(strokes - 1) + "]");
    
	xmlStructure	= "<STROKE>\n";
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	//update the colors to the XML structure when the mouse is released
    XML.setTo("//DRAWING"); // set back to the root node
    
    // did we already make a color? Because if so, we don't
    // even need to go down into that node, we can just use the //
    // syntax to set it because there's only 1 in the document
    if(XML.exists("BACKGROUND"))
    {
        XML.setValue("//RED", ofToString(red));
        XML.setValue("//GREEN", ofToString(green));
        XML.setValue("//BLUE", ofToString(blue));
    }
    else
    {
        // otherwise, we haven't made one, so let's go ahead
        // and do that! First make a <BACKGROUND> and <COLOR>
        
        XML.addChild("BACKGROUND");
        XML.setTo("BACKGROUND");
        XML.addChild("COLOR");
        XML.setTo("COLOR");
        
        // now add a RED, GREEN, and BLUE to it.
        XML.addValue("RED", ofToString(red));
        XML.addValue("GREEN", ofToString(green));
        XML.addValue("BLUE", ofToString(blue));
        
    }
}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

