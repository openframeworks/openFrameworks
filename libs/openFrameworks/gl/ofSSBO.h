#pragma once

#include "ofBufferObject.h"

class ofSSBO {
    private:
        GLuint bindingIndex;

        ofBufferObject buffer;

    public:
        ofSSBO();
        ofSSBO(const ofSSBO& ssbo);
        ofSSBO& operator=(const ofSSBO& ssbo);
        ~ofSSBO();

        const ofBufferObject &getBuffer() const;

        void setData(GLsizeiptr bytes, const void *data, GLenum usage);
        void updateData(GLintptr offset, GLsizeiptr bytes, const void *data);

        void setBindingIndex(const GLuint binding);
        GLuint getBindingIndex() const;

        void bind() const;
        void unbind() const;
};
