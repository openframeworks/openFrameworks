#include "ofBufferObject.h"
#include "ofConstants.h"
#include "ofAppRunner.h"

ofBufferObject::Data::Data()
:id(0)
,size(0)
,lastTarget(GL_ARRAY_BUFFER)
,useDSA(false){
	
	// tig: glGenBuffers does not actually create a buffer, it just 
	//      returns the next available name, and only a subsequent 
	//      call to bind() will actualy initialize the buffer in
	//      memory. 
	//
	//      This is why, for direct state access, we need to call
	//      glCreateBuffers(), so that the buffer is initialized
	//      when we pin data to it using setData()
	// 
	//      see also: https://www.opengl.org/registry/specs/ARB/direct_state_access.txt
#ifdef GLEW_ARB_direct_state_access
	if (ofGLCheckExtension("GL_ARB_direct_state_access")) {
		useDSA = true;
		// the above condition is only true if GLEW can provide us
		// with direct state access methods. we use this to test
		// whether the driver is OpenGL 4.5 ready.
		glCreateBuffers(1,&id);
		return;
	}
#endif

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
	return data.get() != nullptr;
}

void ofBufferObject::bind(GLenum target) const{
	if(data){
		glBindBuffer(target, data->id);
		data->lastTarget = target;
		data->isBound = true;
	}
}

void ofBufferObject::unbind(GLenum target) const{
	glBindBuffer(target, 0);
	if(data){
		data->isBound = false;
	}
}

#ifndef TARGET_OPENGLES
void ofBufferObject::bindBase(GLenum target,GLuint index) const{
	if(data){
		glBindBufferBase(target,index,data->id);
		data->lastTarget = target;
		data->isBound = true;
	}
}

void ofBufferObject::unbindBase(GLenum target,GLuint index) const{
	glBindBufferBase(target,index,0);
	if(data){
		data->isBound = false;
	}
}

void ofBufferObject::bindRange(GLenum target,GLuint index, GLintptr offset, GLsizeiptr size) const{
	if(data){
		glBindBufferRange(target,index,data->id,offset,size);
		data->lastTarget = target;
		data->isBound = true;
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

void ofBufferObject::setData(GLsizeiptr bytes, const void * data, GLenum usage){
	if(!this->data) return;
	this->data->size = bytes;

#ifdef GLEW_ARB_direct_state_access
	if (this->data->useDSA) {
		glNamedBufferData(this->data->id, bytes, data, usage);
		return;
	}
#endif

	/// --------| invariant: direct state access is not available
	bind(this->data->lastTarget);
	glBufferData(this->data->lastTarget, bytes, data, usage);
	unbind(this->data->lastTarget);
}

void ofBufferObject::updateData(GLintptr offset, GLsizeiptr bytes, const void * data){
	if(!this->data) return;

#ifdef GLEW_ARB_direct_state_access
	if(this->data->useDSA){
		glNamedBufferSubData(this->data->id,offset,bytes,data);
		return;
	}
#endif

	/// --------| invariant: direct state access is not available

	bind(this->data->lastTarget);
	glBufferSubData(this->data->lastTarget,offset,bytes,data);
	unbind(this->data->lastTarget);
}

void ofBufferObject::updateData(GLsizeiptr bytes, const void * data){
    updateData(0,bytes,data);
}

#ifndef TARGET_OPENGLES
void * ofBufferObject::map(GLenum access){
	if(!this->data) return nullptr;

#ifdef GLEW_ARB_direct_state_access
	if (data->useDSA) {
		return glMapNamedBuffer(data->id,access);
	}
#endif

	/// --------| invariant: direct state access is not available
	if(!data->isBound){
		// if the buffer wasn't already bound and the operation
		// is one of unpack/pack buffer alternate between the 2
		// since the tipical use is to pack to copy to the buffer
		// then unpack to copy from it.
		// for more advanced usages one can just bind the buffer
		// before mapping
		if(data->lastTarget==GL_PIXEL_PACK_BUFFER){
			data->lastTarget = GL_PIXEL_UNPACK_BUFFER;
		}else if(data->lastTarget == GL_PIXEL_UNPACK_BUFFER){
			data->lastTarget = GL_PIXEL_PACK_BUFFER;
		}
		glBindBuffer(data->lastTarget, data->id);
	}

	auto ret = glMapBuffer(data->lastTarget,access);

	if(!data->isBound){
		unbind(data->lastTarget);
	}

	return ret;
}

void ofBufferObject::unmap(){
	if(!this->data) return;

#ifdef GLEW_ARB_direct_state_access
	if (data->useDSA) {
		glUnmapNamedBuffer(data->id);
		return;
	}
#endif

	/// --------| invariant: direct state access is not available
	if(!data->isBound){
		glBindBuffer(data->lastTarget, data->id);
	}

	glUnmapBuffer(data->lastTarget);

	if(!data->isBound){
		unbind(data->lastTarget);
	}
}

void * ofBufferObject::mapRange(GLintptr offset, GLsizeiptr length, GLenum access){
	if(!this->data) return nullptr;

#ifdef GLEW_ARB_direct_state_access
	if (data->useDSA) {
		return glMapBufferRange(data->id,offset,length,access);
	}
#endif

	/// --------| invariant: direct state access is not available

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
