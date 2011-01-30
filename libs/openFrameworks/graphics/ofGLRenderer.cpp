/*
 * ofShapeGLRenderers.cpp
 *
 *  Created on: 21/01/2011
 *      Author: arturo
 */
#include "ofGLRenderer.h"
#include "ofShape.h"
#include "ofMesh.h"
#include "ofPath.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofVertexData.h"


void ofGLRenderer::draw(ofVertexData & vertexData){
	glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), vertexData.getVerticesPointer());
	if(vertexData.getNumNormals()){
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, vertexData.getNormalsPointer());
	}
	if(vertexData.getNumColors()){
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4,GL_FLOAT, 0, vertexData.getColorsPointer());
	}
	if(vertexData.getNumIndices()){
		glEnableClientState(GL_INDEX_ARRAY);
		glIndexPointer(GL_UNSIGNED_INT,0,vertexData.getIndexPointer());
	}
	if(vertexData.getNumTexCoords()){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, vertexData.getTexCoordsPointer());
	}

	if(vertexData.getNumIndices()){
		glDrawElements(ofGetGLPrimitiveMode(vertexData.getMode()), vertexData.getNumIndices(),GL_UNSIGNED_INT,vertexData.getIndexPointer());
	}else{
		glDrawArrays(ofGetGLPrimitiveMode(vertexData.getMode()), 0, vertexData.getNumVertices());
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
}

void ofGLRenderer::draw(ofPolyline & poly){
	glVertexPointer(poly.is3D()?3:2, GL_FLOAT, sizeof(ofVec3f), &poly.getVertices()[0].x);
	glDrawArrays(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());;
}

void ofGLRenderer::draw(ofShape & shape){
	ofPushStyle();
	if(shape.isFilled()){
		vector<ofVertexData> & mesh = shape.getTessellation();
		ofSetColor( shape.getFillColor() * ofGetStyle().color);
		glEnableClientState(GL_VERTEX_ARRAY);
		for(int i=0;i<(int)mesh.size();i++){
			draw(mesh[i]);
		}
	}
	if(shape.hasOutline()){
		ofSetColor( shape.getStrokeColor() * ofGetStyle().color);
		vector<ofPolyline> & outlines = shape.getOutline();
		for(int i=0; i<(int)outlines.size(); i++)
			draw(outlines[i]);
	}
	ofPopStyle();
}

void ofGLRenderer::draw(ofPath & path){
	ofShape & shape = path.getShape();
	draw(shape);
}


//----------------------------------------------------------
void ofGLRenderer::pushView() {
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ofRectangle currentViewport;
	currentViewport.set(viewport[0], viewport[1], viewport[2], viewport[3]);
	viewportHistory.push_front(currentViewport);

	if( viewportHistory.size() > OF_MAX_VIEWPORT_HISTORY ){
		viewportHistory.pop_back();
		//should we warn here?
		//ofLog(OF_LOG_WARNING, "ofPushView - warning: you have used ofPushView more than %i times without calling ofPopView - check your code!", OF_MAX_VIEWPORT_HISTORY);
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}


//----------------------------------------------------------
void ofGLRenderer::popView() {
	if( viewportHistory.size() ){
		ofRectangle viewRect = viewportHistory.front();
		ofViewport(viewRect.x, viewRect.y, viewRect.width, viewRect.height);
		viewportHistory.pop_front();
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


//----------------------------------------------------------
void ofGLRenderer::viewport(float x, float y, float width, float height, bool invertY) {

	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();

	if (invertY){
		y = ofGetHeight() - (y + height);
	}

	glViewport(x, y, width, height);
}

//----------------------------------------------------------
ofRectangle ofGLRenderer::getCurrentViewport(){

	// I am using opengl calls here instead of returning viewportRect
	// since someone might use glViewport instead of ofViewport...

	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);
	ofRectangle view;
	view.x = viewport[0];
	view.y = viewport[1];
	view.width = viewport[2];
	view.height = viewport[3];
	return view;

}

//----------------------------------------------------------
int ofGLRenderer::getViewportWidth(){
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);
	return viewport[2];
}

//----------------------------------------------------------
int ofGLRenderer::getViewportHeight(){
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);
	return viewport[3];
}

//----------------------------------------------------------
void ofGLRenderer::setCoordHandedness(ofHandednessType handedness) {
	coordHandedness = handedness;
}

//----------------------------------------------------------
ofHandednessType ofGLRenderer::getCoordHandedness() {
	return coordHandedness;
}

