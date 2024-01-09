/*
 * ofMatrixStack.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: arturo
 */

#include "ofMatrixStack.h"
#include "ofAppBaseWindow.h"
#include "ofGraphicsBaseTypes.h"
#include "ofLog.h"

#define GLM_FORCE_CTOR_INIT
#include "glm/mat4x4.hpp"
#include "glm/gtx/transform.hpp"

using std::swap;
using std::make_pair;
using std::pair;

ofMatrixStack::ofMatrixStack(const ofAppBaseWindow * window)
:vFlipped(true)
,orientation(OF_ORIENTATION_DEFAULT)
,handedness(OF_LEFT_HANDED)
,currentRenderSurface(nullptr)
,currentWindow(const_cast<ofAppBaseWindow*>(window))
,currentMatrixMode(OF_MATRIX_MODELVIEW)
,currentMatrix(&modelViewMatrix)
,flipRenderSurfaceMatrix(true)
,modelMatrix(1)
,viewMatrix(1)
,viewInverse(1)
,modelViewMatrix(1)
,projectionMatrix(1)
,textureMatrix(1)
,modelViewProjectionMatrix(1)
,orientedProjectionMatrix(1)
,orientationMatrix(1)
,orientationMatrixInverse(1)
{

}

void ofMatrixStack::setRenderSurface(const ofBaseDraws & renderSurface_){
	currentRenderSurface = const_cast<ofBaseDraws*>(&renderSurface_);
	flipRenderSurfaceMatrix = true;
	setOrientation(orientation,vFlipped);
}

void ofMatrixStack::setRenderSurfaceNoMatrixFlip(const ofBaseDraws & renderSurface_) {
	currentRenderSurface = const_cast<ofBaseDraws*>(&renderSurface_);
	flipRenderSurfaceMatrix = false;
	setOrientation(orientation, vFlipped);
}

void ofMatrixStack::setRenderSurface(const ofAppBaseWindow & window){
	currentWindow = const_cast<ofAppBaseWindow*>(&window);
	currentRenderSurface = nullptr;
	setOrientation(orientation,vFlipped);
}

void ofMatrixStack::setOrientation(ofOrientation _orientation, bool vFlip){
	vFlipped = vFlip;
	orientation = _orientation;

	if(vFlip){
		handedness = OF_LEFT_HANDED;
	}else{
		handedness = OF_RIGHT_HANDED;
	}

	orientationMatrix = glm::mat4(1.0);

	bool vFlipMatrix = customMatrixNeedsFlip();

	if(vFlipMatrix)
		orientationMatrix = glm::scale(orientationMatrix, glm::vec3(1.f,-1.f,1.f));

	if(!doesHWOrientation()){
		switch(orientation) {
			case OF_ORIENTATION_180:
				orientationMatrix = glm::rotate(orientationMatrix, glm::pi<float>(), glm::vec3{0.f, 0.f, 1.f});
				break;

			case OF_ORIENTATION_90_RIGHT:
				orientationMatrix = glm::rotate(orientationMatrix, glm::half_pi<float>(), glm::vec3{0.f, 0.f, 1.f});
				break;

			case OF_ORIENTATION_90_LEFT:
				orientationMatrix = glm::rotate(orientationMatrix, -glm::half_pi<float>(), glm::vec3{0.f, 0.f, 1.f});
				break;

			case OF_ORIENTATION_DEFAULT:
			default:
				break;
		}
	}

	orientationMatrixInverse = glm::inverse(orientationMatrix);
	orientedProjectionMatrix = orientationMatrix * projectionMatrix;
	modelViewProjectionMatrix = orientedProjectionMatrix * modelViewMatrix;
}

ofOrientation ofMatrixStack::getOrientation() const{
	return orientation;
}

bool ofMatrixStack::isVFlipped() const{
	return vFlipped;
}

bool ofMatrixStack::customMatrixNeedsFlip() const{
	return vFlipped != (bool(currentRenderSurface) && flipRenderSurfaceMatrix);
}

int ofMatrixStack::getRenderSurfaceWidth() const{
	if(currentRenderSurface){
		return currentRenderSurface->getWidth();
	}else if(currentWindow){
		return currentWindow->getWindowSize().x;
	}else{
		return 0;
	}
}

int ofMatrixStack::getRenderSurfaceHeight() const{
	if(currentRenderSurface){
		return currentRenderSurface->getHeight();
	}else if(currentWindow){
		return currentWindow->getWindowSize().y;
	}else{
		return 0;
	}
}

ofMatrixMode ofMatrixStack::getCurrentMatrixMode() const{
	return currentMatrixMode;
}

ofHandednessType ofMatrixStack::getHandedness() const{
	return handedness;
}


bool ofMatrixStack::doesHWOrientation() const{
	return currentRenderSurface || (currentWindow && currentWindow->doesHWOrientation());
}

