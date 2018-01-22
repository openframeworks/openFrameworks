#include "ofCamera.h"
#include "ofLog.h"
#include "ofRectangle.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofGraphicsBaseTypes.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"

using namespace std;

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
float ofCamera::getImagePlaneDistance(const ofRectangle & viewport) const {
	return viewport.height / (2.0f * tanf(PI * fov / 360.0f));
}

//----------------------------------------
void ofCamera::begin(const ofRectangle & viewport) {
	ofGetCurrentRenderer()->bind(*this,viewport);
}

// if begin(); pushes first, then we need an end to pop
//----------------------------------------
void ofCamera::end() {
	ofGetCurrentRenderer()->unbind(*this);
}

//----------------------------------------
glm::mat4 ofCamera::getProjectionMatrix(const ofRectangle & viewport) const {
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
		auto projection = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
		projection = glm::translate(glm::mat4(), {-lensOffset.x, -lensOffset.y, 0.f}) * projection;
		return projection;
	}
}

//----------------------------------------
glm::mat4 ofCamera::getModelViewMatrix() const {
	return glm::inverse(getGlobalTransformMatrix());
}

//----------------------------------------
glm::mat4 ofCamera::getModelViewProjectionMatrix(const ofRectangle & viewport) const {
	return getProjectionMatrix(viewport) * getModelViewMatrix();
}

//----------------------------------------
glm::vec3 ofCamera::worldToScreen(glm::vec3 WorldXYZ, const ofRectangle & viewport) const {
	auto CameraXYZ = worldToCamera(WorldXYZ, viewport);
	
	glm::vec3 ScreenXYZ;
	ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x;
	ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y;
	ScreenXYZ.z = CameraXYZ.z;

	return ScreenXYZ;
}

//----------------------------------------
glm::vec3 ofCamera::screenToWorld(glm::vec3 ScreenXYZ, const ofRectangle & viewport) const {
	//convert from screen to camera
	glm::vec3 CameraXYZ;
	CameraXYZ.x = 2.0f * (ScreenXYZ.x - viewport.x) / viewport.width - 1.0f;
	CameraXYZ.y = 1.0f - 2.0f *(ScreenXYZ.y - viewport.y) / viewport.height;
	CameraXYZ.z = ScreenXYZ.z;

	return cameraToWorld(CameraXYZ, viewport);
}

//----------------------------------------
glm::vec3 ofCamera::worldToCamera(glm::vec3 WorldXYZ, const ofRectangle & viewport) const {
	auto MVPmatrix = getModelViewProjectionMatrix(viewport);
	if(vFlip){
		MVPmatrix = glm::scale(glm::mat4(1.0), glm::vec3(1.f,-1.f,1.f)) * MVPmatrix;
	}
	auto camera = MVPmatrix * glm::vec4(WorldXYZ, 1.0);
	return camera.xyz() / camera.w;
	
}

//----------------------------------------
glm::vec3 ofCamera::cameraToWorld(glm::vec3 CameraXYZ, const ofRectangle & viewport) const {
	auto MVPmatrix = getModelViewProjectionMatrix(viewport);
	if(vFlip){
		MVPmatrix = glm::scale(glm::mat4(1.0), glm::vec3(1.f,-1.f,1.f)) * MVPmatrix;
	}
	auto world = glm::inverse(MVPmatrix) * glm::vec4(CameraXYZ, 1.0);
	return world.xyz() / world.w;
}

//----------------------------------------
void ofCamera::calcClipPlanes(const ofRectangle & viewport) {
	// autocalculate near/far clip planes if not set by user
	if(nearClip == 0 || farClip == 0) {
		float dist = getImagePlaneDistance(viewport);
		nearClip = (nearClip == 0) ? dist / 100.0f : nearClip;
		farClip = (farClip == 0) ? dist * 10.0f : farClip;
	}
}

ofRectangle ofCamera::getViewport() const{
	return getRenderer()->getCurrentViewport();
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

void ofCamera::drawFrustum(const ofRectangle & viewport) const {
	//FIXME: this is not using the viewport?
	ofPushMatrix(); // we assume we are currently in world space.
	ofMultMatrix( getGlobalTransformMatrix() ); 
	// Move origin to camera origin == global transform of camera == inverse (view matrix)
	// Which brings us into view space


	// We want to draw the frustum of camera 0. To this end, we grab the matrix which transforms
	// from view space into clip space (i.e. the projection matrix),
	// then we take our unit clip cube (i.e. the cube that delimits clip space,
	// and is defined to be +-1 onto each x, y , z),and transform this clip cube back 
	// into view space. We transform it back into
	// viewspace by applying the inverse transform viewspace -> clipspace
	// which is the inverse of applying the projection matrix, which is applying
	// the inverse projection matrix.

	// the edges of our unit cube in clip space:

	glm::vec3 clipCube[8] = {
		{ -1,-1,-1 }, { -1, 1,-1 }, { 1,-1,-1 }, { 1, 1,-1 },
		{ -1,-1, 1 }, { -1, 1, 1 }, { 1,-1, 1 }, { 1, 1, 1 },
	};
	// since the clip cube is expressed in clip (=projection) space, we want this 
	// transformed back into our current space, view space, i.e. apply the inverse 
	// projection matrix to it.


	// calculate projection matrix using frustum: 

	glm::mat4x4 projectionMatrixInverse = glm::inverse( getProjectionMatrix(viewport) );

	for ( int i = 0; i < 8; i++ ) {
		clipCube[i] = projectionMatrixInverse * glm::vec4(clipCube[i], 1.f);
	}


	// now draw our clip cube side edge rays - note that since the coordinates are
	// now in world space, we can draw them without applying any additional trans-
	// formations.
	for ( int i = 0; i < 4; i++ ) {
		ofDrawLine( clipCube[i], clipCube[i + 4] );
	}

	ofPushStyle();
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofNoFill();
	//// draw the clip cube cap
	ofDrawRectangle( clipCube[0], clipCube[3].x - clipCube[0].x, clipCube[3].y - clipCube[0].y );
	ofDrawRectangle( clipCube[4], clipCube[7].x - clipCube[4].x, clipCube[7].y - clipCube[4].y );
	ofPopStyle();

	ofPopMatrix();
}
