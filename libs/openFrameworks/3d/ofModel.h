//TODO: should textures be ofTexture or ofImage (dynamically drawing to model texture?)

#pragma once

#include "ofMesh.h"
#include "ofUtils.h"
#include "ofImage.h"
#include "ofConstants.h"
#include "ofMeshRenderer.h"
#include <map>

class ofModel{
public:
	ofModel();
	~ofModel();
		
	void drawWireframe();
	void drawFaces();
	void drawVertices();
	
	void enableTextures();
	void disableTextures();
	void enableNormals();
	void disableNormals();
	void enableColors();
	void disableColors();
	
	void setRenderMethod(meshRenderMethod m);
	
	vector<ofMesh*> meshes;
	vector<ofMeshRenderer> renderers;
	vector<ofImage> textures;
	map<int, int> textureLinks;
	
protected:
	bool bUsingTextures, bUsingNormals, bUsingColors;
	void updateRenderers();
	void bindTextureForMesh(int id);
	void unbindTextureForMesh(int id);

private:
	meshRenderMethod renderMethod;
};
