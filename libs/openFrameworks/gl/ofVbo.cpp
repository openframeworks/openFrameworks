//TODO:
//GL Error checking
// handle idling of arrays: possibly let gl create memory and use map buffers for streaming
// index updating/deleting?
// setVertexData with float* should know about ofVec3f vs ofVec2f?


#include "ofUtils.h"
#include "ofVbo.h"
#include "ofShader.h"

#include <map>

static map<GLuint,int> & getIds(){
	static map<GLuint,int> * ids = new map<GLuint,int>;
	return *ids;
}

static map<GLuint,int> & getVAOIds(){
	static map<GLuint,int> * ids = new map<GLuint,int>;
	return *ids;
}

//--------------------------------------------------------------
static void retain(GLuint id){
	if(id==0) return;
	if(getIds().find(id)!=getIds().end()){
		getIds()[id]++;
	}else{
		getIds()[id]=1;
	}
}

//--------------------------------------------------------------
static void release(GLuint id){
	if(getIds().find(id)!=getIds().end()){
		getIds()[id]--;
		if(getIds()[id]==0){
			glDeleteBuffers(1, &id);
			getIds().erase(id);
		}
	}else{
		ofLog(OF_LOG_WARNING,"ofVbo: releasing id not found, this shouldn't be happening releasing anyway");
		glDeleteBuffers(1, &id);
	}
}

//--------------------------------------------------------------
static void retainVAO(GLuint id){
	if(id==0) return;
	if(getVAOIds().find(id)!=getVAOIds().end()){
		getVAOIds()[id]++;
	}else{
		getVAOIds()[id]=1;
	}
}

//--------------------------------------------------------------
static void releaseVAO(GLuint id){
	if(getVAOIds().find(id)!=getVAOIds().end()){
		getVAOIds()[id]--;
		if(getVAOIds()[id]==0){
			glDeleteVertexArrays(1, &id);
			getVAOIds().erase(id);
		}
	}else{
		ofLog(OF_LOG_WARNING,"ofVbo: releasing VAO id not found, this shouldn't be happening releasing anyway");
		glDeleteVertexArrays(1, &id);
	}
}

//--------------------------------------------------------------
ofVbo::ofVbo()
{
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

	vertId     = 0;
	normalId   = 0;
	colorId    = 0;
	texCoordId = 0;
	indexId    = 0;

	totalVerts = 0;
	totalIndices = 0;
	
	texCoordStride = sizeof(ofVec2f);
	normalStride = sizeof(ofVec3f);
	colorStride = sizeof(ofFloatColor);

	bAllocated		= false;

	vaoChanged 		= false;
	vaoID			= 0;
	binded   = false;
}

ofVbo::ofVbo(const ofVbo & mom){
	bUsingVerts = mom.bUsingVerts;
	bUsingTexCoords = mom.bUsingTexCoords;
	bUsingColors = mom.bUsingColors;
	bUsingNormals = mom.bUsingNormals;
	bUsingIndices = mom.bUsingIndices;

	vertSize		= mom.vertSize;
	vertStride      = mom.vertStride;
	colorStride		= mom.colorStride;
	normalStride    = mom.normalStride;
	texCoordStride  = mom.texCoordStride;

	vertUsage		= mom.vertUsage;
	colorUsage		= mom.colorUsage;
	normUsage		= mom.normUsage;
	texUsage		= mom.texUsage;

	vertId     = mom.vertId;
	retain(vertId);
	normalId   = mom.normalId;
	retain(normalId);
	colorId    = mom.colorId;
	retain(colorId);
	texCoordId = mom.texCoordId;
	retain(texCoordId);
	indexId    = mom.indexId;
	retain(indexId);
	vaoID	   = mom.vaoID;
	retainVAO(vaoID);
	vaoChanged = mom.vaoChanged;


	totalVerts = mom.totalVerts;
	totalIndices = mom.totalIndices;

	texCoordStride = sizeof(ofVec2f);
	normalStride = sizeof(ofVec3f);
	colorStride = sizeof(ofFloatColor);

	bAllocated		= mom.bAllocated;

	binded   = mom.binded;
}

