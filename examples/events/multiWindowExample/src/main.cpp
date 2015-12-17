#include "ofAppRunner.h"
#include "ofApp.h"
#include "GuiApp.h"
#include "ofAppGLFWWindow.h"


int main(){
	ofGLFWWindowSettings settings;

	settings.width = 600;
	settings.height = 600;
	settings.setPosition(ofVec2f(300,0));
	settings.resizable = true;
	auto mainWindow = ofCreateWindow(settings);

	settings.width = 300;
	settings.height = 300;
	settings.setPosition(ofVec2f(0,0));
	settings.resizable = false;
	auto guiWindow = ofCreateWindow(settings);

	auto mainApp = std::make_shared<ofApp>();
	auto guiApp = std::make_shared<GuiApp>();

	mainApp->gui = guiApp;

	ofRunApp(guiWindow, guiApp);
	ofRunApp(mainWindow, mainApp);

	return ofRunMainLoop();
}
