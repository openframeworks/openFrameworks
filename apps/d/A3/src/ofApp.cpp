#include "ofApp.h"

ofPolyline p1, p2;
#define NEUEFPS

bool vs = false;
//int fps = 120/1;
int fps = 333;
//ofTimerFpsX t { fps };
ofTimerFpsX t;

GLFWwindow* wn;

void ofApp::setup(){
	ofRectangle windowRect { 100, 100, 600, 400 };
	
#ifdef NW
	ofGLFWWindowSettings settings;
	settings.windowName = "second";
	settings.shareContextWith = ofGetCurrentWindow();
	settings.windowMode = OF_FULLSCREEN;
//	settings.fullscreenDisplays = { 1, 2, 3 };
	settings.monitor = 1;
	settings.showOnlyInSelectedMonitor = true;
	auto newWindow { ofCreateWindow(settings) };
	
	ofAddListener(newWindow->events().draw, 
				  this,
				  &ofApp::drawSecondWindow);
#endif
	
//	GLFWwindow * sharedContext = nullptr;
//	GLFWmonitor *monitor = nullptr;
//	GLFWmonitor** monitors;
//	int numberOfMonitors;
//	monitors = glfwGetMonitors(&numberOfMonitors);
//	if (numberOfMonitors > 1) {
//		monitor = monitors[1];
//	}
//	wn = glfwCreateWindow(windowRect.width, windowRect.height, "", monitor, sharedContext);

	ofSetCircleResolution(96);
	cout << "GLFW version " << glfwGetVersionString() << endl;
#ifdef NEUEFPS
//	ofSetFrameRate(fps);
	ofSetFrameRate(0);
	vs = false;
	ofSetVerticalSync(vs);
#else
	ofSetFrameRate(fps); //40 eh especial pq eh 25ms
	ofSetVerticalSync(false);
#endif
//	glfwWindowHint(GLFW_REFRESH_RATE, fps);
	ofSetBackgroundAuto(false);
	
	cout << "getwindows size " << ofGetMainLoop()->getWindows().size() << endl;

}

void ofApp::update() {
#ifdef NEUEFPS
//	t.waitNext();
#endif
}

void ofApp::draw(){
//	cout << ofGetSeconds() << endl;
//	cout << ofGetElapsedTimeMicros() / 1000000 << endl;
//	cout << ofGetElapsedTimeMillis() / (float)1000000000 << endl;
	
	
	ofGetMainLoop()->ofBeginWindow(1);
	ofClear(255, 0, 0);
	ofSetColor(0, 255, 0);
	ofDrawRectangle(50, 50, ofGetWindowWidth()-100, ofGetWindowHeight()-100);
	ofSetColor(0);
	ofDrawBitmapString("second window", 40, 40);
	ofGetMainLoop()->ofEndWindow();
	
	string s { ofToString(ofGetMouseX()) + " : " + ofToString(ofGetMouseY()) };
//	ofSetWindowTitle(s);
//	cout << ofGetWindowHeight() << endl;
//	ofScale(2, 2);
//	ofSetColor(255, 0, 0);
//	p1.draw();
//	ofSetColor(0, 255, 0);
//	p2.draw();
	count.tick();
//	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	int w = 800;
	int h = 600 + sin(ofGetElapsedTimef() * 0.5) * 150;
//		ofSetWindowShape(w, h);
	int x = 40;
	int y = 100 + sin(ofGetElapsedTimef() * 0.4) * 100;
//	ofSetWindowPosition(w, h);
	
	ofRectangle re = ofRectangle(x, y, ofGetWindowWidth(), ofGetWindowHeight());
//	ofSetWindowRect(re);

//	ofBackground(0, 60, 130);

	ofSetColor(0, 0, 255, 3);
	ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	
	ofSetColor(255, 0, 0);
	ofDrawCircle(200, 200, 200);
	ofDrawLine(0, 0, 400, 400);

	ofSetColor(0, 255, 0 );
	float yy = ofGetWindowHeight() * .5;
	ofDrawLine(0, yy, 600, yy);
	ofSetColor(0, 255, 255 );
	yy = ofGetWindowHeight() * .5 + (fps-count.get())* 100.0;
	ofDrawLine(0, yy, 600, yy);

	
	
	string t = 	"of  " + ofToString(ofGetFrameRate()) + "\n" +
				"new " +ofToString(count.get()) + " \n" +
				ofToString(fps-count.get()) + "\n" +
				ofToString(vs) + "\n"
	;

	ofSetColor(255);
	ofDrawBitmapString(t, 200, 30);
//	ofSetWindowTitle(t);
	
	
	ofSetColor(0, 255, 0);
	ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 30);
	
//	for (int a=0; a<ofGetWindowWidth()/200; a++) {
//		float h = a*9;
//		ofSetColor(ofColor::fromHsb(h, 255, 255));
//		ofDrawCircle(a*200 , 200, 100, 100);
//	}
}

void ofApp::keyPressed(int key) {
	static int m = 0;
	if (key == 'r') {
		m = (m+1)%4;
		cout << m << endl;
	}
	if (key == 't') {
		m = (m-1)%4;
		cout << m << endl;
	}
	
	if (key == 'w') {
		cout << ofGetMainLoop()->getWindows()[1]->getWindowRect() << endl;
	}
	if (key == 'v') {
		vs ^= 1;
		ofSetVerticalSync(vs);
		cout << "vs " << vs << endl;
	}
	if (key == 'r') {
		ofSetFrameRate(ofRandom(25, 100));
	}
	if (key == 'q') {
		setupSecond();
	}
	if (key == 'w') {
		closeSecond();
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
	else if (key == 'g') {
		cout << int(ofGetWindowMode()) << endl;
	}
	else if (key == 's') {
		cout << "ofGetWindowRect " << ofGetWindowRect() << endl;
		cout << "ofGetWindowSize " << ofGetWindowSize() << endl;
#ifdef FORK
		cout << "ofGetScreenSize " << ofGetScreenSize() << endl;
#endif
//		cout << "ofGetWindowPosition " << ofGetWindowPosition() << endl;
		cout << "ofGetWindowPositionX " << ofGetWindowPositionX() << endl;
		cout << "ofGetWindowPositionY " << ofGetWindowPositionY() << endl;
//		auto windowP = ((ofAppGLFWWindow*)ofGetWindowPtr())->getGLFWWindow();
		glm::vec2 scale;
		glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &scale.x, &scale.y);
		cout << "glfwGetMonitorContentScale " << scale << endl;

		cout << "getPixelScreenCoordScale" << ((ofAppGLFWWindow*)ofGetWindowPtr())->getPixelScreenCoordScale() << endl;


		
	}
}
