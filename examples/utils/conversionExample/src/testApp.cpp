#include "testApp.h"

//--------------------------------------------------------------
void splitTest(string splitInput, string delimeter) {
	cout << "splitting '" << splitInput << "' with '" << delimeter << "'" << endl;
	cout << "ignoreEmpty:false trim:false " << ofToString(ofSplitString(splitInput, delimeter, false, false)) << endl;
	cout << "ignoreEmpty:false trim:true " << ofToString(ofSplitString(splitInput, delimeter, false, true)) << endl;
	cout << "ignoreEmpty:true trim:false " << ofToString(ofSplitString(splitInput, delimeter, true, false)) << endl;
	cout << "ignoreEmpty:true trim:true " << ofToString(ofSplitString(splitInput, delimeter, true, true)) << endl;
	cout << endl;
}

//--------------------------------------------------------------
void testApp::setup() {
	string splitInput = " a,,b,c,";
	splitTest(" a,,b, c , ", ",");
	splitTest(" a<><>b<> c <> ", "<>");
	 
	 cout << "empty" << endl;
	 cout << ofToInt("") << endl;
	 cout << ofToFloat("") << endl;
	 cout << ofToBool("") << endl;
	 cout << ofToChar("") << endl;
	 cout << endl;
	 
	 cout << "strings" << endl;
	 cout << ofToString(123) << endl;
	 cout << ofToString(123.4) << endl;
	 cout << ofToString(123.4f) << endl;
	 cout << ofToString(123, 5) << endl;
	 cout << ofToString(123.4, 5) << endl;
	 cout << ofToString(123.4f, 5) << endl;
	 cout << endl;
	 
	 cout << "ints" << endl;
	 cout << ofToInt("123.") << endl;
	 cout << ofToInt("123.4") << endl;
	 cout << ofToInt("123.4f") << endl;
	 cout << ofToInt("+123.4") << endl;
	 cout << endl;
	 
	 cout << "float" << endl;
	 cout << ofToFloat("123.") << endl;
	 cout << ofToFloat("123.4") << endl;
	 cout << ofToFloat("123.4f") << endl;
	 cout << ofToFloat("+123.4") << endl;
	 cout << endl;
	 
	 cout << "bool" << endl;
	 cout << ofToBool("1") << endl;
	 cout << ofToBool("0") << endl;
	 cout << ofToBool("true") << endl;
	 cout << ofToBool("false") << endl;
	 cout << ofToBool("True") << endl;
	 cout << ofToBool("False") << endl;
	 cout << endl;
	 
	 cout << "char" << endl;
	 cout << ofToChar("a") << endl;
	 cout << ofToChar("b") << endl;
	 cout << ofToChar("1") << endl;
	 cout << ofToChar("0") << endl;
	 cout << endl;	
	 
	 cout << "to binary" << endl;
	 cout << ofToBinary(8) << endl;
	 cout << ofToBinary(64) << endl;
	 cout << ofToBinary(1024) << endl;
	 cout << ofToBinary('a') << endl;
	 cout << ofToBinary(8.5) << endl;
	 cout << ofToBinary(8.f) << endl;
	 cout << ofToBinary(8.5f) << endl;
	 cout << ofToBinary(9.f) << endl;
	 string strVal = "abc";
	 cout << ofToBinary(strVal) << endl;
	 cout << ofToBinary("abc") << endl;
	 cout << endl;
	 
	 cout << "to binary" << endl;
	 cout << ofBinaryToInt(ofToBinary(8)) << endl;
	 cout << ofBinaryToInt(ofToBinary(64)) << endl;
	 cout << ofBinaryToInt(ofToBinary(1024)) << endl;
	 cout << ofBinaryToChar(ofToBinary('a')) << endl;
	 cout << ofBinaryToFloat(ofToBinary(8.5f)) << endl;
	 cout << ofBinaryToFloat(ofToBinary(64.5f)) << endl;
	 cout << ofBinaryToString(ofToBinary("abc")) << endl;
	 cout << endl;
	 
	 cout << "to hex" << endl;
	 cout << ofToHex('a') << endl;
	 cout << ofToHex(128) << endl;
	 cout << ofToHex(128.f) << endl;
	 cout << ofToHex(128.) << endl;
	 cout << ofToHex("abc") << endl;
	 cout << ofToHex("abcd") << endl;
	 string val = "abcdefg";
	 cout << ofToHex(val) << endl;
	 cout << endl;
	 
	 cout << "hex to int" << endl;
	 cout << ofHexToInt("0xff") << endl;
	 cout << ofHexToInt("ff") << endl;
	 cout << ofHexToInt(ofToHex(128)) << endl;
	 cout << ofHexToFloat(ofToHex(128.5f)) << endl;
	 cout << ofHexToChar(ofToHex('a')) << endl;
	 cout << ofHexToString(ofToHex("abc")) << endl;
	 cout << endl;
}

//--------------------------------------------------------------
void testApp::update() {
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(0);
	ofSetColor(255);
	ofDrawBitmapString("See the console for output.", 10, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
