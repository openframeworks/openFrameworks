
#include "aiMesh.h"
#include "ofVbo.h"
#include "ofMaterial.h"


class ofxAssimpMeshHelper{
    public:
	ofxAssimpMeshHelper(){
		cout << "mesh helper cons"<< endl;
	}
    
    // pointer to the aiMesh we represent.
    aiMesh* mesh;
    
    // VBOs
    ofVbo vbo;
    
    // texture
    ofTexture texture;
    vector<ofIndexType> indices;
    
    // Material 
    ofMaterial material;
    
    ofBlendMode blendMode;
    
    bool twoSided;
    bool hasChanged;

    std::vector<aiVector3D> animatedPos;
    std::vector<aiVector3D> animatedNorm;

    ofMesh cachedMesh;
    bool validCache;
};
