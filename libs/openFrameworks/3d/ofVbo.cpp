/*
 *  ofVbo.cpp
 *  ofVBO
 *
 *  Created by Todd Vanderlin on 1/12/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

//modified by Keith Pasko

//TODO: GL Error checking

#include "ofVbo.h"

//--------------------------------------------------------------
ofVbo::ofVbo() {
	bUsingVerts		 = false;
	bUsingTexCoords  = false;
	bUsingColors	 = false;
	bUsingNormals	 = false;
	
	vertUsage		 = -1;
	colorUsage		 = -1;
	normUsage		 = -1;
	texUsage		 = -1;
}

//--------------------------------------------------------------
ofVbo::~ofVbo(){
	clear();
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const ofVec3f * verts, int total, int usage) {
	if(verts == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad vertex data!\n");
		return;	
	}
	vertUsage = usage;
	if(!bUsingVerts) {
		bAllocated  = true;
		bUsingVerts = true;
		glGenBuffers(1, &vertId);
	}
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertId);
	vertData = (float*)&verts[0].x;
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, total * sizeof(ofVec3f), vertData, usage);
}

//--------------------------------------------------------------
void ofVbo::setColorData(const ofColor * colors, int total, int usage) {
	if(colors == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad color data!\n");
		return;	
	}
	colorUsage = usage;
	if(!bUsingColors) {
		bUsingColors = true;
		glGenBuffers(1, &colorId);
	}
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, colorId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, total * sizeof(ofColor), &colors[0].r, usage);	
}

//--------------------------------------------------------------
void ofVbo::setIndexData(const GLuint * indices, int total){
	if(indices == NULL){
		ofLog(OF_LOG_WARNING,"ofVbo: bad index data!\n");
		return;
	}
	
	glGenBuffers(1, &indexId);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexId);
	indexData = (GLuint*)indices;
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLuint) * total, indexData, GL_STATIC_DRAW); 
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const ofColor * colors, int total) {
	if(bUsingColors && colorUsage == GL_STREAM_DRAW) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, colorId);
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, total*sizeof(ofColor), &colors[0].r);
	}
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const ofVec3f * verts, int total) {
	if(bUsingVerts && vertUsage == GL_STREAM_DRAW) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertId);
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, total*sizeof(ofVec3f), &verts[0].x);
	}
}

//--------------------------------------------------------------
float* ofVbo::getVertPointer(){
	return vertData;
}

//--------------------------------------------------------------
float* ofVbo::getColorPointer(){
	return colorData;
}

/*
float* ofVbo::getNormalPointer(){
	return normalData;
}
float* ofVbo::getTexCoordPointer(){
	return texCoordData;
}
 */

//--------------------------------------------------------------
GLuint ofVbo::getVertId(){
	return vertId;
}

//--------------------------------------------------------------
GLuint ofVbo::getColorId(){
	return colorId;
}

//--------------------------------------------------------------
GLuint ofVbo::getNormalId(){
	return normalId;
}

//--------------------------------------------------------------
GLuint ofVbo::getTexCoordId(){
	return texCoordId;
}

//--------------------------------------------------------------
void ofVbo::bind(){
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if(bUsingVerts){
		glEnableClientState(GL_VERTEX_ARRAY);		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertId);
		//glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), 0);
		glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), 0);
	}
	//gluErrorString(glGetError());
	
	if(bUsingColors){
		/*
		glBindBuffer(GL_ARRAY_BUFFER, colorId);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, sizeof(ofColor), 0);
		 */
	}
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexId);
	//gluErrorString(glGetError());

}

//--------------------------------------------------------------
void ofVbo::unbind() {
	//if(bUsingVerts)	 glDisableClientState(GL_VERTEX_ARRAY);
	if(bUsingColors) glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	
	glPopClientAttrib();
	glPopAttrib();	
	//glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
}

//--------------------------------------------------------------
void ofVbo::draw(int mode, int first, int total) {
	if(bAllocated){
		bind();
		glDrawArrays(mode, first, total);
		unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::draw(int amt, int drawMode) {
	if(bAllocated){
		bind();
//		GLuint vao;
//		glGenVertexArraysAPPLE(1,&vao);
//		glBindVertexArrayAPPLE(vao);
		glDrawElements(drawMode, amt, GL_UNSIGNED_INT, NULL);
		unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::clear(){
	glDeleteBuffers(1, &vertId);
	glDeleteBuffers(1, &indexId);
	glDeleteBuffers(1, &normalId);
	glDeleteBuffers(1, &colorId);
	glDeleteBuffers(1, &texCoordId);
	
	indexId = 0;
	indexId = 0;
	normalId = 0;
	texCoordId = 0;
	colorId = 0;
}






