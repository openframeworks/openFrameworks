
#include "aiMesh.h"
#include "ofVbo.h"
#include "ofMaterial.h"


struct ofxAssimpMeshHelper{
    public:
    
    // pointer to the aiMesh we represent.
    aiMesh* mesh;
    
    // VBOs
    ofVbo vbo;
    
    // texture
    ofImage texture;
    vector<ofIndexType> indices;
    
    // Material 
    ofMaterial material;
    
    ofBlendMode blendMode;
    
    bool twoSided;
};
