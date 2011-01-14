#include "ofAssimpMeshLoader.h"
#import "aiConfig.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//-------------------------------------------
static ofColor aiColorToOfColor(const aiColor4D& c){
	return ofColor(255*c.r,255*c.g,255*c.b,255*c.a);
}

static void aiMeshToOfMesh(const aiMesh* aim, ofMesh& ofm){
	// copy vertices
	for (int i=0; i < aim->mNumVertices;i++){
		ofm.vertices.push_back(ofVec3f(aim->mVertices[i].x,aim->mVertices[i].y,aim->mVertices[i].z));
	}
	
	//copy faces
	for (int i=0; i < aim->mNumFaces;i++){	
		ofm.faces.push_back(ofFace());
		for (int j=0; j<aim->mFaces[i].mNumIndices; j++){
			ofm.faces.back().indices.push_back(aim->mFaces[i].mIndices[j]);
		}
		if(ofm.faces.back().indices.size()>3){
			ofLog(OF_LOG_WARNING,"non-triangular face found: face # " + ofToString(ofm.faces.size()-1));
		}
	}	
	
	if(aim->HasNormals()){
		for (int i=0; i < aim->mNumVertices;i++){
			ofm.normals.push_back(ofVec3f(aim->mNormals[i].x,aim->mNormals[i].y,aim->mNormals[i].z));
		}
	}else{
		ofm.usingNormals = false;
	}
	
	// aiVector3D * 	mTextureCoords [AI_MAX_NUMBER_OF_TEXTURECOORDS]
	// just one for now
	if(aim->GetNumUVChannels()>0){
		for (int i=0; i < aim->mNumVertices;i++){
			ofm.texCoords.push_back(ofVec2f(aim->mTextureCoords[0][i].x,aim->mTextureCoords[0][i].y));
		}
	}else{
		ofm.usingTexCoords = false;
	}
	
	//aiColor4D * 	mColors [AI_MAX_NUMBER_OF_COLOR_SETS]
	
	// just one for now		
	if(aim->GetNumColorChannels()>0){
		for (int i=0; i < aim->mNumVertices;i++){
			ofm.colors.push_back(aiColorToOfColor(aim->mColors[0][i]));
		}
	}else{
		ofm.usingColors = false;
	}	
	// copy name
	ofm.name = string(aim->mName.data);
	// copy face type
	ofm.faceType = aim->mPrimitiveTypes;
	
	//	ofm->materialId = aim->mMaterialIndex;	
}


ofxAssimpModelLoader::ofxAssimpModelLoader(){
	scene = NULL;
}

//------------------------------------------
void ofxAssimpModelLoader::loadModel(string modelName){
	
    
    // if we have a model loaded, unload the fucker.
    if(scene != NULL){
        aiReleaseImport(scene);
        scene = NULL;
        
        deleteGLResources();   
    }
    
    // Load our new path.
    string filepath = ofToDataPath(modelName);
	
    ofLog(OF_LOG_VERBOSE, "loading model %s", filepath.c_str());
    
    // only ever give us triangles.
    aiSetImportPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
    aiSetImportPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);
    
    // aiProcess_FlipUVs is for VAR code. Not needed otherwise. Not sure why.
    scene = (aiScene*) aiImportFile(filepath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph | aiProcess_Triangulate | aiProcess_FlipUVs | 0 );
    
    if(scene){        
        ofLog(OF_LOG_VERBOSE, "initted scene with %i meshes & %i animations", scene->mNumMeshes, scene->mNumAnimations);
        
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        loadGLResources();
        glPopClientAttrib();
        glPopAttrib();
    }
}

