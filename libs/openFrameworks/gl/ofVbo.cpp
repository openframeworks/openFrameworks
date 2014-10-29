//TODO:
//GL Error checking
// handle idling of arrays: possibly let gl create memory and use map buffers for streaming
// index updating/deleting?
// setVertexData with float* should know about ofVec3f vs ofVec2f?


#include "ofUtils.h"
#include "ofVbo.h"
#include "ofShader.h"
#include "ofGLProgrammableRenderer.h"

#include <map>
#include <set>

bool ofVbo::vaoChecked = false;
bool ofVbo::supportVAOs = true;

#ifdef TARGET_OPENGLES
	#include <dlfcn.h>
	typedef void (* glGenVertexArraysType) (GLsizei n,  GLuint *arrays);
	glGenVertexArraysType glGenVertexArraysFunc;
	#define glGenVertexArrays								glGenVertexArraysFunc

	typedef void (* glDeleteVertexArraysType) (GLsizei n,  GLuint *arrays);
	glDeleteVertexArraysType glDeleteVertexArraysFunc;
	#define glDeleteVertexArrays							glDeleteVertexArraysFunc

	typedef void (* glBindVertexArrayType) (GLuint array);
	glBindVertexArrayType glBindVertexArrayFunc;
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
			glDeleteVertexArrays(1, &id);
			getVAOIds().erase(id);
		}
	}else{
		ofLogWarning("ofVbo") << "releaseVAO(): something's wrong here, releasing unknown vertex array object id " << id;
		glDeleteVertexArrays(1, &id);
	}
}

#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
static set<ofVbo*> & allVbos(){
	static set<ofVbo*> * allVbos = new set<ofVbo*>;
	return *allVbos;
}

static void registerVbo(ofVbo*vbo){
	allVbos().insert(vbo);
}

static void unregisterVbo(ofVbo*vbo){
	allVbos().erase(vbo);
}

void ofRegenerateAllVbos(){
	set<ofVbo*>::iterator it;
	for(it=allVbos().begin();it!=allVbos().end();it++){
		(*it)->clear();
	}
}
#endif

//--------------------------------------------------------------
ofVbo::VertexAttribute::VertexAttribute()
:stride(0)
,offset(0)
,numCoords(0)
,location(0)
,normalize(false){

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
void ofVbo::VertexAttribute::enable() const{
	bind();
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, numCoords, GL_FLOAT, normalize?GL_TRUE:GL_FALSE, stride, (void*)offset);
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
	buffer.setData(bytes,data,usage);
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
	bBound   = false;
}

