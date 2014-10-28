#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	
	currentIndex = 0;

	//Path to the comma delimited file
	string filePath = "morse.csv";
	
	//Use a ofTrueTypeFont for scalable text
	font.load("frabk.ttf", 122);
	
	//Load file placed in bin/data
	ofFile file(filePath);
	
	if(!file.exists()){
		ofLogError("The file " + filePath + " is missing");
	}
	
	ofBuffer buffer(file);

	//Read file line by line
	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
		string line = *it;
		
		//Split line into strings
		vector<string> words = ofSplitString(line, ",");
		
		//Store strings into a custom container
		MorseCodeSymbol symbol;
		symbol.character = words[0];
		symbol.code = words[1];
		
		//Save MorseCodeSymbols for later
		morseCodeSymbols.push_back(symbol);
		
		//Debug output
		ofLogVerbose("symbol.character: " + symbol.character);
		ofLogVerbose("symbol.code: " + symbol.code);
	}
	
	//Load our Morse code sounds
	player.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	//Update our MorseCodePlayer with the app
	player.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofDrawBitmapString("PRESS A KEY (A-Z, 0-9) TO PLAY MORSE CODE", 20, 20);
	string line1 = currentSymbol.character;
	string line2 = currentSymbol.code;
	font.drawString(line1, (ofGetWidth() - font.stringWidth(line1))/2, ofGetHeight()/2);
	font.drawString(line2, (ofGetWidth() - font.stringWidth(line2))/2, ofGetHeight()/2+font.stringHeight(line1));
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	//Create a comparable string from an int
	string myKey;
	myKey = (char) key;
	myKey = ofToUpper(myKey);
	
	for (unsigned int i=0; i<morseCodeSymbols.size(); i++) {
		if (morseCodeSymbols[i].character == myKey){
			currentSymbol = morseCodeSymbols[i];
			player.playCode(currentSymbol.code);
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
void ofApp::dragEvent(ofDragInfo dragInfo)
{
	

}




