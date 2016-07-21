#include "ofCamera.h"
#include "ofLog.h"

//----------------------------------------
ofCamera::ofCamera() :
isOrtho(false),
fov(60),
nearClip(0),
farClip(0),
lensOffset(0.0f, 0.0f),
forceAspectRatio(false),
aspectRatio(4./3.),
vFlip(false)
{
}

//----------------------------------------
ofCamera::~ofCamera() {}

//----------------------------------------
void ofCamera::setFov(float f) {
	fov = f;
}

//----------------------------------------
void ofCamera::setNearClip(float f) {
	nearClip = f;
}

//----------------------------------------
void ofCamera::setFarClip(float f) {
	farClip = f;
}

//----------------------------------------
void ofCamera::setLensOffset(const glm::vec2 & lensOffset){
	this->lensOffset = lensOffset;
}

//----------------------------------------
void ofCamera::setAspectRatio(float aspectRatio){
	this->aspectRatio = aspectRatio;
	setForceAspectRatio(true);
}

//----------------------------------------
void ofCamera::setForceAspectRatio(bool forceAspectRatio){
	this->forceAspectRatio = forceAspectRatio;
}

//----------------------------------------
void ofCamera::setupPerspective(bool _vFlip, float fov, float nearDist, float farDist, const glm::vec2 & lensOffset){
	ofRectangle orientedViewport = getRenderer()->getNativeViewport();
	float eyeX = orientedViewport.width / 2;
	float eyeY = orientedViewport.height / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	setFov(fov);
	setNearClip(nearDist);
	setFarClip(farDist);
	setLensOffset(lensOffset);
	setForceAspectRatio(false);

	setPosition(eyeX,eyeY,dist);
	lookAt({eyeX,eyeY,0.f}, {0.f,1.f,0.f});
	vFlip = _vFlip;
}

//----------------------------------------
void ofCamera::setupOffAxisViewPortal(const glm::vec3 & topLeft, const glm::vec3 & bottomLeft, const glm::vec3 & bottomRight){
	auto bottomEdge = bottomRight - bottomLeft; // plane x axis
	auto leftEdge = topLeft - bottomLeft; // plane y axis
	auto bottomEdgeNorm = glm::normalize(bottomEdge);
	auto leftEdgeNorm = glm::normalize(leftEdge);
	auto bottomLeftToCam = this->getPosition() - bottomLeft;
	
	auto cameraLookVector = glm::cross(leftEdgeNorm, bottomEdgeNorm);
	auto cameraUpVector = glm::cross(bottomEdgeNorm, cameraLookVector);
	
	lookAt(cameraLookVector + this->getPosition(), cameraUpVector);

	//lensoffset
	glm::vec2 lensOffset;
	lensOffset.x = -glm::dot(bottomLeftToCam, bottomEdgeNorm) * 2.0f / glm::length(bottomEdge) + 1.0f;
	lensOffset.y = -glm::dot(bottomLeftToCam, leftEdgeNorm) * 2.0f / glm::length(leftEdge) + 1.0f;
	setLensOffset(lensOffset);
	setAspectRatio( glm::length(bottomEdge) / glm::length(leftEdge) );
	auto distanceAlongOpticalAxis = fabs(glm::dot(bottomLeftToCam, cameraLookVector));
	setFov(2.0f * RAD_TO_DEG * atan( (glm::length(leftEdge) / 2.0f) / distanceAlongOpticalAxis));
}


//----------------------------------------
void ofCamera::setVFlip(bool vflip){
	vFlip = vflip;
}

//----------------------------------------
bool ofCamera::isVFlipped() const{
	return vFlip;
}

//----------------------------------------
void ofCamera::enableOrtho() {
	isOrtho = true;
}

//----------------------------------------
void ofCamera::disableOrtho() {
	isOrtho = false;
}

//----------------------------------------
bool ofCamera::getOrtho() const {
	return isOrtho;
}

//----------------------------------------
float ofCamera::getImagePlaneDistance(ofRectangle viewport) const {
	return getViewport(viewport).height / (2.0f * tanf(PI * fov / 360.0f));
}

//----------------------------------------
void ofCamera::begin(ofRectangle viewport) {
	ofGetCurrentRenderer()->bind(*this,getViewport(viewport));
}

// if begin(); pushes first, then we need an end to pop
//----------------------------------------
void ofCamera::end() {
	ofGetCurrentRenderer()->unbind(*this);
}

