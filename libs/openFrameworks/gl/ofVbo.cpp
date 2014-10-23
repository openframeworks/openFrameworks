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

	vertexAttribute.numCoords = 3;
	colorAttribute.numCoords = 4;
	normalAttribute.numCoords = 3;
	texCoordAttribute.numCoords = 2;

	vertexAttribute.location = ofShader::POSITION_ATTRIBUTE;
	colorAttribute.location = ofShader::COLOR_ATTRIBUTE;
	normalAttribute.location = ofShader::NORMAL_ATTRIBUTE;
	texCoordAttribute.location = ofShader::TEXCOORD_ATTRIBUTE;

	// tig: note that we set the 'Normalize' flag to true here, assuming that mesh normals need to be
	// normalized while being uploaded to GPU memory.
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	// Normalizing the normals on the shader is probably faster, but sending non-normalized normals is
	// more prone to lead to artifacts difficult to diagnose, especially with the built-in 3D primitives.
	// If you need to optimise this, and you've dug this far through the code, you are most probably
	// able to roll your own client code for binding & rendering vbos anyway...
	normalAttribute.normalize = true;

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

	vertexAttribute = mom.vertexAttribute;
	colorAttribute = mom.colorAttribute;
	normalAttribute = mom.normalAttribute;
	texCoordAttribute = mom.texCoordAttribute;
	indexAttribute = mom.indexAttribute;
	customAttributes = mom.customAttributes;

	totalVerts = mom.totalVerts;
	totalIndices = mom.totalIndices;
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

	vertexAttribute = mom.vertexAttribute;
	colorAttribute = mom.colorAttribute;
	normalAttribute = mom.normalAttribute;
	texCoordAttribute = mom.texCoordAttribute;
	indexAttribute = mom.indexAttribute;
	customAttributes = mom.customAttributes;

	totalVerts = mom.totalVerts;
	totalIndices = mom.totalIndices;
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

#ifdef TARGET_OPENGLES
	if(!vaoChecked){
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
	}
#else
	if(!vaoChecked){
		supportVAOs = ofIsGLProgrammableRenderer();// || glewIsSupported("GL_ARB_vertex_array_object"); <- this should work but has false positives on some cards like emulation in vm's
		vaoChecked = true;
	}
#endif


	if(!vertexAttribute.isAllocated()) {
		bUsingVerts = true;
		vaoChanged=true;
		vertexAttribute.allocate();
		#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
			registerVbo(this);
		#endif
	}

	vertexAttribute.numCoords = numCoords;
	vertexAttribute.stride = stride==0?numCoords*sizeof(float):stride;
	vertexAttribute.offset = 0;
	totalVerts = total;
	
	vertexAttribute.setData(total * vertexAttribute.stride, vert0x, usage);

}

//--------------------------------------------------------------
void ofVbo::setColorData(const ofFloatColor * colors, int total, int usage) {
	setColorData(&colors[0].r,total,usage,sizeof(ofFloatColor));
}

//--------------------------------------------------------------
void ofVbo::setColorData(const float * color0r, int total, int usage, int stride) {
	if(!colorAttribute.isAllocated()) {
		colorAttribute.allocate();
		enableColors();
	}
	colorAttribute.stride = stride==0?4*sizeof(float):stride;
	colorAttribute.offset = 0;
	
	colorAttribute.setData(total * colorAttribute.stride, color0r, usage);
}

//--------------------------------------------------------------
void ofVbo::setNormalData(const ofVec3f * normals, int total, int usage) {
	setNormalData(&normals[0].x,total,usage,sizeof(ofVec3f));
}

//--------------------------------------------------------------
void ofVbo::setNormalData(const float * normal0x, int total, int usage, int stride) {
	if(!normalAttribute.isAllocated()) {
		normalAttribute.allocate();
		enableNormals();
	}
	normalAttribute.stride = stride==0?3*sizeof(float):stride;
	normalAttribute.offset = 0;
	
	normalAttribute.setData(total * normalAttribute.stride, normal0x, usage);
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const ofVec2f * texCoords, int total, int usage) {
	setTexCoordData(&texCoords[0].x,total,usage,sizeof(ofVec2f));
}

