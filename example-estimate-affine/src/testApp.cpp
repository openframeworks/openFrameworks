#include "testApp.h"

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
	ofSetVerticalSync(true);
	
	ofMatrix4x4 rigid;
	ofVec3f translation(ofRandomf(), ofRandomf(), ofRandomf());
	ofQuaternion rotation(ofRandomf(), ofVec3f(ofRandomf(), ofRandomf(), ofRandomf()));
	rigid.translate(translation);
	rigid.rotate(rotation);
	
	vector<ofVec3f> from;
	for(int i = 0; i < 4; i++) {
		from.push_back(ofVec3f(ofRandom(1,2), ofRandom(1,2), ofRandom(5,6)));
		from.push_back(ofVec3f(ofRandom(3,4), ofRandom(3,4), ofRandom(5,6)));
		from.push_back(ofVec3f(ofRandom(1,2), ofRandom(3,4), ofRandom(5,6)));
		from.push_back(ofVec3f(ofRandom(3,4), ofRandom(1,2), ofRandom(5,6)));
	}
	
	vector<ofVec3f> to;
	for(int i = 0; i < from.size(); i++) {
		// opencv assumes you're doing premultiplication
		to.push_back(rigid.preMult(from[i]));
	}
	
	ofMatrix4x4 rigidEstimate = estimateAffine3D(from, to);

	cout << "original matrix: " << endl << rigid << endl;
	cout << "estimated as: " << endl << rigidEstimate << endl;
	
	for(int i = 0; i < from.size(); i++) {
		// opencv assumes you're doing premultiplication
		ofVec3f after = rigidEstimate.preMult(from[i]);
		cout << from[i] << " -> " << to[i] << " estimated as: " << after << endl;
	}
	
	ofVec3f decompTranslation, decompScale;
	ofQuaternion decompRotation, decompSo;
	rigidEstimate.decompose(decompTranslation, decompRotation, decompScale, decompSo);
	cout << "translation: " << translation << endl;
	cout << "estimated as: " << decompTranslation << endl;
	cout << "rotation: " << endl << rotation << endl;
	cout << "estimated as: " << endl << decompRotation << endl;
}

void testApp::update() {
}

void testApp::draw() {
	ofBackground(0);
	ofDrawBitmapString("See console window for results.", 10, 20);
}
