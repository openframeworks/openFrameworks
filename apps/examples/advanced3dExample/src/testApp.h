#pragma once

#include "ofMain.h"
#include "Swarm.h"
#include "Grid.h"

#include "OrthoCamera.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
	
		void setupViewports();
		void drawScene();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	

		//cameras
		ofEasyCam				camEasyCam;
		OrthoCamera				camFront;
		OrthoCamera				camTop;
		OrthoCamera				camLeft;

	
		//camera pointers
		ofCamera*				cameras[4];
		int						iCurrentCamera;
	
		//viewports
		ofRectangle				viewMain;
		ofRectangle				viewGrid[4];	
	
		//my custom node
		Swarm					nodeSwarm;
		Grid					nodeGrid;

};

/*

 - uncomment out the world drawing things (axis+box)
 - link the camera to the target
 - inverse the rotation

*/


/*
 Can we change the global size of objects?
	there appears to be an arbitrary scale of '100'
	i.e. a ofBox(100) fills the screen.
 
 Should customDraw be public?
 Add customUpdate?
 
 test isActive on camera end
 
*/