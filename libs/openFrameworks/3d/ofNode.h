
#pragma once

#include "ofConstants.h"
#include "ofParameter.h"
#include <array>
#include "glm/mat4x4.hpp"

class ofBaseRenderer;


/// \brief A generic 3d object in space with transformation (position, rotation, scale).

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

	/// \brief Set parent for the node. The node will inherit transformations from parent.
	///
	/// \param parent Reference to the ofNode which becomes the parent node.
	/// \param bMaintainGlobalTransform Boolean if maintain child's global
	///        transformations (default = false). 
	void setParent(ofNode& parent, bool bMaintainGlobalTransform = false);

	/// \brief Remove parent node linking.
	///
	/// \param bMaintainGlobalTransform Boolean if maintain child's global
	///        transformations (default = false).
	void clearParent(bool bMaintainGlobalTransform = false);

	/// \brief Get the parent node of this node.
	///
	/// \returns Pointer to parent ofNode.
	ofNode* getParent() const;

	/// \}
	/// \name Getters
	/// \{
	
	/// \brief Get node's local position as a 3D vector.
	///
	/// \returns A 3D vector with the local coordinates.
	glm::vec3 getPosition() const;

	/// \brief Get node's local x position. 
	/// 
	/// \returns Local x coordinate as a float.
	float getX() const;

	/// \brief Get node's local y position.
	///
	/// \returns Local y coordinate as a float.
	float getY() const;

	/// \brief Get node's local z position.
	///
	/// \returns Local z coordinate as a float.
	float getZ() const;
	
	/// \brief Get the node's local x axis as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local x axis direction.
	glm::vec3 getXAxis() const;

	/// \brief Get the node's local y axis as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local y axis direction.
	glm::vec3 getYAxis() const;

	/// \brief Get the node's local z axis as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local z axis direction.
	glm::vec3 getZAxis() const;
	
	/// \brief Get direction of node's side aka local x axis, as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local x axis direction.
	glm::vec3 getSideDir() const;
	
	/// \brief Get direction the node looks at aka local -z axis, as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local -z axis direction.
	glm::vec3 getLookAtDir()const;

	/// \brief Get direction of node's top aka local y axis, as 3d vector.
	///
	/// \returns A normalized 3D vector of the node's local y axis direction.
	glm::vec3 getUpDir() const;
	
	OF_DEPRECATED_MSG("Use Deg/Rad versions.", float getPitch() const);
	OF_DEPRECATED_MSG("Use Deg/Rad versions.", float getHeading() const);
	OF_DEPRECATED_MSG("Use Deg/Rad versions.", float getRoll() const);

	/// \brief Get pitch of node, aka the rotation along local x axis.
	/// \returns The rotation around the local x axis in degrees, as a float.
	float getPitchDeg() const;

	/// \brief Get heading of node, aka the rotation along local y axis.
	/// \returns The rotation around the local y axis in degrees, as a float.
	float getHeadingDeg() const;

	/// \brief Get roll of node, aka the rotation along local z axis.
	/// \returns The rotation around the local z axis in degrees, as a float.
	float getRollDeg() const;

	/// \brief Get pitch of node, aka the rotation along local x axis.
	/// \returns The rotation around the local x axis in degrees, as a float.
	float getPitchRad() const;

	/// \brief Get heading of node, aka the rotation along local y axis.
	/// \returns The rotation around the local y axis in degrees, as a float.
	float getHeadingRad() const;

	/// \brief Get roll of node, aka the rotation along local z axis.
	/// \returns The rotation around the local z axis in degrees, as a float.
	float getRollRad() const;
	
	/// \brief Get the local orientation of the node as a quaternion.
	/// \returns A quaternion of local orientation (useful for complex rotations)
	glm::quat getOrientationQuat() const;
	
	OF_DEPRECATED_MSG("Use the Deg/Rad version.", glm::vec3 getOrientationEuler() const);

	/// \brief Get local orientation of node in degrees around x, y, and z axes.
	/// \returns The local x, y and z axes orientation in degrees, as a 3D vector.
	glm::vec3 getOrientationEulerDeg() const;

	/// \brief Get local orientation of node in degrees around x, y, and z axes.
	/// \returns The local x, y and z axes orientation in degrees, as a 3D vector.
	glm::vec3 getOrientationEulerRad() const;
	
	/// \brief Get local scale of node in xyz axes where 1 is default.
	///
	/// \returns The local scale in the xyz axes where 1 = 100% of size.
	glm::vec3 getScale() const;

	/// \brief Get node's local transformations (position, orientation, scale).
	/// 
	/// \returns A refrence to mat4 containing node's local transformations.
	/// \sa https://open.gl/transformations
	const glm::mat4& getLocalTransformMatrix() const;
	
	// TODO: optimize and cache these
	// (parent would need to know about its children so it can inform them 
	// to update their global matrices if it itself transforms)
	
	/// \brief Get node's global transformations (position, orientation, scale).
	/// \returns A refrence to mat4 containing node's global transformations.
	/// \sa https://open.gl/transformations
	glm::mat4 getGlobalTransformMatrix() const;
	
	/// \brief Get node's global position as a 3D vector.
	/// \returns A 3D vector with the global coordinates.
	glm::vec3 getGlobalPosition() const;
	
	/// \brief Get the global orientation of the node as a quaternion.
	/// \returns An quaternion of the global orientations(useful for complex rotations)
	glm::quat getGlobalOrientation() const;
	
	/// \brief Get global scale of node in xyz axes where 1 is default.
	///
	/// \returns The global scale in the xyz axes where 1 = 100% of size.
	glm::vec3 getGlobalScale() const;

	/// \}
	/// \name Setters
	/// \{
	
	/// \brief Set the local position of the node using xyz coordinates.
	///
	/// \param px Desired local x coordinate as a float.
	/// \param py Desired local y coordinate as a float.
	/// \param pz Desired local z coordinate as a float.
	void setPosition(float px, float py, float pz);
	
	/// \brief Set the local position of the node using a 3D vector of coordinates.
	/// 
	/// \param p Desired local xyz coordinates as ref to 3D vector.	
	void setPosition(const glm::vec3& p);
	
	/// \brief Set the global position of the node using xyz coordinates.
	///
	/// \param px Desired global x coordinate as a float.
	/// \param py Desired global y coordinate as a float.
	/// \param pz Desired global z coordinate as a float.	
	void setGlobalPosition(float px, float py, float pz);

	/// \brief Set the global position of the node using a 3D vector of coordinates.
	///
	/// \param p Desired global xyz coordinates as ref to 3D vector.
	void setGlobalPosition(const glm::vec3& p);

	/// \brief Set local orientation with a quaternion.
	///
	/// \param q Desired local orientation as ref to an glm::quat.
	void setOrientation(const glm::quat& q);
	
	/// \brief Set local orientation with xyz euler angles.
	///
	/// \param eulerAngles Desired local xyz angles in degrees, as ref to 3D vector.
	/// \note Using euler angles can cause gimbal lock.
	/// \sa https://en.wikipedia.org/wiki/Gimbal_lock
	void setOrientation(const glm::vec3& eulerAngles);
	
	/// \brief Set global orientation with a quaternion.
	///
	/// \param q Desired global orientation as ref to an glm::quat.
	void setGlobalOrientation(const glm::quat& q);

	/// \brief Set local uniform scale (x, y, and z are equally scaled).
	///
	/// \param s Desired scale for all axes as a float where 1 = 100%.
	void setScale(float s);
	
	/// \brief Set local scale for xyz axes individually.
	///
	/// \param sx Desired local scale for x axis as a float where 1 = 100%.
	/// \param sy Desired local scale for y axis as a float where 1 = 100%.
	/// \param sz Desired local scale for z axis as a float where 1 = 100%.
	void setScale(float sx, float sy, float sz);
	
	/// \brief Set local scale for xyz axes individually with a 3D vector.
	///
	/// \param s Desired local scale for all axes as ref to 3D vector where 1 = 100%.
	void setScale(const glm::vec3& s);
	
	/// \}
	/// \name Modifiers
	/// \{

	/// \brief Move node by relative amount with xyz coordinates.
	///
	/// \param x Desired relative position change along x axis as a float.
	/// \param y Desired relative position change along y axis as a float.
	/// \param z Desired relative position change along z axis as a float.
	void move(float x, float y, float z);			
	
	/// \brief Move node by relative amount with xyz as ref to 3D vector.
	/// \param offset Desired relative position change along all axes as ref to 3D vector.
	void move(const glm::vec3& offset);

	/// \brief Move node left+right relative to current position (in local x axis).
	///
	/// \param amount Desired relative position change along local x axis as float.
	void truck(float amount);

	/// \brief Move node up+down relative to current position (in local y axis).
	///
	/// \param amount Desired relative position change along local y axis as float.
	void boom(float amount);

	/// \brief Move node backward+forward relative to current position (in local z axis).
	///
	/// \param amount Desired relative position change along local z axis as float.
	void dolly(float amount);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void tilt(float degrees));

	/// \brief Tilt up+down relative to current orientation (around local x axis).
	///
	/// \param degrees Desired relative rotation change along local x axis in degrees as float.
	void tiltDeg(float degrees);

	/// \brief Tilt up+down relative to current orientation (around local x axis).
	///
	/// \param radians Desired relative rotation change along local x axis in radians as float.
	void tiltRad(float radians);
	
	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void pan(float degrees));

	/// \brief Rotate left+right relative to current orientation (around local y axis).
	///
	/// \param degrees Desired relative rotation change along local y axis in degrees as float.
	void panDeg(float degrees);

	/// \brief Rotate left+right relative to current orientation (around local y axis).
	///
	/// \param radians Desired relative rotation change along local y axis in radians as float.
	void panRad(float radians);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void roll(float degrees));

	/// \brief Roll left+right relative to current orientation (around local z axis).
	///
	/// \param degrees Desired relative rotation change along local z axis in degrees as float.
	void rollDeg(float degrees);

	/// \brief Roll left+right relative to current orientation (around local z axis).
	///
	/// \param radians Desired relative rotation change along local z axis in radians as float.
	void rollRad(float radians);
	
	/// \brief Rotate relative to current orientation by quaternion.
	///
	/// \param q Desired relative rotation change as a ref to quaternion.
	void rotate(const glm::quat& q);
	
	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void rotate(float degrees, const glm::vec3& v));

	/// \brief Rotate relative to current orientation around arbitrary axis.
	///
	/// \param degrees Desired relative angle change in degrees as float.
	/// \param v Desired axis to rotate around as a ref to cartesian 3D Vector.
	void rotateDeg(float degrees, const glm::vec3& v);

	/// \brief Rotate relative to current orientation around arbitrary axis.
	///
	/// \param radians Desired relative angle change in radians as float.
	/// \param v Desired axis to rotate around as a ref to cartesian 3D Vector.
	void rotateRad(float radians, const glm::vec3& v);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void rotate(float degrees, float vx, float vy, float vz));

	/// \brief Rotate relative to current orientation around arbitrary axis.
	///
	/// \param degrees Desired relative angle change in degrees as float.
	/// \param vx X angle of the axis to rotate around in degrees as float.
	/// \param vy Y angle of the axis to rotate around in degrees as float.
	/// \param vz Z angle of the axis to rotate around in degrees as float.
	void rotateDeg(float degrees, float vx, float vy, float vz);

	/// \brief Rotate relative to current orientation around arbitrary axis.
	///
	/// \param radians Desired relative angle change in radians as float.
	/// \param vx X angle of the axis to rotate around in radians as float.
	/// \param vy Y angle of the axis to rotate around in radians as float.
	/// \param vz Z angle of the axis to rotate around in radians as float.
	void rotateRad(float radians, float vx, float vy, float vz);
	
	/// \brief Rotate relative to current orientation by quaternion around point.
	///
	/// \param q Desired relative rotation change as a ref to quaternion.
	/// \param point Point to rotate around in local xyz coordinates as ref to 3D vector.	
	void rotateAround(const glm::quat& q, const glm::vec3& point);

	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void rotateAround(float degrees, const glm::vec3& axis, const glm::vec3& point));
	
	/// \brief Rotate relative to current orientation around arbitrary axis around point.
	///
	/// \param degrees Desired relative angle change in degrees as float.
	/// \param axis The arbitrary axis to rotate around as ref to cartesian 3D vector.
	/// \param point Point to rotate around in local xyz coordinates as ref to 3D vector.
	void rotateAroundDeg(float degrees, const glm::vec3& axis, const glm::vec3& point);

	/// \brief Rotate relative to current orientation around arbitrary axis around point.
	///
	/// \param radians Desired relative angle change in degrees as float.
	/// \param axis The arbitrary axis to rotate around as ref to cartesian 3D vector.
	/// \param point Point to rotate around in local xyz coordinates as ref to 3D vector.
	void rotateAroundRad(float radians, const glm::vec3& axis, const glm::vec3& point);

	/// \brief Orient node to look at point (-z axis pointing to global position).
	///
	/// \param lookAtPosition XYZ coordinates of point to look at as ref to 3D vector.
	/// \note This version calculates the up vector automatically to try to keep
	///       it relatively consistant with the original angle.
	void lookAt(const glm::vec3& lookAtPosition);

	/// \brief Orient node to look at point (-z axis pointing to global position).
	/// \param lookAtPosition XYZ coordinates of point to look at as ref to 3D vector.
	/// \param upVector The desired up axis as a cartesian 3D vector.
	void lookAt(const glm::vec3& lookAtPosition, glm::vec3 upVector);

	/// \brief Orient node to look at another node (-z axis pointing at other node).
	///
	// \param lookAtNode A reference to the node to look at.
	/// \note This version calculates the up vector automatically to try to keep
	///       it relatively consistant with the original angle.
	void lookAt(const ofNode& lookAtNode);
	
	/// \brief Orient node to look at another node (-z axis pointing at other node).
	/// \param lookAtNode A reference to the node to look at.
	/// \param upVector The desired up axis as a ref to cartesian 3D vector.
	void lookAt(const ofNode& lookAtNode, const glm::vec3& upVector);
	
	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void orbit(float longitude, float latitude, float radius, const glm::vec3& centerPoint = glm::vec3(0, 0, 0)));
	OF_DEPRECATED_MSG("Use the Deg/Rad version.", void orbit(float longitude, float latitude, float radius, ofNode& centerNode));

	/// \brief Orbit node around a global position at a specific radius.
	///
	/// \param longitude The longitudinal position of the node in degrees as float.
	/// \param latitude The latitudinal position of the node in degrees as float.
	/// \param radius The desired radius from the position in degrees as float.
	/// \param centerPoint The global position to orbit around as ref to 3D vector.
	///        Default = (0, 0, 0).	
	void orbitDeg(float longitude, float latitude, float radius, const glm::vec3& centerPoint = glm::vec3(0, 0, 0));

	/// \brief Orbit node around another node at a specific radius.
	/// \param longitude The longitudinal position of the node in degrees as float.
	/// \param latitude The latitudinal position of the node in degrees as float.
	/// \param radius The desired radius from the position in degrees as float.
	/// \param centerNode The global position to orbit around as ref to 3D vector.
	///        Default = (0, 0, 0).	
	void orbitDeg(float longitude, float latitude, float radius, ofNode& centerNode);

	/// \brief Orbit node around a global position at a specific radius.
	///
	/// \param longitude The longitudinal position of the node in radians as float.
	/// \param latitude The latitudinal position of the node in radians as float.
	/// \param radius The desired radius from the position in degrees as float.
	/// \param centerPoint The global node to orbit around as ref to 3D vector.
	///        Default = (0, 0, 0).	
	void orbitRad(float longitude, float latitude, float radius, const glm::vec3& centerPoint = glm::vec3(0, 0, 0));

	/// \brief Orbit node around another node at a specific radius.
	/// \param longitude The longitudinal position of the node in radians as float.
	/// \param latitude The latitudinal position of the node in radians as float.
	/// \param radius The desired radius from the position in degrees as float.
	/// \param centerNode The global node to orbit around as ref to 3D vector.
	///        Default = (0, 0, 0).	
	void orbitRad(float longitude, float latitude, float radius, ofNode& centerNode);
	
	/// \}
	/// \name OpenGL Transformation
	/// \{
	
	/// \brief Set opengl renderer's modelview matrix to this nodes transform.
	/// \param renderer A pointer to the renderer you want to set to this node's transform;
	/// \note If you want to draw something at the position+orientation+scale of this node,
	/// call ofNode::transform(); write your draw code, and ofNode::restoreTransform();
	/// OR A simpler way is to extend ofNode and override ofNode::customDraw();
	void transformGL(ofBaseRenderer * renderer = nullptr) const;
	
	/// \brief Restore opengl renderer's previous modelview transform matrix.
	/// \param renderer A pointer to the renderer you want to restore transformation to.
	void restoreTransformGL(ofBaseRenderer * renderer = nullptr) const;
	
	
	/// \brief Reset this node's transformations, position, rotation and scale.
	void resetTransform();
	
	/// \}
	/// \name Drawing
	/// \{

	/// \brief If you extend ofNode and wish to change the way it draws, extend this.
	/// \param renderer A pointer to the renderer you want to draw to.
	/// \note Try to not use global functions for rendering and instead use the passed
	/// renderer.
	virtual void customDraw(const ofBaseRenderer * renderer) const;
	
	/// \brief If you extend ofNode and wish to change the way it draws, extend this.
	/// \note Try to not use global functions for rendering and instead use the passed
	/// renderer.
	virtual void customDraw();

	
	/// \brief Draw the node as a white cube with xyz axes. 
	/// \note do NOT override this. 
	/// It transforms the node to its position+orientation+scale
	/// and calls the virtual 'customDraw' method above which you CAN override.
	virtual void draw() const;

	/// \}
	
protected:
	void createMatrix();
	void updateAxis();
	
	/// \brief Classes extending ofNode can override this method to get
	///        notified when the position changed.
	virtual void onPositionChanged() {}

	/// \brief Classes extending ofNode can override this methods to get notified
	///        when the orientation changed.
	virtual void onOrientationChanged() {}

	/// \brief Classes extending ofNode can override this methods to get notified 
	///        when the scale changed.
	virtual void onScaleChanged() {}

	ofNode * parent = nullptr;

private:
	void onParentPositionChanged(glm::vec3 & position) {onPositionChanged();}
	void onParentOrientationChanged(glm::quat & orientation) {onOrientationChanged();}
	void onParentScaleChanged(glm::vec3 & scale) {onScaleChanged();}

	ofParameter<glm::vec3> position;
	ofParameter<glm::quat> orientation;
	ofParameter<glm::vec3> scale;

	std::array<glm::vec3,3> axis;

	glm::mat4 localTransformMatrix;
	bool legacyCustomDrawOverrided;
	std::set<ofNode*> children;

	void addListener(ofNode & node);
	void removeListener(ofNode & node);
//	glm::mat4 globalTransformMatrix;
};
