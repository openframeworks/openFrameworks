//TODO: Add edge flags?

#pragma once
#include "ofConstants.h"
#include "ofGraphicsConstants.h"
#include "ofBufferObject.h"
#include <map>

template<typename T>
class ofColor_;
typedef ofColor_<float> ofFloatColor;

class ofVec2f;
class ofVec3f;

template<class V, class N, class C, class T>
class ofMesh_;
using ofMesh = ofMesh_<ofDefaultVertexType, ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>;

class ofVbo {
public:
	
	ofVbo();
	ofVbo(const ofVbo & mom);
	ofVbo & operator=(const ofVbo& mom);
	~ofVbo();

	void setMesh(const ofMesh & mesh, int usage);
	void setMesh(const ofMesh & mesh, int usage, bool useColors, bool useTextures, bool useNormals);
	
	void setVertexData(const glm::vec3 * verts, int total, int usage);
	void setVertexData(const glm::vec2 * verts, int total, int usage);
	void setVertexData(const ofVec3f * verts, int total, int usage);
	void setVertexData(const ofVec2f * verts, int total, int usage);

	void setColorData(const ofFloatColor * colors, int total, int usage);
	void setNormalData(const glm::vec3 * normals, int total, int usage);
	void setNormalData(const ofVec3f * normals, int total, int usage);
	void setTexCoordData(const glm::vec2 * texCoords, int total, int usage);
	void setTexCoordData(const ofVec2f * texCoords, int total, int usage);
	void setIndexData(const ofIndexType * indices, int total, int usage);

	void setVertexData(const float * vert0x, int numCoords, int total, int usage, int stride=0);
	void setColorData(const float * color0r, int total, int usage, int stride=0);
	void setNormalData(const float * normal0x, int total, int usage, int stride=0);
	void setTexCoordData(const float * texCoord0x, int total, int usage, int stride=0);

	void setAttributeData(int location, const float * vert0x, int numCoords, int total, int usage, int stride=0);

#ifndef TARGET_OPENGLES
	/// used to send an attribute per instance(s) instead of per vertex.
	/// will send per vertex if set to 0 or to the number of instances if >0
	///
	/// see textureBufferInstancedExample
	/// and https://www.opengl.org/sdk/docs/man4/html/glVertexAttribDivisor.xhtml
	void setAttributeDivisor(int location, int divisor);
#endif

	void setVertexBuffer(ofBufferObject & buffer, int numCoords, int stride, int offset=0);
	void setColorBuffer(ofBufferObject & buffer, int stride, int offset=0);
	void setNormalBuffer(ofBufferObject & buffer, int stride, int offset=0);
	void setTexCoordBuffer(ofBufferObject & buffer, int stride, int offset=0);
	void setIndexBuffer(ofBufferObject & buffer);
	
	void setAttributeBuffer(int location, ofBufferObject & buffer, int numCoords, int stride, int offset=0);

	ofBufferObject & getVertexBuffer();
	ofBufferObject & getColorBuffer();
	ofBufferObject & getNormalBuffer();
	ofBufferObject & getTexCoordBuffer();
	ofBufferObject & getIndexBuffer();
	ofBufferObject & getAttributeBuffer(int location);

	const ofBufferObject & getVertexBuffer() const;
	const ofBufferObject & getColorBuffer() const;
	const ofBufferObject & getNormalBuffer() const;
	const ofBufferObject & getTexCoordBuffer() const;
	const ofBufferObject & getIndexBuffer() const;

	const ofBufferObject & getAttributeBuffer(int location) const;

	void updateMesh(const ofMesh & mesh);

	void updateVertexData(const glm::vec3 * verts, int total);
	void updateVertexData(const glm::vec2 * verts, int total);
	void updateVertexData(const ofVec3f * verts, int total);
	void updateVertexData(const ofVec2f * verts, int total);
	void updateColorData(const ofFloatColor * colors, int total);
	void updateNormalData(const glm::vec3 * normals, int total);
	void updateNormalData(const ofVec3f * normals, int total);
	void updateTexCoordData(const glm::vec2 * texCoords, int total);
	void updateTexCoordData(const ofVec2f * texCoords, int total);
	void updateIndexData(const ofIndexType * indices, int total);
	
	void updateVertexData(const float * ver0x, int total);
	void updateColorData(const float * color0r, int total);
	void updateNormalData(const float * normal0x, int total);
	void updateTexCoordData(const float * texCoord0x, int total);

	void updateAttributeData(int location, const float * vert0x, int total);

	void enableColors();
	void enableNormals();
	void enableTexCoords();
	void enableIndices();

	void disableColors();
	void disableNormals();
	void disableTexCoords();
	void disableIndices();

	GLuint getVaoId() const;
	GLuint getVertId() const;
	GLuint getColorId() const;
	GLuint getNormalId() const;
	GLuint getTexCoordId() const;
	GLuint getIndexId() const;
	
	/// returns OpenGL memory object id for GL buffer holding attribute data
	GLuint  getAttributeId(int AttrPos_) const;
	
	bool getIsAllocated() const;
	bool getUsingVerts() const;
	bool getUsingColors() const;
	bool getUsingNormals() const;
	bool getUsingTexCoords() const;
	bool getUsingIndices() const;
	
	void draw(int drawMode, int first, int total) const;
	void drawElements(int drawMode, int amt, int offsetelements = 0) const;
	
	void drawInstanced(int drawMode, int first, int total, int primCount) const;
	void drawElementsInstanced(int drawMode, int amt, int primCount) const;
	
	void bind() const;
	void unbind() const;

	void clear();

	void clearVertices();
	void clearNormals();
	void clearColors();
	void clearTexCoords();
	void clearIndices();
	
	void clearAttribute(int attributePos_);

	int getNumVertices() const;
	int getNumIndices() const;
	
	bool hasAttribute(int attributePos_) const;

private:

	struct VertexAttribute{
		VertexAttribute();
		bool isAllocated() const;
		void allocate();
		void bind() const;
		void unbind() const;
		void setData(GLsizeiptr bytes, const void * data, GLenum usage);
		void updateData(GLintptr offset, GLsizeiptr bytes, const void * data);
		void setData(const float * attrib0x, int numCoords, int total, int usage, int stride, bool normalize=false);
		void setBuffer(ofBufferObject & buffer, int numCoords, int stride, int offset);
		void enable() const;
		void disable() const;
		GLuint getId() const;
		ofBufferObject buffer;
		GLsizei stride;
		GLsizeiptr offset;
		int numCoords;
		GLuint location;
		bool normalize;
		int divisor;
	};

	struct IndexAttribute{
		IndexAttribute();
		bool isAllocated() const;
		void allocate();
		void bind() const;
		void unbind() const;
		void setData(GLsizeiptr bytes, const void * data, GLenum usage);
		void updateData(GLintptr offset, GLsizeiptr bytes, const void * data);
		GLuint getId() const;
		ofBufferObject buffer;
	};

	GLuint vaoID;
	mutable bool vaoChanged;

	IndexAttribute indexAttribute;

	mutable bool bUsingVerts;		// need at least vertex data
	mutable bool bUsingTexCoords;
	mutable bool bUsingColors;
	mutable bool bUsingNormals;
	mutable bool bUsingIndices;

	int	totalVerts;
	int	totalIndices;

	VertexAttribute positionAttribute;
	VertexAttribute colorAttribute;
	VertexAttribute texCoordAttribute;
	VertexAttribute normalAttribute;
	std::map<int,VertexAttribute> customAttributes;
	
	static bool vaoChecked;
	static bool vaoSupported;

	VertexAttribute & getOrCreateAttr(int location);
};
