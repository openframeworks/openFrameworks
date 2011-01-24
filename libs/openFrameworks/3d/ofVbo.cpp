//TODO:
//GL Error checking
//inlining?
// handle idling of arrays: possibly let gl create memory and use map buffers for streaming

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
void ofVbo::setIndexData(const GLuint * indices, int total){
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
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLuint) * total, indexData, GL_STATIC_DRAW); 
}

//TODO: updpate methods
/*
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
}

//--------------------------------------------------------------
void ofVbo::unbind() {
	//if(bUsingVerts)	 glDisableClientState(GL_VERTEX_ARRAY);
	if(bUsingColors) glDisableClientState(GL_COLOR_ARRAY);
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
	//TODO: smartly handle this
	/*
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
	 */
}
