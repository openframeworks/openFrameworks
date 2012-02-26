#pragma once

#include "ofMain.h"
#include "CBLinuxProject.h"
#include "CBWinProject.h"
#include "visualStudioProject.h"
#include "xcodeProject.h"
#include <Poco/Path.h>
#include "ofxGui.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void generateExamples();
        ofFileDialogResult makeNewProjectViaDialog();

        void createProjectPressed(bool & pressed);
        void createAndOpenPressed(bool & pressed);
        void changeOFRootPressed(bool & pressed);

		baseProject * project;
    
        string projectPath;
        string platform;

        ofxPanel panelAddons, panelOptions;
        ofxButton createProject, createAndOpen, changeOFRoot;

};
