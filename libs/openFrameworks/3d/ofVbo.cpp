//TODO:
//GL Error checking
// handle idling of arrays: possibly let gl create memory and use map buffers for streaming
// index updating/deleting?
// setVertexData with float* should know about ofVec3f vs ofVec2f?

#include "ofVbo.h"
#include "ofUtils.h"

//--------------------------------------------------------------
ofVbo::Data::Data(){
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
ofVbo::Data::~Data(){
	clear();
}

//--------------------------------------------------------------
void ofVbo::Data::clear(){

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

	if(bUsingIndices){
		glDeleteBuffers(1, &indexId);
		bUsingIndices = false;
	}

	vertId = 0;
	normalId = 0;
	texCoordId = 0;
	colorId = 0;
	indexId = 0;
}

//--------------------------------------------------------------
ofVbo::ofVbo():
data(NULL)
{
}

//--------------------------------------------------------------
ofVbo::~ofVbo(){

}

//--------------------------------------------------------------
void ofVbo::setMesh(const ofMesh & mesh, int usage){
	setVertexData(mesh.getVerticesPointer(),mesh.getNumVertices(),usage,sizeof(ofVec3f));
	setColorData(mesh.getColorsPointer(),mesh.getNumColors(),usage,sizeof(ofColor));
	setNormalData(mesh.getNormalsPointer(),mesh.getNumNormals(),usage,sizeof(ofVec3f));
	setTexCoordData(mesh.getTexCoordsPointer(),mesh.getNumTexCoords(),usage,sizeof(ofVec2f));
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const ofVec3f * verts, int total, int usage) {
	setVertexData(&verts[0].x,3,total,usage,sizeof(ofVec3f));
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const ofVec2f * verts, int total, int usage) {
	setVertexData(&verts[0].x,2,total,usage,sizeof(ofVec2f));
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const float * vert0x, int numCoords, int total, int usage, int stride) {
	if(vert0x == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad vertex data!\n");
		return;	
	}
	allocate();
	if(!data->bUsingVerts) {
		data->bAllocated  = true;
		data->bUsingVerts = true;
		glGenBuffers(1, &(data->vertId));
	}

	data->vertUsage = usage;
	data->vertSize = numCoords;
	data->vertStride = stride;
	data->totalVerts = total;
	
	glBindBuffer(GL_ARRAY_BUFFER, data->vertId);
	glBufferData(GL_ARRAY_BUFFER, total * stride, vert0x, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------
void ofVbo::setColorData(const ofColor * colors, int total, int usage) {
	setColorData(&colors[0].r,total,usage,sizeof(ofColor));
}

//--------------------------------------------------------------
void ofVbo::setColorData(const float * color0r, int total, int usage, int stride) {
	if(color0r == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad color data!\n");
		return;	
	}
	allocate();
	if(!data->bUsingColors) {
		data->bUsingColors = true;
		glGenBuffers(1, &(data->colorId));
	}
	data->colorUsage = usage;
	data->colorStride = stride;
	
	glBindBuffer(GL_ARRAY_BUFFER, data->colorId);
	glBufferData(GL_ARRAY_BUFFER, total * stride, color0r, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------
void ofVbo::setNormalData(const ofVec3f * normals, int total, int usage) {
	setNormalData(&normals[0].x,total,usage,sizeof(ofVec3f));
}

//--------------------------------------------------------------
void ofVbo::setNormalData(const float * normal0x, int total, int usage, int stride) {
	if(normal0x == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad normal data!\n");
		return;	
	}
	allocate();
	if(!data->bUsingNormals) {
		data->bUsingNormals = true;
		glGenBuffers(1, &(data->normalId));
	}
	data->normUsage = usage;
	data->normalStride = stride;
	
	glBindBuffer(GL_ARRAY_BUFFER, data->normalId);
	glBufferData(GL_ARRAY_BUFFER, total * stride, normal0x, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const ofVec2f * texCoords, int total, int usage) {
	setTexCoordData(&texCoords[0].x,total,usage,sizeof(ofVec2f));
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const float * texCoord0x, int total, int usage, int stride) {
	if(texCoord0x == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad texCoord data!\n");
		return;	
	}
	allocate();
	if(!data->bUsingTexCoords) {
		data->bUsingTexCoords = true;
		glGenBuffers(1, &(data->texCoordId));
	}
	data->texUsage = usage;
	data->texCoordStride = stride;
	
	glBindBuffer(GL_ARRAY_BUFFER, data->texCoordId);
	glBufferData(GL_ARRAY_BUFFER, total * stride, texCoord0x, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//--------------------------------------------------------------
void ofVbo::setIndexData(const ofIndexType * indices, int total, int usage){
	if(indices == NULL){
		ofLog(OF_LOG_WARNING,"ofVbo: bad index data!\n");
		return;
	}
	allocate();
	if(!data->bUsingIndices){
		data->bUsingIndices = true;
		glGenBuffers(1, &(data->indexId));
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ofIndexType) * total, &indices[0], usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------
void ofVbo::updateMesh(const ofMesh & mesh){
	ofMesh * nonconstMesh = (ofMesh*)&mesh;
	if(nonconstMesh->haveVertsChanged()) updateVertexData(mesh.getVerticesPointer(),mesh.getNumVertices());
	if(nonconstMesh->haveColorsChanged()) updateColorData(mesh.getColorsPointer(),mesh.getNumColors());
	if(nonconstMesh->haveNormalsChanged()) updateNormalData(mesh.getNormalsPointer(),mesh.getNumNormals());
	if(nonconstMesh->haveTexCoordsChanged()) updateTexCoordData(mesh.getTexCoordsPointer(),mesh.getNumTexCoords());
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const ofVec3f * verts, int total) {
	updateVertexData(&verts[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const ofVec2f * verts, int total) {
	updateVertexData(&verts[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const float * vert0x, int total) {
	if(data && data->bUsingVerts){
		glBindBuffer(GL_ARRAY_BUFFER, data->vertId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*data->vertStride, vert0x);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const ofColor * colors, int total) {
	updateColorData(&colors[0].r,total);
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const float * color0r, int total) {
	if(data && data->bUsingColors) {
		glBindBuffer(GL_ARRAY_BUFFER, data->colorId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*data->colorStride, color0r);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const ofVec3f * normals, int total) {
	updateNormalData(&normals[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const float * normal0x, int total) {
	if(data && data->bUsingNormals) {
		glBindBuffer(GL_ARRAY_BUFFER, data->normalId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*data->normalStride, normal0x);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const ofVec2f * texCoords, int total) {
	updateTexCoordData(&texCoords[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const float * texCoord0x, int total) {
	if(data && data->bUsingTexCoords) {
		glBindBuffer(GL_ARRAY_BUFFER, data->texCoordId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*data->texCoordStride, texCoord0x);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//--------------------------------------------------------------
void ofVbo::updateIndexData(const ofIndexType * indices, int total) {
	if(data && data->bUsingIndices) {
		glBindBuffer(GL_ARRAY_BUFFER, data->indexId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*sizeof(ofIndexType), &indices[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//--------------------------------------------------------------
bool ofVbo::getIsAllocated(){
	return data && data->bAllocated;
}	

//--------------------------------------------------------------
bool ofVbo::getUsingVerts(){
	return data && data->bUsingVerts;
}	

//--------------------------------------------------------------
bool ofVbo::getUsingColors(){
	return data && data->bUsingColors;
}	

//--------------------------------------------------------------
bool ofVbo::getUsingNormals(){
	return data && data->bUsingNormals;
}

//--------------------------------------------------------------
bool ofVbo::getUsingTexCoords(){
	return data && data->bUsingTexCoords;
}

//--------------------------------------------------------------
bool ofVbo::getUsingIndices(){
	return data &&  data->bUsingIndices;
}

//--------------------------------------------------------------
GLuint ofVbo::getVertId(){
	return data && data->vertId;
}

//--------------------------------------------------------------
GLuint ofVbo::getColorId(){
	return data && data->colorId;
}

//--------------------------------------------------------------
GLuint ofVbo::getNormalId(){
	return data && data->normalId;
}

//--------------------------------------------------------------
GLuint ofVbo::getTexCoordId(){
	return data && data->texCoordId;
}

//--------------------------------------------------------------
GLuint ofVbo::getIndexId(){
	return data && data->indexId;
}

//--------------------------------------------------------------
void ofVbo::bind(){
	
//#ifndef TARGET_OPENGLES
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
//#endif
	if(!data) return;
	
	if(data->bUsingVerts){
		glEnableClientState(GL_VERTEX_ARRAY);		
		glBindBuffer(GL_ARRAY_BUFFER, data->vertId);
		glVertexPointer(data->vertSize, GL_FLOAT, data->vertStride, 0);
	}
	
	if(data->bUsingColors) {
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, data->colorId);
		glColorPointer(4, GL_FLOAT, data->colorStride, 0);
	}
	
	if(data->bUsingNormals) {
		glEnableClientState(GL_NORMAL_ARRAY);		
		glBindBuffer(GL_ARRAY_BUFFER, data->normalId);
		glNormalPointer(GL_FLOAT, data->normalStride, 0);
	}
	
	if(data->bUsingTexCoords) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);		
		glBindBuffer(GL_ARRAY_BUFFER, data->texCoordId);
		glTexCoordPointer(2, GL_FLOAT, data->texCoordStride, 0);
	}
}

//--------------------------------------------------------------
void ofVbo::unbind() {
	if(!data) return;
//	if(bUsingVerts)  glDisableClientState(GL_VERTEX_ARRAY);
	if(data->bUsingColors) glDisableClientState(GL_COLOR_ARRAY);
	if(data->bUsingNormals) glDisableClientState(GL_NORMAL_ARRAY);
	if(data->bUsingTexCoords) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_EDGE_FLAG_ARRAY);
	
//#ifndef	TARGET_OPENGLES
//	glPopClientAttrib();
//	glPopAttrib();	
//#endif
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------
void ofVbo::draw(int drawMode, int first, int total) {
	if(data && data->bAllocated) {
		bind();
		glDrawArrays(drawMode, first, total);
		unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::drawElements(int drawMode, int amt) {
	if(data && data->bAllocated){
		bind();
		if(data->bUsingIndices){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->indexId);
#ifdef TARGET_OPENGLES
			glDrawElements(drawMode, amt, GL_UNSIGNED_SHORT, NULL);
#else
			glDrawElements(drawMode, amt, GL_UNSIGNED_INT, NULL);
#endif
		}
		unbind();
	}
}

void ofVbo::clear(){
	if(data) data->clear();
}

void ofVbo::allocate(){
	if(!data) data = new Data;
}
