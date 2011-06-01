#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include <unistd.h> 


extern "C" {
	
	typedef struct { float x,y; } mtPoint;
	typedef struct { mtPoint pos,vel; } mtReadout;
	
	typedef struct {
		int frame;
		double timestamp;
		int identifier, state, foo3, foo4;
		mtReadout normalized;
		float size;
		int zero1;
		float angle, majorAxis, minorAxis; // ellipsoid
		mtReadout mm;
		int zero2[2];
		float unk2;
	} Finger;
	
	typedef void *MTDeviceRef;
	typedef int (*MTContactCallbackFunction)(int,Finger*,int,double,int);
	
	MTDeviceRef MTDeviceCreateDefault();
	void MTRegisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction);
	void MTUnregisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction);
	void MTDeviceStart(MTDeviceRef);
	void MTDeviceStop(MTDeviceRef);
	void MTDeviceRelease(MTDeviceRef);
	
}





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

		void exit();
		
		MTDeviceRef _mt_device;
};

#endif