ofVbo::ofVbo(const ofVbo & mom){
	bUsingVerts = mom.bUsingVerts;
	bUsingTexCoords = mom.bUsingTexCoords;
	bUsingColors = mom.bUsingColors;
	bUsingNormals = mom.bUsingNormals;
	bUsingIndices = mom.bUsingIndices;

	mFixedFunctionPositionAttribute = mom.mFixedFunctionPositionAttribute;
	mFixedFunctionColorAttribute = mom.mFixedFunctionColorAttribute;
	mFixedFunctionTexCoordAttribute = mom.mFixedFunctionTexCoordAttribute;
	mFixedFunctionNormalAttribute = mom.mFixedFunctionNormalAttribute;

	customAttributes = mom.customAttributes;
	
	totalVerts = mom.totalVerts;
	totalIndices = mom.totalIndices;
	indexAttribute = mom.indexAttribute;
	bBound   = mom.bBound;

	if(supportVAOs){
		vaoID	   = mom.vaoID;
		retainVAO(vaoID);
		vaoChanged = mom.vaoChanged;
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

	mFixedFunctionPositionAttribute = mom.mFixedFunctionPositionAttribute;
	mFixedFunctionColorAttribute = mom.mFixedFunctionColorAttribute;
	mFixedFunctionTexCoordAttribute = mom.mFixedFunctionTexCoordAttribute;
	mFixedFunctionNormalAttribute = mom.mFixedFunctionNormalAttribute;

	customAttributes = mom.customAttributes;

	totalVerts = mom.totalVerts;
	totalIndices = mom.totalIndices;
	indexAttribute = mom.indexAttribute;
	bBound   = mom.bBound;

	if(supportVAOs){
		vaoID	   = mom.vaoID;
		retainVAO(vaoID);
		vaoChanged = mom.vaoChanged;
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
void ofVbo::setVertexData(const ofVec3f * verts, int total, int usage) {
	setVertexData(&verts[0].x,3,total,usage,sizeof(ofVec3f));
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const ofVec2f * verts, int total, int usage) {
	setVertexData(&verts[0].x,2,total,usage,sizeof(ofVec2f));
}

//--------------------------------------------------------------
void ofVbo::setVertexData(const float * vert0x, int numCoords, int total, int usage, int stride) {
	if (isProgrammable()) {
		setAttributeData(ofShader::POSITION_ATTRIBUTE, vert0x, numCoords, total, usage, stride);
	} else {
		#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
			registerVbo(this);
		#endif
		setVertexAttributeData(mFixedFunctionPositionAttribute, vert0x, numCoords, total, usage, stride);
		bUsingVerts = true;
		totalVerts = total;
	}
}

//--------------------------------------------------------------
void ofVbo::setColorData(const ofFloatColor * colors, int total, int usage) {
	setColorData(&colors[0].r,total,usage,sizeof(ofFloatColor));
}

//--------------------------------------------------------------
void ofVbo::setColorData(const float * color0r, int total, int usage, int stride) {
	if (isProgrammable()) {
		setAttributeData(ofShader::COLOR_ATTRIBUTE, color0r, 4, total, usage, stride);
	} else {
		setVertexAttributeData(mFixedFunctionColorAttribute, color0r, 4, total, usage, stride);
	}
	enableColors();
}

//--------------------------------------------------------------
void ofVbo::setNormalData(const ofVec3f * normals, int total, int usage) {
	setNormalData(&normals[0].x,total,usage,sizeof(ofVec3f));
}

//--------------------------------------------------------------
void ofVbo::setNormalData(const float * normal0x, int total, int usage, int stride) {
	if (isProgrammable()) {
		setAttributeData(ofShader::NORMAL_ATTRIBUTE, normal0x, 3, total, usage, stride);
		// tig: note that we set the 'Normalize' flag to true here, assuming that mesh normals need to be
		// normalized while being uploaded to GPU memory.
		// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
		// Normalizing the normals on the shader is probably faster, but sending non-normalized normals is
		// more prone to lead to artifacts difficult to diagnose, especially with the built-in 3D primitives.
		// If you need to optimise this, and you've dug this far through the code, you are most probably
		// able to roll your own client code for binding & rendering vbos anyway...
		customAttributes[ofShader::NORMAL_ATTRIBUTE].normalize = true;
	} else {
		setVertexAttributeData(mFixedFunctionNormalAttribute, normal0x, 3, total, usage, stride);
		mFixedFunctionNormalAttribute.normalize = true;
	}
	enableNormals();
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const ofVec2f * texCoords, int total, int usage) {
		setTexCoordData(&texCoords[0].x,total, usage, sizeof(ofVec2f));
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const float * texCoord0x, int total, int usage, int stride) {
	if (isProgrammable()) {
		setAttributeData(ofShader::TEXCOORD_ATTRIBUTE, texCoord0x, 2, total, usage, stride);
	} else {
		setVertexAttributeData(mFixedFunctionTexCoordAttribute, texCoord0x, 2, total, usage, stride);
	}
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
void ofVbo::setAttributeData(int location, const float * attrib0x, int numCoords, int total, int usage, int stride){

	if (!vaoChecked) checkVAO();
	
	bool programmable = isProgrammable();
	
	if(!hasAttribute(location)){
		customAttributes[location].allocate();
		vaoChanged = true;
		
		if (programmable) {
			// makes sure default shaders bind to correct locations
			
			switch (location){
				case ofShader::POSITION_ATTRIBUTE: {
				#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
					registerVbo(this);
				#endif
					bUsingVerts = true;
				}
					break;
				case ofShader::COLOR_ATTRIBUTE:
					bUsingColors = true;
					break;
				case ofShader::NORMAL_ATTRIBUTE: {
					bUsingNormals = true;
				}
					break;
				case ofShader::TEXCOORD_ATTRIBUTE:
					bUsingTexCoords = true;
					break;
				default:
					break;
			}
		}
	}

	if(programmable && location == ofShader::POSITION_ATTRIBUTE){
		totalVerts = total;
	}

	GLsizeiptr size = (stride == 0) ? numCoords * sizeof(float) : stride;
	customAttributes[location].stride = size;
	customAttributes[location].numCoords = numCoords;
	customAttributes[location].offset = 0;
	customAttributes[location].location = location;
	customAttributes[location].setData(total * size, attrib0x, usage);
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
	if (isProgrammable()){
		updateAttributeData(ofShader::POSITION_ATTRIBUTE, vert0x, total);
	} else {
		mFixedFunctionPositionAttribute.updateData(0, total * mFixedFunctionPositionAttribute.stride, vert0x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const ofFloatColor * colors, int total) {
	updateColorData(&colors[0].r,total);
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const float * color0r, int total) {
	if (isProgrammable()){
		updateAttributeData(ofShader::COLOR_ATTRIBUTE, color0r, total);
	} else {
		mFixedFunctionColorAttribute.updateData(0, total * mFixedFunctionColorAttribute.stride, color0r);
	}
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const ofVec3f * normals, int total) {
	updateNormalData(&normals[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const float * normal0x, int total) {
	if (isProgrammable()){
		updateAttributeData(ofShader::NORMAL_ATTRIBUTE, normal0x, total);
	} else {
		mFixedFunctionNormalAttribute.updateData(0, total * mFixedFunctionNormalAttribute.stride, normal0x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const ofVec2f * texCoords, int total) {
	updateTexCoordData(&texCoords[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const float * texCoord0x, int total) {
	if (isProgrammable()){
		updateAttributeData(ofShader::TEXCOORD_ATTRIBUTE, texCoord0x, total);
	} else {
		mFixedFunctionTexCoordAttribute.updateData(0, total * mFixedFunctionTexCoordAttribute.stride, texCoord0x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateIndexData(const ofIndexType * indices, int total) {
	if(indexAttribute.isAllocated()) {
		indexAttribute.updateData(0, total*sizeof(ofIndexType), indices);
	}
}

void ofVbo::updateAttributeData(int location, const float * attr0x, int total){
	if(customAttributes.find(location)!=customAttributes.end() && customAttributes[location].isAllocated()) {
		customAttributes[location].updateData(0, total*customAttributes[location].stride, attr0x);
	}
}

void ofVbo::enableColors() {
	if (isProgrammable()) {
		if ( hasAttribute(ofShader::COLOR_ATTRIBUTE) && !bUsingColors) {
			bUsingColors=true;
			vaoChanged = true;
		}
	} else if (!bUsingColors && mFixedFunctionColorAttribute.isAllocated()) {
		bUsingColors=true;
		vaoChanged = true;
	}
}

void ofVbo::enableNormals(){
	if (isProgrammable()) {
		if(hasAttribute(ofShader::NORMAL_ATTRIBUTE) && !bUsingNormals){
			bUsingNormals=true;
			vaoChanged = true;
		}
	}
	else if (!bUsingNormals && mFixedFunctionNormalAttribute.isAllocated()) {
		bUsingNormals=true;
		vaoChanged = true;
	}
}

void ofVbo::enableTexCoords(){
	if (isProgrammable()) {
		if(hasAttribute(ofShader::TEXCOORD_ATTRIBUTE) && !bUsingTexCoords){
			bUsingTexCoords=true;
			vaoChanged = true;
		}
	} else if (!bUsingTexCoords && mFixedFunctionTexCoordAttribute.isAllocated()){
		
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
	if (isProgrammable()) {
		return hasAttribute(ofShader::POSITION_ATTRIBUTE);
	} else {
		return mFixedFunctionPositionAttribute.isAllocated();
	}
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
	if (isProgrammable()){
		return getAttributeId(ofShader::POSITION_ATTRIBUTE);
	} else {
		return mFixedFunctionPositionAttribute.getId();
	}
}

//--------------------------------------------------------------
GLuint ofVbo::getColorId() const{
	if (isProgrammable()){
		return getAttributeId(ofShader::COLOR_ATTRIBUTE);
	} else {
		return mFixedFunctionColorAttribute.getId();
	}
}

//--------------------------------------------------------------
GLuint ofVbo::getNormalId() const {
	if (isProgrammable()){
		return getAttributeId(ofShader::NORMAL_ATTRIBUTE);
	} else {
		return mFixedFunctionNormalAttribute.getId();
	}
}

//--------------------------------------------------------------
GLuint ofVbo::getTexCoordId() const {
	if (isProgrammable()) {
		return getAttributeId(ofShader::TEXCOORD_ATTRIBUTE);
	} else {
		return mFixedFunctionTexCoordAttribute.getId();
	}
}

//--------------------------------------------------------------
GLuint ofVbo::getIndexId() const {
	return indexAttribute.getId();
}

//--------------------------------------------------------------
GLuint ofVbo::getAttributeId(int location_) const {
	if (!hasAttribute(location_)) {
		ofLogWarning() << "No attribute id found for attribute pos: " << location_;
		return 0;
	}
	return (customAttributes.at(location_).getId());
}

//--------------------------------------------------------------
void ofVbo::setVertexBuffer(ofBufferObject & buffer, int numCoords, int stride, int offset){
	if (isProgrammable()) {
		setAttributeBuffer(ofShader::POSITION_ATTRIBUTE, buffer, numCoords, stride, offset);
	} else {
		setVertexAttributeBuffer(mFixedFunctionPositionAttribute, buffer, numCoords, stride, offset);
		bUsingVerts = true;
		// TODO: Check: we have no perfect way of knowing the new number of total vertices,
		// since the buffer does not tell us, so we try to calculate based on the data size
		// and the number of coordinates.
		totalVerts = buffer.size() / (numCoords * sizeof(float));
	}

}

//--------------------------------------------------------------
void ofVbo::setColorBuffer(ofBufferObject & buffer, int stride, int offset){
	if (isProgrammable()) {
		setAttributeBuffer(ofShader::COLOR_ATTRIBUTE, buffer, 4, stride, offset);
	} else {
		setVertexAttributeBuffer(mFixedFunctionColorAttribute, buffer, 4, stride, offset);
		enableColors();
	}
}

//--------------------------------------------------------------
void ofVbo::setNormalBuffer(ofBufferObject & buffer, int stride, int offset){
	if (isProgrammable()) {
		setAttributeBuffer(ofShader::NORMAL_ATTRIBUTE, buffer, 3, stride, offset);
	} else {
		setVertexAttributeBuffer(mFixedFunctionNormalAttribute, buffer, 3, stride, offset);
		enableNormals();
	}
}

//--------------------------------------------------------------
void ofVbo::setTexCoordBuffer(ofBufferObject & buffer, int stride, int offset){
	if (isProgrammable()) {
		setAttributeBuffer(ofShader::TEXCOORD_ATTRIBUTE, buffer, 2, stride, offset);
	} else {
		setVertexAttributeBuffer(mFixedFunctionTexCoordAttribute, buffer, 2, stride, offset);
		enableTexCoords();
	}
}

//--------------------------------------------------------------
void ofVbo::setIndexBuffer(ofBufferObject & buffer){
	indexAttribute.buffer = buffer;
	vaoChanged = true;
	bUsingIndices = true;
}

//--------------------------------------------------------------
void ofVbo::setAttributeBuffer(int location, ofBufferObject & buffer, int numCoords, int stride, int offset){

	if (isProgrammable()) {
		switch (location){
			case ofShader::POSITION_ATTRIBUTE:
				// if the position buffer has been updated, we need to make sure
				// to update the number of vertices for this vbo. we calculate this
				// based on hte buffer size (given in bytes), the number of coords
				// and the sizeof of float
				totalVerts = buffer.size() / (numCoords * sizeof(float));
				bUsingVerts = true;
				break;
			case ofShader::COLOR_ATTRIBUTE:
				bUsingColors = true;
				break;
			case ofShader::NORMAL_ATTRIBUTE:
				bUsingNormals = true;
				break;
			case ofShader::TEXCOORD_ATTRIBUTE:
				bUsingTexCoords = true;
				break;
			default:
				break;
		}
	}

	setVertexAttributeBuffer(customAttributes[location], buffer, numCoords, stride, offset);
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getVertexBuffer(){
	return isProgrammable() ? getAttributeBuffer(ofShader::POSITION_ATTRIBUTE) : mFixedFunctionPositionAttribute.buffer;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getColorBuffer(){
	return isProgrammable() ? getAttributeBuffer(ofShader::COLOR_ATTRIBUTE) : mFixedFunctionColorAttribute.buffer;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getNormalBuffer(){
	return isProgrammable() ? getAttributeBuffer(ofShader::NORMAL_ATTRIBUTE) : mFixedFunctionNormalAttribute.buffer;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getTexCoordBuffer(){
	return isProgrammable() ? getAttributeBuffer(ofShader::TEXCOORD_ATTRIBUTE) : mFixedFunctionTexCoordAttribute.buffer;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getIndexBuffer(){
	return indexAttribute.buffer;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getAttributeBuffer(int attributePos_) {
	return customAttributes.at(attributePos_).buffer;
}

//--------------------------------------------------------------
const ofBufferObject & ofVbo::getVertexBuffer() const{
	return getVertexBuffer();
}

//--------------------------------------------------------------
const ofBufferObject & ofVbo::getColorBuffer() const{
	return getColorBuffer();
}

//--------------------------------------------------------------
const ofBufferObject & ofVbo::getNormalBuffer() const{
	return getNormalBuffer();
}

//--------------------------------------------------------------
const ofBufferObject & ofVbo::getTexCoordBuffer() const{
	return getTexCoordBuffer();
}

//--------------------------------------------------------------
const ofBufferObject & ofVbo::getAttributeBuffer(int attributePos_) const{
	return getAttributeBuffer(attributePos_);
}


//--------------------------------------------------------------
const ofBufferObject & ofVbo::getIndexBuffer() const{
	return getIndexBuffer();
}

//--------------------------------------------------------------
void ofVbo::bind() const{
	if(supportVAOs){
		if(vaoID==0){
			glGenVertexArrays(1, &const_cast<ofVbo*>(this)->vaoID);
			if(vaoID!=0){
				retainVAO(vaoID);
			}else{
				supportVAOs = false;
				ofLogVerbose("ofVbo") << "bind(): error allocating VAO, disabling VAO support";
			}
		}

		glBindVertexArray(vaoID);
	}

	if(vaoChanged || !supportVAOs){
		bool programmable = isProgrammable();
		if(bUsingVerts){
			if(!programmable){
				customAttributes.at(ofShader::POSITION_ATTRIBUTE).bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(mFixedFunctionPositionAttribute.numCoords, GL_FLOAT,
								mFixedFunctionPositionAttribute.stride,
								(void*)mFixedFunctionPositionAttribute.offset);
				#endif
			}
		}else if(supportVAOs){
			if(!programmable){
				#ifndef TARGET_PROGRAMMABLE_GL
				glDisableClientState(GL_VERTEX_ARRAY);
				#endif
			} else if (hasAttribute(ofShader::POSITION_ATTRIBUTE)){
				customAttributes.at(ofShader::POSITION_ATTRIBUTE).disable();
			}
		}

		if(bUsingColors) {
			if(!programmable){
				mFixedFunctionColorAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(mFixedFunctionColorAttribute.numCoords, GL_FLOAT,
							   mFixedFunctionColorAttribute.stride,
							   (void*)mFixedFunctionColorAttribute.offset);
				#endif
			}
		}else if(supportVAOs){
			if(!programmable){
				#ifndef TARGET_PROGRAMMABLE_GL
				glDisableClientState(GL_COLOR_ARRAY);
				#endif
			} else if (hasAttribute(ofShader::COLOR_ATTRIBUTE)) {
				customAttributes.at(ofShader::COLOR_ATTRIBUTE).disable();
			}
		}

		if(bUsingNormals) {
			if(!programmable){
				mFixedFunctionNormalAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, mFixedFunctionNormalAttribute.stride,
								(void*)mFixedFunctionNormalAttribute.offset);
				#endif
			}
		}else if(supportVAOs){
			if(!programmable){
				#ifndef TARGET_PROGRAMMABLE_GL
				glDisableClientState(GL_NORMAL_ARRAY);
				#endif
			} else if (hasAttribute(ofShader::NORMAL_ATTRIBUTE)) {
				customAttributes.at(ofShader::NORMAL_ATTRIBUTE).disable();
			}
		}

		if(bUsingTexCoords) {
			if(!programmable){
				mFixedFunctionTexCoordAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(mFixedFunctionTexCoordAttribute.numCoords,
								  GL_FLOAT, mFixedFunctionTexCoordAttribute.stride,
								  (void*)mFixedFunctionTexCoordAttribute.offset);
				#endif
			}
		}else if(supportVAOs){
			if(!programmable){
				#ifndef TARGET_PROGRAMMABLE_GL
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				#endif
			} else if (hasAttribute(ofShader::TEXCOORD_ATTRIBUTE)) {
				customAttributes.at(ofShader::TEXCOORD_ATTRIBUTE).disable();
			}
		}
        
        if (bUsingIndices) {
            indexAttribute.bind();
        }

		map<int,VertexAttribute>::const_iterator it;
		for(it = customAttributes.begin();it!=customAttributes.end();it++){
			if(!programmable){
				it->second.enable();
			} else {
				if((bUsingVerts && it->first == ofShader::POSITION_ATTRIBUTE) ||
					(bUsingColors && it->first == ofShader::COLOR_ATTRIBUTE) ||
					(bUsingTexCoords && it->first == ofShader::TEXCOORD_ATTRIBUTE) ||
					(bUsingNormals && it->first == ofShader::NORMAL_ATTRIBUTE) ||
					it->first > 3){
						it->second.enable();
				}
			}
		}

		vaoChanged=false;
	}

	shared_ptr<ofGLProgrammableRenderer> renderer = ofGetGLProgrammableRenderer();
	if(renderer){
		renderer->setAttributes(bUsingVerts,bUsingColors,bUsingTexCoords,bUsingNormals);
	}
	bBound   = true;
}

//--------------------------------------------------------------
void ofVbo::unbind() const{
	if(supportVAOs){
		glBindVertexArray(0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if(!isProgrammable()){
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
	bBound   = false;
}

//--------------------------------------------------------------
void ofVbo::draw(int drawMode, int first, int total) const{
	if(hasAttribute(ofShader::POSITION_ATTRIBUTE)) {
		bool wasBound = bBound;
		if(!wasBound) bind();
		glDrawArrays(drawMode, first, total);
		if(!wasBound) unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::drawElements(int drawMode, int amt) const{
	if(hasAttribute(ofShader::POSITION_ATTRIBUTE)) {
		bool wasBound = bBound;
		if(!wasBound) bind();
#ifdef TARGET_OPENGLES
        glDrawElements(drawMode, amt, GL_UNSIGNED_SHORT, NULL);
#else
        glDrawElements(drawMode, amt, GL_UNSIGNED_INT, NULL);
#endif
		
		if(!wasBound) unbind();
	}
}

//--------------------------------------------------------------
// tig: this, being a key feature of OpenGL VBOs, allows to render massive
// amounts of geometry simultaneously without clogging the memory bus;
// as discussed in: http://poniesandlight.co.uk/code/ofxVboMeshInstanced/
void ofVbo::drawInstanced(int drawMode, int first, int total, int primCount) const{
	if(hasAttribute(ofShader::POSITION_ATTRIBUTE)) {
		bool wasBound = bBound;
		if(!wasBound) bind();
#ifdef TARGET_OPENGLES
		// todo: activate instancing once OPENGL ES supports instancing, starting with version 3.0
		// unfortunately there is currently no easy way within oF to query the current OpenGL version.
		// https://www.khronos.org/opengles/sdk/docs/man3/xhtml/glDrawElementsInstanced.xml
		ofLogWarning("ofVbo") << "drawInstanced(): hardware instancing is not supported on OpenGL ES < 3.0";
		// glDrawArraysInstanced(drawMode, first, total, primCount);
#else
		glDrawArraysInstanced(drawMode, first, total, primCount);
#endif
		if(!wasBound) unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::drawElementsInstanced(int drawMode, int amt, int primCount) const{
	if(hasAttribute(ofShader::POSITION_ATTRIBUTE)) {
		bool wasBound = bBound;
		if(!wasBound) bind();
#ifdef TARGET_OPENGLES
        // todo: activate instancing once OPENGL ES supports instancing, starting with version 3.0
        // unfortunately there is currently no easy way within oF to query the current OpenGL version.
        // https://www.khronos.org/opengles/sdk/docs/man3/xhtml/glDrawElementsInstanced.xml
        ofLogWarning("ofVbo") << "drawElementsInstanced(): hardware instancing is not supported on OpenGL ES < 3.0";
        // glDrawElementsInstanced(drawMode, amt, GL_UNSIGNED_SHORT, NULL, primCount);
#else
        glDrawElementsInstanced(drawMode, amt, GL_UNSIGNED_INT, NULL, primCount);
#endif
		
		if(!wasBound) unbind();
	}
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
	if(supportVAOs && vaoID!=0){
		releaseVAO(vaoID);
		vaoID=0;
	}
	#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
		unregisterVbo(this);
	#endif
}


//--------------------------------------------------------------
void ofVbo::clearVertices(){
	if (isProgrammable()) {
		clearAttribute(ofShader::POSITION_ATTRIBUTE);
	} else {
		mFixedFunctionPositionAttribute = VertexAttribute();
		bUsingVerts = false;
		totalVerts = 0;
	}
}

//--------------------------------------------------------------
void ofVbo::clearNormals(){
	if (isProgrammable()){
		clearAttribute(ofShader::NORMAL_ATTRIBUTE);
	} else {
		mFixedFunctionNormalAttribute = VertexAttribute();
		bUsingNormals = false;
	}
}

//--------------------------------------------------------------
void ofVbo::clearColors(){
	if (isProgrammable()){
		clearAttribute(ofShader::COLOR_ATTRIBUTE);
	} else {
		mFixedFunctionColorAttribute = VertexAttribute();
		bUsingColors = false;
	}
	
}

//--------------------------------------------------------------
void ofVbo::clearTexCoords(){
	if (isProgrammable()){
		clearAttribute(ofShader::TEXCOORD_ATTRIBUTE);
	} else {
		mFixedFunctionTexCoordAttribute = VertexAttribute();
		bUsingTexCoords = false;
	}
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

	// --------! invariant: attribute exists
	
	customAttributes.erase(attributePos_);
	
	if (isProgrammable()) {
		switch (attributePos_){
			case ofShader::POSITION_ATTRIBUTE:
				bUsingVerts = false;
				totalVerts = 0;
				break;
			case ofShader::COLOR_ATTRIBUTE:
				bUsingColors = false;
				break;
			case ofShader::NORMAL_ATTRIBUTE:
				bUsingNormals = false;
				break;
			case ofShader::TEXCOORD_ATTRIBUTE:
				bUsingTexCoords = false;
				break;
			default:
				break;
		}
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

void ofVbo::checkVAO(){
	if (vaoChecked) return;
#ifdef TARGET_OPENGLES
	if(ofGetGLProgrammableRenderer()){
		glGenVertexArrays = (glGenVertexArraysType)dlsym(RTLD_DEFAULT, "glGenVertexArrays");
		glDeleteVertexArrays =  (glDeleteVertexArraysType)dlsym(RTLD_DEFAULT, "glDeleteVertexArrays");
		glBindVertexArray =  (glBindVertexArrayType)dlsym(RTLD_DEFAULT, "glBindVertexArrayArrays");
	}else{
		glGenVertexArrays = (glGenVertexArraysType)dlsym(RTLD_DEFAULT, "glGenVertexArraysOES");
		glDeleteVertexArrays =  (glDeleteVertexArraysType)dlsym(RTLD_DEFAULT, "glDeleteVertexArraysOES");
		glBindVertexArray =  (glBindVertexArrayType)dlsym(RTLD_DEFAULT, "glBindVertexArrayArraysOES");
	}
	vaoChecked = true;
	supportVAOs = glGenVertexArrays && glDeleteVertexArrays && glBindVertexArray;
#else
	supportVAOs = isProgrammable();// || glewIsSupported("GL_ARB_vertex_array_object"); <- this should work but has false positives on some cards like emulation in vm's
	vaoChecked = true;
#endif
}

//--------------------------------------------------------------
void ofVbo::disableVAOs(){
	supportVAOs = false;
	vaoChecked = true;
}

//--------------------------------------------------------------
void ofVbo::enableVAOs(){
	supportVAOs = true;
	vaoChecked = false;
}

//--------------------------------------------------------------

bool ofVbo::isProgrammable(){
	// we cache this for speed, and also to make sure that upon ofExit() we don't have to deal with the
	// embarassment of a non-existent renderer to query.
	static bool programmable = ofIsGLProgrammableRenderer();
	return programmable;
}
