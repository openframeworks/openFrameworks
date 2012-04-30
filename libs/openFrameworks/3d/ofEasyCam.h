#pragma once

#include "ofCamera.h"
#include "ofEvents.h"

class ofEasyCam : public ofCamera {
public:
	ofEasyCam();
	~ofEasyCam();

	// TODO: this should be ofGetViewRect() eventually
	virtual void begin(ofRectangle viewport = ofGetCurrentViewport());
	void reset();

	//----------------------------------------
	// advanced functions

	void setTarget(const ofVec3f& target);
	void setTarget(ofNode& target);
	ofNode& getTarget();

	void setDistance(float distance);
	float getDistance() const;

	// drag is how quickly the camera picks up and slows down
	// it is a normalized value between 0-1
	void setDrag(float drag);
	float getDrag() const;
	// the translation key is the key used to switch between rotation and translation.
	// translation happens only when the key is pressed.
	void setTranslationKey(char key);
	char getTranslationKey();
	//--
	void mouseDragged(ofMouseEventArgs& mouse);
	void mouseMoved(ofMouseEventArgs& mouse);
	void mousePressed(ofMouseEventArgs& mouse);
	void mouseReleased(ofMouseEventArgs& mouse);
	void keyPressed(ofKeyEventArgs& key);
	void keyReleased(ofKeyEventArgs& key);

	
	// enable or disable mouse input to navigate
	void enableMouseInput();
	void disableMouseInput();
	bool getMouseInputEnabled();
	
	
	
private:
	void setDistance(float distance, bool save);

	ofNode target;
	
	bool bApplyInertia;
	bool bDoTranslate;
	bool bDoRotate;
	
	bool bInsideArcball;
	
	float xRot;
	float yRot;
	float zRot;
	
	float moveX;
	float moveY;
	float moveZ;
	
	ofVec2f mouseVel;
	
	void updateRotation();
	void updateTranslation();
	
	char doTranslationKey;
	
	float drag;
	bool bMouseInputEnabled;
	
	unsigned long lastTap;

	bool bDistanceSet;
	float lastDistance;
		
	ofQuaternion curRot;  
	//a place to store the mouse position so we can measure incremental change  
    ofVec2f lastMouse;
	//slows down the rotation 1 = 1 degree per pixel
	float sensitivity;
	float sensitivityZ;

	
};
