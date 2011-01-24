//TODO:
//GL Error checking
//inlining?
// handle idling of arrays: possibly let gl create memory and use map buffers for streaming
// how are we going to handle the OF_VBO_STATIC vs OF_VBO_STREAM settings? Should it auto-set in update intead of check?
// index updating/deleting?

#include "ofVbo.h"



//--------------------------------------------------------------
ofVbo::ofVbo() {
	bUsingVerts = false;
	bUsingTexCoords = false;
	bUsingColors = false;
	bUsingNormals = false;
	bUsingIndices = false;
	
	vertUsage = -1;
	colorUsage = -1;
	normUsage = -1;
	texUsage = -1;
	
	bAllocated = false;
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
void ofVbo::setNormalData(const ofVec3f * normals, int total, int usage) {
	if(normals == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad normal data!\n");
		return;	
	}
	normUsage = usage;
	if(!bUsingNormals) {
		bUsingNormals = true;
		glGenBuffers(1, &normalId);
	}
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, total * sizeof(ofVec3f), &normals[0].x, usage);	
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const ofVec2f * texCoords, int total, int usage) {
	if(texCoords == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad texCoord data!\n");
		return;	
	}
	texUsage = usage;
	if(!bUsingTexCoords) {
		bUsingTexCoords = true;
		glGenBuffers(1, &texCoordId);
	}
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, texCoordId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, total * sizeof(ofVec2f), &texCoords[0].x, usage);	
}


//--------------------------------------------------------------
void ofVbo::setIndexData(const GLuint * indices, int total, int usage){
	if(indices == NULL){
		ofLog(OF_LOG_WARNING,"ofVbo: bad index data!\n");
		return;
	}
	
	if(!bUsingIndices){
		bUsingIndices = true;
		glGenBuffers(1, &indexId);	
	}
	
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexId);
	indexData = (GLuint*)indices;
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLuint) * total, indexData, usage); 
}

/*
//--------------------------------------------------------------
void ofVbo::updateVertexData(const ofVec3f * verts, int total) {
	if(bUsingVerts){
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertId);
		vertData = (float*)&verts[0].x;
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, total*sizeof(ofVec3f), vertData);
	}
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const ofColor * colors, int total) {
	if(bUsingColors) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, colorId);
		colorData = (float*)&colors[0].r;
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, total*sizeof(ofColor), colorData);
	}
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const ofVec3f * normals, int total) {
	if(bUsingNormals) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalId);
		normalData = (float*)&normals[0].x;
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, total*sizeof(ofVec3f), normalData);
	}
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const ofVec2f * texCoords, int total) {
	if(bUsingTexCoords) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, texCoordId);
		texCoordData = (float*)&texCoords[0].x;
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, total*sizeof(ofVec2f), texCoordData);
	}
}

//--------------------------------------------------------------
void ofVbo::updateIndexData(const GLuint * indices, int total) {
	if(bUsingIndices) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, indexId);
		indexData = (GLuint*)&indices[0];
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, total*sizeof(GLuint), indexData);
	}
}
*/

//--------------------------------------------------------------
bool ofVbo::getIsAllocated(){
	return bAllocated;
}	

//--------------------------------------------------------------
bool ofVbo::getUsingVerts(){
	return bUsingVerts;
}	

//--------------------------------------------------------------
bool ofVbo::getUsingColors(){
	return bUsingColors;
}	

//--------------------------------------------------------------
bool ofVbo::getUsingNormals(){
	return bUsingNormals;
}

//--------------------------------------------------------------
bool ofVbo::getUsingTexCoords(){
	return bUsingTexCoords;
}

//--------------------------------------------------------------
bool ofVbo::getUsingIndices(){
	return bUsingIndices;
}

//--------------------------------------------------------------
float* ofVbo::getVertPointer(){
	return vertData;
}

//--------------------------------------------------------------
float* ofVbo::getColorPointer(){
	return colorData;
}

//--------------------------------------------------------------
float* ofVbo::getNormalPointer(){
	return normalData;
}

//--------------------------------------------------------------
float* ofVbo::getTexCoordPointer(){
	return texCoordData;
}

//--------------------------------------------------------------
GLuint* ofVbo::getIndexPointer(){
	return indexData;
}

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
GLuint ofVbo::getIndexId(){
	return indexId;
}

//--------------------------------------------------------------
void ofVbo::bind(){
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	
	if(bUsingVerts){
		glEnableClientState(GL_VERTEX_ARRAY);		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertId);
		glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), 0);
	}
	
	if(bUsingColors){
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER_ARB, colorId);
		glColorPointer(4, GL_FLOAT, sizeof(ofColor), 0);
	}
	
	if(bUsingNormals){
		glEnableClientState(GL_NORMAL_ARRAY);		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, normalId);
		glNormalPointer(GL_FLOAT, sizeof(ofVec3f), 0);
	}
	
	if(bUsingTexCoords){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, texCoordId);
		glTexCoordPointer(2, GL_FLOAT, sizeof(ofVec2f), 0);
	}
}

//--------------------------------------------------------------
void ofVbo::unbind() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glPopClientAttrib();
	glPopAttrib();	
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}

//--------------------------------------------------------------
void ofVbo::draw(int drawMode, int first, int total) {
	if(bAllocated){
		bind();
		glDrawArrays(drawMode, first, total);
		unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::drawElements(int drawMode, int amt) {
	if(bAllocated){
		bind();
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indexId);
		glDrawElements(drawMode, amt, GL_UNSIGNED_INT, NULL);
		unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::clear(){

	if (bUsingVerts){
		glDeleteBuffers(1, &vertId);
		bUsingVerts = false;
	}
	
	if(bUsingNormals){
		glDeleteBuffers(1, &normalId);
		bUsingNormals = false;
	}
	
	if(bUsingColors){
		glDeleteBuffers(1, &colorId);
		bUsingColors = false;
	}
	
	if(bUsingTexCoords){
		glDeleteBuffers(1, &texCoordId);
		bUsingTexCoords = false;
	}
	
	vertId = 0;
	normalId = 0;
	texCoordId = 0;
	colorId = 0;
	
	//indexId = 0;
	//glDeleteBuffers(1, &indexId);
}
