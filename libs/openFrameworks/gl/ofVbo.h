//TODO: Add edge flags?

#pragma once
#include "ofConstants.h"
#include "ofVec3f.h"
#include "ofColor.h"
#include "ofUtils.h"
#include "ofMesh.h"
#include "ofGLUtils.h"

class ofVbo {
public:
	
	ofVbo();
	ofVbo(const ofVbo & mom);
	ofVbo & operator=(const ofVbo& mom);
	~ofVbo();

	void setMesh(const ofMesh & mesh, int usage);

	void setVertexData(const ofVec3f * verts, int total, int usage);
	void setVertexData(const ofVec2f * verts, int total, int usage);

	void setColorData(const ofColor * colors, int total, int usage);	
	void setNormalData(const ofVec3f * normals, int total, int usage);	
	void setTexCoordData(const ofVec2f * texCoords, int total, int usage);
	void setIndexData(const ofIndexType * indices, int total, int usage);

	void setVertexData(const float * vert0x, int numCoords, int total, int usage, int stride=0);
	void setColorData(const float * color0r, int total, int usage, int stride=0);
	void setNormalData(const float * normal0x, int total, int usage, int stride=0);
	void setTexCoordData(const float * texCoord0x, int total, int usage, int stride=0);
	
	void updateMesh(const ofMesh & mesh);

	void updateVertexData(const ofVec3f * verts, int total);
	void updateVertexData(const ofVec2f * verts, int total);
	void updateColorData(const ofColor * colors, int total);	
	void updateNormalData(const ofVec3f * normals, int total);	
	void updateTexCoordData(const ofVec2f * texCoords, int total);
	void updateIndexData(const ofIndexType * indices, int total);
	
	void updateVertexData(const float * ver0x, int total);
	void updateColorData(const float * color0r, int total);
	void updateNormalData(const float * normal0x, int total);
	void updateTexCoordData(const float * texCoord0x, int total);
	
	GLuint getVertId();
	GLuint getColorId();
	GLuint getNormalId();
	GLuint getTexCoordId();
	GLuint getIndexId();
	
	bool getIsAllocated();
	bool getUsingVerts();
	bool getUsingColors();
	bool getUsingNormals();
	bool getUsingTexCoords();
	bool getUsingIndices();
	
	void draw(int drawMode, int first, int total);
	void drawElements(int drawMode, int amt);
	void bind();
	void unbind();
	void clear();

private:

	GLuint indexId;

	GLuint vertId;
	GLuint colorId;
	GLuint normalId;
	GLuint texCoordId;

	bool bAllocated;

	bool bUsingVerts;		// need at least vertex data
	bool bUsingTexCoords;
	bool bUsingColors;
	bool bUsingNormals;
	bool bUsingIndices;

	GLsizei vertStride;
	GLsizei colorStride;
	GLsizei normalStride;
	GLsizei texCoordStride;

	int		vertSize;
	int		totalVerts;

	int vertUsage;
	int colorUsage;
	int normUsage;
	int texUsage;


};