//--------------------------------------------------------------
void ofVbo::setTexCoordData(const float * texCoord0x, int total, int usage, int stride) {
	if(!texCoordAttribute.isAllocated()) {
		texCoordAttribute.allocate();
		enableTexCoords();
	}
	texCoordAttribute.stride = stride==0?2*sizeof(float):stride;
	texCoordAttribute.offset = 0;
	
	texCoordAttribute.setData(total * texCoordAttribute.stride, texCoord0x, usage);
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
	if(customAttributes.find(location)==customAttributes.end()){
		customAttributes[location].allocate();
		vaoChanged = true;
	}

	GLsizeiptr size = (stride == 0) ? numCoords * sizeof(float) : stride;
	customAttributes[location].stride = size;
	customAttributes[location].numCoords = numCoords;
	customAttributes[location].offset = 0;

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
	if(vertexAttribute.isAllocated()){
		vertexAttribute.updateData(0, total*vertexAttribute.stride, vert0x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const ofFloatColor * colors, int total) {
	updateColorData(&colors[0].r,total);
}

//--------------------------------------------------------------
void ofVbo::updateColorData(const float * color0r, int total) {
	if(colorAttribute.isAllocated()) {
		colorAttribute.updateData(0, total*colorAttribute.stride, color0r);
	}
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const ofVec3f * normals, int total) {
	updateNormalData(&normals[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateNormalData(const float * normal0x, int total) {
	if(normalAttribute.isAllocated()) {
		normalAttribute.updateData(0, total*normalAttribute.stride, normal0x);
	}
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const ofVec2f * texCoords, int total) {
	updateTexCoordData(&texCoords[0].x,total);
}

//--------------------------------------------------------------
void ofVbo::updateTexCoordData(const float * texCoord0x, int total) {
	if(texCoordAttribute.isAllocated()) {
		texCoordAttribute.updateData(0, total*texCoordAttribute.stride, texCoord0x);
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

void ofVbo::enableColors(){
	if(colorAttribute.isAllocated() && !bUsingColors){
		bUsingColors=true;
		vaoChanged = true;
	}
}

void ofVbo::enableNormals(){
	if(normalAttribute.isAllocated() && !bUsingNormals){
		bUsingNormals=true;
		vaoChanged = true;
	}
}

void ofVbo::enableTexCoords(){
	if(texCoordAttribute.isAllocated() && !bUsingTexCoords){
		bUsingTexCoords=true;
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
	return vertexAttribute.isAllocated();
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
	return vertexAttribute.getId();
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
void ofVbo::setVertexBuffer(ofBufferObject & buffer, int numCoords, int stride, int offset){
	vertexAttribute.buffer = buffer;
	vaoChanged = true;
	bUsingVerts = true;
	vertexAttribute.numCoords = numCoords;
	vertexAttribute.stride = stride==0?numCoords*sizeof(float):stride;
	vertexAttribute.offset = offset;
	//totalVerts = total;
	if(!vaoChecked){
		supportVAOs = ofIsGLProgrammableRenderer();// || glewIsSupported("GL_ARB_vertex_array_object"); <- this should work but has false positives on some cards like emulation in vm's
		vaoChecked = true;
	}
}

//--------------------------------------------------------------
void ofVbo::setColorBuffer(ofBufferObject & buffer, int stride, int offset){
	colorAttribute.buffer = buffer;
	vaoChanged = true;
	bUsingColors = true;
	colorAttribute.stride = stride==0?4*sizeof(float):stride;
	colorAttribute.offset = offset;
}

//--------------------------------------------------------------
void ofVbo::setNormalBuffer(ofBufferObject & buffer, int stride, int offset){
	normalAttribute.buffer = buffer;
	vaoChanged = true;
	bUsingNormals = true;
	normalAttribute.stride = stride==0?3*sizeof(float):stride;
	normalAttribute.offset = offset;
}

//--------------------------------------------------------------
void ofVbo::setTexCoordBuffer(ofBufferObject & buffer, int stride, int offset){
	texCoordAttribute.buffer = buffer;
	vaoChanged = true;
	bUsingNormals = true;
	texCoordAttribute.stride = stride==0?2*sizeof(float):stride;
	texCoordAttribute.offset = offset;
}

//--------------------------------------------------------------
void ofVbo::setIndexBuffer(ofBufferObject & buffer){
	indexAttribute.buffer = buffer;
	vaoChanged = true;
	bUsingIndices = true;
}

//--------------------------------------------------------------
void ofVbo::setAttributeBuffer(int location, ofBufferObject & buffer, int numCoords, int stride, int offset){
	customAttributes[location].buffer = buffer;
	customAttributes[location].numCoords = numCoords;
	customAttributes[location].stride = stride;
	customAttributes[location].offset = offset;
}

//--------------------------------------------------------------
ofBufferObject & ofVbo::getVertexBuffer(){
	return vertexAttribute.buffer;
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
const ofBufferObject & ofVbo::getVertexBuffer() const{
	return vertexAttribute.buffer;
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
const ofBufferObject & ofVbo::getIndexBuffer() const{
	return indexAttribute.buffer;
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
		bool programmable = ofIsGLProgrammableRenderer();
		if(bUsingVerts){
			if(!programmable){
				vertexAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertexAttribute.numCoords, GL_FLOAT, vertexAttribute.stride, (void*)vertexAttribute.offset);
				#endif
			}else{
				vertexAttribute.enable();
			}
		}else if(supportVAOs){
			if(!programmable){
				#ifndef TARGET_PROGRAMMABLE_GL
				glDisableClientState(GL_VERTEX_ARRAY);
				#endif
			}else{
				vertexAttribute.disable();
			}
		}

		if(bUsingColors) {
			if(!programmable){
				colorAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(4, GL_FLOAT, colorAttribute.stride, (void*)colorAttribute.offset);
				#endif
			}else{
				colorAttribute.enable();
			}
		}else if(supportVAOs){
			if(!programmable){
				#ifndef TARGET_PROGRAMMABLE_GL
				glDisableClientState(GL_COLOR_ARRAY);
				#endif
			}else{
				colorAttribute.disable();
			}
		}

		if(bUsingNormals) {
			if(!programmable){
				normalAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, normalAttribute.stride, (void*)normalAttribute.offset);
				#endif
			}else{
				normalAttribute.enable();
			}
		}else if(supportVAOs){
			if(!programmable){
				#ifndef TARGET_PROGRAMMABLE_GL
				glDisableClientState(GL_NORMAL_ARRAY);
				#endif
			}else{
				normalAttribute.disable();
			}
		}

		if(bUsingTexCoords) {
			if(!programmable){
				texCoordAttribute.bind();
				#ifndef TARGET_PROGRAMMABLE_GL
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, texCoordAttribute.stride, (void*)texCoordAttribute.offset);
				#endif
			}else{
				texCoordAttribute.enable();
			}
		}else if(supportVAOs){
			if(!programmable){
				#ifndef TARGET_PROGRAMMABLE_GL
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				#endif
			}else{
				texCoordAttribute.disable();
			}
		}
        
        if (bUsingIndices) {
            indexAttribute.bind();
        }

		map<int,VertexAttribute>::const_iterator it;
		for(it=customAttributes.begin();it!=customAttributes.end();it++){
			it->second.enable();
			if(ofIsGLProgrammableRenderer()){
				bUsingVerts |= it->first == ofShader::POSITION_ATTRIBUTE;
				bUsingColors |= it->first == ofShader::COLOR_ATTRIBUTE;
				bUsingTexCoords |= it->first == ofShader::TEXCOORD_ATTRIBUTE;
				bUsingNormals |= it->first == ofShader::NORMAL_ATTRIBUTE;
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
	bBound   = false;
}

//--------------------------------------------------------------
void ofVbo::draw(int drawMode, int first, int total) const{
	if(vertexAttribute.isAllocated()) {
		bool wasBinded = bBound;
		if(!wasBinded) bind();
		glDrawArrays(drawMode, first, total);
		if(!wasBinded) unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::drawElements(int drawMode, int amt) const{
	if(vertexAttribute.isAllocated()){
		bool wasBinded = bBound;
		if(!wasBinded) bind();
#ifdef TARGET_OPENGLES
        glDrawElements(drawMode, amt, GL_UNSIGNED_SHORT, NULL);
#else
        glDrawElements(drawMode, amt, GL_UNSIGNED_INT, NULL);
#endif
		
		if(!wasBinded) unbind();
	}
}

//--------------------------------------------------------------
// tig: this, being a key feature of OpenGL VBOs, allows to render massive
// amounts of geometry simultaneously without clogging the memory bus;
// as discussed in: http://poniesandlight.co.uk/code/ofxVboMeshInstanced/
void ofVbo::drawInstanced(int drawMode, int first, int total, int primCount) const{
	if(vertexAttribute.isAllocated()) {
		bool wasBinded = bBound;
		if(!wasBinded) bind();
#ifdef TARGET_OPENGLES
		// todo: activate instancing once OPENGL ES supports instancing, starting with version 3.0
		// unfortunately there is currently no easy way within oF to query the current OpenGL version.
		// https://www.khronos.org/opengles/sdk/docs/man3/xhtml/glDrawElementsInstanced.xml
		ofLogWarning("ofVbo") << "drawInstanced(): hardware instancing is not supported on OpenGL ES < 3.0";
		// glDrawArraysInstanced(drawMode, first, total, primCount);
#else
		glDrawArraysInstanced(drawMode, first, total, primCount);
#endif
		if(!wasBinded) unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::drawElementsInstanced(int drawMode, int amt, int primCount) const{
	if(vertexAttribute.isAllocated()){
		bool wasBinded = bBound;
		if(!wasBinded) bind();
#ifdef TARGET_OPENGLES
        // todo: activate instancing once OPENGL ES supports instancing, starting with version 3.0
        // unfortunately there is currently no easy way within oF to query the current OpenGL version.
        // https://www.khronos.org/opengles/sdk/docs/man3/xhtml/glDrawElementsInstanced.xml
        ofLogWarning("ofVbo") << "drawElementsInstanced(): hardware instancing is not supported on OpenGL ES < 3.0";
        // glDrawElementsInstanced(drawMode, amt, GL_UNSIGNED_SHORT, NULL, primCount);
#else
        glDrawElementsInstanced(drawMode, amt, GL_UNSIGNED_INT, NULL, primCount);
#endif
		
		if(!wasBinded) unbind();
	}
}

//--------------------------------------------------------------
void ofVbo::clear(){
	clearVertices();
	clearNormals();
	clearColors();
	clearTexCoords();
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
	if(vertexAttribute.isAllocated()){
		vertexAttribute = VertexAttribute();
		bUsingVerts = false;
		totalVerts = 0;
	}
}

//--------------------------------------------------------------
void ofVbo::clearNormals(){
	if(normalAttribute.isAllocated()){
		normalAttribute = VertexAttribute();
		bUsingNormals = false;
	}
}

//--------------------------------------------------------------
void ofVbo::clearColors(){
	if(colorAttribute.isAllocated()){
		colorAttribute = VertexAttribute();
		bUsingColors = false;
	}
}

//--------------------------------------------------------------
void ofVbo::clearTexCoords(){
	if(texCoordAttribute.isAllocated()){
		texCoordAttribute = VertexAttribute();
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
void ofVbo::disableVAOs(){
	supportVAOs = false;
	vaoChecked = true;
}

//--------------------------------------------------------------
void ofVbo::enableVAOs(){
	supportVAOs = true;
	vaoChecked = false;
}
