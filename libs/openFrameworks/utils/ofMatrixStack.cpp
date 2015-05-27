/*
 * ofMatrixStack.cpp
 *
 *  Created on: Apr 3, 2013
 *      Author: arturo
 */

#include "ofMatrixStack.h"
#include "ofAppBaseWindow.h"
#include "ofFbo.h"

ofMatrixStack::ofMatrixStack(const ofAppBaseWindow * window)
:vFlipped(true)
,orientation(OF_ORIENTATION_DEFAULT)
,handedness(OF_LEFT_HANDED)
,currentFbo(NULL)
,currentWindow(const_cast<ofAppBaseWindow*>(window))
,currentMatrixMode(OF_MATRIX_MODELVIEW)
,currentMatrix(&modelViewMatrix)
{

}

void ofMatrixStack::setRenderSurface(const ofFbo & fbo){
	currentFbo = const_cast<ofFbo*>(&fbo);
	setOrientation(orientation,vFlipped);
}

void ofMatrixStack::setRenderSurface(const ofAppBaseWindow & window){
	currentWindow = const_cast<ofAppBaseWindow*>(&window);
	currentFbo = NULL;
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

	orientationMatrix.makeIdentityMatrix();

	bool vFlipMatrix = customMatrixNeedsFlip();

	if(vFlipMatrix)
		orientationMatrix.scale(1,-1,1);

	if(!doesHWOrientation()){
		switch(orientation) {
			case OF_ORIENTATION_180:
				orientationMatrix.rotate(180,0,0,1);
				break;

			case OF_ORIENTATION_90_RIGHT:
				orientationMatrix.rotate(90,0,0,1);
				break;

			case OF_ORIENTATION_90_LEFT:
				orientationMatrix.rotate(-90,0,0,1);
				break;

			case OF_ORIENTATION_DEFAULT:
			default:
				break;
		}
	}

	orientationMatrixInverse = orientationMatrix.getInverse();
	orientedProjectionMatrix = projectionMatrix * orientationMatrix;
	modelViewProjectionMatrix = modelViewMatrix * orientedProjectionMatrix;
}

ofOrientation ofMatrixStack::getOrientation() const{
	return orientation;
}

bool ofMatrixStack::isVFlipped() const{
	return vFlipped;
}

bool ofMatrixStack::customMatrixNeedsFlip() const{
	return vFlipped != bool(currentFbo);
}

int ofMatrixStack::getRenderSurfaceWidth() const{
	if(currentFbo){
		return currentFbo->getWidth();
	}else if(currentWindow){
		return currentWindow->getWindowSize().x;
	}else{
		return 0;
	}
}

