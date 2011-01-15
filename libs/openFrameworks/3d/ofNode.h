
#pragma once

#include "ofVectorMath.h"

// TODO: look at optimizing matrix updates

// Info:
// Nodes 'look' along -ve z axis


// a 3d transformation
class ofNode {
public:
	ofNode();
	virtual ~ofNode() {}

	
	// draw functions. do NOT override these
	// transforms the node to its position and calls the virtual 'customDraw' methods which you CAN override
	void draw();
	void debugDraw();
	

	// transformations are inherited from parent node
	// set to NULL if not needed (default)
	void setParent(ofNode* parent);
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
	//	ofMatrix3x3 getOrientationMatrix() const;
	
	ofVec3f getScale() const;

	
	const ofMatrix4x4& getMatrix();
	
	// TODO: optimize and cache these
	ofMatrix4x4 getGlobalMatrix();
	ofVec3f getGlobalPosition();
	ofQuaternion getGlobalOrientation();
//	ofVec3f getGlobalScale();

	
	
	// Set Transformations

	// directly set transformation matrix
	// TODO:
	void setMatrix(ofMatrix4x4 &m44) {}
	void setMatrix(float *m44) {}
	
	// position
	void setPosition(float px, float py, float pz);
	void setPosition(const ofVec3f& p);

	// orientation
	void setOrientation(const ofQuaternion& q);			// set as quaternion
	void setOrientation(const ofVec3f& eulerAngles);	// or euler can be useful, but prepare for gimbal lock
//	void setOrientation(const ofMatrix3x3& orientation);// or set as m33 if you have transformation matrix
	
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
	
	
	// orient node to look at position (-ve z axis pointing to node)
	void lookAt(const ofVec3f& lookAtPosition, const ofVec3f& upVector = ofVec3f(0, 1, 0));
	void lookAt(ofNode& lookAtNode, const ofVec3f& upVector = ofVec3f(0, 1, 0));
	
	
	// orbit object around target at radius
	void orbit(float longitude, float latitude, float radius, const ofVec3f& centerPoint = ofVec3f(0, 0, 0));
	void orbit(float longitude, float latitude, float radius, ofNode& centerNode);
	
	void resetTransform();
	
protected:
	ofNode *parent;
	
	ofVec3f position;
	ofQuaternion orientation;
	ofVec3f scale;
	
	ofVec3f axis[3];
	
	void updateMatrix();

	virtual void customDraw() {};	// override this one
	virtual void customDebugDraw();
	
private:
//	bool		isMatrixDirty;
	ofMatrix4x4 transformMatrix;
};
