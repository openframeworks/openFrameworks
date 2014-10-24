#include "ofBufferObject.h"
#include "ofConstants.h"
#include "ofAppRunner.h"

ofBufferObject::Data::Data()
:id(0)
,size(0)
,lastTarget(GL_ARRAY_BUFFER){
	glGenBuffers(1,&id);
}

ofBufferObject::Data::~Data(){
	glDeleteBuffers(1,&id);
}

ofBufferObject::ofBufferObject()
{

}

void ofBufferObject::allocate(){
	data = shared_ptr<Data>(new Data());
}

void ofBufferObject::allocate(GLsizeiptr bytes, GLenum usage){
	allocate();
	setData(bytes,0,usage);
}

void ofBufferObject::allocate(GLsizeiptr bytes, const void * data, GLenum usage){
	allocate();
	setData(bytes,data,usage);
}

bool ofBufferObject::isAllocated() const{
	return data.get() != NULL;
}

void ofBufferObject::bind(GLenum target) const{
	if(data){
		glBindBuffer(target, data->id);
		data->lastTarget = target;
	}
}

void ofBufferObject::unbind(GLenum target) const{
	glBindBuffer(target, 0);
}

#ifndef TARGET_OPENGLES
void ofBufferObject::bindBase(GLenum target,GLuint index) const{
	if(data){
		glBindBufferBase(target,index,data->id);
		data->lastTarget = target;
	}
}

void ofBufferObject::unbindBase(GLenum target,GLuint index) const{
	glBindBufferBase(target,index,0);
}

void ofBufferObject::bindRange(GLenum target,GLuint index, GLintptr offset, GLsizeiptr size) const{
	if(data){
		glBindBufferRange(target,index,data->id,offset,size);
		data->lastTarget = target;
	}
}

void ofBufferObject::unbindRange(GLenum target,GLuint index) const{
	glBindBufferBase(target,index,0);
}
#endif

GLuint ofBufferObject::getId() const{
	if(data) return data->id;
	else return 0;
}

#ifndef TARGET_OPENGLES
bool isOpenGL45(){
	return (ofGetOpenGLVersionMajor()==4 &&  ofGetOpenGLVersionMinor()>=5) || ofGetOpenGLVersionMajor()>4;
}
#endif

void ofBufferObject::setData(GLsizeiptr bytes, const void * data, GLenum usage){
	if(!this->data) return;
	this->data->size = bytes;
#ifdef glNamedBufferData
	if(isOpenGL45()){
		glNamedBufferData(this->data->id, bytes, data, usage);
		return;
	}
#endif
	bind(this->data->lastTarget);
	glBufferData(this->data->lastTarget, bytes, data, usage);
	unbind(this->data->lastTarget);
}

void ofBufferObject::updateData(GLintptr offset, GLsizeiptr bytes, const void * data){
	if(!data) return;
#ifdef glNamedBufferSubData
	if(isOpenGL45()){
		glNamedBufferSubData(this->data->id,offset,bytes,data);
		return;
	}
#endif
	bind(this->data->lastTarget);
	glBufferSubData(this->data->lastTarget,offset,bytes,data);
	unbind(this->data->lastTarget);
}

#ifndef TARGET_OPENGLES
void * ofBufferObject::map(GLenum access){
#ifdef glMapNamedBuffer
	if(isOpenGL45()){
		return glMapNamedBuffer(data->id,access);
	}
#endif
	if(data->lastTarget==GL_PIXEL_PACK_BUFFER){
		bind(GL_PIXEL_UNPACK_BUFFER);
	}else{
		bind(data->lastTarget);
	}
	return glMapBuffer(data->lastTarget,access);
}

void ofBufferObject::unmap(){
#ifdef glUnmapNamedBuffer
	if(isOpenGL45()){
		glUnmapNamedBuffer(data->id);
	}
#endif
	glUnmapBuffer(data->lastTarget);
	unbind(data->lastTarget);
}

void * ofBufferObject::mapRange(GLintptr offset, GLsizeiptr length, GLenum access){
#ifdef glMapNamedBufferRange
	if(isOpenGL45()){
		return glMapBufferRange(data->id,offset,length,access);
	}
#endif
	bind(data->lastTarget);
	return glMapBufferRange(data->lastTarget,offset,length,access);
}

void ofBufferObject::unmapRange(){
	unmap();
}

void ofBufferObject::copyTo(ofBufferObject & dstBuffer){
	bind(GL_COPY_READ_BUFFER);
	dstBuffer.bind(GL_COPY_WRITE_BUFFER);
	glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,0,size());
	unbind(GL_COPY_READ_BUFFER);
	dstBuffer.unbind(GL_COPY_WRITE_BUFFER);
}
#endif

GLsizeiptr ofBufferObject::size() const{
	if (data) return data->size;
	else return 0;
}