int ofMatrixStack::getRenderSurfaceHeight() const{
	if(currentFbo){
		return currentFbo->getHeight();
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
	return currentFbo || (currentWindow && currentWindow->doesHWOrientation());
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
	if(currentFbo){
		return ofRectangle(0,0,currentFbo->getWidth(),currentFbo->getHeight());
	}else if(currentWindow){
		return ofRectangle(0,0,currentWindow->getWidth(),currentWindow->getHeight());
	}else{
		return ofRectangle();
	}
}

void ofMatrixStack::nativeViewport(ofRectangle viewport){
	currentViewport=viewport;
}

const ofMatrix4x4 & ofMatrixStack::getViewMatrix() const{
	return viewMatrix;
}

const ofMatrix4x4 & ofMatrixStack::getProjectionMatrix() const{
	return orientedProjectionMatrix;
}

const ofMatrix4x4 & ofMatrixStack::getModelViewMatrix() const{
	return modelViewMatrix;
}

const ofMatrix4x4 & ofMatrixStack::getModelViewProjectionMatrix() const{
	return modelViewProjectionMatrix;
}

const ofMatrix4x4 & ofMatrixStack::getTextureMatrix() const{
	return textureMatrix;
}

const ofMatrix4x4 & ofMatrixStack::getCurrentMatrix() const{
	return *currentMatrix;
}

const ofMatrix4x4 & ofMatrixStack::getProjectionMatrixNoOrientation() const{
	return projectionMatrix;
}

const ofMatrix4x4 & ofMatrixStack::getOrientationMatrix() const{
	return orientationMatrix;
}

const ofMatrix4x4 & ofMatrixStack::getOrientationMatrixInverse() const{
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
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << "extra modelview matrices on the stack, did you forget to pop somewhere?";
	}
	
	tmpCounter = 0;
	while (!projectionMatrixStack.empty()){
		projectionMatrixStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << "extra projection matrices on the stack, did you forget to pop somewhere?";
	}

	tmpCounter = 0;
	while (!textureMatrixStack.empty()){
		textureMatrixStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << "extra texture matrices on the stack, did you forget to pop somewhere?";
	}

	tmpCounter = 0;
	while (!viewportHistory.empty()){
		viewportHistory.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << "extra viewports on the stack, did you forget to popView() somewhere?";
	}

	tmpCounter = 0;
	while (!orientationStack.empty()){
		orientationStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << "extra orientations on the stack, did you forget to popView() somewhere?";
	}

	tmpCounter = 0;
	while (!viewMatrixStack.empty()){
		viewMatrixStack.pop();
		tmpCounter++;
	}
	if (tmpCounter > 0 ){
		ofLogWarning("ofMatrixStack") << "clearStacks(): found " << tmpCounter << "extra view matrices on the stack, did you forget to popView() somewhere?";
	}
}

void ofMatrixStack::translate(float x, float y, float z){
	currentMatrix->glTranslate(x, y, z);
	updatedRelatedMatrices();
}

void ofMatrixStack::scale(float xAmnt, float yAmnt, float zAmnt){
	currentMatrix->glScale(xAmnt, yAmnt, zAmnt);
	updatedRelatedMatrices();
}

void ofMatrixStack::rotate(float degrees, float vecX, float vecY, float vecZ){
	currentMatrix->glRotate(degrees, vecX, vecY, vecZ);
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
	currentMatrix->makeIdentityMatrix();
	updatedRelatedMatrices();
}

void ofMatrixStack::loadMatrix (const float * m){
	currentMatrix->set(m);
	updatedRelatedMatrices();
}

void ofMatrixStack::multMatrix (const float * m){
	currentMatrix->preMult(m);
	updatedRelatedMatrices();
}

void ofMatrixStack::loadViewMatrix(const ofMatrix4x4 & matrix){
	ofMatrixMode lastMatrixMode = currentMatrixMode;
	currentMatrixMode = OF_MATRIX_MODELVIEW;
	viewMatrix = matrix;
	loadMatrix(matrix.getPtr());
	currentMatrixMode = lastMatrixMode;
}

void ofMatrixStack::multViewMatrix(const ofMatrix4x4 & matrix){
	ofMatrixMode lastMatrixMode = currentMatrixMode;
	currentMatrixMode = OF_MATRIX_MODELVIEW;
	viewMatrix.preMult(matrix);
	multMatrix(matrix.getPtr());
	currentMatrixMode = lastMatrixMode;
}


void ofMatrixStack::updatedRelatedMatrices(){
	switch(currentMatrixMode){
	case OF_MATRIX_MODELVIEW:
		modelViewProjectionMatrix = modelViewMatrix * orientedProjectionMatrix;
		break;
	case OF_MATRIX_PROJECTION:
		orientedProjectionMatrix = projectionMatrix * orientationMatrix;
		modelViewProjectionMatrix = modelViewMatrix * orientedProjectionMatrix;
		break;
	default:
		break;
	}
}

bool ofMatrixStack::doesHardwareOrientation() const{
	if(currentFbo){
		return true;
	}else{
		return currentWindow->doesHWOrientation();
	}
}
