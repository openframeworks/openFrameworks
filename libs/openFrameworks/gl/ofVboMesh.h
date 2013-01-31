#pragma once

#include "ofMesh.h"
#include "ofVbo.h"

class ofVboMesh: public ofMesh{
public:
	using ofMesh::draw;
	ofVboMesh();
	ofVboMesh(const ofMesh & mom);
	void setUsage(int usage);

    void enableColors();
    void enableTextures();
    void enableNormals();
    void enableIndices();

    void disableColors();
    void disableTextures();
    void disableNormals();
    void disableIndices();

    bool usingColors();
    bool usingTextures();
    bool usingNormals();
    bool usingIndices();

	void draw(ofPolyRenderMode drawMode);

private:
	void updateVbo();
	ofVbo vbo;
	int usage;
	int vboNumVerts, vboNumIndices, vboNumNormals, vboNumTexCoords, vboNumColors;
};
