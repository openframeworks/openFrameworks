#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("URL file loader progress");
	ofSetBackgroundColor(30);

	const std::string url = "https://github.com/openframeworks/openFrameworks/archive/refs/heads/master.zip";
	ofHttpRequest request(url, ofToDataPath("openFrameworks-master.zip"), true);
	request.progressCallback = [this](const ofHttpRequest &, float value) {
		progress.store(value);
	};
	request.done = [this](const ofHttpResponse & response) {
		status = response.status;
		finished = true;
	};
	loader.handleRequestAsync(request);
}

//--------------------------------------------------------------
void ofApp::draw() {
	const float value = progress.load();
	const float width = ofGetWidth() - 80.0f;

	ofSetColor(90);
	ofDrawRectangle(40, 100, width, 24);
	ofSetColor(70, 180, 255);
	ofDrawRectangle(40, 100, width * value, 24);

	ofSetColor(255);
	ofDrawBitmapString("Downloading openFrameworks-master.zip", 40, 70);
	ofDrawBitmapString(ofToString(value * 100.0f, 1) + "%", 40, 150);
	if (finished) {
		ofDrawBitmapString("Finished with HTTP status " + ofToString(status), 40, 185);
	}
}
