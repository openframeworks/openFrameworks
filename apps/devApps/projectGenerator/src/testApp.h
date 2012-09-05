#pragma once

//#define COMMAND_LINE_ONLY

#include "ofMain.h"
#include "CBLinuxProject.h"
#include "CBWinProject.h"
#include "visualStudioProject.h"
#include "xcodeProject.h"
#include <Poco/Path.h>

#ifndef COMMAND_LINE_ONLY
#include "ofxGui.h"
#endif

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
 
		void setupForTarget(int targ);
        void generateExamplesCB(bool & pressed); //stupid as its a button 
		void generateExamples();
		
        ofFileDialogResult makeNewProjectViaDialog();
        ofFileDialogResult updateProjectViaDialog();

        void createProjectPressed(bool & pressed);
        void updateProjectPressed(bool & pressed);
        void createAndOpenPressed(bool & pressed);
        void changeOFRootPressed(bool & pressed);
		
		void setupDrawableOFPath();
		
		baseProject * project;
    
        string projectPath;
        string target;
		vector <int> targetsToMake;
		bool buildAllExamples;

#ifndef COMMAND_LINE_ONLY
		string drawableOfPath;
		ofRectangle ofPathRect;
		ofPoint ofPathDrawPoint;

        ofxPanel panelAddons, panelOptions;
        ofxButton createProject, updateProject, createAndOpen, changeOFRoot;

		ofxPanel examplesPanel;
		ofxToggle osxToggle, iosToggle, wincbToggle, winvsToggle, linuxcbToggle, linux64cbToggle;
		ofxButton generateButton;
#endif
};
