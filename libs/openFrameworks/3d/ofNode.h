
#pragma once

#include "ofVectorMath.h"
#include "of3dUtils.h"
#include "ofAppRunner.h"


// a generic 3d object in space with transformation (position, rotation, scale)
// with API to move around in global or local space
// and virtual draw method

// Info:
// uses right-handed coordinate system
// ofNodes 'look' along -ve z axis
// All set* methods work in local space unless stated otherwise

// TODO:
// cache inverseMatrix, local and global


class ofNode {
public:
	ofNode();
	virtual ~ofNode() {}

	// set parent to link nodes
	// transformations are inherited from parent node
	// set to NULL if not needed (default)
	void setParent(ofNode& parent, bool bMaintainGlobalTransform = false);
	void clearParent(bool bMaintainGlobalTransform = false);
	ofNode* getParent() const;

	
	//----------------------------------------
	// Get transformations
	
	ofVec3f getPosition() const;
	float getX() const;
	float getY() const;
	float getZ() const;
	
	ofVec3f getXAxis() const;
	ofVec3f getYAxis() const;
	ofVec3f getZAxis() const;
	
	ofVec3f getSideDir() const;		// x axis
	ofVec3f getLookAtDir()const;	// -z axis
	ofVec3f getUpDir() const;		// y axis
	
	float getPitch() const;
	float getHeading() const;
	float getRoll() const;
	
	ofQuaternion getOrientationQuat() const;
	ofVec3f getOrientationEuler() const;
	ofVec3f getScale() const;

	const ofMatrix4x4& getLocalTransformMatrix() const;
	
	// TODO: optimize and cache these
	// (parent would need to know about its children so it can inform them 
	// to update their global matrices if it itself transforms)
	ofMatrix4x4 getGlobalTransformMatrix() const;
	ofVec3f getGlobalPosition() const;
	ofQuaternion getGlobalOrientation() const;
	ofVec3f getGlobalScale() const;

	
	
	// Set Transformations

	// directly set transformation matrix
	void setTransformMatrix(const ofMatrix4x4 &m44);
	
	// position
	void setPosition(float px, float py, float pz);
	void setPosition(const ofVec3f& p);
	
	void setGlobalPosition(float px, float py, float pz);
	void setGlobalPosition(const ofVec3f& p);


	// orientation
	void setOrientation(const ofQuaternion& q);			// set as quaternion
	void setOrientation(const ofVec3f& eulerAngles);	// or euler can be useful, but prepare for gimbal lock
//	void setOrientation(const ofMatrix3x3& orientation);// or set as m33 if you have transformation matrix
	
	void setGlobalOrientation(const ofQuaternion& q);

	
	// scale set and get
	void setScale(float s);
	void setScale(float sx, float sy, float sz);
	void setScale(const ofVec3f& s);
	
	
	// helpful move methods
	void move(float x, float y, float z);			// move by arbitrary amount
	void move(const ofVec3f& offset);				// move by arbitrary amount
	void truck(float amount);						// move sideways (in local x axis)
	void boom(float amount);						// move up+down (in local y axis)
	void dolly(float amount);						// move forward+backward (in local z axis)
	
	
	// helpful rotation methods
	void tilt(float degrees);						// tilt up+down (around local x axis)
	void pan(float degrees);						// rotate left+right (around local y axis)
	void roll(float degrees);						// roll left+right (around local z axis)
	void rotate(const ofQuaternion& q);				// rotate by quaternion
	void rotate(float degrees, const ofVec3f& v);	// rotate around arbitrary axis by angle
	void rotate(float degrees, float vx, float vy, float vz);
	
	void rotateAround(const ofQuaternion& q, const ofVec3f& point);	// rotate by quaternion around point
	void rotateAround(float degrees, const ofVec3f& axis, const ofVec3f& point);	// rotate around arbitrary axis by angle around point
	
	// orient node to look at position (-ve z axis pointing to node)
	void lookAt(const ofVec3f& lookAtPosition, ofVec3f upVector = ofVec3f(0, 1, 0));
	void lookAt(const ofNode& lookAtNode, const ofVec3f& upVector = ofVec3f(0, 1, 0));
	
	
	// orbit object around target at radius
	void orbit(float longitude, float latitude, float radius, const ofVec3f& centerPoint = ofVec3f(0, 0, 0));
	void orbit(float longitude, float latitude, float radius, ofNode& centerNode);
	
	
	// set opengl's modelview matrix to this nodes transform
	// if you want to draw something at the position+orientation+scale of this node...
	// ...call ofNode::transform(); write your draw code, and ofNode::restoreTransform();
	// OR A simpler way is to extend ofNode and override ofNode::customDraw();
	void transformGL(ofBaseRenderer * renderer = ofGetCurrentRenderer().get()) const;
	void restoreTransformGL(ofBaseRenderer * renderer = ofGetCurrentRenderer().get()) const;
	
	
	// resets this node's transformation
	void resetTransform();
	

	// if you extend ofNode and wish to change the way it draws, extend this
	// try to not use global functions for rendering and instead use the passed
	// renderer
	virtual void customDraw(const ofBaseRenderer * renderer) const;
	virtual void customDraw();

	
	// draw function. do NOT override this
	// transforms the node to its position+orientation+scale
	// and calls the virtual 'customDraw' method above which you CAN override
	virtual void draw() const;
	
protected:
	ofNode *parent;
	
	void createMatrix();
	void updateAxis();
	
	// classes extending ofNode can override these methods to get notified 
	virtual void onPositionChanged() {}
	virtual void onOrientationChanged() {}
	virtual void onScaleChanged() {}

private:
	ofVec3f position;
	ofQuaternion orientation;
	ofVec3f scale;
	
	ofVec3f axis[3];
	
	ofMatrix4x4 localTransformMatrix;
	bool legacyCustomDrawOverrided;
//	ofMatrix4x4 globalTransformMatrix;
};
