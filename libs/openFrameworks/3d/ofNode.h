
#pragma once

#include "ofVectorMath.h"
#include "of3dUtils.h"
#include "ofAppRunner.h"


/// \brief a generic 3d object in space with transformation (position, rotation, scale)
/// The ofNode is the base of all things 3d. It lets you represent a location 
/// and orientation in 3d space and also allows you to add children or parents 
/// so that sets of nodes can move together. This is handy for representing 
/// complex 3d models that are linked together, the same way that your hand is 
/// linked to your wrist (hopefully), which is linked to your elbow (hopefully),
/// and so on. Nodes are the base of the ofPrimitives, ofCamera, and 
/// ofEasyCamera, among other things.
///
/// with API to move around in global or local space
/// and virtual draw method
///
/// Info:
/// uses right-handed coordinate system
/// ofNodes 'look' along -ve z axis
/// All set* methods work in local space unless stated otherwise
///
/// TODO: cache inverseMatrix, local and global

/// To get the current position, check out: getX(), getY(), getZ(). 
/// To get the axis of the node call getXAxis() (or the y and z variants for 
/// those other axes). Another really useful feature of the ofNode is that you 
/// can get the [Eulerian angles](http://en.wikipedia.org/wiki/Euler_angles) 
/// of each node: getPitch(), getHeading(), getRoll(). 
/// The global transformation matrix of the ofNode is also available using the 
/// getGlobalTransformMatrix(), very handy for figuring out things in relation 
/// to the OpenGL representation of your OF world. getGlobalOrientation() also 
/// is handy, returning a ofQuaternion that you can use to find out whether 
/// your node is upside down in relation to the rest of your OF world (really 
/// an OpenGL context, but let's not get into that quite yet).

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

	/// \brief get x coordinate.
	float getX() const;

	/// \brief get y coordinate.
	float getY() const;

	/// \brief get z coordinate.
	float getZ() const;
	
	/// \brief get x axis.
	ofVec3f getXAxis() const;

	/// \brief get y axis.
	ofVec3f getYAxis() const;

	/// \brief get z axis.
	ofVec3f getZAxis() const;
	
	/// \brief get x axis.
	/// \returns x axis.
	ofVec3f getSideDir() const;
	
	/// \brief get -z axis.
	/// \returns -z axis.
	ofVec3f getLookAtDir()const;

	/// \brief get y axis.
	/// \returns y axis.
	ofVec3f getUpDir() const;
	
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
	
	/// \brief move by arbitrary amount
	void move(float x, float y, float z);			
	
	/// \brief  move by arbitrary amount
	void move(const ofVec3f& offset);	

	/// \brief  move sideways (in local x axis)
	void truck(float amount);

	/// \brief move up+down (in local y axis)
	void boom(float amount);

	/// \brief move forward+backward (in local z axis)					
	void dolly(float amount);						
	
	/// \brief tilt up+down (around local x axis)
	void tilt(float degrees);						
	
	/// \brief rotate left+right (around local y axis)
	void pan(float degrees);

	/// \brief roll left+right (around local z axis)
	void roll(float degrees);					
	
	/// \brief  rotate by quaternion
	void rotate(const ofQuaternion& q);	
	
	/// \brief rotate around arbitrary axis by angle
	void rotate(float degrees, const ofVec3f& v);

	void rotate(float degrees, float vx, float vy, float vz);
	
	/// \brief rotate by quaternion around point
	void rotateAround(const ofQuaternion& q, const ofVec3f& point);
	
	/// \brief rotate around arbitrary axis by angle around point
	void rotateAround(float degrees, const ofVec3f& axis, const ofVec3f& point);	

	/// \brief  orient node to look at position (-z axis pointing to position)
	void lookAt(const ofVec3f& lookAtPosition, ofVec3f upVector = ofVec3f(0, 1, 0));
	
	/// \brief  orient node to look at node (-z axis pointing to node)
	void lookAt(const ofNode& lookAtNode, const ofVec3f& upVector = ofVec3f(0, 1, 0));
	
	/// \brief orbit object around target at radius
	void orbit(float longitude, float latitude, float radius, const ofVec3f& centerPoint = ofVec3f(0, 0, 0));
	void orbit(float longitude, float latitude, float radius, ofNode& centerNode);
	
	
	/// \brief set opengl's modelview matrix to this nodes transform
	/// if you want to draw something at the position+orientation+scale of this node...
	/// ...call ofNode::transform(); write your draw code, and ofNode::restoreTransform();
	/// OR A simpler way is to extend ofNode and override ofNode::customDraw();
	void transformGL(ofBaseRenderer * renderer = ofGetCurrentRenderer().get()) const;
	void restoreTransformGL(ofBaseRenderer * renderer = ofGetCurrentRenderer().get()) const;
	
	
	/// \brief resets this node's transformation
	void resetTransform();
	

	/// \brief if you extend ofNode and wish to change the way it draws, extend this
	/// try to not use global functions for rendering and instead use the passed
	/// renderer
	virtual void customDraw(const ofBaseRenderer * renderer) const;
	virtual void customDraw();

	
	/// \brief draw function. 
	/// do NOT override this
	/// transforms the node to its position+orientation+scale
	/// and calls the virtual 'customDraw' method above which you CAN override
	virtual void draw() const;
	
protected:
	ofNode *parent;
	
	void createMatrix();
	void updateAxis();
	
	/// \brief classes extending ofNode can override this method to get notified when the position changed.
	virtual void onPositionChanged() {}

	/// \brief classes extending ofNode can override this methods to get notified when the orientation changed.
	virtual void onOrientationChanged() {}

	/// \brief classes extending ofNode can override this methods to get notified when the scale changed.
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
