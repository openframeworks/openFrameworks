/*
 * ofShapeGLRenderers.cpp
 *
 *  Created on: 21/01/2011
 *      Author: arturo
 */
#include "ofShapeGLRenderers.h"
#include "ofShape.h"
#include "ofMesh.h"
#include "ofPath.h"


void ofVARenderer::draw(ofPolyline & poly){
	cout << poly.size() << (poly.is3D()?"3D":"2D") << endl;
	glVertexPointer(poly.is3D()?3:2, GL_FLOAT, sizeof(ofVec3f), &poly.getVertices()[0].x);
	glDrawArrays(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());;
}

void ofVARenderer::draw(ofShape & shape){
	ofPushStyle();
	ofMesh & mesh = shape.getMesh();
	ofSetColor( shape.getFillColor() * ofGetStyle().color);
	glEnableClientState(GL_VERTEX_ARRAY);
	for(int i=0;i<(int)mesh.elements.size();i++){
		ofPrimitiveMode mode = mesh.elements[i].getMode();
		GLuint currentTriType = ofGetGLPrimitiveMode(mode);
		glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &mesh.elements[i].getVerticesPointer()->x);
		glDrawArrays(currentTriType, 0, mesh.elements[i].getNumVertices());
	}
	if(shape.hasOutline()){
		ofSetColor( shape.getStrokeColor() * ofGetStyle().color);
		draw(shape.getOutline());
	}
	ofPopStyle();
}

void ofVARenderer::draw(ofPath & path){
	ofShape & shape = path.getShape();
	draw(shape);
}


//----------------------------------------------------------
void ofVARenderer::pushView() {
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
void ofVARenderer::popView() {
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
void ofVARenderer::viewport(float x, float y, float width, float height, bool invertY) {

	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();

	if (invertY){
		y = ofGetHeight() - (y + height);
	}

	glViewport(x, y, width, height);
}

//----------------------------------------------------------
ofRectangle ofVARenderer::getCurrentViewport(){

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
int ofVARenderer::getViewportWidth(){
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);
	return viewport[2];
}

//----------------------------------------------------------
int ofVARenderer::getViewportHeight(){
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);
	return viewport[3];
}

//----------------------------------------------------------
void ofVARenderer::setCoordHandedness(ofHandednessType handedness) {
	coordHandedness = handedness;
}

//----------------------------------------------------------
ofHandednessType ofVARenderer::getCoordHandedness() {
	return coordHandedness;
}

//----------------------------------------------------------
void ofVARenderer::setupScreenPerspective(float width, float height, bool vFlip, float fov, float nearDist, float farDist) {
	if(width == 0) width = ofGetViewportWidth();
	if(height == 0) height = ofGetViewportHeight();

	float eyeX = width / 2;
	float eyeY = height / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) width / height;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, nearDist, farDist);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0, 0, 1, 0);

	ofSetCoordHandedness(OF_RIGHT_HANDED);

	if(vFlip) {
		glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
		glTranslatef(0, -height, 0);       // shift origin up to upper-left corner.
		ofSetCoordHandedness(OF_LEFT_HANDED);
	}
}

//----------------------------------------------------------
void ofVARenderer::setupScreenOrtho(float width, float height, bool vFlip, float nearDist, float farDist) {
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
void ofVARenderer::setupGraphicDefaults(){

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
void ofVARenderer::setupScreen(){
	ofSetupScreenPerspective();	// assume defaults
}



//our openGL wrappers
//----------------------------------------------------------
void ofVARenderer::pushMatrix(){
	glPushMatrix();
}

//----------------------------------------------------------
void ofVARenderer::popMatrix(){
	glPopMatrix();
}

//----------------------------------------------------------
void ofVARenderer::translate(const ofPoint& p){
	glTranslatef(p.x, p.y, p.z);
}


//----------------------------------------------------------
void ofVARenderer::translate(float x, float y, float z){
	glTranslatef(x, y, z);
}

//----------------------------------------------------------
void ofVARenderer::scale(float xAmnt, float yAmnt, float zAmnt){
	glScalef(xAmnt, yAmnt, zAmnt);
}

//----------------------------------------------------------
void ofVARenderer::rotate(float degrees, float vecX, float vecY, float vecZ){
	glRotatef(degrees, vecX, vecY, vecZ);
}

//----------------------------------------------------------
void ofVARenderer::rotateX(float degrees){
	glRotatef(degrees, 1, 0, 0);
}

//----------------------------------------------------------
void ofVARenderer::rotateY(float degrees){
	glRotatef(degrees, 0, 1, 0);
}

//----------------------------------------------------------
void ofVARenderer::rotateZ(float degrees){
	glRotatef(degrees, 0, 0, 1);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofVARenderer::rotate(float degrees){
	glRotatef(degrees, 0, 0, 1);
}


















void ofVboRenderer::draw(ofShape & shape){
	ofMesh & mesh = shape.getMesh();

	ofSetColor( shape.getFillColor() );
	vbos.resize(mesh.elements.size());
	for(int i=0; i<(int)mesh.elements.size(); i++){
		vbos[i].setDrawType(GL_STATIC_DRAW_ARB);
		vbos[i].setUseIndices(false);
		vbos[i].setVertexData(&mesh.elements[i]);
		if(shape.isFilled())
			vbos[i].drawFaces();
		else
			vbos[i].drawWireframe();
	}
	if(shape.hasOutline()){
		if(shape.getOutline().hasChanged()){
			vboOutline.setVertexData(&shape.getOutline().getVertices()[0],shape.getOutline().size(),GL_STATIC_DRAW_ARB);
		}
		ofSetColor( shape.getStrokeColor() );
		vboOutline.draw(GL_LINE_LOOP,0,shape.getOutline().size());
	}
}

void ofVboRenderer::draw(ofPath & path){
	ofShape & shape = path.getShape();
	draw(shape);
}
