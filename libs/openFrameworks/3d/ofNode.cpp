
#include "ofNode.h"
#include "ofMain.h"


ofNode::ofNode() : parent(NULL) {
	setPosition(ofVec3f(0, 0, 0));
	setOrientation(ofVec3f(0, 0, 0));
	setScale(1);
}


//----------------------------------------
void ofNode::setParent(ofNode* parent) {
	this->parent = parent;
}


//----------------------------------------
ofNode* ofNode::getParent() const {
	return parent;
}


//----------------------------------------
void ofNode::setPosition(float px, float py, float pz) {
	setPosition(ofVec3f(px, py, pz));
}


//----------------------------------------
void ofNode::setPosition(const ofVec3f& p) {
	position = p;
	localTransformMatrix.setTranslation(position);
	onPositionChanged();
}


//----------------------------------------
void ofNode::setGlobalPosition(float px, float py, float pz) {
	setGlobalPosition(ofVec3f(px, py, pz));
}

//----------------------------------------
void ofNode::setGlobalPosition(const ofVec3f& p) {
	if(parent == NULL) {
		setPosition(p);
	} else {
		setPosition(p * ofMatrix4x4::getInverseOf(parent->getGlobalTransformMatrix()));
	}
}



//----------------------------------------
ofVec3f ofNode::getPosition() const {
	return position;
}


//----------------------------------------
float ofNode::getX() const {
	return position.x;
}


//----------------------------------------
float ofNode::getY() const {
	return position.y;
}


//----------------------------------------
float ofNode::getZ() const {
	return position.z;
}


//----------------------------------------
void ofNode::setOrientation(const ofQuaternion& q) {
	orientation = q;
	createMatrix();
	onOrientationChanged();
}


//----------------------------------------
void ofNode::setOrientation(const ofVec3f& eulerAngles) {
	setOrientation(ofQuaternion(eulerAngles.y, ofVec3f(0, 1, 0), eulerAngles.x, ofVec3f(1, 0, 0), eulerAngles.z, ofVec3f(0, 0, 1)));
}

//----------------------------------------
void ofNode::setGlobalOrientation(const ofQuaternion& q) {
	if(parent == NULL) {
		setOrientation(q);
	} else {
		ofMatrix4x4 invParent(ofMatrix4x4::getInverseOf(parent->getGlobalTransformMatrix()));
		ofMatrix4x4 m44(ofMatrix4x4(q) * invParent);
		setOrientation(m44.getRotate());
	}
}


//----------------------------------------
ofQuaternion ofNode::getOrientationQuat() const {
	return orientation;
}


//----------------------------------------
// ref at http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
ofVec3f ofNode::getOrientationEuler() const {
	ofQuaternion q1 = orientation;
	float test = q1.x()*q1.y() + q1.z()*q1.w();
	float heading;
	float attitude;
	float bank;
	if (test > 0.499) { // singularity at north pole
		heading = 2 * atan2(q1.x(), q1.w());
		attitude = PI/2;
		bank = 0;
	} else if (test < -0.499) { // singularity at south pole
		heading = -2 * atan2(q1.x(), q1.w());
		attitude = - PI/2;
		bank = 0;
	} else {
		float sqx = q1.x() * q1.x();
		float sqy = q1.y() * q1.y();
		float sqz = q1.z() * q1.z();
		heading = atan2(2.0f * q1.y() * q1.w() - 2.0f * q1.x() * q1.z(), 1.0f - 2.0f*sqy - 2.0f*sqz);
		attitude = asin(2*test);
		bank = atan2(2.0f*q1.x() * q1.w() - 2.0f * q1.y() * q1.z(), 1.0f - 2.0f*sqx - 2.0f*sqz);
	}
	
	return ofVec3f(ofRadToDeg(attitude), ofRadToDeg(heading), ofRadToDeg(bank));
}


