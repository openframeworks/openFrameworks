#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	currentIndex = 0;

	//Path to the comma delimited file
	string filePath = "morse.csv";
	
	//Use a ofTrueTypeFont for scalable text
	font.loadFont("frabk.ttf", 122);
	
	//Load file placed in bin/data
	ofFile file(filePath);
	
	if(!file.exists()){
		ofLogError("The file " + filePath + " is missing");
	}
	
	ofBuffer buffer(file);
	
	//Read file line by line
	while (!buffer.isLastLine()) {
		string line = buffer.getNextLine();
		
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
void testApp::update(){
	//Update our MorseCodePlayer with the app
	player.update();

}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofDrawBitmapString("PRESS A KEY (A-Z, 0-9) TO PLAY MORSE CODE", 20, 20);
	string line1 = currentSymbol.character;
	string line2 = currentSymbol.code;
	font.drawString(line1, (ofGetWidth() - font.stringWidth(line1))/2, ofGetHeight()/2);
	font.drawString(line2, (ofGetWidth() - font.stringWidth(line2))/2, ofGetHeight()/2+font.stringHeight(line1));
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	//Create a comparable string from an int
	string myKey;
	myKey = (char) key;
	myKey = ofToUpper(myKey);
	
	for (int i=0; i<morseCodeSymbols.size(); i++) {
		if (morseCodeSymbols[i].character == myKey){
			currentSymbol = morseCodeSymbols[i];
			player.playCode(currentSymbol.code);
		}
		
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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
void testApp::dragEvent(ofDragInfo dragInfo)
{
	

}




