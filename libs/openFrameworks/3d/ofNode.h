
#pragma once

#include "ofVectorMath.h"
#include "of3dUtils.h"
#include "ofAppRunner.h"
#include "ofParameter.h"
#include <array>


/// \brief A generic 3d object in space with transformation (position, rotation, scale).
///
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

// TODO: cache inverseMatrix, local and global
class ofNode {
public:
	/// \cond INTERNAL
	
	ofNode();
	virtual ~ofNode();
	ofNode(const ofNode & node);
	ofNode(ofNode && node);
	ofNode & operator=(const ofNode & node);
	ofNode & operator=(ofNode && node);

	/// \endcond

	/// \name Parent Node
	/// \{

	/// \brief Set parent to link nodes transformations. This will inherit
	/// transformations from parent node. 
	void setParent(ofNode& parent, bool bMaintainGlobalTransform = false);

	/// \brief Remove parent node linking
	void clearParent(bool bMaintainGlobalTransform = false);

	/// \brief Get the parent node that this node is linked to
	ofNode* getParent() const;

	/// \}
	/// \name Getters
	/// \{
	
	/// \brief Get position as a 3d vector
	ofVec3f getPosition() const;

	/// \brief Get x coordinate
	float getX() const;

	/// \brief Get y coordinate
	float getY() const;

	/// \brief Get z coordinate
	float getZ() const;
	
	/// \brief Get x axis as 3d vector
	ofVec3f getXAxis() const;

	/// \brief Get y axis as 3d vector
	ofVec3f getYAxis() const;

	/// \brief Get z axis as 3d vector
	ofVec3f getZAxis() const;
	
	/// \brief Get x axis as 3d vector
	/// \returns x axis.
	ofVec3f getSideDir() const;
	
	/// \brief Get -z axis as 3d vector
	/// \returns -z axis.
	ofVec3f getLookAtDir()const;

	/// \brief Get y axis as 3d vector
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

	/// \}	
	/// \name Setters
	/// \{

	/// \brief Directly set the transformation matrix
	void setTransformMatrix(const ofMatrix4x4 &m44);
	
	/// \brief Set the position of the node
	void setPosition(float px, float py, float pz);
	void setPosition(const ofVec3f& p);
	
	void setGlobalPosition(float px, float py, float pz);
	void setGlobalPosition(const ofVec3f& p);

	/// \brief Set orientation with a quaternion
	void setOrientation(const ofQuaternion& q);
	
	/// \brief Set orientation with a euler angles
	/// \note Prepare for gimbal lock
	void setOrientation(const ofVec3f& eulerAngles);
	
	void setGlobalOrientation(const ofQuaternion& q);

	void setScale(float s);
	void setScale(float sx, float sy, float sz);
	void setScale(const ofVec3f& s);
	
	/// \}
	/// \name Modifiers
	/// \{

	/// \brief Move by arbitrary amount
	void move(float x, float y, float z);			
	
	/// \brief Move by arbitrary amount
	void move(const ofVec3f& offset);	

	/// \brief Move sideways (in local x axis)
	void truck(float amount);

	/// \brief Move up+down (in local y axis)
	void boom(float amount);

	/// \brief Move forward+backward (in local z axis)					
	void dolly(float amount);						
	
	/// \brief Tilt up+down (around local x axis)
	void tilt(float degrees);						
	
	/// \brief Rotate left+right (around local y axis)
	void pan(float degrees);

	/// \brief Roll left+right (around local z axis)
	void roll(float degrees);					
	
	/// \brief Rotate by quaternion
	void rotate(const ofQuaternion& q);	
	
	/// \brief Rotate around arbitrary axis by angle
	void rotate(float degrees, const ofVec3f& v);

	/// \brief Rotate around arbitrary axis by angle
	void rotate(float degrees, float vx, float vy, float vz);
	
	/// \brief Rotate by quaternion around point
	void rotateAround(const ofQuaternion& q, const ofVec3f& point);
	
	/// \brief Rotate around arbitrary axis by angle around point
    void rotateAround(float degrees, const ofVec3f& axis, const ofVec3f& point);

    /// \brief Orient node to look at position (-z axis pointing to position)
    ///
    /// This version calculates the up vector by rotating {0,1,0} by the same
    /// angle that will rotate {0,0,1} to the current position - lookAtPosition
    void lookAt(const ofVec3f& lookAtPosition);

	/// \brief Orient node to look at position (-z axis pointing to position)
    void lookAt(const ofVec3f& lookAtPosition, ofVec3f upVector);

    /// \brief Orient node to look at node (-z axis pointing to node)
    ///
    /// This version calculates the up vector by rotating {0,1,0} by the same
    /// angle that will rotate {0,0,1} to the current position - lookAtPosition
    void lookAt(const ofNode& lookAtNode);
	
	/// \brief Orient node to look at node (-z axis pointing to node)
    void lookAt(const ofNode& lookAtNode, const ofVec3f& upVector);
	
	/// \brief Orbit object around target at radius
	void orbit(float longitude, float latitude, float radius, const ofVec3f& centerPoint = ofVec3f(0, 0, 0));
	void orbit(float longitude, float latitude, float radius, ofNode& centerNode);
	
	/// \}
	/// \name OpenGL Transformation
	/// \{
	
	/// \brief Set opengl's modelview matrix to this nodes transform
	/// if you want to draw something at the position+orientation+scale of this node...
	/// ...call ofNode::transform(); write your draw code, and ofNode::restoreTransform();
	/// OR A simpler way is to extend ofNode and override ofNode::customDraw();
	void transformGL(ofBaseRenderer * renderer = nullptr) const;
	void restoreTransformGL(ofBaseRenderer * renderer = nullptr) const;
	
	
	/// \brief Resets this node's transformation
	void resetTransform();
	
	/// \}
	/// \name Drawing
	/// \{

	/// \brief If you extend ofNode and wish to change the way it draws, extend this
	/// try to not use global functions for rendering and instead use the passed
	/// renderer
	virtual void customDraw(const ofBaseRenderer * renderer) const;
	virtual void customDraw();

	
	/// \brief Draw function. 
	/// do NOT override this
	/// transforms the node to its position+orientation+scale
	/// and calls the virtual 'customDraw' method above which you CAN override
	virtual void draw() const;

	/// \}
	
protected:
	void createMatrix();
	void updateAxis();
	
	/// \brief classes extending ofNode can override this method to get
	/// notified when the position changed.
	virtual void onPositionChanged() {}

	/// \brief classes extending ofNode can override this methods to get notified when the orientation changed.
	virtual void onOrientationChanged() {}

	/// \brief classes extending ofNode can override this methods to get notified when the scale changed.
	virtual void onScaleChanged() {}

	ofNode * parent;

private:
	void onParentPositionChanged(ofVec3f & position) {onPositionChanged();}
	void onParentOrientationChanged(ofQuaternion & orientation) {onOrientationChanged();}
	void onParentScaleChanged(ofVec3f & scale) {onScaleChanged();}

	ofParameter<ofVec3f> position;
	ofParameter<ofQuaternion> orientation;
	ofParameter<ofVec3f> scale;

	std::array<ofVec3f,3> axis;

	ofMatrix4x4 localTransformMatrix;
	bool legacyCustomDrawOverrided;
	std::set<ofNode*> children;

	void addListener(ofNode & node);
	void removeListener(ofNode & node);
//	ofMatrix4x4 globalTransformMatrix;
};
