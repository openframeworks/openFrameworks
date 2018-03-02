#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// load the file and check if it was loaded ok. Also check it's
	// format is correct, it needs to have:
	//
	// - a root called drawing which
	// - contains a background element which itself contains nodes
	//   red, green and blue
	if(!xml.load("points.xml")){
		ofLogError() << "Couldn't load file";
	}
	auto drawing = xml.getChild("drawing");
	if(!drawing){
		drawing = xml.appendChild("drawing");
	}

	// Look for a background tag with children red, green and blue
	bg = drawing.findFirst("background[red and green and blue]");
	if(!bg){
		// remove any possible malformed background tag
		drawing.removeChild("background");

		// create the correct background tag and add color components
		bg = drawing.appendChild("background");
		bg.appendChild("red").set(255);
		bg.appendChild("green").set(255);
		bg.appendChild("blue").set(255);


		/*
		// Load OF icon from svg into our custom xml format.
		// Needs ofxSvg in the project, won't compile otherwise
		ofxSVG svg;
		svg.load("of.svg");
		for(auto path: svg.getPaths()){
			path.setFilled(false);
			path.setStrokeWidth(1);
			for(auto & outline: path.getOutline()){
				auto stroke = drawing.appendChild("stroke");
				for(auto & p: outline){
					auto pt = stroke.appendChild("pt");
					pt.setAttribute("x", int(p.x * 4 + ofGetWidth()/2));
					pt.setAttribute("y", int(p.y * 4 + ofGetHeight()/2));
				}
				auto pt = stroke.appendChild("pt");
				pt.setAttribute("x", int(outline[0].x * 4 + ofGetWidth()/2));
				pt.setAttribute("y", int(outline[0].y * 4 + ofGetHeight()/2));
			}
		}*/
	}


	// select background and read it's values
	auto background = xml.findFirst("//background");
	if(background){ // this will always be true, only to document how to search
		bgColor.r = background.getChild("red").getIntValue();
		bgColor.g = background.getChild("green").getIntValue();
		bgColor.b = background.getChild("blue").getIntValue();
	}

	// select all strokes and iterate through them
	// for each stroke, create a new mesh
	auto strokesXml = xml.find("//drawing/stroke");
	for(auto & stroke: strokesXml){
		strokes.emplace_back();
		strokes.back().setMode(OF_PRIMITIVE_LINE_STRIP);

		// for each pt in the stroke insert a new
		// vertex in the mesh
		auto pts = stroke.getChildren("pt");
		for(auto & pt: pts){
			auto x = pt.getAttribute("x").getIntValue();
			auto y = pt.getAttribute("y").getIntValue();
			strokes.back().addVertex({x,y,0});
		}
	}

	//load a monospaced font
	//which we will use to show part of the xml structure
	ttf.load("mono.ttf", 7);

	ofDisableAntiAliasing();
}

//--------------------------------------------------------------
void ofApp::update(){
	if(xmlChanged){
		ofBuffer auxBuffer;
		auxBuffer.set(xml.toString("  "));
		xmlText.clear();
		auto i = 0;
		for(auto & line: auxBuffer.getReverseLines()){
			xmlText = line + "\n" + xmlText;
			++i;
			if(i>58) break;
		}
		xmlChanged = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(bgColor);
	ofSetColor(0);
	for(auto & stroke: strokes){
		stroke.draw();
	}

	//--------
	//we make a black area on the left
	//which we can print the xml text on
	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, 200);
	ofDrawRectangle(0, 0, 160, ofGetHeight());
	ofDisableAlphaBlending();

	//our text that shows how the <STROKE> data looks in the xml file
	ofSetColor(240, 240, 240);

	string drawString = "How the data is stored:\n\n" + xmlText;
	ttf.drawString(drawString, 5, 10);

	//the message bars at the top and bottom of the app
	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, 200);

	ofDrawRectangle(160, 0, ofGetWidth()-160, 20);
	ofDrawRectangle(160, ofGetHeight()-20, ofGetWidth()-160, 20);

	//we draw our status message at the top
	ofSetColor(240, 240, 240);
	ttf.drawString("Save settings to XML hit 's' key", 170, 12);

	//instructions at the bottom
	ttf.drawString("mouse drag changes background color and records stroke", 168, ofGetHeight() - 9);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 's'){
		if(!xml.save("points.xml")){
			ofLogError() << "Couldn't save points.xml";
		}
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
	bgColor.r		= xpct * 255.0f;
	bgColor.g		= ypct * 255.0f;
	bgColor.b		= (int)(bgColor.r - bgColor.g) % 255;

	bg.getChild("red").set(bgColor.r);
	bg.getChild("green").set(bgColor.g);
	bg.getChild("blue").set(bgColor.b);

	//-------------
	//we also want to record the stroke
	//lets store the drag of the user.
	auto pt = currentStroke.appendChild("pt");
	pt.setAttribute("x", x);
	pt.setAttribute("y", y);
	strokes.back().addVertex({x,y,0});
	xmlChanged = true;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	// Insert a new stroke tag inside drawing before the background tag
	currentStroke = xml.getChild("drawing").insertChildBefore("stroke", bg);
	strokes.emplace_back();
	strokes.back().setMode(OF_PRIMITIVE_LINE_STRIP);
	xmlChanged = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