//----------------------------------------
glm::mat4 ofCamera::getProjectionMatrix(ofRectangle viewport) const {
	viewport = getViewport(viewport);
	// autocalculate near/far clip planes if not set by user
	const_cast<ofCamera*>(this)->calcClipPlanes(viewport);

	if(isOrtho) {
		return glm::ortho(
			viewport.x - viewport.width/2,
			viewport.x + viewport.width/2,
			viewport.y - viewport.height/2,
			viewport.y + viewport.height/2,
			nearClip,
			farClip
		);
	}else{
		float aspect = forceAspectRatio ? aspectRatio : viewport.width/viewport.height;
		auto projection = glm::perspective(ofDegToRad(fov), aspect, nearClip, farClip);
		projection = glm::translate(projection, {-lensOffset.x, -lensOffset.y, 0.f});
		return projection;
	}
}

//----------------------------------------
glm::mat4 ofCamera::getModelViewMatrix() const {
	return glm::inverse(getGlobalTransformMatrix());
}

//----------------------------------------
glm::mat4 ofCamera::getModelViewProjectionMatrix(ofRectangle viewport) const {
	viewport = getViewport(viewport);
	return getProjectionMatrix(viewport) * getModelViewMatrix();
}

//----------------------------------------
glm::vec3 ofCamera::worldToScreen(glm::vec3 WorldXYZ, ofRectangle viewport) const {
	viewport = getViewport(viewport);

	auto CameraXYZ4 = getModelViewProjectionMatrix(viewport) * glm::vec4(WorldXYZ, 1.0);
	auto CameraXYZ = CameraXYZ4.xyz() / CameraXYZ4.w;
	glm::vec3 ScreenXYZ;

	ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x;
	ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y;

	ScreenXYZ.z = CameraXYZ.z;

	return ScreenXYZ;

}

//----------------------------------------
glm::vec3 ofCamera::screenToWorld(glm::vec3 ScreenXYZ, ofRectangle viewport) const {
	viewport = getViewport(viewport);

	//convert from screen to camera
	glm::vec4 CameraXYZ;
	CameraXYZ.x = 2.0f * (ScreenXYZ.x - viewport.x) / viewport.width - 1.0f;
	CameraXYZ.y = 1.0f - 2.0f *(ScreenXYZ.y - viewport.y) / viewport.height;
	CameraXYZ.z = ScreenXYZ.z;
	CameraXYZ.w = 1.0;

	//get inverse camera matrix
	auto inverseCamera = glm::inverse(getModelViewProjectionMatrix(viewport));

	//convert camera to world
	auto world = inverseCamera * CameraXYZ;
	return world.xyz() / world.w;

}

//----------------------------------------
glm::vec3 ofCamera::worldToCamera(glm::vec3 WorldXYZ, ofRectangle viewport) const {
	auto camera = getModelViewProjectionMatrix(getViewport(viewport)) * glm::vec4(WorldXYZ, 1.0);
	return camera.xyz() / camera.w;
}

//----------------------------------------
glm::vec3 ofCamera::cameraToWorld(glm::vec3 CameraXYZ, ofRectangle viewport) const {
	auto inverseCamera = glm::inverse(getModelViewProjectionMatrix(getViewport(viewport)));

	auto world = inverseCamera * glm::vec4(CameraXYZ, 1.0);
	return world.xyz() / world.w;
}

//----------------------------------------
void ofCamera::calcClipPlanes(const ofRectangle & viewport) {
	// autocalculate near/far clip planes if not set by user
	if(nearClip == 0 || farClip == 0) {
		float dist = getImagePlaneDistance(getViewport(viewport));
		nearClip = (nearClip == 0) ? dist / 100.0f : nearClip;
		farClip = (farClip == 0) ? dist * 10.0f : farClip;
	}
}

ofRectangle ofCamera::getViewport(const ofRectangle & viewport) const{
	if(viewport.isZero()){
		return getRenderer()->getCurrentViewport();
	}else{
		return viewport;
	}
}

shared_ptr<ofBaseRenderer> ofCamera::getRenderer() const{
	if(!renderer){
		return ofGetCurrentRenderer();
	}else{
		return renderer;
	}
}

void ofCamera::setRenderer(shared_ptr<ofBaseRenderer> _renderer){
	renderer = _renderer;
}