//----------------------------------------
void ofNode::setScale(float s) {
	setScale(s, s, s);
}


//----------------------------------------
void ofNode::setScale(float sx, float sy, float sz) {
	setScale(ofVec3f(sx, sy, sz));
}


//----------------------------------------
void ofNode::setScale(const ofVec3f& s) {
	this->scale = s;
	createMatrix();
	onScaleChanged();
}


//----------------------------------------
ofVec3f ofNode::getScale() const {
	return scale;
}

//----------------------------------------
void ofNode::move(float x, float y, float z) {
	move(ofVec3f(x, y, z));
}


//----------------------------------------
void ofNode::move(const ofVec3f& offset) {
	position += offset;
	localTransformMatrix.setTranslation(position);
	onPositionChanged();
}


//----------------------------------------
void ofNode::truck(float amount) {
	move(getXAxis() * amount);
}


//----------------------------------------
void ofNode::boom(float amount) {
	move(getYAxis() * amount);
}


//----------------------------------------
void ofNode::dolly(float amount) {
	move(getZAxis() * amount);
}


//----------------------------------------
void ofNode::tilt(float degrees) {
	rotate(degrees, getXAxis());
}


//----------------------------------------
void ofNode::pan(float degrees) {
	rotate(degrees, getYAxis());
}


//----------------------------------------
void ofNode::roll(float degrees) {
	rotate(degrees, getZAxis());
}


//----------------------------------------
void ofNode::rotate(const ofQuaternion& q) {
	orientation *= q;
	createMatrix();
}

//----------------------------------------
void ofNode::rotate(float degrees, const ofVec3f& v) {
	rotate(ofQuaternion(degrees, v));
}


//----------------------------------------
void ofNode::rotate(float degrees, float vx, float vy, float vz) {
	rotate(ofQuaternion(degrees, ofVec3f(vx, vy, vz)));
}

//----------------------------------------
void ofNode::rotateAround(const ofQuaternion& q, const ofVec3f& point) {
	ofMatrix4x4 m = getLocalTransformMatrix();
	//	m.setTranslation(point);
	//	m.rotate(q);
	
	onOrientationChanged();
	onPositionChanged();
}


//----------------------------------------
void ofNode::rotateAround(float degrees, const ofVec3f& axis, const ofVec3f& point) {
	
}



//----------------------------------------
void ofNode::lookAt(const ofVec3f& lookAtPosition, ofVec3f upVector) {
	//		localTransformMatrix.makeLookAtMatrix(position, lookAtPosition, upVector);
//	ofVec3f savePos = getGlobalPosition();
	if(parent) upVector = upVector * ofMatrix4x4::getInverseOf(parent->getGlobalTransformMatrix());
	ofVec3f zaxis = (getGlobalPosition() - lookAtPosition).normalized();
	ofVec3f xaxis = upVector.getCrossed(zaxis).normalized();
	ofVec3f yaxis = zaxis.getCrossed(xaxis);
	
	ofMatrix4x4 m;
	m._mat[0].set(xaxis.x, xaxis.y, xaxis.z, 0);
	m._mat[1].set(yaxis.x, yaxis.y, yaxis.z, 0);
	m._mat[2].set(zaxis.x, zaxis.y, zaxis.z, 0);
	
	setGlobalOrientation(m.getRotate());
//	setGlobalPosition(position);
	//	_mat[3].set(-xaxis.dot(eye), -yaxis.dot(eye), -zaxis.dot(eye), 1);
	
//	orientation = localTransformMatrix.getRotate();
//	createMatrix();
//	onOrientationChanged();
}

void ofNode::lookAt(ofNode& lookAtNode, const ofVec3f& upVector) {
	lookAt(lookAtNode.getGlobalPosition(), upVector);
}

//----------------------------------------
ofVec3f ofNode::getXAxis() const {
	return axis[0];
}


//----------------------------------------
ofVec3f ofNode::getYAxis() const {
	return axis[1];
}


