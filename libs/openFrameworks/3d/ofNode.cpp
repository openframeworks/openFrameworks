
#include "ofNode.h"
#include "ofMath.h"
#include "ofLog.h"
#include "of3dGraphics.h"
#include "ofVec3f.h"

//----------------------------------------
ofNode::ofNode()
:parent(nullptr)
,legacyCustomDrawOverrided(true){
	setPosition({0.f, 0.f, 0.f});
	setOrientation({0.f, 0.f, 0.f});
	setScale(1.f);
	position.disableEvents();
	scale.disableEvents();
	orientation.disableEvents();
}

//----------------------------------------
ofNode::~ofNode(){
	if(parent){
		parent->removeListener(*this);
	}
	for(auto child: children){
		child->clearParent();
	}
}

//----------------------------------------
ofNode::ofNode(const ofNode & node)
:parent(node.parent)
,axis(node.axis)
,localTransformMatrix(node.localTransformMatrix)
,legacyCustomDrawOverrided(true){
	if(parent){
		parent->addListener(*this);
	}
	position = node.position;
	orientation = node.orientation;
	scale = node.scale;
	position.disableEvents();
	scale.disableEvents();
	orientation.disableEvents();
}

//----------------------------------------
ofNode::ofNode(ofNode && node)
:parent(node.parent)
,position(std::move(node.position))
,orientation(std::move(node.orientation))
,scale(std::move(node.scale))
,axis(std::move(node.axis))
,localTransformMatrix(std::move(node.localTransformMatrix))
,legacyCustomDrawOverrided(std::move(node.legacyCustomDrawOverrided))
,children(std::move(node.children)){
	if(parent){
		parent->addListener(*this);
	}
}

//----------------------------------------
ofNode & ofNode::operator=(const ofNode & node){
	if(this == &node) return *this;
	parent = node.parent;
	position = node.position;
	orientation = node.orientation;
	scale = node.scale;
	axis = node.axis;
	position.disableEvents();
	scale.disableEvents();
	orientation.disableEvents();
	localTransformMatrix = node.localTransformMatrix;
	legacyCustomDrawOverrided = true;
	if(parent){
		parent->addListener(*this);
	}
	return *this;
}

//----------------------------------------
ofNode & ofNode::operator=(ofNode && node){
	if(this == &node) return *this;
	parent = node.parent;
	position = std::move(node.position);
	orientation = std::move(node.orientation);
	scale = std::move(node.scale);
	axis = std::move(node.axis);
	localTransformMatrix = std::move(node.localTransformMatrix);
	legacyCustomDrawOverrided = std::move(node.legacyCustomDrawOverrided);
	children = std::move(node.children);
	if(parent){
		parent->addListener(*this);
	}
	return *this;
}

//----------------------------------------
void ofNode::addListener(ofNode & node){
	position.addListener(&node, &ofNode::onParentPositionChanged);
	orientation.addListener(&node, &ofNode::onParentOrientationChanged);
	scale.addListener(&node, &ofNode::onParentScaleChanged);
	position.enableEvents();
	orientation.enableEvents();
	scale.enableEvents();
	children.insert(&node);
}

//----------------------------------------
void ofNode::removeListener(ofNode & node){
	position.removeListener(&node, &ofNode::onParentPositionChanged);
	orientation.removeListener(&node, &ofNode::onParentOrientationChanged);
	scale.removeListener(&node, &ofNode::onParentScaleChanged);
	if(position.getNumListeners()==0){
		position.disableEvents();
		scale.disableEvents();
		orientation.disableEvents();
	}
	children.erase(&node);
}

//----------------------------------------
void ofNode::setParent(ofNode& parent, bool bMaintainGlobalTransform) {
    if(bMaintainGlobalTransform) {
		auto postParentGlobalTransform = glm::inverse(parent.getGlobalTransformMatrix()) * getGlobalTransformMatrix();
		parent.addListener(*this);
		setTransformMatrix(postParentGlobalTransform);
	} else {
		parent.addListener(*this);
	}
	this->parent = &parent;
}

//----------------------------------------
void ofNode::clearParent(bool bMaintainGlobalTransform) {
	if(parent){
		parent->removeListener(*this);
	}
    if(bMaintainGlobalTransform) {
		auto globalTransform = getGlobalTransformMatrix();
        this->parent = nullptr;
        setTransformMatrix(globalTransform);
    } else {
        this->parent = nullptr;
    }
}

//----------------------------------------
ofNode* ofNode::getParent() const {
	return parent;
}

#include "glm/gtx/matrix_decompose.hpp"

//----------------------------------------
void ofNode::setTransformMatrix(const glm::mat4 &m44) {
	localTransformMatrix = m44;

	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m44, scale, orientation, translation, skew, perspective);
	this->position = translation;
	this->scale = scale;
	this->orientation = orientation;
	updateAxis();
	
	onPositionChanged();
	onOrientationChanged();
	onScaleChanged();
}

//----------------------------------------
void ofNode::setPosition(float px, float py, float pz) {
	setPosition({px, py, pz});
}

