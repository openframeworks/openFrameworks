
#include "ofNode3d.h"
#include "ofMain.h"


ofNode3d::ofNode3d() : parent(NULL)//, /*targetNode(NULL), */ isMatrixDirty(false) {
{
	setPosition(ofVec3f(0, 0, 0));
	setOrientation(ofVec3f(0, 0, 0));
	setScale(1);
}


//----------------------------------------
void ofNode3d::setParent(ofNode3d* parent) {
	this->parent = parent;
}


//----------------------------------------
ofNode3d* ofNode3d::getParent() const {
	return parent;
}


//----------------------------------------
//void ofNode3d::setTargetNode(ofNode3d* targetNode) {
//	this->targetNode = targetNode;
//}
//
//
////----------------------------------------
//ofNode3d* ofNode3d::getTargetNode() const {
//	return targetNode;
//}
//

//----------------------------------------
void ofNode3d::setPosition(float px, float py, float pz) {
	setPosition(ofVec3f(px, py, pz));
}


//----------------------------------------
void ofNode3d::setPosition(const ofVec3f& p) {
	position = p;
	//	isMatrixDirty = true;
	updateMatrix();
	//	transformMatrix.setTranslation(position);
}


//----------------------------------------
ofVec3f ofNode3d::getPosition() const {
	return position;
}


//----------------------------------------
float ofNode3d::getX() const {
	return position.x;
}


//----------------------------------------
float ofNode3d::getY() const {
	return position.y;
}


//----------------------------------------
float ofNode3d::getZ() const {
	return position.z;
}


//----------------------------------------
void ofNode3d::setOrientation(const ofQuaternion& q) {
	orientation = q;
	//	isMatrixDirty = true;
	updateMatrix();
	//	transformMatrix.setRotate(orientation);
}


//----------------------------------------
void ofNode3d::setOrientation(const ofVec3f& eulerAngles) {
	setOrientation(ofQuaternion(eulerAngles.y, ofVec3f(0, 1, 0), eulerAngles.x, ofVec3f(1, 0, 0), eulerAngles.z, ofVec3f(0, 0, 1)));
}


//----------------------------------------
//void ofNode3d::setOrientation(const ofMatrix3x3& orientation) {
//}


//----------------------------------------
ofQuaternion ofNode3d::getOrientationQuat() const {
	return orientation;
}


