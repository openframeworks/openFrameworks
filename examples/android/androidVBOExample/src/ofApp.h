#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

#define GRID_WIDTH  16
#define GRID_HEIGHT 16
#define LENGTH		2 * 8

class ofApp : public ofxAndroidApp {
	
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

	ofVbo	vbo;
	glm::vec3 pos[GRID_WIDTH*GRID_HEIGHT*LENGTH];
	glm::vec3 center;

	float   restLength;
	int		total;
	int		space;
	int		count;
	bool	bPause;
	int		zoom, zoomTarget;
};