//----------------------------------------
void ofNode::setPosition(const glm::vec3& p) {
	position = p;
	createMatrix();
	onPositionChanged();
}

//----------------------------------------
void ofNode::setGlobalPosition(float px, float py, float pz) {
	setGlobalPosition({px, py, pz});
}

//----------------------------------------
void ofNode::setGlobalPosition(const glm::vec3& p) {
	if(parent == nullptr) {
		setPosition(p);
	} else {
		auto newP = glm::inverse(parent->getGlobalTransformMatrix()) * glm::vec4(p, 1.0);
		setPosition(newP.xyz() / newP.w);
	}
}

//----------------------------------------
glm::vec3 ofNode::getPosition() const {
	return position;
}

//----------------------------------------
float ofNode::getX() const {
	return position->x;
}

//----------------------------------------
float ofNode::getY() const {
	return position->y;
}

//----------------------------------------
float ofNode::getZ() const {
	return position->z;
}

//----------------------------------------
void ofNode::setOrientation(const glm::quat& q) {
	orientation = q;
	createMatrix();
	onOrientationChanged();
}

//----------------------------------------
void ofNode::setOrientation(const glm::vec3& eulerAngles) {
	setOrientation(glm::angleAxis(ofDegToRad(eulerAngles.x), glm::vec3(1, 0, 0)) * glm::angleAxis(ofDegToRad(eulerAngles.z), glm::vec3(0, 0, 1)) * glm::angleAxis(ofDegToRad(eulerAngles.y), glm::vec3(0, 1, 0)));
}

//----------------------------------------
void ofNode::setGlobalOrientation(const glm::quat& q) {
	if(parent == nullptr) {
		setOrientation(q);
	} else {
		auto invParent = glm::inverse(parent->getGlobalTransformMatrix());
		auto m44 = q * glm::toQuat(invParent);
		setOrientation(m44);
	}
}

//----------------------------------------
glm::quat ofNode::getOrientationQuat() const {
	return orientation;
}

//----------------------------------------
glm::vec3 ofNode::getOrientationEuler() const {
	return glm::eulerAngles((const glm::quat&)orientation);
}

//----------------------------------------
void ofNode::setScale(float s) {
	setScale(s, s, s);
}

//----------------------------------------
void ofNode::setScale(float sx, float sy, float sz) {
	setScale({sx, sy, sz});
}

//----------------------------------------
void ofNode::setScale(const glm::vec3& s) {
	this->scale = s;
	createMatrix();
	onScaleChanged();
}

//----------------------------------------
glm::vec3 ofNode::getScale() const {
	return scale;
}

//----------------------------------------
void ofNode::move(float x, float y, float z) {
	move({x, y, z});
}

