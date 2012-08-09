#pragma once

#include <ofBaseApp.h>
#include "ofAppBaseWindow.h"
#include <GL/glfw3.h>
#include <map>

#define OF_MAX_NUM_KEYS 512

/*****************
/ WINDOW LISTENER
*******************/

class ofWindowListener : public ofBaseApp {
	public:
		virtual void setup(ofWindow * f){
			setup();
		}
		virtual void setup(){
		}
		virtual void update(ofWindow * f){
			update();
		}
		virtual void update(ofWindowEventArgs & f){
			update(f.window);
		}
		virtual void update(){
		}

		virtual void draw(ofWindow * f){
			draw();
		}
		virtual void draw(ofWindowEventArgs & e){
			draw(e.window);
		}
		virtual void draw(){
		}

		//KEY
		virtual void keyPressed(int key){
		}
		virtual void keyReleased(int key, ofWindow * f){
			keyReleased(key);
		}
		virtual void keyReleased(ofKeyEventArgs & e){
			keyReleased(e.key, e.window);
		}
		virtual void keyReleased(int key){
		}
		virtual void keyPressed(int key, ofWindow * f){
			keyPressed(key);
		}
		virtual void keyPressed(ofKeyEventArgs & e){
			keyPressed(e.key, e.window);
		}

		//WINDOW
		virtual void windowMoved(int x, int y, ofWindow * f){
			windowMoved(x, y);
		}
		virtual void windowMoved(ofMoveEventArgs & e){
			windowMoved(e.x, e.y, e.window);
		}
		virtual void windowMoved(int x, int y){
		}

		virtual void windowResized(int w, int h, ofWindow * f){
			windowResized(w, h);
		}
		virtual void windowResized(ofResizeEventArgs & e){
			windowResized(e.width, e.height, e.window);
		}
		virtual void windowResized(int w, int h){
		}
		virtual void windowClosed(ofWindowEventArgs & e){
			windowClosed(e.window);
		}
		virtual void windowClosed(ofWindow * window){
			windowClosed();
		}
		virtual void windowClosed(){
		}

		//MOUSE
		virtual void mouseMoved(int x, int y, ofWindow * f){
			mouseMoved(x, y);
		}
		virtual void mouseMoved(ofMouseEventArgs & e){
			mouseMoved(e.x, e.y, e.window);
		}
		virtual void mouseMoved(int x, int y){
		}
		virtual void mouseDragged(int x, int y, int button,  ofWindow * f){
			mouseDragged(x, y, button);
		}
		virtual void mouseDragged(ofMouseEventArgs & e){
			mouseDragged(e.x, e.y, e.button, e.window);
		}
		virtual void mouseDragged(int x, int y, int button){
		}
		virtual void mousePressed(int x, int y, int btn, ofWindow * f){
			mousePressed(x, y, btn);
		}
		virtual void mousePressed(ofMouseEventArgs & e){
			mousePressed(e.x, e.y, e.button, e.window);
		}
		virtual void mousePressed(int x, int y, int btn){
		}
		virtual void mouseReleased(int x, int y, int btn, ofWindow * f){
			mouseReleased(x, y, btn);
			mouseReleased();
		}
		virtual void mouseReleased(ofMouseEventArgs & e){
			mouseReleased(e.x, e.y, e.button, e.window);
		}
		virtual void mouseReleased(int x, int y, int btn){
		}
		virtual void mouseReleased(){
		}

		virtual void scrolled(ofScrollEventArgs & e){
			scrolled(e.deltaX, e.deltaY, e.window);
		}
		virtual void scrolled(float deltaX, float deltaY, ofWindow * win){
			scrolled(deltaX, deltaY);
		}
		virtual void scrolled(float deltaX, float deltaY){
		}

		virtual void dragEvent(ofDragInfo info, ofWindow * f){
			dragEvent(info);
		}
		virtual void dragEvent(ofDragInfo info){
		}

		bool isUpdated;
};

///////////////////////////////////////////
// HELPERS FOR ofRunFensterApp

class ofWindowToOfBaseApp : public ofWindowListener {

	public:
		ofWindowToOfBaseApp(ofBaseApp * base){
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

		void keyPressed(int key){
			baseApp->keyPressed(key);
		}
		void keyReleased(int key){
			baseApp->keyReleased(key);
		}

		void mouseMoved(int x, int y){
			baseApp->mouseX = x;
			baseApp->mouseY = y;
			baseApp->mouseMoved(x, y);
		}
		void mouseDragged(int x, int y, int button){
			baseApp->mouseX = x;
			baseApp->mouseY = y;
			baseApp->mouseDragged(x, y, button);
		}
		void mousePressed(int x, int y, int button){
			baseApp->mouseX = x;
			baseApp->mouseY = y;
			baseApp->mousePressed(x, y, button);
		}
		void mouseReleased(){
			baseApp->mouseReleased();
		}
		void mouseReleased(int x, int y, int button){
			baseApp->mouseX = x;
			baseApp->mouseY = y;
			baseApp->mouseReleased(x, y, button);
		}

		void dragEvent(ofDragInfo dragInfo){
			baseApp->dragEvent(dragInfo);
		}
		void gotMessage(ofMessage msg){
			baseApp->gotMessage(msg);
		}

	private:
		ofBaseApp * baseApp;
};

typedef std::vector <ofWindowListener *> ofWindowListenerList;

/*****************
/ WINDOW
*******************/

class ofWindow : public ofAppBaseWindow {
	public:
		ofWindow();
		~ofWindow();

		void enableContext();
		void initializeWindow();

		void setWindowPositionAndShape(ofRectangle rect);
		void setWindowPositionAndShape(int x, int y, int width, int height);

		void addListener(ofWindowListener * listener);
		void addListener(ofBaseApp * app);

		void setup();

		void update(ofEventArgs & e);
		void update();
		void draw(ofEventArgs & e);
		void draw();

		ofPoint getWindowPosition();
		void setWindowPosition(int x, int y);
		void setWindowPosition(ofPoint pos);

		ofPoint getWindowSize();
		void setWindowShape(int x, int y);
		void setWidth(int w);
		void setHeight(int h);

		int getWidth();
		int getHeight();

		int getX();
		int getY();
		int setX(int x);
		int setY(int y);

		bool isKeyPressed(int key);
		bool isClosed();

		void setTitle(string title);
		string getTitle();

		void close();

		ofWindowEvents events;

		static int lastWindowID;

		int mouseX, mouseY;
		int oldMouseX, oldMouseY;

	private:
		GLFWwindow getGlfwWindow();

		void destroy();

		void mouseMoved(int mouseX, int mouseY);
		void mouseDragged(int mouseX, int mouseY, int button);
		void mousePressed(int button);
		void mousePressed(int mouseX, int mouseY, int button);
		void mouseReleased(int button);
		void mouseReleased(int mouseX, int mouseY, int button);

		void keyPressed(int key);
		void keyReleased(int key);

		void scrolled(float deltaX, float deltaY);

		void windowMoved(int x, int y);
		void windowFocused();
		void windowUnfocused();
		void windowClosed();
		void windowResized(int w, int h);
		
		void updateMouse(int x, int y);

		ofWindowListenerList listeners;
		ofRectangle previousShape;
		bool isFocused;
		string title;
		GLFWwindow window;

		bool isInitialized;
		int id;
		int width;
		int height;
		int x;
		int y;

		bool keyState[OF_MAX_NUM_KEYS];

		friend class ofWindowManager;
};