//----------------------------------------
ofVec3f ofNode::getZAxis() const {
	return axis[2];
}

//----------------------------------------
ofVec3f ofNode::getSideDir() const {
	return getXAxis();
}

//----------------------------------------
ofVec3f ofNode::getLookAtDir() const {
	return -getZAxis();
}

//----------------------------------------
ofVec3f ofNode::getUpDir() const {
	return getYAxis();
}


//----------------------------------------
float ofNode::getPitch() const {
	return getOrientationEuler().x;
}


//----------------------------------------
float ofNode::getHeading() const {
	return getOrientationEuler().y;
}


//----------------------------------------
float ofNode::getRoll() const {
	return getOrientationEuler().z;
}


//----------------------------------------
//void ofNode::setTransformMatrix(const ofMatrix4x4& matrix) {
//	this->localTransformMatrix = matrix;
//}

//----------------------------------------
const ofMatrix4x4& ofNode::getLocalTransformMatrix() const {
	return localTransformMatrix;
}

//----------------------------------------
ofMatrix4x4 ofNode::getGlobalTransformMatrix() const {
	if(parent) return getLocalTransformMatrix() * parent->getGlobalTransformMatrix();
	else return getLocalTransformMatrix();
	
	//	return globalTransformMatrix;
}

//----------------------------------------
ofVec3f ofNode::getGlobalPosition() const {
	return getGlobalTransformMatrix().getTranslation();
}

//----------------------------------------
ofQuaternion ofNode::getGlobalOrientation() const {
	return getGlobalTransformMatrix().getRotate();
}

//----------------------------------------
//ofVec3f getGlobalScale() {
//	return 
//}


//----------------------------------------
void ofNode::orbit(float longitude, float latitude, float radius, const ofVec3f& centerPoint) {
	ofMatrix4x4 m;
	
	// find position
	ofVec3f p(0, 0, radius);
	p.rotate(latitude, ofVec3f(1, 0, 0));
	p.rotate(longitude, ofVec3f(0, 1, 0));
	p += centerPoint;
	setPosition(p);
	
	// find upvector for lookat
	//	ofVec3f u((p-centerPoint).getCrossed(ofVec3f(0, 0, 1)).getNormalized());
	//	if(u.lengthSquared() > 0) {	// if it exists
	//		
	//	} else {
	//		//		u.set(0, <#float _y#>, <#float _z#>)
	//	}
	//	
	//	
	//	
	lookAt(centerPoint);//, v - centerPoint);
}

//----------------------------------------
void ofNode::orbit(float longitude, float latitude, float radius, ofNode& centerNode) {
	orbit(longitude, latitude, radius, centerNode.getGlobalPosition());
}


//----------------------------------------
void ofNode::resetTransform() {
	setPosition(ofVec3f());
	setOrientation(ofVec3f());
}

//----------------------------------------
void ofNode::draw() {
	transformGL();
	customDraw();
	restoreTransformGL();
}


//----------------------------------------
void ofNode::transformGL() const {
	glPushMatrix();
	glMultMatrixf(getGlobalTransformMatrix().getPtr());
}

//----------------------------------------
void ofNode::restoreTransformGL() const {
	glPopMatrix();
}

//----------------------------------------
void ofNode::createMatrix() {
	//if(isMatrixDirty) {
	//	isMatrixDirty = false;
	localTransformMatrix.makeScaleMatrix(scale);
	localTransformMatrix.rotate(orientation);
	localTransformMatrix.setTranslation(position);
	
	if(scale[0]>0) axis[0] = getLocalTransformMatrix().getRowAsVec3f(0)/scale[0];
	if(scale[1]>0) axis[1] = getLocalTransformMatrix().getRowAsVec3f(1)/scale[1];
	if(scale[2]>0) axis[2] = getLocalTransformMatrix().getRowAsVec3f(2)/scale[2];
}


