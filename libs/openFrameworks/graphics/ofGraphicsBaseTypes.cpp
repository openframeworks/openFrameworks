#include "ofConstants.h"
#include "ofGraphicsBaseTypes.h"
#include "glm/vec2.hpp"
#include "glm/trigonometric.hpp"
#include "ofRectangle.h"

ofStyle::ofStyle(){
	bFill				= true;
	blendingMode		= OF_BLENDMODE_ALPHA;
	smoothing			= false;
	circleResolution	= 20;
	sphereResolution	= 20;
	curveResolution		= 20;
	lineWidth			= 1.0;
	polyMode			= OF_POLY_WINDING_ODD;
	rectMode			= OF_RECTMODE_CORNER;
	#ifdef TARGET_OPENGLES
		drawBitmapMode		= OF_BITMAPMODE_SIMPLE;
	#else
		drawBitmapMode		= OF_BITMAPMODE_MODEL_BILLBOARD;
	#endif
	bgColor.set(60, 60, 60);
	//depthTest = false;
}



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
