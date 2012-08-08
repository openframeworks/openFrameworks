#pragma once

#include "ofWindow.h"
#include "ofBaseTypes.h"

typedef ofPtr <ofWindow> ofWindowPtr;
typedef vector <ofWindowPtr> ofWindowList;

class ofWindowManager : public ofAppBaseWindow {
	public:
		ofWindowManager();
		~ofWindowManager();

		ofWindow * createWindow(int w = 800, int h = 600);
		ofWindow * createWindow(int x, int y, int width, int height);
		void deleteWindow(ofWindow * win);
		void deleteWindow(int id);

		void initializeWindow();
		void setupOpenGL(int w, int h, int screenMode);
		void runAppViaInfiniteLoop(ofBaseApp * appPtr);

		void update();
		void draw();

		ofWindow * getLastCreatedWindow();
		ofWindow * getMainWindow();
		ofWindow * getWindowById(int id);

		void setFrameRate(float targetRate);
		float getFrameRate();
		void setActiveWindow(ofWindow * win);

		ofPoint getWindowPosition();
		ofPoint getWindowSize();

		int     getWidth();
		int     getHeight();

		//would like to have these functions private. but don't know how...
		void glfwWindowFocus(GLFWwindow glfwWin, int action);
		void glfwWindowSize(GLFWwindow glfwWin, int w, int h);
		int glfwWindowClose(GLFWwindow glfwWin);
		void glfwWindowRefresh(GLFWwindow glfwWin);
		void glfwWindowIconify(GLFWwindow glfwWin, int action);
		void glfwMouseButton(GLFWwindow glfwWin, int button, int action);
		void glfwMousePos(GLFWwindow glfwWin, int mouseX, int mouseY);
		void glfwCursorEnter(GLFWwindow glfwWin, int action);
		void glfwScroll(GLFWwindow glfwWin, float deltaX, float deltaY);
		void glfwKey(GLFWwindow glfwWin, int key, int action);
		void glfwChar(GLFWwindow glfwWin, int key);

	private:
		ofWindow * getWindowByGlfw(GLFWwindow win);
		void initializeGLFW();
		void addWindow(ofWindow * win);
		void removeWindow(ofWindow * win);

		ofWindowList windows;
		ofWindow * mainWindow;
		ofWindow * activeWindow;
};


/* OF C-STYLE FUNCTIONS */
ofWindowManager * ofGetWindowManager();
ofWindow * ofCreateWindow(int x, int y, int width, int height);
ofWindow * ofCreateWindow(int width = 800, int height = 600);
ofWindow * ofGetMainWindow();
ofWindow * ofGetLastCreatedWindow();
