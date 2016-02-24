
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
		ofMatrix4x4 postParentGlobalTransform = getGlobalTransformMatrix() * parent.getGlobalTransformMatrix().getInverse();
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
        ofMatrix4x4 globalTransform(getGlobalTransformMatrix());
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

//----------------------------------------
void ofNode::setTransformMatrix(const ofMatrix4x4 &m44) {
	localTransformMatrix = m44;

	ofVec3f position;
	ofVec3f scale;
	ofQuaternion orientation;
	ofQuaternion so;
	localTransformMatrix.decompose(position, orientation, scale, so);
	this->position = toGlm(position);
	this->scale = toGlm(scale);
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
	localTransformMatrix.setTranslation(toOf(position));
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
		setPosition(toGlm(toOf(p) * ofMatrix4x4::getInverseOf(parent->getGlobalTransformMatrix())));
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
void ofNode::setOrientation(const ofQuaternion& q) {
	orientation = q;
	createMatrix();
	onOrientationChanged();
}

//----------------------------------------
void ofNode::setOrientation(const glm::vec3& eulerAngles) {
	setOrientation(ofQuaternion(eulerAngles.x, ofVec3f(1, 0, 0), eulerAngles.z, ofVec3f(0, 0, 1), eulerAngles.y, ofVec3f(0, 1, 0)));
}

//----------------------------------------
void ofNode::setGlobalOrientation(const ofQuaternion& q) {
	if(parent == nullptr) {
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
glm::vec3 ofNode::getOrientationEuler() const {
	return toGlm(orientation->getEuler());
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
	localTransformMatrix.setTranslation(toOf(position));
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
	onOrientationChanged();
}

//----------------------------------------
void ofNode::rotate(float degrees, const glm::vec3& v) {
	rotate(ofQuaternion(degrees, toOf(v)));
}

//----------------------------------------
void ofNode::rotate(float degrees, float vx, float vy, float vz) {
	rotate(ofQuaternion(degrees, ofVec3f(vx, vy, vz)));
}

//----------------------------------------
void ofNode::rotateAround(const ofQuaternion& q, const glm::vec3& point) {
	//	ofLogVerbose("ofNode") << "rotateAround(const ofQuaternion& q, const ofVec3f& point) not implemented yet";
	//	ofMatrix4x4 m = getLocalTransformMatrix();
	//	m.setTranslation(point);
	//	m.rotate(q);
	
	setGlobalPosition(toGlm(toOf(getGlobalPosition() - point) * q) + point);
	
	onOrientationChanged();
	onPositionChanged();
}

//----------------------------------------
void ofNode::rotateAround(float degrees, const glm::vec3& axis, const glm::vec3& point) {
	rotateAround(ofQuaternion(degrees, toOf(axis)), point);
}

//----------------------------------------
void ofNode::lookAt(const glm::vec3& lookAtPosition){
    auto relPosition = (getGlobalPosition() - lookAtPosition);
    auto radius = relPosition.length();
    if(radius>0){
        auto latitude = ofRadToDeg(acos(relPosition.y / radius)) - 90;
        auto longitude = ofRadToDeg(atan2(relPosition.x , relPosition.z));
        ofQuaternion q(latitude, ofVec3f(1,0,0), longitude, ofVec3f(0,1,0), 0, ofVec3f(0,0,1));
        setGlobalOrientation(q);
    }

}

//----------------------------------------
void ofNode::lookAt(const glm::vec3& lookAtPosition, glm::vec3 upVector) {
	if(parent) upVector = toGlm(toOf(upVector) * ofMatrix4x4::getInverseOf(parent->getGlobalTransformMatrix()));
	auto zaxis = glm::normalize(getGlobalPosition() - lookAtPosition);
	if (zaxis.length() > 0) {
		auto xaxis = glm::normalize(glm::cross(upVector, zaxis));
		auto yaxis = glm::cross(zaxis, xaxis);
		
		ofMatrix4x4 m;
		m._mat[0].set(xaxis.x, xaxis.y, xaxis.z, 0);
		m._mat[1].set(yaxis.x, yaxis.y, yaxis.z, 0);
		m._mat[2].set(zaxis.x, zaxis.y, zaxis.z, 0);
		
		setGlobalOrientation(m.getRotate());
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
	if(scale->x>0) axis[0] = toGlm(getLocalTransformMatrix().getRowAsVec3f(0)/scale->x);
	if(scale->y>0) axis[1] = toGlm(getLocalTransformMatrix().getRowAsVec3f(1)/scale->y);
	if(scale->z>0) axis[2] = toGlm(getLocalTransformMatrix().getRowAsVec3f(2)/scale->z);
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
const ofMatrix4x4& ofNode::getLocalTransformMatrix() const {
	return localTransformMatrix;
}

//----------------------------------------
ofMatrix4x4 ofNode::getGlobalTransformMatrix() const {
	if(parent) return getLocalTransformMatrix() * parent->getGlobalTransformMatrix();
	else return getLocalTransformMatrix();
}

//----------------------------------------
glm::vec3 ofNode::getGlobalPosition() const {
	return toGlm(getGlobalTransformMatrix().getTranslation());
}

//----------------------------------------
ofQuaternion ofNode::getGlobalOrientation() const {
	return getGlobalTransformMatrix().getRotate();
}

//----------------------------------------
glm::vec3 ofNode::getGlobalScale() const {
	if(parent) return getScale()*parent->getGlobalScale();
	else return getScale();
}

//----------------------------------------
void ofNode::orbit(float longitude, float latitude, float radius, const glm::vec3& centerPoint) {
	ofQuaternion q(latitude, ofVec3f(1,0,0), longitude, ofVec3f(0,1,0), 0, ofVec3f(0,0,1));
	setPosition(toGlm(toOf(glm::vec3(0,0,radius)-centerPoint)*q) + centerPoint);
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
	//if(isMatrixDirty) {
	//	isMatrixDirty = false;
	localTransformMatrix.makeScaleMatrix(toOf(scale));
	localTransformMatrix.rotate(orientation);
	localTransformMatrix.setTranslation(toOf(position));
	
	updateAxis();
}


