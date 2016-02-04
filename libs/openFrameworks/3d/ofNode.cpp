
#include "ofNode.h"
#include "ofMath.h"
#include "ofLog.h"
#include "of3dGraphics.h"

//----------------------------------------
ofNode::ofNode()
:parent(nullptr)
,legacyCustomDrawOverrided(true){
	setPosition(ofVec3f(0, 0, 0));
	setOrientation(ofVec3f(0, 0, 0));
	setScale(1);
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
	ofQuaternion orientation;
	ofVec3f scale;
	ofQuaternion so;
	localTransformMatrix.decompose(position, orientation, scale, so);
	this->position = position;
	this->orientation = orientation;
	this->scale = scale;
	updateAxis();
	
	onPositionChanged();
	onOrientationChanged();
	onScaleChanged();
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
	if(parent == nullptr) {
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
void ofNode::setOrientation(const ofVec3f& eulerAngles) {
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
ofVec3f ofNode::getOrientationEuler() const {
	return orientation->getEuler();
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
	onOrientationChanged();
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
	//	ofLogVerbose("ofNode") << "rotateAround(const ofQuaternion& q, const ofVec3f& point) not implemented yet";
	//	ofMatrix4x4 m = getLocalTransformMatrix();
	//	m.setTranslation(point);
	//	m.rotate(q);
	
	setGlobalPosition((getGlobalPosition() - point)* q + point); 
	
	onOrientationChanged();
	onPositionChanged();
}

//----------------------------------------
void ofNode::rotateAround(float degrees, const ofVec3f& axis, const ofVec3f& point) {
	rotateAround(ofQuaternion(degrees, axis), point);
}

//----------------------------------------
void ofNode::lookAt(const ofVec3f& lookAtPosition){
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
void ofNode::lookAt(const ofVec3f& lookAtPosition, ofVec3f upVector) {
	if(parent) upVector = upVector * ofMatrix4x4::getInverseOf(parent->getGlobalTransformMatrix());	
	ofVec3f zaxis = (getGlobalPosition() - lookAtPosition).getNormalized();
	if (zaxis.length() > 0) {
		ofVec3f xaxis = upVector.getCrossed(zaxis).getNormalized();
		ofVec3f yaxis = zaxis.getCrossed(xaxis);
		
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
void ofNode::lookAt(const ofNode& lookAtNode, const ofVec3f& upVector) {
	lookAt(lookAtNode.getGlobalPosition(), upVector);
}

//----------------------------------------
void ofNode::updateAxis() {
	if(scale->x>0) axis[0] = getLocalTransformMatrix().getRowAsVec3f(0)/scale->x;
	if(scale->y>0) axis[1] = getLocalTransformMatrix().getRowAsVec3f(1)/scale->y;
	if(scale->z>0) axis[2] = getLocalTransformMatrix().getRowAsVec3f(2)/scale->z;
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
const ofMatrix4x4& ofNode::getLocalTransformMatrix() const {
	return localTransformMatrix;
}

//----------------------------------------
ofMatrix4x4 ofNode::getGlobalTransformMatrix() const {
	if(parent) return getLocalTransformMatrix() * parent->getGlobalTransformMatrix();
	else return getLocalTransformMatrix();
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
ofVec3f ofNode::getGlobalScale() const {
	if(parent) return getScale()*parent->getGlobalScale();
	else return getScale();
}

//----------------------------------------
void ofNode::orbit(float longitude, float latitude, float radius, const ofVec3f& centerPoint) {
	ofQuaternion q(latitude, ofVec3f(1,0,0), longitude, ofVec3f(0,1,0), 0, ofVec3f(0,0,1));
	setPosition((ofVec3f(0,0,radius)-centerPoint)*q +centerPoint);
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
	setPosition(ofVec3f());
	setOrientation(ofVec3f());
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
	localTransformMatrix.makeScaleMatrix(scale);
	localTransformMatrix.rotate(orientation);
	localTransformMatrix.setTranslation(position);
	
	updateAxis();
}


