#include "ofGraphicsBaseTypes.h"
#include "glm/vec2.hpp"
#include "glm/trigonometric.hpp"
#include "ofRectangle.h"

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
	// Not Implemented
}

void ofBaseRenderer::setPlaneResolution(int columns, int rows) {
	// Not Implemented
}

glm::vec2 ofBaseRenderer::getPlaneResolution() const {
	return glm::vec2();
}

void ofBaseRenderer::drawPlane(float x, float y, float width, float height) const {

}

void ofBaseRenderer::drawPlane(float x, float y, float z, float width, float height) const {

}

void ofBaseRenderer::drawPlane(const glm::vec3 &position, float width, float height) const {

}

void ofBaseRenderer::drawPlane(float width, float height) const {

}

void ofBaseRenderer::setSphereResolution(int res) {

}

int ofBaseRenderer::getSphereResolution() const {
	return 0;
}

void ofBaseRenderer::drawSphere(float x, float y, float radius) const {

}

void ofBaseRenderer::drawSphere(float x, float y, float z, float radius) const {

}

void ofBaseRenderer::drawSphere(const glm::vec3 &position, float radius) const {

}

void ofBaseRenderer::drawSphere(float radius) const {

}

void ofBaseRenderer::setIcoSphereResolution(int res) {

}

int ofBaseRenderer::getIcoSphereResolution() const {
	return 0;
}

void ofBaseRenderer::drawIcoSphere(float x, float y, float z, float radius) const {

}

void ofBaseRenderer::drawIcoSphere(float x, float y, float radius) const {

}

void ofBaseRenderer::drawIcoSphere(const glm::vec3 &position, float radius) const {

}

void ofBaseRenderer::drawIcoSphere(float radius) const {

}

void
ofBaseRenderer::setCylinderResolution(int radiusSegments, int heightSegments, int capSegments) {

}

glm::vec3 ofBaseRenderer::getCylinderResolution() const {
	return glm::vec3();
}

void ofBaseRenderer::drawCylinder(float x, float y, float radius, float height) const {

}

void ofBaseRenderer::drawCylinder(float x, float y, float z, float radius, float height) const {

}

void ofBaseRenderer::drawCylinder(float radius, float height) const {

}

void ofBaseRenderer::drawCylinder(const glm::vec3 &position, float radius, float height) const {

}

void ofBaseRenderer::setConeResolution(int radiusSegments, int heightSegments, int capSegments) {

}

glm::vec3 ofBaseRenderer::getConeResolution() const {
	return glm::vec3();
}

void ofBaseRenderer::drawCone(float x, float y, float z, float radius, float height) const {

}

void ofBaseRenderer::drawCone(float x, float y, float radius, float height) const {

}

void ofBaseRenderer::drawCone(const glm::vec3 &position, float radius, float height) const {

}

void ofBaseRenderer::drawCone(float radius, float height) const {

}

void ofBaseRenderer::setBoxResolution(int res) {

}

void ofBaseRenderer::setBoxResolution(int resWidth, int resHeight, int resDepth) {

}

glm::vec3 ofBaseRenderer::getBoxResolution() const {
	return glm::vec3();
}

void
ofBaseRenderer::drawBox(float x, float y, float z, float width, float height, float depth) const {

}

void ofBaseRenderer::drawBox(float x, float y, float z, float size) const {

}

void
ofBaseRenderer::drawBox(const glm::vec3 &position, float width, float height, float depth) const {

}

void ofBaseRenderer::drawBox(const glm::vec3 &position, float size) const {

}

void ofBaseRenderer::drawBox(float size) const {

}

void ofBaseRenderer::drawBox(float width, float height, float depth) const {

}

void ofBaseRenderer::drawAxis(float size) const {

}

void ofBaseRenderer::drawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y,
							  bool z) const {

}

void ofBaseRenderer::drawGridPlane(float stepSize, size_t numberOfSteps, bool labels) const {

}

void ofBaseRenderer::drawArrow(const glm::vec3 &start, const glm::vec3 &end, float headSize) const {

}

void ofBaseRenderer::drawRotationAxes(float radius, float stripWidth, int circleRes) const {

}