void ofMatrixStack::viewport(float x, float y, float width, float height, bool vflip){
	if(!doesHWOrientation() && (orientation==OF_ORIENTATION_90_LEFT || orientation==OF_ORIENTATION_90_RIGHT)){
		swap(width,height);
		swap(x,y);
	}

	if(width < 0 || height < 0){
		width = getRenderSurfaceWidth();
		height = getRenderSurfaceHeight();
		vflip = isVFlipped();
	}

	if (vflip){
		y = getRenderSurfaceHeight() - (y + height);
	}

	currentViewport.set(x,y,width,height);
}

ofRectangle ofMatrixStack::getCurrentViewport() const{
	ofRectangle tmpCurrentViewport = currentViewport;
	if (isVFlipped()){
		tmpCurrentViewport.y = getRenderSurfaceHeight() - (tmpCurrentViewport.y + tmpCurrentViewport.height);
	}

	if(!doesHWOrientation() && (orientation==OF_ORIENTATION_90_LEFT || orientation==OF_ORIENTATION_90_RIGHT)){
		swap(tmpCurrentViewport.width,tmpCurrentViewport.height);
		swap(tmpCurrentViewport.x,tmpCurrentViewport.y);
	}
	return tmpCurrentViewport;
}

ofRectangle ofMatrixStack::getNativeViewport() const{
	return currentViewport;
}

ofRectangle ofMatrixStack::getFullSurfaceViewport() const{
	if(currentRenderSurface){
		return ofRectangle(0,0,currentRenderSurface->getWidth(),currentRenderSurface->getHeight());
	}else if(currentWindow){
		return ofRectangle(0,0,currentWindow->getWidth(),currentWindow->getHeight());
	}else{
		return ofRectangle();
	}
}

void ofMatrixStack::nativeViewport(ofRectangle viewport){
	currentViewport=viewport;
}

const glm::mat4 & ofMatrixStack::getModelMatrix() const{
	return modelMatrix;
}

const glm::mat4 & ofMatrixStack::getViewMatrix() const{
	return viewMatrix;
}

const glm::mat4 & ofMatrixStack::getViewInverse() const{
	return viewInverse;
}

const glm::mat4 & ofMatrixStack::getProjectionMatrix() const{
	return orientedProjectionMatrix;
}

const glm::mat4 & ofMatrixStack::getModelViewMatrix() const{
	return modelViewMatrix;
}

const glm::mat4 & ofMatrixStack::getModelViewProjectionMatrix() const{
	return modelViewProjectionMatrix;
}

const glm::mat4 & ofMatrixStack::getTextureMatrix() const{
	return textureMatrix;
}

const glm::mat4 & ofMatrixStack::getCurrentMatrix() const{
	return *currentMatrix;
}

const glm::mat4 & ofMatrixStack::getProjectionMatrixNoOrientation() const{
	return projectionMatrix;
}

const glm::mat4 & ofMatrixStack::getOrientationMatrix() const{
	return orientationMatrix;
}

const glm::mat4 & ofMatrixStack::getOrientationMatrixInverse() const{
	return orientationMatrixInverse;
}

void ofMatrixStack::pushView(){
	viewportHistory.push(currentViewport);

	ofMatrixMode currentMode = currentMatrixMode;

	matrixMode(OF_MATRIX_PROJECTION);
	pushMatrix();

	matrixMode(OF_MATRIX_MODELVIEW);
	pushMatrix();

	matrixMode(currentMode);

	viewMatrixStack.push(viewMatrix);

	orientationStack.push(make_pair(orientation,vFlipped));
}

void ofMatrixStack::popView(){
	if(!viewMatrixStack.empty()){
		viewMatrix = viewMatrixStack.top();
		viewInverse = glm::inverse(viewMatrix);
		viewMatrixStack.pop();
	}

	if(!orientationStack.empty()){
		pair<ofOrientation,bool> orientationFlip = orientationStack.top();
		setOrientation(orientationFlip.first,orientationFlip.second);
		orientationStack.pop();
	}

	if( viewportHistory.size() ){
		currentViewport = viewportHistory.top();
		viewportHistory.pop();
	}

	ofMatrixMode currentMode = currentMatrixMode;

	matrixMode(OF_MATRIX_PROJECTION);
	popMatrix();

	matrixMode(OF_MATRIX_MODELVIEW);
	popMatrix();

	matrixMode(currentMode);
}

void ofMatrixStack::pushMatrix(){
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelViewMatrixStack.push(modelViewMatrix);
		break;
	case OF_MATRIX_PROJECTION:
		projectionMatrixStack.push(projectionMatrix);
		break;
	case OF_MATRIX_TEXTURE:
		textureMatrixStack.push(textureMatrix);
		break;
	}
}