//----------------------------------------
// ref at http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
ofVec3f ofNode3d::getOrientationEuler() const {
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
//ofMatrix3x3 ofNode3d::getOrientationMatrix() const {
//}


//----------------------------------------
void ofNode3d::setScale(float s) {
	setScale(s, s, s);
}


//----------------------------------------
void ofNode3d::setScale(float sx, float sy, float sz) {
	setScale(ofVec3f(sx, sy, sz));
}


//----------------------------------------
void ofNode3d::setScale(const ofVec3f& s) {
	this->scale = s;
	//	isMatrixDirty = true;
	updateMatrix();
}


//----------------------------------------
ofVec3f ofNode3d::getScale() const {
	return scale;
}

//----------------------------------------
void ofNode3d::move(float x, float y, float z) {
	move(ofVec3f(x, y, z));
}


//----------------------------------------
void ofNode3d::move(const ofVec3f& offset) {
	position += offset;
	//	isMatrixDirty = true;
	updateMatrix();
	//	transformMatrix.translate(offset);
}


//----------------------------------------
void ofNode3d::truck(float amount) {
	move(getXAxis() * amount);
}


//----------------------------------------
void ofNode3d::boom(float amount) {
	move(getYAxis() * amount);
}


//----------------------------------------
void ofNode3d::dolly(float amount) {
	move(getZAxis() * amount);
}


//----------------------------------------
void ofNode3d::tilt(float degrees) {
	rotate(degrees, getXAxis());
}


//----------------------------------------
void ofNode3d::pan(float degrees) {
	rotate(degrees, getYAxis());
}


//----------------------------------------
void ofNode3d::roll(float degrees) {
	rotate(degrees, getZAxis());
}


//----------------------------------------
void ofNode3d::rotate(const ofQuaternion& q) {
	orientation *= q;
	//	isMatrixDirty = true;
	updateMatrix();
	//	transformMatrix.rotate(q);
}

//----------------------------------------
void ofNode3d::rotate(float degrees, const ofVec3f& v) {
	rotate(ofQuaternion(degrees, v));
}


//----------------------------------------
void ofNode3d::rotate(float degrees, float vx, float vy, float vz) {
	rotate(ofQuaternion(degrees, ofVec3f(vx, vy, vz)));
}


//----------------------------------------
void ofNode3d::lookAt(const ofVec3f& lookAtPosition, const ofVec3f& upVector) {
	ofMatrix4x4 m;
	transformMatrix.makeLookAtMatrix(position, lookAtPosition, upVector);
	//	transformMatrix.scale(scale);
	orientation = transformMatrix.getRotate();
	updateMatrix();
}

void ofNode3d::lookAt(ofNode3d& lookAtNode, const ofVec3f& upVector) {
	lookAt(lookAtNode.getGlobalPosition());
}

//----------------------------------------
ofVec3f ofNode3d::getXAxis() {
	return axis[0];
}


//----------------------------------------
ofVec3f ofNode3d::getYAxis() {
	return axis[1];
}


//----------------------------------------
ofVec3f ofNode3d::getZAxis() {
	return axis[2];
}


//----------------------------------------
float ofNode3d::getPitch() const {
	return getOrientationEuler().x;
}


//----------------------------------------
float ofNode3d::getHeading() const {
	return getOrientationEuler().y;
}


//----------------------------------------
float ofNode3d::getRoll() const {
	return getOrientationEuler().z;
}


//----------------------------------------
//void ofNode3d::setMatrix(const ofMatrix4x4& matrix) {
//	this->transformMatrix = matrix;
//}

//----------------------------------------
const ofMatrix4x4& ofNode3d::getMatrix() {
	//	updateMatrix();
	return transformMatrix;
}

//----------------------------------------
ofMatrix4x4 ofNode3d::getGlobalMatrix() {
	if(parent) return getMatrix() * parent->getGlobalMatrix();
	else return getMatrix();
}

//----------------------------------------
ofVec3f ofNode3d::getGlobalPosition() {
	return getGlobalMatrix().getTranslation();
}

//----------------------------------------
ofQuaternion ofNode3d::getGlobalOrientation() {
	return getGlobalMatrix().getRotate();
}

//----------------------------------------
//ofVec3f getGlobalScale() {
//	return 
//}


//----------------------------------------
void ofNode3d::orbit(float longitude, float latitude, float radius, const ofVec3f& centerPoint) {
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

void ofNode3d::orbit(float longitude, float latitude, float radius, ofNode3d& centerNode) {
	orbit(longitude, latitude, radius, centerNode.getGlobalPosition());
}


void ofNode3d::resetTransform() {
	setPosition(ofVec3f());
	setOrientation(ofVec3f());
}

//----------------------------------------
void ofNode3d::draw() {
	glPushMatrix();
	glMultMatrixf(getGlobalMatrix().getPtr());
	customDraw();
	glPopMatrix();
}



//----------------------------------------
void ofNode3d::debugDraw() {
	glPushMatrix();
	glMultMatrixf(getGlobalMatrix().getPtr());
	customDebugDraw();
	glPopMatrix();
}


//----------------------------------------
void ofNode3d::customDebugDraw() {
	ofSetColor(255, 255, 0);
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
void ofNode3d::updateMatrix() {
	//	if(targetNode) lookAt(targetNode->getGlobalMatrix().getTranslation());
	
	//if(isMatrixDirty) {
	//	isMatrixDirty = false;
	transformMatrix.makeScaleMatrix(scale);
	transformMatrix.rotate(orientation);
	transformMatrix.setTranslation(position);
	
	if(scale[0]>0) axis[0] = getMatrix().getRowAsVec3f(0)/scale[0];
	if(scale[1]>0) axis[1] = getMatrix().getRowAsVec3f(1)/scale[1];
	if(scale[2]>0) axis[2] = getMatrix().getRowAsVec3f(2)/scale[2];
	//}
}



