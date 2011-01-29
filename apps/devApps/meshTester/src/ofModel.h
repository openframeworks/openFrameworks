#pragma once

#include "ofMesh.h"
#include "ofUtils.h"
#include "ofModelLoader.h"

class ofModel{
public:
	ofModel();
	~ofModel();
	
	void loadModel(string filename);	
	
	void drawWireframe();
	void drawFaces();
	void drawVertices();
	
	void enableTexCoords();
	void disableTexCoords();
	void enableNormals();
	void disableNormals();
	void enableColors();
	void disableColors();
	
	vector<ofMesh> meshes;
	vector<ofTexture> textures;
};
