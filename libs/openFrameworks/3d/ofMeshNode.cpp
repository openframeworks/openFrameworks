#include "ofMeshNode.h"
#include "ofMaterial.h"

ofMeshNode::ofMeshNode()
:mesh(NULL) 
,diffuse_material(NULL)
{
}

ofMeshNode::ofMeshNode(ofMesh* pMesh):mesh(pMesh) { 
}

ofMeshNode::ofMeshNode(ofMesh& rMesh):mesh(&rMesh) { 
}

ofMeshNode& ofMeshNode::setMesh(ofMesh& rMesh) {
	mesh = &rMesh;
	return *this;
}

ofMeshNode& ofMeshNode::addMaterial(ofMaterial* pMaterial) {
	materials.push_back(pMaterial);
	if(pMaterial->enabled_maps & OF_DIFFUSE_MAP) {
		diffuse_material = pMaterial;
		if(mesh != NULL) {
			mesh->enableTexCoords();
		}
	}
	return *this;
}



void ofMeshNode::customDraw() {

	if(diffuse_material != NULL && mesh != NULL) {
		diffuse_material->begin();
		mesh->draw(OF_MESH_FILL);
		diffuse_material->end();
	}

}
