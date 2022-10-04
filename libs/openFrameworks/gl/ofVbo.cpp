//TODO:
//GL Error checking
// handle idling of arrays: possibly let gl create memory and use map buffers for streaming
// index updating/deleting?
// setVertexData with float* should know about ofVec3f vs ofVec2f?


#include "ofUtils.h"
#include "ofVbo.h"
#include "ofShader.h"
#include "ofGLUtils.h"
#include "ofMesh.h"
#include "ofGLBaseTypes.h"

#ifdef TARGET_ANDROID
#include "ofAppAndroidWindow.h"
#endif


bool ofVbo::vaoSupported=true;
bool ofVbo::vaoChecked=false;

using std::map;

#ifdef TARGET_OPENGLES
	#include <dlfcn.h>
	typedef void (* glGenVertexArraysType) (GLsizei n,  GLuint *arrays);
	glGenVertexArraysType glGenVertexArraysFunc = nullptr;
	#define glGenVertexArrays								glGenVertexArraysFunc

	typedef void (* glDeleteVertexArraysType) (GLsizei n,  GLuint *arrays);
	glDeleteVertexArraysType glDeleteVertexArraysFunc = nullptr;
	#define glDeleteVertexArrays							glDeleteVertexArraysFunc

	typedef void (* glBindVertexArrayType) (GLuint array);
	glBindVertexArrayType glBindVertexArrayFunc = nullptr;
	#define glBindVertexArray								glBindVertexArrayFunc
#endif

static map<GLuint,int> & getVAOIds(){
	static map<GLuint,int> * ids = new map<GLuint,int>;
	return *ids;
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
#ifdef TARGET_ANDROID
			if (!ofAppAndroidWindow::isSurfaceDestroyed())
#endif
				glDeleteVertexArrays(1, &id);
			getVAOIds().erase(id);
		}
	}else{
		ofLogWarning("ofVbo") << "releaseVAO(): something's wrong here, releasing unknown vertex array object id " << id;

#ifdef TARGET_ANDROID
		if (!ofAppAndroidWindow::isSurfaceDestroyed())
#endif
			glDeleteVertexArrays(1, &id);
	}
}

//--------------------------------------------------------------
ofVbo::VertexAttribute::VertexAttribute()
:stride(0)
,offset(0)
,numCoords(0)
,location(0)
,normalize(false)
,divisor(0){

}

//--------------------------------------------------------------
bool ofVbo::VertexAttribute::isAllocated() const{
	return buffer.isAllocated();
}


//--------------------------------------------------------------
void ofVbo::VertexAttribute::allocate(){
	buffer.allocate();
}

//--------------------------------------------------------------
void ofVbo::VertexAttribute::bind() const{
	buffer.bind(GL_ARRAY_BUFFER);
}

//--------------------------------------------------------------
void ofVbo::VertexAttribute::setData(GLsizeiptr bytes, const void * data, GLenum usage){
	buffer.setData(bytes,data,usage);
}

//--------------------------------------------------------------
void ofVbo::VertexAttribute::unbind() const{
	buffer.unbind(GL_ARRAY_BUFFER);
}

//--------------------------------------------------------------
GLuint ofVbo::VertexAttribute::getId() const{
	return buffer.getId();
}

//--------------------------------------------------------------
void ofVbo::VertexAttribute::updateData(GLintptr offset, GLsizeiptr bytes, const void * data){
	buffer.updateData(offset,bytes,data);
}


//--------------------------------------------------------------
void ofVbo::VertexAttribute::setData(const float * attrib0x, int numCoords, int total, int usage, int stride, bool normalize){
	if (!isAllocated()) {
		allocate();
	}
	GLsizeiptr size = (stride == 0) ? numCoords * sizeof(float) : stride;
	this->stride = size;
	this->numCoords = numCoords;
	this->offset = 0;
	this->normalize = normalize;
	setData(total * size, attrib0x, usage);
};

//--------------------------------------------------------------
void ofVbo::VertexAttribute::setBuffer(ofBufferObject & buffer, int numCoords, int stride, int offset){
	this->buffer = buffer;
	this->offset = offset;
	this->numCoords = numCoords;
	GLsizeiptr size = (stride == 0) ? numCoords * sizeof(float) : stride;
	this->stride = size;

};

