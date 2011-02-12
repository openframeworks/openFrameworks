#pragma once

#include "ofNode.h"


class ofMesh;
class ofMaterial;
/**
 * An ofMeshNode is a "renderable" 3D object using a mesh.
 *
 */
class ofMeshNode : public ofNode {
public:
	ofMeshNode();
	ofMeshNode(ofMesh& rMesh);
	ofMeshNode(ofMesh* pMesh);
	
	ofMeshNode& setMesh(ofMesh& rMesh);
	ofMeshNode& addMaterial(ofMaterial* pMaterial);
	virtual void customDraw();

protected:	
	ofMesh *mesh;
	vector<ofMaterial*> materials;
	ofMaterial* diffuse_material;
};