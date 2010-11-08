#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxShader.h"

class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
};

#endif