//----------------------------------------------------------
void ofGLRenderer::setupScreenPerspective(float width, float height, int orientation, bool vFlip, float fov, float nearDist, float farDist) {
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();
	if( orientation == 0 ) orientation = ofGetOrientation();

	float w = width;
	float h = height;

	//we do this because ofGetWidth and ofGetHeight return orientated widths and height
	//for the camera we need width and height of the actual screen
	if( orientation == OF_ORIENTATION_90_LEFT || orientation == OF_ORIENTATION_90_RIGHT ){
		h = width;
		w = height;
	}

	float eyeX = w / 2;
	float eyeY = h / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) w / h;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, nearDist, farDist);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0, 0, 1, 0);

	//note - theo checked this on iPhone and Desktop for both vFlip = false and true
	switch(orientation) {
		case OF_ORIENTATION_180:
			glRotatef(-180, 0, 0, 1);
			if(vFlip){
				glScalef(1, -1, 1);
				glTranslatef(-width, 0, 0);
			}else{
				glTranslatef(-width, -height, 0);
			}

			break;

		case OF_ORIENTATION_90_RIGHT:
			glRotatef(-90, 0, 0, 1);
			if(vFlip){
				glScalef(-1, 1, 1);
			}else{
				glScalef(-1, -1, 1);
				glTranslatef(0, -height, 0);
			}
			break;

		case OF_ORIENTATION_90_LEFT:
			glRotatef(90, 0, 0, 1);
			if(vFlip){
				glScalef(-1, 1, 1);
				glTranslatef(-width, -height, 0);
			}else{
				glScalef(-1, -1, 1);
				glTranslatef(-width, 0, 0);
			}
			break;

		case OF_ORIENTATION_DEFAULT:
		default:
			if(vFlip){
				glScalef(1, -1, 1);
				glTranslatef(0, -height, 0);
			}
			break;
	}

}

//----------------------------------------------------------
void ofGLRenderer::setupScreenOrtho(float width, float height, bool vFlip, float nearDist, float farDist) {
	if(width == 0) width = ofGetViewportWidth();
	if(height == 0) height = ofGetViewportHeight();

	#ifndef TARGET_OPENGLES

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		ofSetCoordHandedness(OF_RIGHT_HANDED);

		if(vFlip) {
			glOrtho(0, width, height, 0, nearDist, farDist);
			ofSetCoordHandedness(OF_LEFT_HANDED);
		}
		else glOrtho(0, width, 0, height, nearDist, farDist);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	#else
		//FIX: is here http://stackoverflow.com/questions/2847574/opengl-es-2-0-equivalent-of-glortho
		ofLog(OF_LOG_ERROR, "ofSetupScreenOrtho - you can't use glOrtho with iphone / ES at the moment");
	#endif
}

//----------------------------------------------------------
//Resets openGL parameters back to OF defaults
void ofGLRenderer::setupGraphicDefaults(){

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	ofDisableSmoothing();
	ofEnableAlphaBlending();
	ofBackground(200, 200, 200);
	ofSetColor(255, 255, 255, 255);
}

//----------------------------------------------------------
void ofGLRenderer::setupScreen(){
	ofSetupScreenPerspective();	// assume defaults
}



//our openGL wrappers
//----------------------------------------------------------
void ofGLRenderer::pushMatrix(){
	glPushMatrix();
}

//----------------------------------------------------------
void ofGLRenderer::popMatrix(){
	glPopMatrix();
}

//----------------------------------------------------------
void ofGLRenderer::translate(const ofPoint& p){
	glTranslatef(p.x, p.y, p.z);
}


//----------------------------------------------------------
void ofGLRenderer::translate(float x, float y, float z){
	glTranslatef(x, y, z);
}

//----------------------------------------------------------
void ofGLRenderer::scale(float xAmnt, float yAmnt, float zAmnt){
	glScalef(xAmnt, yAmnt, zAmnt);
}

//----------------------------------------------------------
void ofGLRenderer::rotate(float degrees, float vecX, float vecY, float vecZ){
	glRotatef(degrees, vecX, vecY, vecZ);
}

//----------------------------------------------------------
void ofGLRenderer::rotateX(float degrees){
	glRotatef(degrees, 1, 0, 0);
}

//----------------------------------------------------------
void ofGLRenderer::rotateY(float degrees){
	glRotatef(degrees, 0, 1, 0);
}

//----------------------------------------------------------
void ofGLRenderer::rotateZ(float degrees){
	glRotatef(degrees, 0, 0, 1);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofGLRenderer::rotate(float degrees){
	glRotatef(degrees, 0, 0, 1);
}
