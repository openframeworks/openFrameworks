#pragma once

#include <ofBaseApp.h>
#include "ofAppBaseWindow.h"

class ofWindow;

class windowEvents
{
public:
	ofEvent<ofEventArgs> 		setup;
	ofEvent<ofEventArgs> 		update;
	ofEvent<ofEventArgs> 		draw;
	ofEvent<ofEventArgs> 		exit;
	ofEvent<ofResizeEventArgs> 	windowResized;

	ofEvent<ofKeyEventArgs> 	keyPressed;
	ofEvent<ofKeyEventArgs> 	keyReleased;

	ofEvent<ofMouseEventArgs> 	mouseMoved;
	ofEvent<ofMouseEventArgs> 	mouseDragged;
	ofEvent<ofMouseEventArgs> 	mousePressed;
	ofEvent<ofMouseEventArgs> 	mouseReleased;

	ofEvent<ofTouchEventArgs>	touchDown;
	ofEvent<ofTouchEventArgs>	touchUp;
	ofEvent<ofTouchEventArgs>	touchMoved;
	ofEvent<ofTouchEventArgs>	touchDoubleTap;
	ofEvent<ofTouchEventArgs>	touchCancelled;

	ofEvent<ofMessage>			messageEvent;
	ofEvent<ofDragInfo>			fileDragEvent;
};

/*****************
/ WINDOW LISTENER
*******************/

class ofWindowListener: public ofBaseApp
{
public:
	virtual void setup(ofWindow* f) {
		setup();
	}
	virtual void setup() {};
	virtual void update(ofWindow* f) {
		update();
	}
	virtual void update() {};
	virtual void draw(ofWindow* f) {
		draw();
	}
	virtual void draw() {};
	virtual void keyPressed(int key, ofWindow* f) {
		keyPressed(key);
	}

	//KEY
	virtual void keyPressed(int key) {};
	virtual void keyReleased(int key, ofWindow* f) {
		keyReleased(key);
	}
	virtual void keyReleased(int key) {};

	//WINDOW
	virtual void windowMoved(int x, int y, ofWindow* f) {
		windowMoved(x, y);
	}
	virtual void windowMoved(int x, int y) {};

	//MOUSE
	virtual void mouseMoved(int x, int y, ofWindow* f) {
		mouseMoved(x, y);
	}
	virtual void mouseMoved(int x, int y) {};
	virtual void mouseDragged(int x, int y, int button,  ofWindow* f) {
		mouseDragged(x, y, button);
	}
	virtual void mouseDragged(int x, int y, int button) {};
	virtual void mousePressed(int x, int y, int btn, ofWindow* f) {
		mousePressed(x, y, btn);
	}
	virtual void mousePressed(int x, int y, int btn) {};
	virtual void mouseReleased(int x, int y, int btn, ofWindow* f) {
		mouseReleased(x, y, btn);
		mouseReleased();
	}
	virtual void mouseReleased(int x, int y, int btn) {}
	virtual void mouseReleased() {};

	virtual void dragEvent(ofDragInfo info){}
	virtual void dragEvent(ofDragInfo info, ofWindow* f){dragEvent(info);}

	bool isUpdated;
};

///////////////////////////////////////////
// HELPERS FOR ofRunFensterApp

class ofWindowToOfBaseApp: public ofWindowListener{

public:
	ofWindowToOfBaseApp(ofBaseApp* base){
		mouseX = mouseY = 0;
		baseApp = base;
	}

	void setup(){
		baseApp->setup();
	}
	void update(){
		baseApp->update();
	}
	void draw(){
		baseApp->draw();
	}
	void exit(){
		baseApp->exit();
	}

	void windowResized(int w, int h){
		baseApp->windowResized(w, h);
	}

	void keyPressed( int key ){
		baseApp->keyPressed(key);
	}
	void keyReleased( int key ){
		baseApp->keyReleased(key);
	}

	void mouseMoved( int x, int y ){
		baseApp->mouseX = x;
		baseApp->mouseY = y;
		baseApp->mouseMoved(x, y);
	}
	void mouseDragged( int x, int y, int button ){
		baseApp->mouseX = x;
		baseApp->mouseY = y;
		baseApp->mouseDragged(x, y, button);
	}
	void mousePressed( int x, int y, int button ){
		baseApp->mouseX = x;
		baseApp->mouseY = y;
		baseApp->mousePressed(x, y, button);
	}
	void mouseReleased(){
		baseApp->mouseReleased();
	}
	void mouseReleased(int x, int y, int button ){
		baseApp->mouseX = x;
		baseApp->mouseY = y;
		baseApp->mouseReleased(x, y, button);
	}

	void dragEvent(ofDragInfo dragInfo) {
		baseApp->dragEvent(dragInfo);
	}
	void gotMessage(ofMessage msg){
		baseApp->gotMessage(msg);
	}

private:
	ofBaseApp* baseApp;
};

typedef std::vector<ofWindowListener*> ofWindowListenerList;

/*****************
/ WINDOW
*******************/

class ofWindow: public ofAppBaseWindow, public ofRectangle{
public:
	ofWindow();
	~ofWindow();  
    
	virtual void enableContext() = 0;
	virtual void initializeWindow() = 0;

	void addListener(ofWindowListener* listener);

	void setup();

	virtual void processEvents() = 0;

	void update(ofEventArgs& e);
	void update();
	void draw(ofEventArgs& e);
	void draw();


protected:
	virtual void postDraw(){};
	
private:
	ofWindowListenerList listeners;
};
