#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
	ofDisableAntiAliasing();
    
	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( xml.load("mySettings.xml") ){
		message = "mySettings.xml loaded!";
	}else{
    
        // Ok, we didn't get a file, but that's ok, we're going
        // to go ahead and start making some stuff anyways! First, to
        // make a correct XML document, we need a root element, so
        // let's do that:
        
		xml.addChild("drawing");
        
        // now we set our current element so we're on the right
        // element, so when we add new nodes, they're still inside
        // the root element
		xml.setTo("drawing");
        
		message = "unable to load mySettings.xml check data/ folder";
	}
    
	//read the colors from XML or, if they don't exist, because we've
    // loaded them from elsewhere, let's just make some stuff up

    //the double forward slash //RED means: search from the root of the xml for a tag could RED
	//otherwise the search would be relative to where you are in the xml structure. // = absolute
	if(xml.exists("//red")) {
		red	= xml.getValue<int>("//red");
    } else {
        red = 170;
    }
       
	if(xml.exists("//green")) {
		green	= xml.getValue<int>("//green");
    } else {
        green = 190;
    }
    
	if(xml.exists("//blue")) {
		blue	= xml.getValue<int>("//blue");
    } else {
        blue = 240;
    }
    
    // If we have STROKE nodes that we've already created, then we can go ahead and
    // load them into the dragPts so they're drawn to the screen
	if(xml.exists("stroke"))
    {
		// This gets the first stroke (notice the [1], it's just like an array but the first index is 1)
		xml.setTo("stroke[1]");
        
        
        do {
            // set our "current" PT to the first one
			if(xml.getName() == "stroke" && xml.setTo("pt[1]"))
            {
				dragPts.emplace_back();
				dragPts.back().setMode(OF_PRIMITIVE_LINE_STRIP);
                // get each individual x,y for each point
                do {
					int x = xml.getValue<int>("x");
					int y = xml.getValue<int>("y");

					dragPts.back().addVertex({x,y,0.f});
                }
				while(xml.setToSibling()); // go the next PT
                
                // go back up
				xml.setToParent();
            }
            
        }
		while( xml.setToSibling() ); // go to the next STROKE
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

	ofBuffer auxBuffer;
	auxBuffer.set(xml.toString("  "));
	xmlStructure.clear();
	auto i = 0;
	for(auto & line: auxBuffer.getReverseLines()){
		xmlStructure = line + "\n" + xmlStructure;
		++i;
		if(i>58) break;
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	//---------
	//Lets draw the stroke as a continous line
	ofSetHexColor(0x222222);
	for(auto & lines: dragPts){
		lines.draw();
	}

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
	if(xmlStructure.size() > 0) drawString += xmlStructure;
	TTF.drawString(drawString, 5, 10);
    
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
		xml.save("mySettings.xml");

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
	point.addChild("pt");
	point.setTo("pt");
    
	point.setAttribute("x", ofToString(x));
	point.setAttribute("y", ofToString(y));
    
	xml.addXml(point);
    
	//------------
	//we also record the x y points into an array - so we can draw it
	dragPts.back().addVertex({x,y,0.f});


}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	// let's go back to the root (this is the same thing as reset() btw)
	xml.setTo("//drawing");
    
    // add a new stroke and then figure out how many strokes there are
	xml.addChild("stroke");
	int strokes = xml.getNumChildren("stroke");
    
    // use a compound path: STROKE[2] for example to set the current element
    // to the 3rd stroke tag
	xml.setTo("stroke[" + ofToString(strokes) + "]");

	dragPts.emplace_back();
	dragPts.back().setMode(OF_PRIMITIVE_LINE_STRIP);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	//update the colors to the XML structure when the mouse is released
	xml.setTo("//drawing"); // set back to the root node
    
    // did we already make a color? Because if so, we don't
    // even need to go down into that node, we can just use the //
    // syntax to set it because there's only 1 in the document
	if(xml.exists("background"))
    {
		xml.setValue("//red", ofToString(red));
		xml.setValue("//green", ofToString(green));
		xml.setValue("//blue", ofToString(blue));
    }
    else
    {
        // otherwise, we haven't made one, so let's go ahead
        // and do that! First make a <BACKGROUND> and <COLOR>
        
		xml.addChild("background");
		xml.setTo("background");
        
        // now add a RED, GREEN, and BLUE to it.
		xml.addValue("red", ofToString(red));
		xml.addValue("green", ofToString(green));
		xml.addValue("blue", ofToString(blue));
        
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