//--------------------------------------------------------------
void ofVbo::VertexAttribute::enable() const{
	bind();
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, numCoords, GL_FLOAT, normalize?GL_TRUE:GL_FALSE, stride, (void*)offset);
#ifndef TARGET_OPENGLES
	glVertexAttribDivisor(location, divisor);
#endif
	unbind();
}

//--------------------------------------------------------------
void ofVbo::VertexAttribute::disable() const{
	glDisableVertexAttribArray(location);
}

//--------------------------------------------------------------
ofVbo::IndexAttribute::IndexAttribute()
{

}

//--------------------------------------------------------------
bool ofVbo::IndexAttribute::isAllocated() const{
	return buffer.isAllocated();
}

//--------------------------------------------------------------
void ofVbo::IndexAttribute::allocate(){
	buffer.allocate();
}

//--------------------------------------------------------------
void ofVbo::IndexAttribute::bind() const{
	buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
}

//--------------------------------------------------------------
void ofVbo::IndexAttribute::setData(GLsizeiptr bytes, const void * data, GLenum usage){
	buffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	buffer.setData(bytes,data,usage);
	buffer.unbind(GL_ELEMENT_ARRAY_BUFFER);
}

//--------------------------------------------------------------
void ofVbo::IndexAttribute::unbind() const{
	buffer.unbind(GL_ELEMENT_ARRAY_BUFFER);
}

//--------------------------------------------------------------
void ofVbo::IndexAttribute::updateData(GLintptr offset, GLsizeiptr bytes, const void * data){
	buffer.updateData(offset,bytes,data);
}

//--------------------------------------------------------------
GLuint ofVbo::IndexAttribute::getId() const{
	return buffer.getId();
}


//--------------------------------------------------------------
ofVbo::ofVbo(){
	bUsingVerts = false;
	bUsingTexCoords = false;
	bUsingColors = false;
	bUsingNormals = false;
	bUsingIndices = false;

	totalVerts = 0;
	totalIndices = 0;

	vaoChanged 		= false;
	vaoID			= 0;

	positionAttribute.location = ofShader::POSITION_ATTRIBUTE;
	colorAttribute.location = ofShader::COLOR_ATTRIBUTE;
	texCoordAttribute.location = ofShader::TEXCOORD_ATTRIBUTE;
	normalAttribute.location = ofShader::NORMAL_ATTRIBUTE;
}

ofVbo::ofVbo(const ofVbo & mom){
	bUsingVerts = mom.bUsingVerts;
	bUsingTexCoords = mom.bUsingTexCoords;
	bUsingColors = mom.bUsingColors;
	bUsingNormals = mom.bUsingNormals;
	bUsingIndices = mom.bUsingIndices;

	positionAttribute = mom.positionAttribute;
	colorAttribute = mom.colorAttribute;
	texCoordAttribute = mom.texCoordAttribute;
	normalAttribute = mom.normalAttribute;

	customAttributes = mom.customAttributes;

	totalVerts = mom.totalVerts;
	totalIndices = mom.totalIndices;
	indexAttribute = mom.indexAttribute;

	vaoChanged = mom.vaoChanged;
	vaoID = mom.vaoID;

	if(ofIsGLProgrammableRenderer()){
		retainVAO(vaoID);
	}
}

