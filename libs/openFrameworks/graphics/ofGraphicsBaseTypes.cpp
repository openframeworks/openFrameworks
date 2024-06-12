#include "ofGraphicsBaseTypes.h"
#include "ofRectangle.h"

#define GLM_FORCE_CTOR_INIT
#include "glm/vec2.hpp"
#include "glm/trigonometric.hpp"
#include "of3dGraphics.h"
#include "ofSoundBuffer.h"
#include "ofMesh.h"
void ofBaseDraws::draw(const glm::vec2 & point) const {
	draw(point.x, point.y);
}

void ofBaseDraws::draw(const ofRectangle & rect) const {
	draw(rect.x, rect.y, rect.width, rect.height);
}

void ofBaseDraws::draw(const glm::vec2 & point, float w, float h) const {
	draw(point.x, point.y, w, h);
}

void ofBaseRenderer::rotateDeg(float degrees, float vecX, float vecY, float vecZ){
	rotateRad(glm::radians(degrees), vecX, vecY, vecZ);
}

void ofBaseRenderer::rotateXDeg(float degrees){
	rotateXRad(glm::radians(degrees));
}

void ofBaseRenderer::rotateYDeg(float degrees){
	rotateYRad(glm::radians(degrees));
}

void ofBaseRenderer::rotateZDeg(float degrees){
	rotateZRad(glm::radians(degrees));
}

void ofBaseRenderer::rotateDeg(float degrees){
	rotateRad(glm::radians(degrees));
}

void ofBaseRenderer::draw(const ofMesh &mesh, ofPolyRenderMode renderType) const {
	draw(mesh,renderType,mesh.usingColors(),mesh.usingTextures(),mesh.usingNormals());
}

void ofBaseRenderer::setPlaneResolution( int columns, int rows ){
	get3dGraphics().setPlaneResolution(columns,rows);
}

glm::vec2 ofBaseRenderer::getPlaneResolution() const{
	return get3dGraphics().getPlaneResolution();
}

void ofBaseRenderer::drawPlane(float x, float y, float width, float height) const{
	get3dGraphics().drawPlane(x,y,width,height);
}

void ofBaseRenderer::drawPlane(float x, float y, float z, float width, float height) const{
	get3dGraphics().drawPlane(x,y,z,width,height);
}

void ofBaseRenderer::drawPlane(const glm::vec3& position, float width, float height) const{
	get3dGraphics().drawPlane(position,width,height);
}

void ofBaseRenderer::drawPlane( float width, float height ) const{
	get3dGraphics().drawPlane(width,height);
}

void ofBaseRenderer::setSphereResolution(int res){
	get3dGraphics().setSphereResolution(res);
}

int ofBaseRenderer::getSphereResolution() const{
	return get3dGraphics().getSphereResolution();
}

void ofBaseRenderer::drawSphere(float x, float y, float radius) const{
	get3dGraphics().drawSphere(x,y,radius);
}

void ofBaseRenderer::drawSphere(float x, float y, float z, float radius) const{
	get3dGraphics().drawSphere(x,y,z,radius);
}

void ofBaseRenderer::drawSphere(const glm::vec3& position, float radius) const{
	get3dGraphics().drawSphere(position,radius);
}

void ofBaseRenderer::drawSphere(float radius) const{
	get3dGraphics().drawSphere(radius);
}

void ofBaseRenderer::setIcoSphereResolution( int res ){
	get3dGraphics().setIcoSphereResolution(res);
}

int ofBaseRenderer::getIcoSphereResolution() const{
	return get3dGraphics().getIcoSphereResolution();
}

void ofBaseRenderer::drawIcoSphere(float x, float y, float z, float radius) const{
	get3dGraphics().drawIcoSphere(x,y,z,radius);
}

void ofBaseRenderer::drawIcoSphere(float x, float y, float radius) const{
	get3dGraphics().drawIcoSphere(x,y,radius);
}

void ofBaseRenderer::drawIcoSphere(const glm::vec3& position, float radius) const{
	get3dGraphics().drawIcoSphere(position,radius);
}

