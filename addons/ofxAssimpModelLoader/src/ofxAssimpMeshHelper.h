
#include "aiMesh.h"
#include "ofVbo.h"


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
    aiColor4D diffuseColor;
    aiColor4D specularColor;
    aiColor4D ambientColor;
    aiColor4D emissiveColor;
    
    GLfloat opacity;
    GLfloat shininess;
    GLfloat specularStrength;
    
    bool twoSided;
};