void ofMatrixStack::popMatrix(){
	if (currentMatrixMode == OF_MATRIX_MODELVIEW && !modelViewMatrixStack.empty()){
		modelViewMatrix = modelViewMatrixStack.top();
		modelViewMatrixStack.pop();
		modelMatrix = viewInverse * modelViewMatrix;
	} else if (currentMatrixMode == OF_MATRIX_PROJECTION && !projectionMatrixStack.empty()){
		projectionMatrix = projectionMatrixStack.top();
		projectionMatrixStack.pop();
	} else if (currentMatrixMode == OF_MATRIX_TEXTURE && !textureMatrixStack.empty()){
		textureMatrix = textureMatrixStack.top();
		textureMatrixStack.pop();
	} else {
		ofLogWarning("ofMatrixStack") << "popMatrix(): empty matrix stack, cannot pop any further";
	}
	updatedRelatedMatrices();
}

void ofMatrixStack::clearStacks(){
	int tmpCounter = 0;
	while (!modelViewMatrixStack.empty()){
		modelViewMatrixStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << " extra modelview matrices on the stack, did you forget to pop somewhere?";
	}
	
	tmpCounter = 0;
	while (!projectionMatrixStack.empty()){
		projectionMatrixStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << " extra projection matrices on the stack, did you forget to pop somewhere?";
	}

	tmpCounter = 0;
	while (!textureMatrixStack.empty()){
		textureMatrixStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << " extra texture matrices on the stack, did you forget to pop somewhere?";
	}

	tmpCounter = 0;
	while (!viewportHistory.empty()){
		viewportHistory.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << " extra viewports on the stack, did you forget to popView() somewhere?";
	}

	tmpCounter = 0;
	while (!orientationStack.empty()){
		orientationStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << " extra orientations on the stack, did you forget to popView() somewhere?";
	}

	tmpCounter = 0;
	while (!viewMatrixStack.empty()){
		viewMatrixStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << " extra view matrices on the stack, did you forget to popView() somewhere?";
	}
}

void ofMatrixStack::translate(float x, float y, float z){
	*currentMatrix = glm::translate(*currentMatrix, glm::vec3(x, y, z));
	updatedRelatedMatrices();
}

void ofMatrixStack::scale(float xAmnt, float yAmnt, float zAmnt){
	*currentMatrix = glm::scale(*currentMatrix, glm::vec3(xAmnt, yAmnt, zAmnt));
	updatedRelatedMatrices();
}

void ofMatrixStack::rotateRad(float radians, float vecX, float vecY, float vecZ){
	*currentMatrix = glm::rotate(*currentMatrix, radians, glm::vec3(vecX, vecY, vecZ));
	updatedRelatedMatrices();
}

void ofMatrixStack::matrixMode(ofMatrixMode mode){
	currentMatrixMode = mode;
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		currentMatrix = &modelViewMatrix;
		break;
	case OF_MATRIX_PROJECTION:
		currentMatrix = &projectionMatrix;
		break;
	case OF_MATRIX_TEXTURE:
		currentMatrix = &textureMatrix;
		break;
	}
}

void ofMatrixStack::loadIdentityMatrix (void){
	*currentMatrix = glm::mat4(1.0);
	updatedRelatedMatrices();
}

void ofMatrixStack::loadMatrix (const glm::mat4 & m){
	*currentMatrix = glm::mat4(m);
	updatedRelatedMatrices();
}

void ofMatrixStack::multMatrix (const glm::mat4 & m){
	*currentMatrix = *currentMatrix * m;
	updatedRelatedMatrices();
}

void ofMatrixStack::loadViewMatrix(const glm::mat4 & matrix){
	auto lastMatrixMode = currentMatrixMode;
	currentMatrixMode = OF_MATRIX_MODELVIEW;
	viewMatrix = matrix;
	viewInverse = glm::inverse(viewMatrix);
	loadMatrix(matrix);
	currentMatrixMode = lastMatrixMode;
}

void ofMatrixStack::multViewMatrix(const glm::mat4 & matrix){
	ofMatrixMode lastMatrixMode = currentMatrixMode;
	currentMatrixMode = OF_MATRIX_MODELVIEW;
	viewMatrix = viewMatrix * matrix;
	viewInverse = glm::inverse(viewMatrix);
	multMatrix(matrix);
	currentMatrixMode = lastMatrixMode;
}


void ofMatrixStack::updatedRelatedMatrices(){
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelViewProjectionMatrix = orientedProjectionMatrix * modelViewMatrix;
		modelMatrix = viewInverse * modelViewMatrix;
		break;
	case OF_MATRIX_PROJECTION:
		orientedProjectionMatrix = orientationMatrix * projectionMatrix;
		modelViewProjectionMatrix = orientedProjectionMatrix * modelViewMatrix;
		break;
	default:
		break;
	}
}

bool ofMatrixStack::doesHardwareOrientation() const{
	if(currentRenderSurface){
		return true;
	}else{
		return currentWindow->doesHWOrientation();
	}
}
