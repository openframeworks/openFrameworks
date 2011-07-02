#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofMain.h"

#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"

class testApp : public ofxiPhoneApp{

	public:

		void setup();
		void update();
		void draw();

		void touchDown(ofTouchEventArgs &touch);
		void touchMoved(ofTouchEventArgs &touch);
		void touchUp(ofTouchEventArgs &touch);
		void touchDoubleTap(ofTouchEventArgs &touch);
		void touchCancelled(ofTouchEventArgs &touch);

		bool bAnimate;
		bool bAnimateMouse;
		float animationTime;
        ofxAssimpModelLoader model;

        ofVboMesh mesh;
        ofPoint position;
        float normScale;
        ofPoint scale;
        ofPoint sceneCenter;
        ofMaterial material;
        ofTexture tex;
        ofLight	light;
};

