
#include "aiMesh.h"

// This is for the VBO
// TODO: use aiMesh or aiVBOMesh if / when it happens
struct aiVertex 
{
    aiVector3D vPosition;       // 3
    aiVector3D vNormal;         // 6
    aiVector3D vTextureUV;      // 9
    
    aiColor4D  dColorDiffuse;   // 13
    
//    aiVector3D vTangent;        // 13
//    aiVector3D vBitangent;      // 16
//    aiVector3D vTextureUV2;     // 22
    
//    unsigned char mBoneIndices[4];
//    unsigned char mBoneWeights[4];
};

struct ofxAssimpMeshHelper{
    public:
    
    // pointer to the aiMesh we represent.
    aiMesh* mesh;
    
    // VAO that encapsulates all VBO drawing state
    GLuint vao;
    
    // VBOs
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLuint normalBuffer;
    GLuint numIndices;
    
    // texture
    //ofTexture* texture;
    int textureIndex;
    
    // Material 
    // TODO: use ofColor
    aiColor4D diffuseColor;
    aiColor4D specularColor;
    aiColor4D ambientColor;
    aiColor4D emissiveColor;
    
    GLfloat opacity;
    GLfloat shininess;
    GLfloat specularStrength;
    
    bool twoSided;
};
