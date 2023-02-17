#pragma once

#include "ofConstants.h"

template<typename T>
class ofPixels_;

class ofBufferObject {
public:
	ofBufferObject();

	/// creates a buffer without allocating any memory yet
	/// glGenBuffers, allocates a GL buffer
	void allocate();

	/// creates a buffer and allocates the required number of bytes
	/// glGenBuffers + glNamedBufferData: https://www.opengl.org/sdk/docs/man4/html/glBufferData.xhtml
	/// before GL 4.5 emulates glNamedBufferData by binding to last known target
	/// for this buffer uploading data to that target and unbinding again
	void allocate(GLsizeiptr bytes, GLenum usage);
	void allocate(GLsizeiptr bytes, const void * data, GLenum usage);

	template<typename T>
	void allocate(const std::vector<T> & data, GLenum usage){
		allocate(data.size()*sizeof(T),&data[0],usage);
	}

	template<typename T>
	void allocate(const ofPixels_<T> & data, GLenum usage){
		allocate(data.size()*sizeof(T),data.getData(),usage);
	}

	/// true if allocate was called before
	bool isAllocated() const;

	/// glBindBuffer: https://www.opengl.org/sdk/docs/man4/html/glBindBuffer.xhtml
	void bind(GLenum target) const;

	/// binds the passed target to buffer 0
	void unbind(GLenum target) const;

#if !defined(TARGET_OPENGLES) || defined(TARGET_EMSCRIPTEN)
	/// glBindBufferBase: https://www.opengl.org/sdk/docs/man4/html/glBindBufferBase.xhtml
	void bindBase(GLenum target,GLuint index) const;

	/// binds the given target and index to buffer 0
	void unbindBase(GLenum target,GLuint index) const;

	/// glBindBufferRange: https://www.opengl.org/sdk/docs/man4/html/glBindBufferRange.xhtml
	void bindRange(GLenum target,GLuint index, GLintptr offset, GLsizeiptr size) const;

	/// binds the given target and index to 0
	void unbindRange(GLenum target,GLuint index) const;
#endif

	/// returns the id of the buffer if it's allocated or 0 otherwise
	GLuint getId() const;

	/// glNamedBufferData: https://www.opengl.org/sdk/docs/man4/html/glBufferData.xhtml
	/// before GL 4.5 emulates glNamedBufferData by binding to last known target
	/// for this buffer uploading data to that target and unbinding again
	void setData(GLsizeiptr bytes, const void * data, GLenum usage);


	/// glNamedBufferSubData: https://www.opengl.org/sdk/docs/man4/html/glBufferSubData.xhtml
	/// before GL 4.5 emulates glNamedBufferSubData by binding to last known target
	/// for this buffer uploading data to that target and unbinding again
	void updateData(GLintptr offset, GLsizeiptr bytes, const void * data);
    void updateData(GLsizeiptr bytes, const void * data);

	/// typed version of setData, same functionality but guesses the size from the size
	/// of the passed vector and size of the type
	template<typename T>
	void setData(const std::vector<T> & data, GLenum usage){
		setData(data.size()*sizeof(T),&data[0],usage);
	}

	template<typename T>
	void setData(const ofPixels_<T> & data, GLenum usage){
		setData(data.size()*sizeof(T),data.getPixels(),usage);
	}

	/// typed version of updateData, same functionality but guesses the size from the size
	/// of the passed vector and size of the type
	template<typename T>
	void updateData(GLintptr offset, const std::vector<T> & data){
		updateData(offset,data.size()*sizeof(T),&data[0]);
	}

    /// typed version of updateData, same functionality but guesses the size from the size
    /// of the passed vector and size of the type
    template<typename T>
    void updateData(const std::vector<T> & data){
        updateData(0,data.size()*sizeof(T),&data[0]);
    }

#ifndef TARGET_OPENGLES
	/// glMapNamedBuffer: https://www.opengl.org/sdk/docs/man4/html/glMapBuffer.xhtml
	/// before GL 4.5 emulates glMapNamedBuffer by binding to last known target
	/// for this buffer and mapping that target
	void * map(GLenum access);

	/// glUnmapNamedBuffer: https://www.opengl.org/sdk/docs/man4/html/glUnmapBuffer.xhtml
	/// before GL 4.5 emulates glUnmapNamedBuffer by unmapping and unbinding
	/// the last known target for this buffer
	void unmap();

	/// typed version of map, returns an array of T when used like:
	/// buffer.map<Type>(access)
	template<typename T>
	T * map(GLenum access){
		return static_cast<T*>(map(access));
	}

	/// glMapNamedBufferRange: https://www.opengl.org/sdk/docs/man4/html/glMapBufferRange.xhtml
	/// before GL 4.5 emulates glMapNamedBufferRange by binding to last known target
	/// for this buffer and mapping that target
	void * mapRange(GLintptr offset, GLsizeiptr length, GLenum access);

	/// same as unmap, just to make the api more clear
	void unmapRange();

	/// typed version of mapRange, returns an array of T when used like:
	/// buffer.mapRange<Type>(access)
	template<typename T>
	T * mapRange(GLintptr offset, GLsizeiptr length, GLenum access){
		return static_cast<T*>(mapRange(offset,length,access));
	}

	void copyTo(ofBufferObject & dstBuffer) const;
	void copyTo(ofBufferObject & dstBuffer, int readOffset, int writeOffset, size_t size) const;

    void invalidate();
#endif

	GLsizeiptr size() const;

private:
	struct Data{
		Data();
		~Data();
		GLuint id;
		GLsizeiptr size;
		GLenum lastTarget;
		bool isBound;
		bool isDSA;
	};
	std::shared_ptr<Data> data;
};
