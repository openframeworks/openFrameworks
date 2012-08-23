#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"


//#ifdef TARGET_WIN32
//	#define GLUT_BUILDING_LIB
//	#include "glut.h"
//#endif
//#ifdef TARGET_OSX
//	#include <GLUT/glut.h>
//#endif
//#ifdef TARGET_LINUX
//	#include <GL/glut.h>
//#endif

class ofBaseApp;

class ofAppNoWindow : public ofAppBaseWindow {

public:
	ofAppNoWindow();
	~ofAppNoWindow(){}

	void runAppViaInfiniteLoop(ofBaseApp * appPtr);

	static void exitApp();

	int			getFrameNum();
	float		getFrameRate();
	void		setFrameRate(float targetRate);
	
	double		getLastFrameTime() { return lastFrameTime; }



    double			timeNow, timeThen, fps, lastFrameTime;
    int				nFramesForFPS;
    int				nFrameCount;

    bool			bFrameRateSet;
    int 			millisForFrame;
    int 			prevMillis;
    int 			diffMillis;

    float 			frameRate;

    ofBaseApp *		ofAppPtr;
};