void ofBaseRenderer::drawIcoSphere(float radius) const{
	get3dGraphics().drawIcoSphere(radius);
}

void ofBaseRenderer::setCylinderResolution( int radiusSegments, int heightSegments, int capSegments ){
	get3dGraphics().setCylinderResolution(radiusSegments,heightSegments,capSegments);
}

glm::vec3 ofBaseRenderer::getCylinderResolution() const{
	return get3dGraphics().getCylinderResolution();
}

void ofBaseRenderer::drawCylinder(float x, float y, float radius, float height) const{
	get3dGraphics().drawCylinder(x,y,radius,height);
}

void ofBaseRenderer::drawCylinder(float x, float y, float z, float radius, float height) const{
	get3dGraphics().drawCylinder(x,y,z,radius,height);
}

void ofBaseRenderer::drawCylinder(const glm::vec3& position, float radius, float height) const{
	get3dGraphics().drawCylinder(position,radius,height);
}

void ofBaseRenderer::drawCylinder(float radius, float height) const{
	get3dGraphics().drawCylinder(radius,height);
}

void ofBaseRenderer::setConeResolution( int radiusSegments, int heightSegments, int capSegments){
	get3dGraphics().setConeResolution(radiusSegments,heightSegments,capSegments);
}

glm::vec3 ofBaseRenderer::getConeResolution() const{
	return get3dGraphics().getConeResolution();
}

void ofBaseRenderer::drawCone(float x, float y, float z, float radius, float height) const{
	get3dGraphics().drawCone(x,y,z,radius,height);
}

void ofBaseRenderer::drawCone(float x, float y, float radius, float height) const{
	get3dGraphics().drawCone(x,y,radius,height);
}

void ofBaseRenderer::drawCone(const glm::vec3& position, float radius, float height) const{
	get3dGraphics().drawCone(position,radius,height);
}

void ofBaseRenderer::drawCone(float radius, float height) const{
	get3dGraphics().drawCone(radius,height);
}

void ofBaseRenderer::setBoxResolution( int res ){
	get3dGraphics().setBoxResolution(res);
}

void ofBaseRenderer::setBoxResolution( int resWidth, int resHeight, int resDepth ){
	get3dGraphics().setBoxResolution(resWidth,resHeight,resDepth);
}

glm::vec3 ofBaseRenderer::getBoxResolution() const{
	return get3dGraphics().getBoxResolution();
}

void ofBaseRenderer::drawBox( float x, float y, float z, float width, float height, float depth) const{
	get3dGraphics().drawBox(x,y,z,width,height,depth);
}

void ofBaseRenderer::drawBox(float x, float y, float z, float size) const{
	get3dGraphics().drawBox(x,y,z,size);
}

void ofBaseRenderer::drawBox(const glm::vec3& position, float width, float height, float depth) const{
	get3dGraphics().drawBox(position,width,height,depth);
}

void ofBaseRenderer::drawBox(const glm::vec3& position, float size) const{
	get3dGraphics().drawBox(position,size);
}

void ofBaseRenderer::drawBox(float size) const{
	get3dGraphics().drawBox(size);
}

void ofBaseRenderer::drawBox( float width, float height, float depth ) const{
	get3dGraphics().drawBox(width,height,depth);
}

void ofBaseRenderer::drawAxis(float size) const{
	get3dGraphics().drawAxis(size);
}

void ofBaseRenderer::drawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z) const{
	get3dGraphics().drawGrid(stepSize,numberOfSteps,labels,x,y,z);
}

void ofBaseRenderer::drawGridPlane(float stepSize, size_t numberOfSteps, bool labels) const{
	get3dGraphics().drawGridPlane(stepSize,numberOfSteps,labels);
}

void ofBaseRenderer::drawArrow(const glm::vec3& start, const glm::vec3& end, float headSize) const{
	get3dGraphics().drawArrow(start,end,headSize);
}

void ofBaseRenderer::drawRotationAxes(float radius, float stripWidth, int circleRes) const{
	get3dGraphics().drawRotationAxes(radius,stripWidth,circleRes);
}

