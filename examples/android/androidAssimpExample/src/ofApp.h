#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"

class ofApp : public ofxAndroidApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
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

		bool bAnimate;
		bool bAnimateMouse;
        float animationPosition;
        ofxAssimpModelLoader model;
        ofLight	light;

        ofMesh mesh;
        bool loaded;
        int currentModel;
};