//-------------------------------------------
void ofxAssimpModelLoader::loadGLResources(){
	
    ofLog(OF_LOG_VERBOSE, "loading gl resources");
	
    // create new mesh helpers for each mesh, will populate their data later.
    vboMeshes.reserve(scene->mNumMeshes); 
	
    // create OpenGL buffers and populate them based on each meshes pertinant info.
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
        ofLog(OF_LOG_VERBOSE, "loading mesh %u", i);
        // current mesh we are introspecting
        aiMesh* mesh = scene->mMeshes[i];
        
        // the current curVboMesh we will be populating data into.
        ofVboMesh curVboMesh;
        
        curVboMesh.mesh = new ofMesh();
		aiMeshToOfMesh(mesh,*curVboMesh.mesh);
        
        // Load Textures
        int texIndex = 0;
        aiString texPath;
        
        //curVboMesh.texture = NULL;
        
        // Create a VBO for our vertices
        GLuint vhandle;
        glGenBuffers(1, &vhandle);
        
        glBindBuffer(GL_ARRAY_BUFFER, vhandle);
        
		glBufferData(GL_ARRAY_BUFFER, sizeof(aiVertex) * mesh->mNumVertices, NULL, GL_STATIC_DRAW/*GL_STATIC_DRAW GL_STREAM_DRAW*/);
		
        // populate vertices
        aiVertex* verts = (aiVertex*)glMapBuffer(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
        
        for (unsigned int x = 0; x < mesh->mNumVertices; ++x)
        {
            verts->vPosition = mesh->mVertices[x];
            
            if (NULL == mesh->mNormals)
                verts->vNormal = aiVector3D(0.0f,0.0f,0.0f);
            else
                verts->vNormal = mesh->mNormals[x];
            
            if (mesh->HasVertexColors(0))
            {
                verts->dColorDiffuse = mesh->mColors[0][x];
            }
            else
                verts->dColorDiffuse = aiColor4D(1.0, 1.0, 1.0, 1.0);
            
            // This varies slightly form Assimp View, we support the 3rd texture component.
            if (mesh->HasTextureCoords(0))
                verts->vTextureUV = mesh->mTextureCoords[0][x];
            else
                verts->vTextureUV = aiVector3D(0.5f,0.5f, 0.0f);
            ++verts;
        }
        
        glUnmapBufferARB(GL_ARRAY_BUFFER_ARB); //invalidates verts
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // set the mesh vertex buffer handle to our new vertex buffer.
        curVboMesh.vertexBuffer = vhandle;
        
        // Create Index Buffer
        unsigned int nidx;
        switch (mesh->mPrimitiveTypes){
            case aiPrimitiveType_POINT:
                nidx = 1;break;
            case aiPrimitiveType_LINE:
                nidx = 2;break;
            case aiPrimitiveType_TRIANGLE:
                nidx = 3;break;
            default: assert(false);
        }   
        
        GLuint ihandle;
        glGenBuffers(1, &ihandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ihandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->mNumFaces * nidx, NULL, GL_STATIC_DRAW/*GL_STATIC_DRAW GL_STREAM_DRAW*/);
        
        unsigned int* indices = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY_ARB);
        
        // now fill the index buffer
        for (unsigned int x = 0; x < mesh->mNumFaces; ++x){
            for (unsigned int a = 0; a < nidx; ++a){
                *indices++ = mesh->mFaces[x].mIndices[a];
            }
        }
        
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        // set the mesh index buffer handle to our new index buffer.
        curVboMesh.indexBuffer = ihandle;
        curVboMesh.numIndices = mesh->mNumFaces * nidx;
        
        // Create VAO and populate it
        GLuint vaoHandle; 
        glGenVertexArraysAPPLE(1, &vaoHandle);
		
        // TODO: equivalent PC call.
        glBindVertexArrayAPPLE(vaoHandle);
        
        glBindBuffer(GL_ARRAY_BUFFER, curVboMesh.vertexBuffer);
        
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(aiVertex), BUFFER_OFFSET(12));
        
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(3, GL_FLOAT, sizeof(aiVertex), BUFFER_OFFSET(24));
        //TODO: handle second texture
		
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, sizeof(aiVertex), BUFFER_OFFSET(36));
        
        // VertexPointer ought to come last, apparently this is some optimization, since if its set once first, it gets fiddled with every time something else is update.
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(aiVertex), 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curVboMesh.indexBuffer);
        
        glBindVertexArrayAPPLE(0);
		
        // save the VAO handle into our mesh helper
        curVboMesh.vao = vaoHandle;
        
        vboMeshes.push_back(curVboMesh);
    }
    ofLog(OF_LOG_VERBOSE, "finished loading gl resources");
}

//-------------------------------------------
void ofxAssimpModelLoader::deleteGLResources(){
	
    ofLog(OF_LOG_VERBOSE, "deleting gl resources");
	
    for (unsigned int i = 0; i < vboMeshes.size(); ++i){
		
        ofAssimpMesh curVboMesh = vboMeshes[i];
        
        const GLuint indexBuffer = curVboMesh.indexBuffer;
        const GLuint vertexBuffer = curVboMesh.vertexBuffer;
        const GLuint normalBuffer = curVboMesh.normalBuffer;
        const GLuint vaoHandle = curVboMesh.vao;
        
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &indexBuffer);
        glDeleteBuffers(1, &normalBuffer);
        glDeleteVertexArraysAPPLE(1, &vaoHandle);
        
        curVboMesh.indexBuffer = 0;
        curVboMesh.vertexBuffer = 0;
        curVboMesh.normalBuffer = 0;
        curVboMesh.vao = 0; 
        curVboMesh.mesh = NULL;
        
        curVboMesh.textureIndex = -1;
    }
    
    // clear out our meshes array.
    vboMeshes.clear();
    
    // clear out our textures (ofImages)
    textures.clear();
}

//-------------------------------------------
ofxAssimpModelLoader::~ofxAssimpModelLoader(){
}	

void ofxAssimpModelLoader::draw()
{
    if(scene){
        
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        
        glEnable(GL_NORMALIZE);
        
        glPushMatrix();

        if(vboMeshes.size())
        {
            for(int i = 0; i < vboMeshes.size(); i++){
                ofAssimpMesh curVboMesh = vboMeshes.at(i);
                
                // Texture Binding
				/*
                if(curVboMesh.textureIndex!=-1){
                    textures[curVboMesh.textureIndex].getTextureReference().bind();
                }
				 */
                // Set up meterial state.
				
                float dc[4];
                float sc[4];
                float ac[4];
                float emc[4];
				
                // Culling
                if(curVboMesh.twoSided)
                    glEnable(GL_CULL_FACE);
                else 
                    glDisable(GL_CULL_FACE);
				
                // TODO: equivalent VAO callfor linux and windows.
                glBindVertexArrayAPPLE(curVboMesh.vao);        
                glDrawElements(GL_TRIANGLES, curVboMesh.numIndices, GL_UNSIGNED_INT, 0);
                
                // Texture Binding
				/*
                if(curVboMesh.textureIndex!=-1){
                    textures[curVboMesh.textureIndex].getTextureReference().unbind();
                }
				 */
            }
        }        
		
        glPopMatrix();
        
        glPopClientAttrib();
        glPopAttrib();
    }
}
