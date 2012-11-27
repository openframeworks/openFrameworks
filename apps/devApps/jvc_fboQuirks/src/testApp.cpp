#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	//ofDisableArbTex();
	//cout << "ofDisableArbTex" << endl;
	hasStarted = false;
	
	rpiLogo.loadImage("Raspi_Colour_R.png");
	rpiLogo.resize(rpiLogo.getWidth()/2, rpiLogo.getHeight()/2);
	rpiLogo.update();
	ofDirectory logoDirectory(ofToDataPath("of_logos"));
	logoDirectory.listDir();
	vector<ofFile> logosToLoad = logoDirectory.getFiles();
	ofLogVerbose() << "numFiles: " << logosToLoad.size();
	for (int i=0; i<logosToLoad.size(); i++) 
	{
		ofImage image;
		//ofLogVerbose() << "logosToLoad[i].getAbsolutePath(): " << logosToLoad[i].getAbsolutePath();
		image.loadImage(logosToLoad[i]);
		
		logos.push_back(image);
	}
	cout << "passing GL_RGB color space" << endl;

	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	fbo.begin();
		ofClear(0, 0, 0, 0);
		ofBackgroundGradient(ofColor::white, ofColor::black, OF_GRADIENT_CIRCULAR);	
		cout << "ofBackgroundGradient" << endl;
	fbo.end();
	cout << "ofGetCurrentRenderer()->getType()" << ofGetCurrentRenderer()->getType() << endl;
}

//--------------------------------------------------------------
void testApp::exit(){}

//--------------------------------------------------------------
void testApp::update(){
	
	fbo.begin();
	//ofBackgroundGradient(ofColor::white, ofColor::black, OF_GRADIENT_CIRCULAR);
	drawFboContent();
	fbo.end();
	
	
	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	/*if (!hasStarted) 
	{
		return;
	}*/
	ofEnableAlphaBlending();
		fbo.draw(0, 0);
	ofDisableAlphaBlending();
}
void testApp::drawFboContent() {
	
	ofEnableAlphaBlending();
		rpiLogo.draw((ofGetWidth() - rpiLogo.width)/2, (ofGetHeight() - rpiLogo.height)/2);
	ofDisableAlphaBlending();
	ofPushMatrix();
	ofTranslate(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	ofRotate(ofRandom(360));
	
	ofImage &randomImage = logos[ofRandom(logos.size())];
	
	randomImage.draw(0, 0, randomImage.width, randomImage.height);
	ofPopMatrix();
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){}

//--------------------------------------------------------------
void testApp::keyReleased(int key){}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}
