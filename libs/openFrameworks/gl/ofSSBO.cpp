#include "ofSSBO.h"

#include "ofLog.h"

ofSSBO::ofSSBO() {
    bindingIndex = 0;

    buffer.allocate(GL_SHADER_STORAGE_BUFFER);
}

ofSSBO::ofSSBO(const ofSSBO& ssbo) {
    bindingIndex = ssbo.getBindingIndex();

    ssbo.getBuffer().copyTo(this->buffer);
}

ofSSBO& ofSSBO::operator=(const ofSSBO& ssbo) {
    bindingIndex = ssbo.getBindingIndex();

    ssbo.getBuffer().copyTo(this->buffer);

    return *this;
}

ofSSBO::~ofSSBO() {}

const ofBufferObject& ofSSBO::getBuffer() const {
    return buffer;
}

GLuint ofSSBO::getBindingIndex() const {
    return bindingIndex;
}

void ofSSBO::setBindingIndex(const GLuint index) {
    if (index > GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS)
        ofLogError("ofSSBO", "bindingIndex can't be bigger than " + 
                        std::to_string(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS));

    bindingIndex = index;
};

void ofSSBO::setData(GLsizeiptr bytes, const void *data, GLenum usage) {
    buffer.setData(bytes, data, usage);
}

void ofSSBO::updateData(GLintptr offset, GLsizeiptr bytes, const void *data) {
    buffer.updateData(offset, bytes, data);
}

void ofSSBO::bind() const {
    buffer.bind(GL_SHADER_STORAGE_BUFFER);
    buffer.bindBase(GL_SHADER_STORAGE_BUFFER, bindingIndex);
}

void ofSSBO::unbind() const {
    buffer.unbind(GL_SHADER_STORAGE_BUFFER);
    buffer.unbindBase(GL_SHADER_STORAGE_BUFFER, bindingIndex);
}
