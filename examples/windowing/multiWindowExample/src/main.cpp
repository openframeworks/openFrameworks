#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

	ofGLFWWindowSettings settings;
	settings.setSize(600, 600);
	settings.setPosition(glm::vec2(300,0));
	settings.resizable = true;
	auto mainWindow = ofCreateWindow(settings);

	settings.setSize(300, 300);
	settings.setPosition(glm::vec2(0,0));
	settings.resizable = false;
	auto guiWindow = ofCreateWindow(settings);

	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<GuiApp> guiApp(new GuiApp);
	mainApp->gui = guiApp;

	ofRunApp(guiWindow, guiApp);
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

}
