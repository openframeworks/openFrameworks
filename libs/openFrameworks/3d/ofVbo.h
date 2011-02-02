//TODO: Add edge flags?

#pragma once
#include "ofGraphics.h"
#include "ofUtils.h"

class ofVbo {
	
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
	int		vertSize;
	int		totalVerts;
	
	int vertUsage;
	int colorUsage;
	int normUsage;
	int texUsage;
	
public:
	
	ofVbo();
	~ofVbo();

	void setVertexData(const ofVec3f * verts, int total, int usage);
	void setVertexData(const ofVec2f * verts, int total, int usage);

	void setColorData(const ofColor * colors, int total, int usage);	
	void setNormalData(const ofVec3f * normals, int total, int usage);	
	void setTexCoordData(const ofVec2f * texCoords, int total, int usage);	
	void setIndexData(const GLuint * indices, int total, int usage);

	void setVertexData(const float * vert0x, int total, int usage);
	void setColorData(const float * color0r, int total, int usage);	
	void setNormalData(const float * normal0x, int total, int usage);	
	void setTexCoordData(const float * texCoord0x, int total, int usage);	
	
	void updateVertexData(const ofVec3f * verts, int total);
	void updateVertexData(const ofVec2f * verts, int total);
	void updateColorData(const ofColor * colors, int total);	
	void updateNormalData(const ofVec3f * normals, int total);	
	void updateTexCoordData(const ofVec2f * texCoords, int total);	
	void updateIndexData(const GLuint * indices, int total);
	
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
};
