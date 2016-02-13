#include "ofAppRunner.h"
#include "ofApp.h"
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
	// Uncomment next line to share main's OpenGL resources with gui
	//settings.shareContextWith = mainWindow;

	auto guiWindow = ofCreateWindow(settings);
	guiWindow->setVerticalSync(false);

	auto mainApp = std::make_shared<ofApp>();
	mainApp->setupGui();
	ofAddListener(guiWindow->events().draw,mainApp.get(),&ofApp::drawGui);

	ofRunApp(mainWindow, mainApp);
	return ofRunMainLoop();
}