ofVbo & ofVbo::operator=(const ofVbo& mom){
	if(&mom==this) return *this;
	clear();
	bUsingVerts = mom.bUsingVerts;
	bUsingTexCoords = mom.bUsingTexCoords;
	bUsingColors = mom.bUsingColors;
	bUsingNormals = mom.bUsingNormals;
	bUsingIndices = mom.bUsingIndices;

	positionAttribute = mom.positionAttribute;
	colorAttribute = mom.colorAttribute;
	texCoordAttribute = mom.texCoordAttribute;
	normalAttribute = mom.normalAttribute;

	customAttributes = mom.customAttributes;

	totalVerts = mom.totalVerts;
	totalIndices = mom.totalIndices;
	indexAttribute = mom.indexAttribute;

	vaoChanged = mom.vaoChanged;
	vaoID = mom.vaoID;

	if(ofIsGLProgrammableRenderer()){
		retainVAO(vaoID);
	}
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
		ofLogWarning("ofVbo") << "setMesh(): ignoring mesh with no vertices";
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
void ofVbo::setVertexData(const glm::vec3 * verts, int total, int usage) {
	setVertexData(&verts[0].x,3,total,usage,sizeof(glm::vec3));
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const ofVec3f * verts, int total, int usage) {
	setVertexData(&verts[0].x,3,total,usage,sizeof(glm::vec3));
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const glm::vec2 * verts, int total, int usage) {
	setVertexData(&verts[0].x,2,total,usage,sizeof(glm::vec2));
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const ofVec2f * verts, int total, int usage) {
	setVertexData(&verts[0].x,2,total,usage,sizeof(glm::vec2));
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const float * vert0x, int numCoords, int total, int usage, int stride) {
	positionAttribute.setData(vert0x, numCoords, total, usage, stride);
	bUsingVerts = true;
	totalVerts = total;
}

//--------------------------------------------------------------
void ofVbo::setColorData(const ofFloatColor * colors, int total, int usage) {
	setColorData(&colors[0].r,total,usage,sizeof(ofFloatColor));
}

//--------------------------------------------------------------
void ofVbo::setColorData(const float * color0r, int total, int usage, int stride) {
	colorAttribute.setData(color0r, 4, total, usage, stride);
	enableColors();
}

//--------------------------------------------------------------
void ofVbo::setNormalData(const glm::vec3 * normals, int total, int usage) {
	setNormalData(&normals[0].x,total,usage,sizeof(glm::vec3));
}

//--------------------------------------------------------------
void ofVbo::setNormalData(const ofVec3f * normals, int total, int usage) {
	setNormalData(&normals[0].x,total,usage,sizeof(glm::vec3));
}

//--------------------------------------------------------------
void ofVbo::setNormalData(const float * normal0x, int total, int usage, int stride) {
	normalAttribute.setData(normal0x, 3, total, usage, stride);
	enableNormals();
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const glm::vec2 * texCoords, int total, int usage) {
	setTexCoordData(&texCoords[0].x,total, usage, sizeof(glm::vec2));
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const ofVec2f * texCoords, int total, int usage) {
	setTexCoordData(&texCoords[0].x,total, usage, sizeof(glm::vec2));
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const float * texCoord0x, int total, int usage, int stride) {
	texCoordAttribute.setData(texCoord0x, 2, total, usage, stride);
	enableTexCoords();
}


//--------------------------------------------------------------
void ofVbo::setIndexData(const ofIndexType * indices, int total, int usage){
	if(!indexAttribute.isAllocated()){
		indexAttribute.allocate();
		enableIndices();
	}
	totalIndices = total;
	indexAttribute.setData(sizeof(ofIndexType) * total, &indices[0], usage);
}

//--------------------------------------------------------------
ofVbo::VertexAttribute & ofVbo::getOrCreateAttr(int location){
	VertexAttribute * attr = nullptr;
	if (ofIsGLProgrammableRenderer()) {
		switch (location){
			case ofShader::POSITION_ATTRIBUTE:
				attr = &positionAttribute;
				break;
			case ofShader::COLOR_ATTRIBUTE:
				attr = &colorAttribute;
				break;
			case ofShader::NORMAL_ATTRIBUTE:
				attr = &normalAttribute;
				break;
			case ofShader::TEXCOORD_ATTRIBUTE:
				attr = &texCoordAttribute;
				break;
			default:
				customAttributes[location].location = location;
				attr = &customAttributes[location];
				vaoChanged = true;
				break;
		}
	}else{
		customAttributes[location].location = location;
		attr = &customAttributes[location];
		vaoChanged = true;
	}
	return *attr;
}

//--------------------------------------------------------------
void ofVbo::setAttributeData(int location, const float * attrib0x, int numCoords, int total, int usage, int stride){
	if(ofIsGLProgrammableRenderer() && location==ofShader::POSITION_ATTRIBUTE){
		totalVerts = total;
	}

	bool normalize = false;
	if(ofIsGLProgrammableRenderer() && !hasAttribute(location)){
		vaoChanged = true;
		bUsingVerts |= (location == ofShader::POSITION_ATTRIBUTE);
		bUsingColors |= (location == ofShader::COLOR_ATTRIBUTE);
		bUsingNormals |= (location == ofShader::NORMAL_ATTRIBUTE);
		bUsingTexCoords |= (location == ofShader::TEXCOORD_ATTRIBUTE);
	}

	getOrCreateAttr(location).setData(attrib0x,numCoords,total,usage,stride,normalize);
}

#ifndef TARGET_OPENGLES
//--------------------------------------------------------------
void ofVbo::setAttributeDivisor(int location, int divisor){
	getOrCreateAttr(location).divisor = divisor;
}
#endif

//--------------------------------------------------------------
void ofVbo::updateMesh(const ofMesh & mesh){
	updateVertexData(mesh.getVerticesPointer(),mesh.getNumVertices());
	updateColorData(mesh.getColorsPointer(),mesh.getNumColors());
	updateNormalData(mesh.getNormalsPointer(),mesh.getNumNormals());
	updateTexCoordData(mesh.getTexCoordsPointer(),mesh.getNumTexCoords());
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const glm::vec3 * verts, int total) {
	updateVertexData(&verts[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const ofVec3f * verts, int total) {
	updateVertexData(&verts[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const glm::vec2 * verts, int total) {
	updateVertexData(&verts[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const ofVec2f * verts, int total) {
	updateVertexData(&verts[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateVertexData(const float * vert0x, int total) {
	positionAttribute.updateData(0, total * positionAttribute.stride, vert0x);
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const ofFloatColor * colors, int total) {
	updateColorData(&colors[0].r,total);
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const float * color0r, int total) {
	colorAttribute.updateData(0, total * colorAttribute.stride, color0r);
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const glm::vec3 * normals, int total) {
	updateNormalData(&normals[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const ofVec3f * normals, int total) {
	updateNormalData(&normals[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const float * normal0x, int total) {
	normalAttribute.updateData(0, total * normalAttribute.stride, normal0x);
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const glm::vec2 * texCoords, int total) {
	updateTexCoordData(&texCoords[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const ofVec2f * texCoords, int total) {
	updateTexCoordData(&texCoords[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const float * texCoord0x, int total) {
	texCoordAttribute.updateData(0, total * texCoordAttribute.stride, texCoord0x);
}

//--------------------------------------------------------------
void ofVbo::updateIndexData(const ofIndexType * indices, int total) {
	if(indexAttribute.isAllocated()) {
		indexAttribute.updateData(0, total*sizeof(ofIndexType), indices);
	}
}

void ofVbo::updateAttributeData(int location, const float * attr0x, int total){
	VertexAttribute * attr = nullptr;
	if (ofIsGLProgrammableRenderer()) {
		switch (location){
			case ofShader::POSITION_ATTRIBUTE:
				attr = &positionAttribute;
				break;
			case ofShader::COLOR_ATTRIBUTE:
				attr = &colorAttribute;
				break;
			case ofShader::NORMAL_ATTRIBUTE:
				attr = &normalAttribute;
				break;
			case ofShader::TEXCOORD_ATTRIBUTE:
				attr = &texCoordAttribute;
				break;
			default:
				if(customAttributes.find(location)!=customAttributes.end()) {
					attr = &customAttributes[location];
				}
				break;
		}
	} else {
		if(customAttributes.find(location)!=customAttributes.end()) {
			attr = &customAttributes[location];
		}
	}
	if (attr !=nullptr && attr->isAllocated()) {
		attr->updateData(0, total*attr->stride, attr0x);
	}
}

void ofVbo::enableColors() {
	if (!bUsingColors && colorAttribute.isAllocated()) {
		bUsingColors=true;
		vaoChanged = true;
	}
}

void ofVbo::enableNormals(){
	if (!bUsingNormals && normalAttribute.isAllocated()) {
		bUsingNormals=true;
		vaoChanged = true;
	}
}

void ofVbo::enableTexCoords(){
	if (!bUsingTexCoords && texCoordAttribute.isAllocated()){
		bUsingTexCoords = true;
		vaoChanged = true;
	}
}

void ofVbo::enableIndices(){
	if(indexAttribute.isAllocated() && !bUsingIndices){
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
	return positionAttribute.isAllocated();
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
GLuint ofVbo::getVaoId() const{
	return vaoID;
}

//--------------------------------------------------------------
GLuint ofVbo::getVertId() const {
	return positionAttribute.getId();
}

//--------------------------------------------------------------
GLuint ofVbo::getColorId() const{
	return colorAttribute.getId();
}

//--------------------------------------------------------------
GLuint ofVbo::getNormalId() const {
	return normalAttribute.getId();
}

//--------------------------------------------------------------
GLuint ofVbo::getTexCoordId() const {
	return texCoordAttribute.getId();
}

//--------------------------------------------------------------
GLuint ofVbo::getIndexId() const {
	return indexAttribute.getId();
}

//--------------------------------------------------------------
GLuint ofVbo::getAttributeId(int location) const {
	if (!hasAttribute(location)) {
		ofLogWarning() << "No attribute id found for attribute pos: " << location;
		return 0;
	}
	return const_cast<ofVbo*>(this)->getOrCreateAttr(location).getId();
}

//--------------------------------------------------------------
void ofVbo::setVertexBuffer(ofBufferObject & buffer, int numCoords, int stride, int offset){
	positionAttribute.setBuffer(buffer, numCoords, stride, offset);
	bUsingVerts = true;
	vaoChanged = true;
	// Calculate the total number of vertices based on what we know:
	int tmpStride = stride;
	if (tmpStride == 0) {
		// if stride is not given through argument, we need to calculate it based on
		// on the data size and the number of coordinates.
		tmpStride = (numCoords * sizeof(float));
		if (tmpStride == 0) {
			ofLogWarning() << "Setting buffer with 0 vertices.";
			totalVerts = 0;
			return;
		}
	}
	totalVerts = buffer.size() / tmpStride;
}

//--------------------------------------------------------------
void ofVbo::setColorBuffer(ofBufferObject & buffer, int stride, int offset){
	colorAttribute.setBuffer(buffer, 4, stride, offset);
	enableColors();
}

//--------------------------------------------------------------
void ofVbo::setNormalBuffer(ofBufferObject & buffer, int stride, int offset){
	normalAttribute.setBuffer(buffer, 3, stride, offset);
	enableNormals();
}

//--------------------------------------------------------------
void ofVbo::setTexCoordBuffer(ofBufferObject & buffer, int stride, int offset){
	texCoordAttribute.setBuffer(buffer, 2, stride, offset);
	enableTexCoords();
}

//--------------------------------------------------------------
void ofVbo::setIndexBuffer(ofBufferObject & buffer){
	indexAttribute.buffer = buffer;
	vaoChanged = true;
	enableIndices();
}

//--------------------------------------------------------------
void ofVbo::setAttributeBuffer(int location, ofBufferObject & buffer, int numCoords, int stride, int offset){
	if(ofIsGLProgrammableRenderer() && !hasAttribute(location)){
		vaoChanged = true;
		bUsingVerts |= (location == ofShader::POSITION_ATTRIBUTE);
		bUsingColors |= (location == ofShader::COLOR_ATTRIBUTE);
		bUsingNormals |= (location == ofShader::NORMAL_ATTRIBUTE);
		bUsingTexCoords |= (location == ofShader::TEXCOORD_ATTRIBUTE);
	}

	getOrCreateAttr(location).setBuffer(buffer, numCoords, stride, offset);
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getVertexBuffer(){
	return positionAttribute.buffer;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getColorBuffer(){
	return colorAttribute.buffer;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getNormalBuffer(){
	return normalAttribute.buffer;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getTexCoordBuffer(){
	return texCoordAttribute.buffer;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getIndexBuffer(){
	return indexAttribute.buffer;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getAttributeBuffer(int attributePos_) {
	
	if( attributePos_ == ofShader::POSITION_ATTRIBUTE ) {
		return getVertexBuffer();
	} else if( attributePos_ == ofShader::COLOR_ATTRIBUTE ) {
		return getColorBuffer();
	} else if( attributePos_ == ofShader::NORMAL_ATTRIBUTE ) {
		return getNormalBuffer();
	} else if( attributePos_ == ofShader::TEXCOORD_ATTRIBUTE ) {
		return getTexCoordBuffer();
	}
	
	return customAttributes.at(attributePos_).buffer;
}

//--------------------------------------------------------------
const ofBufferObject & ofVbo::getVertexBuffer() const{
	return positionAttribute.buffer;
}

//--------------------------------------------------------------
const ofBufferObject & ofVbo::getColorBuffer() const{
	return colorAttribute.buffer;
}

//--------------------------------------------------------------
const ofBufferObject & ofVbo::getNormalBuffer() const{
	return normalAttribute.buffer;
}

//--------------------------------------------------------------
const ofBufferObject & ofVbo::getTexCoordBuffer() const{
	return texCoordAttribute.buffer;
}

//--------------------------------------------------------------
const ofBufferObject & ofVbo::getAttributeBuffer(int attributePos_) const{
	if( attributePos_ == ofShader::POSITION_ATTRIBUTE ) {
		return getVertexBuffer();
	} else if( attributePos_ == ofShader::COLOR_ATTRIBUTE ) {
		return getColorBuffer();
	} else if( attributePos_ == ofShader::NORMAL_ATTRIBUTE ) {
		return getNormalBuffer();
	} else if( attributePos_ == ofShader::TEXCOORD_ATTRIBUTE ) {
		return getTexCoordBuffer();
	}
	
	return customAttributes.at(attributePos_).buffer;
}


//--------------------------------------------------------------
const ofBufferObject & ofVbo::getIndexBuffer() const{
	return indexAttribute.buffer;
}

//--------------------------------------------------------------
void ofVbo::bind() const{
	bool programmable = ofIsGLProgrammableRenderer();
	if(programmable && (vaoSupported || !vaoChecked)){
		if(vaoID==0){
			#if defined(TARGET_OPENGLES) && !defined(TARGET_EMSCRIPTEN)
			if(glGenVertexArrays==0 && !vaoChecked){
				glGenVertexArrays = (glGenVertexArraysType)dlsym(RTLD_DEFAULT, "glGenVertexArrays");
				glDeleteVertexArrays = (glDeleteVertexArraysType)dlsym(RTLD_DEFAULT, "glDeleteVertexArrays");
				glBindVertexArray = (glBindVertexArrayType)dlsym(RTLD_DEFAULT, "glBindVertexArray");
				vaoChecked = true;
				vaoSupported = glGenVertexArrays;
			}
			#elif  defined(TARGET_EMSCRIPTEN)
				vaoChecked = true;
				vaoSupported = false;
			#else
				vaoChecked = true;
				vaoSupported = true;
			#endif
			if(vaoSupported) glGenVertexArrays(1, &const_cast<ofVbo*>(this)->vaoID);
			if(vaoID!=0){
				retainVAO(vaoID);
				vaoChanged = true;
			}
		}
		if(vaoSupported) glBindVertexArray(vaoID);
	}else{
		vaoSupported = false;
	}

	if(vaoChanged || !vaoSupported){
		if(bUsingVerts){
			if(!programmable){
				positionAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(positionAttribute.numCoords, GL_FLOAT,
								positionAttribute.stride,
								(void*)positionAttribute.offset);
				#endif
			}else{
				positionAttribute.enable();
			}
		}else if(programmable){
			positionAttribute.disable();
		}

		if(bUsingColors) {
			if(!programmable){
				colorAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(colorAttribute.numCoords, GL_FLOAT,
						colorAttribute.stride,
							   (void*)colorAttribute.offset);
				#endif
			}else{
				colorAttribute.enable();
			}
		}else if(programmable){
			colorAttribute.disable();
		}

		if(bUsingNormals) {
			if(!programmable){
				normalAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, normalAttribute.stride,
								(void*)normalAttribute.offset);
				#endif
			}else{
				normalAttribute.enable();
			}
		}else if(programmable){
			normalAttribute.disable();
		}

		if(bUsingTexCoords) {
			if(!programmable){
				texCoordAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(texCoordAttribute.numCoords,
								  GL_FLOAT, texCoordAttribute.stride,
								  (void*)texCoordAttribute.offset);
				#endif
			}else{
				texCoordAttribute.enable();
			}
		}else if(programmable){
			texCoordAttribute.disable();
		}

        if (bUsingIndices) {
            indexAttribute.bind();
        }

		map<int,VertexAttribute>::const_iterator it;
		for(it = customAttributes.begin();it!=customAttributes.end();it++){
			it->second.enable();
		}

		vaoChanged=false;
	}
}

//--------------------------------------------------------------
void ofVbo::unbind() const{
	if(vaoSupported){
		glBindVertexArray(0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if(!ofIsGLProgrammableRenderer()){
		#ifndef TARGET_PROGRAMMABLE_GL
		if(bUsingColors){
			glDisableClientState(GL_COLOR_ARRAY);
		}
		if(bUsingNormals){
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		if(bUsingTexCoords){
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		#endif
	}
}

//--------------------------------------------------------------
void ofVbo::draw(int drawMode, int first, int total) const{
	ofGetGLRenderer()->draw(*this,drawMode,first,total);
}

//--------------------------------------------------------------
void ofVbo::drawElements(int drawMode, int amt, int offsetelements) const{
	ofGetGLRenderer()->drawElements(*this,drawMode,amt,offsetelements);
}

//--------------------------------------------------------------
void ofVbo::drawInstanced(int drawMode, int first, int total, int primCount) const{
	ofGetGLRenderer()->drawInstanced(*this,drawMode,first,total,primCount);
}

//--------------------------------------------------------------
void ofVbo::drawElementsInstanced(int drawMode, int amt, int primCount) const{
	ofGetGLRenderer()->drawElementsInstanced(*this,drawMode,amt,primCount);
}

//--------------------------------------------------------------
void ofVbo::clear(){

	// clear all fixed function attributes

	clearVertices();
	clearColors();
	clearNormals();
	clearTexCoords();

	// we're not using any of these.
	bUsingVerts = false;
	bUsingColors = false;
	bUsingNormals = false;
	bUsingTexCoords = false;

	// clear all custom attributes.
	customAttributes.clear();

	clearIndices();
	if(vaoID!=0){
		releaseVAO(vaoID);
		vaoID=0;
	}
}


//--------------------------------------------------------------
void ofVbo::clearVertices(){
	positionAttribute = VertexAttribute();
	positionAttribute.location = ofShader::POSITION_ATTRIBUTE;
	bUsingVerts = false;
	totalVerts = 0;
}

//--------------------------------------------------------------
void ofVbo::clearNormals(){
	normalAttribute = VertexAttribute();
	normalAttribute.location = ofShader::NORMAL_ATTRIBUTE;
	bUsingNormals = false;
}

//--------------------------------------------------------------
void ofVbo::clearColors(){
	colorAttribute = VertexAttribute();
	colorAttribute.location = ofShader::COLOR_ATTRIBUTE;
	bUsingColors = false;

}

//--------------------------------------------------------------
void ofVbo::clearTexCoords(){
	texCoordAttribute = VertexAttribute();
	texCoordAttribute.location = ofShader::TEXCOORD_ATTRIBUTE;
	bUsingTexCoords = false;
}

//--------------------------------------------------------------
void ofVbo::clearIndices(){
	if(indexAttribute.isAllocated()){
		indexAttribute = IndexAttribute();
		bUsingIndices = false;
		totalIndices = 0;
	}
}

//--------------------------------------------------------------

void ofVbo::clearAttribute(int attributePos_){

	if (!hasAttribute(attributePos_)) return;

	if (ofIsGLProgrammableRenderer()) {
		if(attributePos_>3){
			customAttributes.erase(attributePos_);
		}else{
			switch (attributePos_){
				case ofShader::POSITION_ATTRIBUTE:
					clearVertices();
					break;
				case ofShader::COLOR_ATTRIBUTE:
					clearColors();
					break;
				case ofShader::NORMAL_ATTRIBUTE:
					clearNormals();
					break;
				case ofShader::TEXCOORD_ATTRIBUTE:
					clearTexCoords();
					break;
				default:
					break;
			}
		}
	}else{
		customAttributes.erase(attributePos_);
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

//--------------------------------------------------------------
bool ofVbo::hasAttribute(int attributePos) const {
	if(ofIsGLProgrammableRenderer()){
		switch(attributePos){
		case ofShader::POSITION_ATTRIBUTE:
			return positionAttribute.isAllocated();
		case ofShader::COLOR_ATTRIBUTE:
			return colorAttribute.isAllocated();
		case ofShader::NORMAL_ATTRIBUTE:
			return normalAttribute.isAllocated();
		case ofShader::TEXCOORD_ATTRIBUTE:
			return texCoordAttribute.isAllocated();
		}
	}
	return (customAttributes.find(attributePos) != customAttributes.end());
}