ofVbo & ofVbo::operator=(const ofVbo& mom){
	if(&mom==this) return *this;
	bUsingVerts = mom.bUsingVerts;
	bUsingTexCoords = mom.bUsingTexCoords;
	bUsingColors = mom.bUsingColors;
	bUsingNormals = mom.bUsingNormals;
	bUsingIndices = mom.bUsingIndices;

	vertSize		= mom.vertSize;
	vertStride      = mom.vertStride;
	colorStride		= mom.colorStride;
	normalStride    = mom.normalStride;
	texCoordStride  = mom.texCoordStride;

	vertUsage		= mom.vertUsage;
	colorUsage		= mom.colorUsage;
	normUsage		= mom.normUsage;
	texUsage		= mom.texUsage;

	vertId     = mom.vertId;
	retain(vertId);
	normalId   = mom.normalId;
	retain(normalId);
	colorId    = mom.colorId;
	retain(colorId);
	texCoordId = mom.texCoordId;
	retain(texCoordId);
	indexId    = mom.indexId;
	retain(indexId);
	vaoID	   = mom.vaoID;
	retainVAO(vaoID);
	vaoChanged = mom.vaoChanged;

	totalVerts = mom.totalVerts;
	totalIndices = mom.totalIndices;

	bAllocated		= mom.bAllocated;
	binded   = mom.binded;
	return *this;
}

//--------------------------------------------------------------
ofVbo::~ofVbo(){
	clear();
}

//--------------------------------------------------------------
void ofVbo::setMesh(const ofMesh & mesh, int usage){
	setMesh(mesh,usage,mesh.hasColors(),mesh.hasTexCoords(),mesh.hasNormals());
}