//----------------------------------------
void ofNode::move(const glm::vec3& offset) {
	position += offset;
	createMatrix();
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
void ofNode::rotate(const glm::quat& q) {
	orientation = q * (const glm::quat&)orientation;
	createMatrix();
	onOrientationChanged();
}

//----------------------------------------
void ofNode::rotate(float degrees, const glm::vec3& v) {
	rotate(glm::angleAxis(ofDegToRad(degrees), v));
}

//----------------------------------------
void ofNode::rotate(float degrees, float vx, float vy, float vz) {
	rotate(glm::angleAxis(ofDegToRad(degrees), glm::vec3(vx, vy, vz)));
}

//----------------------------------------
void ofNode::rotateAround(const glm::quat& q, const glm::vec3& point) {
	//	ofLogVerbose("ofNode") << "rotateAround(const glm::quat& q, const ofVec3f& point) not implemented yet";
	//	ofMatrix4x4 m = getLocalTransformMatrix();
	//	m.setTranslation(point);
	//	m.rotate(q);
	
	setGlobalPosition(q * (getGlobalPosition() - point) + point);
	
	onOrientationChanged();
	onPositionChanged();
}

//----------------------------------------
void ofNode::rotateAround(float degrees, const glm::vec3& axis, const glm::vec3& point) {
	rotateAround(glm::angleAxis(ofDegToRad(degrees), axis), point);
}

//----------------------------------------
void ofNode::lookAt(const glm::vec3& lookAtPosition){
    auto relPosition = (getGlobalPosition() - lookAtPosition);
	auto radius = glm::length(relPosition);
    if(radius>0){
		auto latitude = acos(relPosition.y / radius) - glm::half_pi<float>();
		auto longitude = atan2(relPosition.x , relPosition.z);
		glm::quat q = glm::angleAxis(latitude, glm::vec3(1,0,0)) * glm::angleAxis(longitude, glm::vec3(0,1,0)) * glm::angleAxis(0.f, glm::vec3(0,0,1));
        setGlobalOrientation(q);
    }

}

//----------------------------------------
void ofNode::lookAt(const glm::vec3& lookAtPosition, glm::vec3 upVector) {
	if(parent){
		auto upVector4 = glm::inverse(parent->getGlobalTransformMatrix()) * glm::vec4(upVector, 1.0);
		upVector = upVector4.xyz() / upVector4.w;
	}
	auto zaxis = glm::normalize(getGlobalPosition() - lookAtPosition);
	if (glm::length(zaxis) > 0) {
		auto xaxis = glm::normalize(glm::cross(upVector, zaxis));
		auto yaxis = glm::cross(zaxis, xaxis);
		glm::mat4 m;
		m[0] = glm::vec4(xaxis, 0.f);
		m[1] = glm::vec4(yaxis, 0.f);
		m[2] = glm::vec4(zaxis, 0.f);

		setGlobalOrientation(glm::toQuat(m));
	}
}

//----------------------------------------
void ofNode::lookAt(const ofNode& lookAtNode){
    lookAt(lookAtNode.getGlobalPosition());
}

//----------------------------------------
void ofNode::lookAt(const ofNode& lookAtNode, const glm::vec3& upVector) {
	lookAt(lookAtNode.getGlobalPosition(), upVector);
}

//----------------------------------------
void ofNode::updateAxis() {
	if(scale->x>0) axis[0] = (getLocalTransformMatrix()[0]/scale->x).xyz();
	if(scale->y>0) axis[1] = (getLocalTransformMatrix()[1]/scale->y).xyz();
	if(scale->z>0) axis[2] = (getLocalTransformMatrix()[2]/scale->z).xyz();
}

//----------------------------------------
glm::vec3 ofNode::getXAxis() const {
	return axis[0];
}

//----------------------------------------
glm::vec3 ofNode::getYAxis() const {
	return axis[1];
}

//----------------------------------------
glm::vec3 ofNode::getZAxis() const {
	return axis[2];
}

//----------------------------------------
glm::vec3 ofNode::getSideDir() const {
	return getXAxis();
}

//----------------------------------------
glm::vec3 ofNode::getLookAtDir() const {
	return -getZAxis();
}

//----------------------------------------
glm::vec3 ofNode::getUpDir() const {
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
const glm::mat4& ofNode::getLocalTransformMatrix() const {
	return localTransformMatrix;
}

//----------------------------------------
glm::mat4 ofNode::getGlobalTransformMatrix() const {
	if(parent) return parent->getGlobalTransformMatrix() * getLocalTransformMatrix();
	else return getLocalTransformMatrix();
}

//----------------------------------------
glm::vec3 ofNode::getGlobalPosition() const {
	return getGlobalTransformMatrix()[3].xyz();
}

//----------------------------------------
glm::quat ofNode::getGlobalOrientation() const {
	return glm::toQuat(getGlobalTransformMatrix());
}

//----------------------------------------
glm::vec3 ofNode::getGlobalScale() const {
	if(parent) return getScale()*parent->getGlobalScale();
	else return getScale();
}

//----------------------------------------
void ofNode::orbit(float longitude, float latitude, float radius, const glm::vec3& centerPoint) {
	glm::quat q = glm::angleAxis(ofDegToRad(latitude), glm::vec3(1,0,0)) * glm::angleAxis(ofDegToRad(longitude), glm::vec3(0,1,0)) * glm::angleAxis(0.f, glm::vec3(0,0,1));
	setPosition(q * (glm::vec3(0,0,radius)-centerPoint) + centerPoint);
	setOrientation(q);
	onOrientationChanged();
	onPositionChanged();
//	lookAt(centerPoint);//, v - centerPoint);
}

//----------------------------------------
void ofNode::orbit(float longitude, float latitude, float radius, ofNode& centerNode) {
	orbit(longitude, latitude, radius, centerNode.getGlobalPosition());
}

//----------------------------------------
void ofNode::resetTransform() {
	setPosition({0.f,0.f,0.f});
	setOrientation({0.f,0.f,0.f});
    setScale({1.f,1.f,1.f});
}

//----------------------------------------
void ofNode::draw()  const{
	ofGetCurrentRenderer()->draw(*this);
}

//----------------------------------------
void ofNode::customDraw(const ofBaseRenderer * renderer) const{
	const_cast<ofNode*>(this)->customDraw();
	if(!legacyCustomDrawOverrided){
		renderer->drawBox(10);
		renderer->draw(ofMesh::axis(20),OF_MESH_FILL);
	}
}

//----------------------------------------
void ofNode::customDraw(){
	legacyCustomDrawOverrided = false;
}

//----------------------------------------
void ofNode::transformGL(ofBaseRenderer * renderer) const {
	if( renderer == nullptr ) {
		renderer = ofGetCurrentRenderer().get();
	}
	renderer->pushMatrix();
	renderer->multMatrix( getGlobalTransformMatrix() );
}

//----------------------------------------
void ofNode::restoreTransformGL(ofBaseRenderer * renderer) const {
	if( renderer == nullptr ) {
		renderer = ofGetCurrentRenderer().get();
	}
	renderer->popMatrix();
}

//----------------------------------------
void ofNode::createMatrix() {
	localTransformMatrix = glm::translate(glm::mat4(1.0), toGlm(position));
	localTransformMatrix = localTransformMatrix * glm::toMat4((const glm::quat&)orientation);
	localTransformMatrix = glm::scale(localTransformMatrix, toGlm(scale));
	
	updateAxis();
}


