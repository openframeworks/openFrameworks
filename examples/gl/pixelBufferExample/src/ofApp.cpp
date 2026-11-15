#include "ofApp.h"

// this example will probably only work in real time in release mode

//--------------------------------------------------------------
void ofApp::setup(){
	fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGB);
	pixelBufferBack.allocate(ofGetWidth()*ofGetHeight()*3,GL_DYNAMIC_READ);
	pixelBufferFront.allocate(ofGetWidth()*ofGetHeight()*3,GL_DYNAMIC_READ);
	box.set(400);
	box.setResolution(1);
	box.setPosition({ofGetWidth()*0.5, ofGetHeight()*0.5, 0});
	record = false;
}

//--------------------------------------------------------------
void ofApp::update(){

	// Set the angles of rotation on each axis.
	float xAngleRad = ofDegToRad(45.0);
	float yAngleRad = ofDegToRad(ofGetElapsedTimef() * 10);
	float zAngleRad = ofDegToRad(45.0);

	// Construct a quaternion.
	glm::quat quat(1,0,0,0);

	quat *= glm::angleAxis(xAngleRad, glm::vec3(1, 0, 0));
	quat *= glm::angleAxis(yAngleRad, glm::vec3(0, 1, 0));
	quat *= glm::angleAxis(zAngleRad, glm::vec3(0, 0, 1));

	// Set the orientation of the box based on the construxcted quaternion.
	box.setOrientation(quat);

	ofEnableDepthTest();
	fbo.begin();
	ofClear(0,255);
	ofSetColor(100);
	box.setScale(1);
	box.draw();
	ofSetColor(255);
	box.setScale(1.001);
	box.drawWireframe();
	fbo.end();
	ofDisableDepthTest();
#ifndef TARGET_EMSCRIPTEN
	if(record){
		// copy the fbo texture to a buffer
		fbo.getTexture().copyTo(pixelBufferBack);

		// map the buffer so we can access it from the cpu
		// and wrap the memory in an ofPixels to save it
		// easily. Finally unmap it.
		pixelBufferFront.bind(GL_PIXEL_UNPACK_BUFFER);
		unsigned char * p = pixelBufferFront.map<unsigned char>(GL_READ_ONLY);
		pixels.setFromExternalPixels(p,fbo.getWidth(),fbo.getHeight(),OF_PIXELS_RGB);
		ofSaveImage(pixels,ofToString(ofGetFrameNum())+".jpg");
		pixelBufferFront.unmap();

		// swap the front and back buffer so we are always
		// copying the texture to one buffer and reading
		// back from another to avoid stalls
		swap(pixelBufferBack,pixelBufferFront);
	}
#endif
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	fbo.draw(0,0);
	ofDrawBitmapString(ofGetFrameRate(),20,20);
#ifndef TARGET_EMSCRIPTEN
	if(record){
		ofDrawBitmapString("'r' toggles recording (on)",20,40);
	}else{
		ofDrawBitmapString("'r' toggles recording (off)",20,40);
	}
#else
	ofDrawBitmapString("'d' downloads screenshot",20,40);
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key=='r'){
		record = !record;
	}
#ifdef TARGET_EMSCRIPTEN
	if (key=='d') {
		fbo.readToPixels(pixels);
		ofSaveImage(pixels, "screenshot.jpg");
		EM_ASM(
		var content = FS.readFile("/data/screenshot.jpg");
		FS.unlink("/data/screenshot.jpg");
		var a = document.createElement('a');
		a.download = "screenshot.jpg";
		var blob = new Blob([content], {type: "text/plain;charset=utf-8"});
		a.href = URL.createObjectURL(blob);
		document.body.appendChild(a);
		a.click();
		document.body.removeChild(a);
		URL.revokeObjectURL(a.href);
		);
	}
#endif
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
