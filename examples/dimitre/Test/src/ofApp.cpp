#include "ofApp.h"

using namespace std;
string path = "-";
//--------------------------------------------------------------
void ofApp::setup(){
//	cout << "setup ---- " << endl;
//	ofSetDataPathRoot("/Users/z/Desktop/data");
//	cout << defaultWorkingDirectory() << endl;
//	cout << "of::filesystem::current_path() " << of::filesystem::current_path() << endl;
//
//	cout << "ofToDataPath = " << ofToDataPath("") << endl;
//
//	cout << "setup ---- " << endl;

	

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(path, 10, 30);
}

void ofApp::keyPressed(int key){
	if (key == 'a') {
		auto s = ofSystemTextBoxDialog("Anunciação what is your favorite color?" , "Anunciação blue");
	}
	else if (key == 's') {
		ofFileDialogResult result = ofSystemLoadDialog("Anunciação Load file");
		if(result.bSuccess) {
		   path = result.getPath();
		  // load your file at `path`
		}
	}
}
