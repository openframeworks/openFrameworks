//TODO:
//GL Error checking
// handle idling of arrays: possibly let gl create memory and use map buffers for streaming
// index updating/deleting?
// setVertexData with float* should know about ofVec3f vs ofVec2f?

#include "ofVbo.h"
#include "ofUtils.h"



//--------------------------------------------------------------
ofVbo::ofVbo() {
	bUsingVerts = false;
	bUsingTexCoords = false;
	bUsingColors = false;
	bUsingNormals = false;
	bUsingIndices = false;
	
	vertSize		= -1;
	vertStride      = 0;
	
	vertUsage		= -1;
	colorUsage		= -1;
	normUsage		= -1;
	texUsage		= -1;
	
	bAllocated		= false;
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
	
	vertSize   = 3;
	vertStride = sizeof(ofVec3f);
	totalVerts = total;
	
	glBindBuffer(GL_ARRAY_BUFFER, vertId);
	glBufferData(GL_ARRAY_BUFFER, total * vertStride, &verts[0].x, usage);
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const ofVec2f * verts, int total, int usage) {
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
	
	vertSize   = 2;
	vertStride = sizeof(ofVec2f);
	totalVerts = total;
	
	glBindBuffer(GL_ARRAY_BUFFER, vertId);
	glBufferData(GL_ARRAY_BUFFER, total * vertStride, &verts[0].x, usage);
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const float * vert0x, int total, int usage) {
	if(vert0x == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad vertex data!\n");
		return;	
	}
	vertUsage = usage;
	if(!bUsingVerts) {
		bAllocated  = true;
		bUsingVerts = true;
		glGenBuffers(1, &vertId);
	}
	
	// hardwired here for vec3, to be considered...
	vertSize = 3;
	vertStride = sizeof(ofVec3f);
	totalVerts = total;
	
	glBindBuffer(GL_ARRAY_BUFFER, vertId);
	glBufferData(GL_ARRAY_BUFFER, total * sizeof(ofVec3f), vert0x, usage);
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
	
	glBindBuffer(GL_ARRAY_BUFFER, colorId);
	glBufferData(GL_ARRAY_BUFFER, total * sizeof(ofColor), &colors[0].r, usage);	
}

//--------------------------------------------------------------
void ofVbo::setColorData(const float * color0r, int total, int usage) {
	if(color0r == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad color data!\n");
		return;	
	}
	colorUsage = usage;
	if(!bUsingColors) {
		bUsingColors = true;
		glGenBuffers(1, &colorId);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, colorId);
	glBufferData(GL_ARRAY_BUFFER, total * sizeof(ofColor), color0r, usage);	
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
	
	glBindBuffer(GL_ARRAY_BUFFER, normalId);
	glBufferData(GL_ARRAY_BUFFER, total * sizeof(ofVec3f), &normals[0].x, usage);	
}

//--------------------------------------------------------------
void ofVbo::setNormalData(const float * normal0x, int total, int usage) {
	if(normal0x == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad normal data!\n");
		return;	
	}
	normUsage = usage;
	if(!bUsingNormals) {
		bUsingNormals = true;
		glGenBuffers(1, &normalId);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, normalId);
	glBufferData(GL_ARRAY_BUFFER, total * sizeof(ofVec3f), normal0x, usage);	
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
	
	glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
	glBufferData(GL_ARRAY_BUFFER, total * sizeof(ofVec2f), &texCoords[0].x, usage);	
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const float * texCoord0x, int total, int usage) {
	if(texCoord0x == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad texCoord data!\n");
		return;	
	}
	texUsage = usage;
	if(!bUsingTexCoords) {
		bUsingTexCoords = true;
		glGenBuffers(1, &texCoordId);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
	glBufferData(GL_ARRAY_BUFFER, total * sizeof(ofVec2f), texCoord0x, usage);	
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
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * total, &indices[0], usage);
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const ofVec3f * verts, int total) {
	if(bUsingVerts){
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertId);
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, total*sizeof(ofVec3f), (float*)&verts[0].x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const float * vert0x, int total) {
	if(bUsingVerts){
		glBindBuffer(GL_ARRAY_BUFFER, vertId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*sizeof(ofVec3f), vert0x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const ofColor * colors, int total) {
	if(bUsingColors) {
		glBindBuffer(GL_ARRAY_BUFFER, colorId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*sizeof(ofColor), (float*)&colors[0].r);
	}
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const float * color0r, int total) {
	if(bUsingColors) {
		glBindBuffer(GL_ARRAY_BUFFER, colorId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*sizeof(ofColor), color0r);
	}
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const ofVec3f * normals, int total) {
	if(bUsingNormals) {
		glBindBuffer(GL_ARRAY_BUFFER, normalId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*sizeof(ofVec3f), (float*)&normals[0].x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const float * normal0x, int total) {
	if(bUsingNormals) {
		glBindBuffer(GL_ARRAY_BUFFER, normalId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*sizeof(ofVec3f), normal0x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const ofVec2f * texCoords, int total) {
	if(bUsingTexCoords) {
		glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*sizeof(ofVec2f), (float*)&texCoords[0].x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const float * texCoord0x, int total) {
	if(bUsingTexCoords) {
		glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*sizeof(ofVec2f), texCoord0x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateIndexData(const GLuint * indices, int total) {
	if(bUsingIndices) {
		glBindBuffer(GL_ARRAY_BUFFER, indexId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*sizeof(GLuint), (GLuint*)&indices[0]);
	}
}


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
	
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	
	if(bUsingVerts){
		glEnableClientState(GL_VERTEX_ARRAY);		
		glBindBuffer(GL_ARRAY_BUFFER, vertId);
		glVertexPointer(vertSize, GL_FLOAT, vertStride, 0);
	}
	
	if(bUsingColors){
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, colorId);
		glColorPointer(4, GL_FLOAT, sizeof(ofColor), 0);
	}
	
	if(bUsingNormals){
		glEnableClientState(GL_NORMAL_ARRAY);		
		glBindBuffer(GL_ARRAY_BUFFER, normalId);
		glNormalPointer(GL_FLOAT, sizeof(ofVec3f), 0);
	}
	
	if(bUsingTexCoords){
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);		
		glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
		glTexCoordPointer(2, GL_FLOAT, sizeof(ofVec2f), 0);
	}
}

//--------------------------------------------------------------
void ofVbo::unbind() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	//glPopClientAttrib();
	//glPopAttrib();	
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------
void ofVbo::draw(int drawMode, int first, int total) {
	if(bAllocated) {
		bind();
		glDrawArrays(drawMode, first, total);
		unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::drawElements(int drawMode, int amt) {
	if(bAllocated){
		bind();
		if(bUsingIndices){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
			glDrawElements(drawMode, amt, GL_UNSIGNED_INT, NULL);
		}
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
