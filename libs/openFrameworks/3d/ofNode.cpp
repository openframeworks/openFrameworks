
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
	//	isMatrixDirty = true;
	updateMatrix();
	//	transformationMatrix.setTranslation(position);
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
	//	isMatrixDirty = true;
	updateMatrix();
	//	transformationMatrix.setRotate(orientation);
}


//----------------------------------------
void ofNode::setOrientation(const ofVec3f& eulerAngles) {
	setOrientation(ofQuaternion(eulerAngles.y, ofVec3f(0, 1, 0), eulerAngles.x, ofVec3f(1, 0, 0), eulerAngles.z, ofVec3f(0, 0, 1)));
}


//----------------------------------------
//void ofNode::setOrientation(const ofMatrix3x3& orientation) {
//}


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
//ofMatrix3x3 ofNode::getOrientationMatrix() const {
//}


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
	//	isMatrixDirty = true;
	updateMatrix();
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
	//	isMatrixDirty = true;
	updateMatrix();
	//	transformationMatrix.translate(offset);
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
	//	isMatrixDirty = true;
	updateMatrix();
	//	transformationMatrix.rotate(q);
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
//	ofMatrix4x4 m = transformationMatrix;
//	m.setPosition(point);
//	m.rotate(q);
}


//----------------------------------------
void ofNode::rotateAround(float degrees, const ofVec3f& axis, const ofVec3f& point) {
	
}



//----------------------------------------
void ofNode::lookAt(const ofVec3f& lookAtPosition, const ofVec3f& upVector) {
	ofMatrix4x4 m;
	transformationMatrix.makeLookAtMatrix(position, lookAtPosition, upVector);
	//	transformationMatrix.scale(scale);
	orientation = transformationMatrix.getRotate();
	updateMatrix();
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
//void ofNode::setMatrix(const ofMatrix4x4& matrix) {
//	this->transformationMatrix = matrix;
//}

//----------------------------------------
const ofMatrix4x4& ofNode::getMatrix() {
	//	updateMatrix();
	return transformationMatrix;
}

//----------------------------------------
ofMatrix4x4 ofNode::getGlobalMatrix() {
	if(parent) return getMatrix() * parent->getGlobalMatrix();
	else return getMatrix();
}

//----------------------------------------
ofVec3f ofNode::getGlobalPosition() {
	return getGlobalMatrix().getTranslation();
}

//----------------------------------------
ofQuaternion ofNode::getGlobalOrientation() {
	return getGlobalMatrix().getRotate();
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
	transform();
	customDraw();
	restoreTransform();
}



//----------------------------------------
void ofNode::debugDraw() {
	transform();
	customDebugDraw();
	restoreTransform();
}


//----------------------------------------
void ofNode::transform() {
	glPushMatrix();
	glMultMatrixf(getGlobalMatrix().getPtr());
}

//----------------------------------------
void ofNode::restoreTransform() {
	glPopMatrix();
}

//----------------------------------------
void ofNode::customDebugDraw() {
	ofBox(0, 0, 0, 10);
	
	float axisLength = 15;
	// draw world x axis
	ofSetColor(255, 0, 0);
	glPushMatrix();
	glTranslatef(axisLength/2, 0, 0);
	glScalef(axisLength, 2, 2);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
	
	// draw world y axis
	ofSetColor(0, 255, 0);
	glPushMatrix();
	glTranslatef(0, axisLength/2, 0);
	glScalef(2, axisLength, 2);
	ofBox(0, 0, 0, 1);
	glPopMatrix();
	
	// draw world z axis
	ofSetColor(0, 0, 2550);
	glPushMatrix();
	glTranslatef(0, 0, axisLength/2);
	glScalef(2, 2, axisLength);
	ofBox(0, 0, 0, 1);
	glPopMatrix();	
}


//----------------------------------------
void ofNode::updateMatrix() {
	//if(isMatrixDirty) {
	//	isMatrixDirty = false;
	transformationMatrix.makeScaleMatrix(scale);
	transformationMatrix.rotate(orientation);
	transformationMatrix.setTranslation(position);
	
	if(scale[0]>0) axis[0] = getMatrix().getRowAsVec3f(0)/scale[0];
	if(scale[1]>0) axis[1] = getMatrix().getRowAsVec3f(1)/scale[1];
	if(scale[2]>0) axis[2] = getMatrix().getRowAsVec3f(2)/scale[2];
	//}
}



