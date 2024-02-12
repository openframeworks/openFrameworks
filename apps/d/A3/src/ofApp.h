#pragma once

#include "ofMain.h"
#include "GLFW/glfw3.h"

#include "structs.h"

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	ofPolyline poly;
	ofPath path;
	fpsCounter count;
	
	
	void drawSecondWindow(ofEventArgs & args) {
//		cout << "drawSecondWindow" << endl;
//		ofSetCircleResolution(4);
		float s = sin(ofGetElapsedTimef()*3.0) * 40 + 40;
		ofBackground(s);
		int space = 200;
		for (int b=0; b<=ofGetWindowHeight()/space; b++) {
			for (int a=0; a<=ofGetWindowWidth()/space; a++) {
				float h = a*9;
				ofSetColor(ofColor::fromHsb(h, 255, 255));
				ofPushMatrix();
				ofTranslate(a*space + space*.5, b*space + space*.5);
				
				ofDrawCircle(0, 0, space*.5);
				string t = ofToString(a) + " x " + ofToString(b);
				ofSetColor(0);
				ofDrawBitmapString(t, 0, 0);
				ofPopMatrix();
			}
		}
//		drawOut();
	}
	

	std::vector <std::shared_ptr<ofAppBaseWindow>> allWindows;
	std::function<void()> drawFunction = NULL;
	
	vector <glm::ivec2> positionsFromRect(const ofRectangle & rect, int divisions) {
		vector <glm::ivec2> pos;
		int w = rect.width / divisions;
		int h = rect.height / divisions;
		for (int a=0; a<divisions; a++) {
			for (int b=0; b<divisions; b++) {
				pos.emplace_back( a*w + rect.x, a*h + rect.y );
			}
		}
		return pos;
	}

	void setupSecond() {
		ofGLFWWindowSettings settings;
//			ofWindowSettings settings;
	//		settings.windowMode = OF_FULLSCREEN;
		settings.shareContextWith = ofGetCurrentWindow();
		int divisions = 9;
		int w = ofGetScreenWidth() / divisions;
		int h = ofGetScreenHeight() / divisions;
		settings.setSize(w, h);

		auto rect = allMonitors.getRectFromMonitors({ 1, 2, 3 });
//		cout << rect << endl;
		
		auto r2 = allMonitors.getRectForAllMonitors();
//		cout << r2 << endl;
		
		
		for (int a=0; a<divisions; a++) {
			for (int b=0; b<divisions; b++) {
				settings.setPosition({ a*w, b*h });
				settings.windowName = "w" + ofToString(a) + "x" + ofToString(b);
				allWindows.emplace_back(ofCreateWindow(settings));
				ofAddListener(allWindows.back()->events().draw, this, &ofApp::drawWindow);
			}
		}
	}
	

	
	virtual void drawWindow(ofEventArgs & args) {
//		cout << "drawSecondWindow" << endl;
//		ofBackground(0, 255, 0);
		ofSetColor(0);
		ofSetColor(ofColor::fromHsb(ofRandom(0,45), 255, 255));
		ofDrawRectangle(10, 10, 100, 100);
	}

	void closeSecond() {
		for (auto & a : allWindows) {
			a->setWindowShouldClose();
		}
		allWindows.clear();
	}
	

};
