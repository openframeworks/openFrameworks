#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableSmoothing();
	ofSetVerticalSync(true);
	
	video.initGrabber(320, 240);
	finder.setup("haarcascade_frontalface_default.xml");
	usePreview = false;
	
	previewCamera.setDistance(3.0f);
	previewCamera.setNearClip(0.01f);
	previewCamera.setFarClip(500.0f);
	previewCamera.setPosition(0.4f, 0.2f, 0.8f);
	previewCamera.lookAt(ofVec3f(0.0f, 0.0f, 0.0f));
	
	headTrackedCamera.setNearClip(0.01f);
	headTrackedCamera.setFarClip(1000.0f);
	
	//defining the real world coordinates of the window which is being headtracked is important for visual accuracy
	windowWidth = 0.3f;
	windowHeight = 0.2f;
	
	windowTopLeft = ofVec3f(-windowWidth / 2.0f,
							+windowHeight / 2.0f,
							0.0f);
	windowBottomLeft = ofVec3f(-windowWidth / 2.0f,
							   - windowHeight / 2.0f,
							   0.0f);
	windowBottomRight = ofVec3f(+windowWidth / 2.0f,
								-windowHeight / 2.0f,
								0.0f);
	
	//we use this constant since we're using a really hacky headtracking in this example
	//if you use something that can properly locate the head in 3d (like a kinect), you don't need this fudge factor
	viewerDistance = 0.4f;
}

//--------------------------------------------------------------
void testApp::update(){
	video.update();
	finder.findHaarObjects(video.getPixelsRef());
	
	ofVec3f headPosition(0,0,viewerDistance);
	
	if (finder.blobs.size() > 0) {
		//get the head position in camera pixel coordinates
		const ofxCvBlob & blob = finder.blobs.front();
		float cameraHeadX = blob.centroid.x;
		float cameraHeadY = blob.centroid.y;
		
		//do a really hacky interpretation of this, really you should be using something better to find the head (e.g. kinect skeleton tracking)
		
		//since camera isn't mirrored, high x in camera means -ve x in world
		float worldHeadX = ofMap(cameraHeadX, 0, video.getWidth(), windowBottomRight.x, windowBottomLeft.x);
		
		//low y in camera is +ve y in world
		float worldHeadY = ofMap(cameraHeadY, 0, video.getHeight(), windowTopLeft.y, windowBottomLeft.y);
		
		//set position in a pretty arbitrary way
		headPosition = ofVec3f(worldHeadX, worldHeadY, viewerDistance);
	} else {
		if (!video.isInitialized()) {
			//if video isn't working, just make something up
			headPosition = ofVec3f(0.5f * windowWidth * sin(ofGetElapsedTimef()), 0.5f * windowHeight * cos(ofGetElapsedTimef()), viewerDistance);
		}
	}
	
	headPositionHistory.push_back(headPosition);
	while (headPositionHistory.size() > 50.0f){
		headPositionHistory.pop_front();
	}

	//these 2 lines of code must be called every time the head position changes
	headTrackedCamera.setPosition(headPosition);
	headTrackedCamera.setupOffAxisViewPortal(windowTopLeft, windowBottomLeft, windowBottomRight);
}

//--------------------------------------------------------------
void testApp::drawScene(bool isPreview){
	
	ofEnableDepthTest();

	if (isPreview) {
		ofPushStyle();
		ofSetColor(150, 100, 100);
		ofDrawGrid(1.0f, 5.0f, true);
		
		ofSetColor(255);
		
		//--
		//draw camera preview
		//
		headTrackedCamera.transformGL();
		
		ofPushMatrix();
		ofScale(0.002f, 0.002f, 0.002f);
		ofNode().draw();
		ofPopMatrix();
		
		ofMultMatrix(headTrackedCamera.getProjectionMatrix().getInverse());
		
		ofPushStyle();
		ofNoFill();
		ofDrawBox(2.0f);
		ofPopStyle();
		
		headTrackedCamera.restoreTransformGL();
		//
		//--
		
		//--
		//draw window preview
		//
		window.clear();
		window.addVertex(windowTopLeft);
		window.addVertex(windowBottomLeft);
		window.addVertex(windowBottomRight);
		window.setMode(OF_PRIMITIVE_LINE_STRIP);
		window.draw();
		glPointSize(3.0f);
		window.drawVertices();
		//
		//--
	}
	
	ofPushStyle();
	ofNoFill();
	ofColor col(200,100,100);
	for (float z = 0.0f; z > -40.0f; z-= 0.1f){
		col.setHue(int(-z * 100.0f + ofGetElapsedTimef() * 10.0f) % 360);
		ofSetColor(col);
		ofRect(-windowWidth / 2.0f, -windowHeight / 2.0f, z, windowWidth, windowHeight);
	}
	ofPopStyle();
	
	ofPushStyle();
	ofEnableSmoothing();
	ofSetColor(255);
	ofSetLineWidth(5.0f);
	ofBeginShape();
	for (unsigned int i=0; i<headPositionHistory.size(); i++) {
		ofPoint vertex(headPositionHistory[i].x, headPositionHistory[i].y, -float( headPositionHistory.size() - i ) * 0.05f);
		ofCurveVertex(vertex);
	}
	ofEndShape(false);
	ofPopStyle();
	
	ofDisableDepthTest();
}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackgroundGradient(ofColor(50), ofColor(0));
	//------
	//draw the scene
	//
	if (usePreview){
		previewCamera.begin();
	}
	else{
		headTrackedCamera.begin();
	}
	
	drawScene(usePreview);
	
	if (usePreview){
		previewCamera.end();
	}
	else{
		headTrackedCamera.end();
	}
	//
	//------
	
	
	//------
	//draw some overlays
	//
	video.draw(0, 0);
	ofPushStyle();
	ofNoFill();
	for(unsigned int i = 0; i < finder.blobs.size(); i++) {
		ofRectangle cur = finder.blobs[i].boundingRect;
		ofRect(cur.x, cur.y, cur.width, cur.height);
	}
	ofPopStyle();
	
	stringstream message;
	message << "[SPACE] = User preview camera [" << (usePreview ? 'x' : ' ') << "]";
	
	ofDrawBitmapString(message.str(), video.getWidth() + 10, 20);
	
	if (usePreview){
		ofRectangle bottomLeft(0, ofGetHeight() - 200.0f, 300.0f, 200.0f);
		
		ofPushStyle();
		ofSetColor(0);
		ofRect(bottomLeft);
		ofPopStyle();
		
		headTrackedCamera.begin(bottomLeft);
		drawScene(false);
		headTrackedCamera.end();
	}
	//
	//------
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == ' ')
		usePreview = !usePreview;
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
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