//--------------------------------------------------------------
void ofVbo::setMesh(const ofMesh & mesh, int usage, bool useColors, bool useTextures, bool useNormals){
	if(mesh.getVertices().empty()){
		ofLog(OF_LOG_WARNING,"ofVbo: bad vertex data!\n");
		return;
	}
	setVertexData(mesh.getVerticesPointer(),mesh.getNumVertices(),usage);
	if(mesh.hasColors() && useColors){
		setColorData(mesh.getColorsPointer(),mesh.getNumColors(),usage);
		enableColors();
	}else{
		disableColors();
	}
	if(mesh.hasNormals() && useNormals){
		setNormalData(mesh.getNormalsPointer(),mesh.getNumNormals(),usage);
		enableNormals();
	}else{
		disableNormals();
	}
	if(mesh.hasTexCoords() && useTextures){
		setTexCoordData(mesh.getTexCoordsPointer(),mesh.getNumTexCoords(),usage);
		enableTexCoords();
	}else{
		disableTexCoords();
	}
	if(mesh.hasIndices()){
		setIndexData(mesh.getIndexPointer(), mesh.getNumIndices(), usage);
		enableIndices();
	}else{
		disableIndices();
	}
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
	if(vertId==0) {
		bAllocated  = true;
		bUsingVerts = true;
		vaoChanged=true;
		glGenBuffers(1, &(vertId));
		retain(vertId);
	}

	vertUsage = usage;
	vertSize = numCoords;
	vertStride = stride;
	totalVerts = total;
	
	glBindBuffer(GL_ARRAY_BUFFER, vertId);
	glBufferData(GL_ARRAY_BUFFER, total * stride, vert0x, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------
void ofVbo::setColorData(const ofFloatColor * colors, int total, int usage) {
	setColorData(&colors[0].r,total,usage,sizeof(ofFloatColor));
}

//--------------------------------------------------------------
void ofVbo::setColorData(const float * color0r, int total, int usage, int stride) {
	if(color0r == NULL) {
		ofLog(OF_LOG_WARNING,"ofVbo: bad color data!\n");
		return;	
	}
	if(colorId==0) {
		glGenBuffers(1, &(colorId));
		retain(colorId);
		enableColors();
	}
	colorUsage = usage;
	colorStride = stride;
	
	glBindBuffer(GL_ARRAY_BUFFER, colorId);
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
	if(normalId==0) {
		glGenBuffers(1, &(normalId));
		retain(normalId);
		enableNormals();
	}
	normUsage = usage;
	normalStride = stride;
	
	glBindBuffer(GL_ARRAY_BUFFER, normalId);
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
	if(texCoordId==0) {
		glGenBuffers(1, &(texCoordId));
		retain(texCoordId);
		enableTexCoords();
	}
	texUsage = usage;
	texCoordStride = stride;
	
	glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
	glBufferData(GL_ARRAY_BUFFER, total * stride, texCoord0x, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//--------------------------------------------------------------
void ofVbo::setIndexData(const ofIndexType * indices, int total, int usage){
	if(indices == NULL){
		ofLog(OF_LOG_WARNING,"ofVbo: bad index data!\n");
		return;
	}
	if(indexId==0){
		glGenBuffers(1, &(indexId));
		retain(indexId);
		enableIndices();
	}
	
	totalIndices = total;
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ofIndexType) * total, &indices[0], usage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ofVbo::setAttributeData(int location, const float * attrib0x, int numCoords, int total, int usage, int stride){
	if(attrib0x == NULL){
		ofLog(OF_LOG_WARNING,"ofVbo: bad index data!\n");
		return;
	}

	if(attributeIds.find(location)==attributeIds.end()){
		glGenBuffers(1, &(attributeIds[location]));
		retain(attributeIds[location]);
		vaoChanged = true;
	}

	attributeStrides[location] = stride;
	attributeNumCoords[location] = numCoords;

	glBindBuffer(GL_ARRAY_BUFFER, attributeIds[location]);
	glBufferData(GL_ARRAY_BUFFER, total * stride, attrib0x, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
	if(vertId!=0){
		glBindBuffer(GL_ARRAY_BUFFER, vertId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*vertStride, vert0x);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const ofFloatColor * colors, int total) {
	updateColorData(&colors[0].r,total);
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const float * color0r, int total) {
	if(colorId!=0) {
		glBindBuffer(GL_ARRAY_BUFFER, colorId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*colorStride, color0r);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const ofVec3f * normals, int total) {
	updateNormalData(&normals[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const float * normal0x, int total) {
	if(normalId!=0) {
		glBindBuffer(GL_ARRAY_BUFFER, normalId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*normalStride, normal0x);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const ofVec2f * texCoords, int total) {
	updateTexCoordData(&texCoords[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const float * texCoord0x, int total) {
	if(texCoordId!=0) {
		glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*texCoordStride, texCoord0x);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//--------------------------------------------------------------
void ofVbo::updateIndexData(const ofIndexType * indices, int total) {
	if(indexId!=0) {
		glBindBuffer(GL_ARRAY_BUFFER, indexId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*sizeof(ofIndexType), &indices[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void ofVbo::updateAttributeData(int location, const float * attr0x, int total){
	if(attributeIds.find(location)!=attributeIds.end() && attributeIds[location]!=0) {
		glBindBuffer(GL_ARRAY_BUFFER, attributeIds[location]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, total*attributeStrides[location], attr0x);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void ofVbo::enableColors(){
	if(colorId!=0 && !bUsingColors){
		bUsingColors=true;
		vaoChanged = true;
	}
}

void ofVbo::enableNormals(){
	if(normalId!=0 && !bUsingNormals){
		bUsingNormals=true;
		vaoChanged = true;
	}
}

void ofVbo::enableTexCoords(){
	if(texCoordId!=0 && !bUsingTexCoords){
		bUsingTexCoords=true;
		vaoChanged = true;
	}
}

void ofVbo::enableIndices(){
	if(indexId!=0 && !bUsingIndices){
		bUsingIndices=true;
		vaoChanged = true;
	}
}

void ofVbo::disableColors(){
	if(bUsingColors){
		bUsingColors=false;
		vaoChanged = true;
	}
}

void ofVbo::disableNormals(){
	if(bUsingNormals){
		bUsingNormals=false;
		vaoChanged = true;
	}
}

void ofVbo::disableTexCoords(){
	if(bUsingTexCoords){
		bUsingTexCoords=false;
		vaoChanged = true;
	}
}

void ofVbo::disableIndices(){
	if(bUsingIndices){
		bUsingIndices=false;
		vaoChanged = true;
	}
}

//--------------------------------------------------------------
bool ofVbo::getIsAllocated() const {
	return bAllocated;
}	

//--------------------------------------------------------------
bool ofVbo::getUsingVerts() const  {
	return bUsingVerts;
}	

//--------------------------------------------------------------
bool ofVbo::getUsingColors() const {
	return bUsingColors;
}	

//--------------------------------------------------------------
bool ofVbo::getUsingNormals() const {
	return bUsingNormals;
}

//--------------------------------------------------------------
bool ofVbo::getUsingTexCoords() const {
	return bUsingTexCoords;
}

//--------------------------------------------------------------
bool ofVbo::getUsingIndices() const {
	return  bUsingIndices;
}

//--------------------------------------------------------------
GLuint ofVbo::getVertId() const {
	return vertId;
}

//--------------------------------------------------------------
GLuint ofVbo::getColorId() const{
	return colorId;
}

//--------------------------------------------------------------
GLuint ofVbo::getNormalId() const {
	return normalId;
}

//--------------------------------------------------------------
GLuint ofVbo::getTexCoordId() const {
	return texCoordId;
}

//--------------------------------------------------------------
GLuint ofVbo::getIndexId() const {
	return indexId;
}

//--------------------------------------------------------------
void ofVbo::bind(){
	if(vaoID==0){
		glGenVertexArrays(1, &vaoID);
		retainVAO(vaoID);
	}

	glBindVertexArray(vaoID);

	if(vaoChanged){
		if(bUsingVerts){
			glBindBuffer(GL_ARRAY_BUFFER, vertId);
			if(ofGLIsFixedPipeline()){
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertSize, GL_FLOAT, vertStride, 0);
			}else{
				glEnableVertexAttribArray(ofShader::POSITION_ATTRIBUTE);
				glVertexAttribPointer(ofShader::POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, vertStride, 0);
			}
		}else{
			if(ofGLIsFixedPipeline()){
				glDisableClientState(GL_VERTEX_ARRAY);
			}else{
				glDisableVertexAttribArray(ofShader::POSITION_ATTRIBUTE);
			}
		}

		if(bUsingColors) {
			glBindBuffer(GL_ARRAY_BUFFER, colorId);
			if(ofGLIsFixedPipeline()){
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(4, GL_FLOAT, colorStride, 0);
			}else{
				glEnableVertexAttribArray(ofShader::COLOR_ATTRIBUTE);
				glVertexAttribPointer(ofShader::COLOR_ATTRIBUTE, 4, GL_FLOAT, GL_FALSE, colorStride, 0);
			}
		}else{
			if(ofGLIsFixedPipeline()){
				glDisableClientState(GL_COLOR_ARRAY);
			}else{
				glDisableVertexAttribArray(ofShader::COLOR_ATTRIBUTE);
			}
		}

		if(bUsingNormals) {
			glBindBuffer(GL_ARRAY_BUFFER, normalId);
			if(ofGLIsFixedPipeline()){
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, normalStride, 0);
			}else{
				// tig: note that we set the 'Normalize' flag to true here, assuming that mesh normals need to be
				// normalized while being uploaded to GPU memory.
				// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
				// Normalizing the normals on the shader is probably faster, but sending non-normalized normals is
				// more prone to lead to artifacts difficult to diagnose, especially with the built-in 3D primitives.
				// If you need to optimise this, and you've dug this far through the code, you are most probably
				// able to roll your own client code for binding & rendering vbos anyway...
				glEnableVertexAttribArray(ofShader::NORMAL_ATTRIBUTE);
				glVertexAttribPointer(ofShader::NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_TRUE, normalStride, 0);
			}
		}else{
			if(ofGLIsFixedPipeline()){
				glDisableClientState(GL_NORMAL_ARRAY);
			}else{
				glDisableVertexAttribArray(ofShader::NORMAL_ATTRIBUTE);
			}
		}

		if(bUsingTexCoords) {
			glBindBuffer(GL_ARRAY_BUFFER, texCoordId);
			if(ofGLIsFixedPipeline()){
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, texCoordStride, 0);
			}else{
				glEnableVertexAttribArray(ofShader::TEXCOORD_ATTRIBUTE);
				glVertexAttribPointer(ofShader::TEXCOORD_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, texCoordStride, 0);
			}
		}else{
			if(ofGLIsFixedPipeline()){
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}else{
				glDisableVertexAttribArray(ofShader::TEXCOORD_ATTRIBUTE);
			}
		}

		map<int,GLuint>::iterator it;
		for(it=attributeIds.begin();it!=attributeIds.end();it++){
			glEnableVertexAttribArray(it->first);
			glVertexAttribPointer(it->first, attributeNumCoords[it->first], GL_FLOAT, GL_FALSE, attributeStrides[it->first], 0);
		}

		vaoChanged=false;
	}

	if(bUsingVerts){
		ofEnableVertices();
	}else{
		ofDisableVertices();
	}

	if(bUsingColors) {
		ofEnableColorCoords();
	}else{
		ofDisableColorCoords();
	}

	if(bUsingNormals){
		ofEnableNormals();
	}else{
		ofDisableNormals();
	}

	if(bUsingTexCoords) {
		ofEnableTexCoords();
	}else{
		ofDisableTexCoords();
	}
	binded   = true;
}

//--------------------------------------------------------------
void ofVbo::unbind() {
	/*glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	ofDisableVertices();	// tig: oh dear, finding that bug was painful.
	if(bUsingColors) ofDisableColorCoords();
	if(bUsingNormals) ofDisableNormals();
	if(bUsingTexCoords) ofDisableTexCoords();*/
	glBindVertexArray(0);
	binded   = false;
}

//--------------------------------------------------------------
void ofVbo::draw(int drawMode, int first, int total) {
	if(bAllocated) {
		bool wasBinded = binded;
		if(!wasBinded) bind();
		glDrawArrays(drawMode, first, total);
		if(!wasBinded) unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::drawElements(int drawMode, int amt) {
	if(bAllocated){
		bind();
		if(bUsingIndices){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
#ifdef TARGET_OPENGLES
			glDrawElements(drawMode, amt, GL_UNSIGNED_SHORT, NULL);
#else
			glDrawElements(drawMode, amt, GL_UNSIGNED_INT, NULL);
#endif
		}
		unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::clear(){
	clearVertices();
	clearNormals();
	clearColors();
	clearTexCoords();
	clearIndices();
	if(vaoID!=0){
		releaseVAO(vaoID);
		vaoID=0;
	}
	bAllocated		= false;
}


//--------------------------------------------------------------
void ofVbo::clearVertices(){
	if(vertId!=0){
		release(vertId);
		vertId = 0;
		bUsingVerts = false;
		totalVerts = 0;
		vertSize = -1;
		vertStride      = 0;
		vertUsage		= -1;
	}
}

//--------------------------------------------------------------
void ofVbo::clearNormals(){
	if(normalId!=0){
		release(normalId);
		normalId = 0;
		bUsingNormals = false;
		normUsage		= -1;
		normalStride = sizeof(ofVec3f);
	}
}

//--------------------------------------------------------------
void ofVbo::clearColors(){
	if(colorId!=0){
		release(colorId);
		colorId = 0;
		bUsingColors = false;
		colorUsage		= -1;
		colorStride = sizeof(ofFloatColor);
	}
}

//--------------------------------------------------------------
void ofVbo::clearTexCoords(){
	if(texCoordId!=0){
		release(texCoordId);
		texCoordId = 0;
		bUsingTexCoords = false;
		texUsage		= -1;
		texCoordStride = sizeof(ofVec2f);
	}
}

//--------------------------------------------------------------
void ofVbo::clearIndices(){
	if(indexId!=0){
		release(indexId);
		indexId = 0;
		bUsingIndices = false;
		totalIndices = 0;
	}
}

//--------------------------------------------------------------
int ofVbo::getNumIndices() const {
	if (bUsingIndices) {
		return totalIndices;
	} else {
		return 0;
	}
}

//--------------------------------------------------------------

int ofVbo::getNumVertices() const {
	return totalVerts;
}
