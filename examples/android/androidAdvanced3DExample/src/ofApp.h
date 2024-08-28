#pragma once

///////////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////
//
//
// ADVANCED 3D EXAMPLE
//		ofNode3d, ofCamera, ofEasyCam
//
//
///////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////////

#include "ofMain.h"
#include "ofxAndroid.h"

#include "Swarm.h"
#include "Grid.h"

#include "OrthoCamera.h"

#define N_CAMERAS 4

class ofApp : public ofxAndroidApp{

	public:
		void setup();
		void update();
		void draw();
	
		void setupViewports();
		void drawScene(int iCameraDraw);
		void updateMouseRay();
	
		void keyPressed  (int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		//cameras (all these inherit from ofCamera)
		ofEasyCam				camEasyCam;
		OrthoCamera				camFront;
		OrthoCamera				camTop;
		OrthoCamera				camLeft;
	
		//cameras have parent?
		bool					bCamParent;

		//camera pointers
		ofCamera*				cameras[N_CAMERAS];
		int						iMainCamera;
	
		//viewports
		ofRectangle				viewMain;
		ofRectangle				viewGrid[N_CAMERAS];	
	
		//my custom node
		Swarm					nodeSwarm;
		Grid					nodeGrid;

		//ray drawn under mouse cursor [start,end]
		glm::vec3					ray[2];
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
