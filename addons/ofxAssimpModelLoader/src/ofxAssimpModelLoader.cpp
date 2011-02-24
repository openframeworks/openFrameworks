#include "ofxAssimpModelLoader.h"

#include "aiConfig.h"
#include <assert.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

//-------------------------------------------
static void color4_to_float4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

//-------------------------------------------
static void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

//-------------------------------------------
// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
/*static void Color4f(CGLContextObj cgl_ctx, const struct aiColor4D *color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}*/

ofxAssimpModelLoader::ofxAssimpModelLoader(){

    lastAnimationTime = 0;
    currentAnimation = 0;
    animationTime = 0;
    numRotations = 0;
    rotAngle.clear();
    rotAxis.clear();
    scale = ofPoint(1, 1, 1);
	
	scene = NULL;
    normalizeScale = true;   
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
	
	//theo added - so we can have models and their textures in sub folders
	modelFolder = ofFileUtils::getEnclosingDirectoryFromPath(filepath);

    ofLog(OF_LOG_VERBOSE, "loading model %s", filepath.c_str());
    ofLog(OF_LOG_VERBOSE, "loading from folder %s", modelFolder.c_str());
    
    // only ever give us triangles.
    aiSetImportPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
    aiSetImportPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);
    
    // aiProcess_FlipUVs is for VAR code. Not needed otherwise. Not sure why.
    scene = (aiScene*) aiImportFile(filepath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph | aiProcess_Triangulate | aiProcess_FlipUVs | 0 );
    
    if(scene){        
        ofLog(OF_LOG_VERBOSE, "initted scene with %i meshes & %i animations", scene->mNumMeshes, scene->mNumAnimations);

        getBoundingBoxWithMinVector(&scene_min, &scene_max);
        scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
        scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
        scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
        
        // optional normalized scaling
        normalizedScale = scene_max.x-scene_min.x;
        normalizedScale = aisgl_max(scene_max.y - scene_min.y,normalizedScale);
        normalizedScale = aisgl_max(scene_max.z - scene_min.z,normalizedScale);
        normalizedScale = 1.f / normalizedScale;
        normalizedScale *= ofGetWidth() / 2.0;
        
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        
        loadGLResources();
        
        glPopClientAttrib();
        glPopAttrib();

        if(getAnimationCount())
            ofLog(OF_LOG_VERBOSE, "scene has animations");
        else {
            ofLog(OF_LOG_VERBOSE, "no animations");
            
        }
    }
}

//-------------------------------------------
void ofxAssimpModelLoader::loadGLResources(){

    ofLog(OF_LOG_VERBOSE, "loading gl resources");

    // create new mesh helpers for each mesh, will populate their data later.
    modelMeshes.reserve(scene->mNumMeshes); 
        
    // create OpenGL buffers and populate them based on each meshes pertinant info.
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
        ofLog(OF_LOG_VERBOSE, "loading mesh %u", i);
        // current mesh we are introspecting
        aiMesh* mesh = scene->mMeshes[i];
        
        // the current meshHelper we will be populating data into.
        ofxAssimpMeshHelper meshHelper;
        
        meshHelper.mesh = mesh;
        
        // set the mesh's default values.
        aiColor4D dcolor = aiColor4D(0.8f, 0.8f, 0.8f, 1.0f);
        meshHelper.diffuseColor = dcolor;

        aiColor4D scolor = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
        meshHelper.specularColor = scolor;

        aiColor4D acolor = aiColor4D(0.2f, 0.2f, 0.2f, 1.0f);
        meshHelper.ambientColor = acolor;

        aiColor4D ecolor = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
        meshHelper.emissiveColor = ecolor;

        // Handle material info
        aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];
        
        // Load Textures
        int texIndex = 0;
        aiString texPath;
        
        //meshHelper.texture = NULL;
        
        // TODO: handle other aiTextureTypes
        if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath)){
            // This is magic. Thanks Kyle.
            
            textures.push_back(ofImage());            
            
            ofImage& image = textures.back();                

            ofLog(OF_LOG_VERBOSE, "loading image from %s", texPath.data);
            
            image.loadImage(modelFolder + texPath.data);
            image.update();
            
            ofLog(OF_LOG_VERBOSE, "texture width: %f height %f", image.getWidth(), image.getHeight());
            
            //meshHelper.texture = &(image.getTextureReference()); 
            meshHelper.textureIndex = textures.size()-1;
        }
        
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &dcolor))
            meshHelper.diffuseColor = dcolor;
        
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &scolor))
            meshHelper.specularColor = scolor;
        
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &acolor))
            meshHelper.ambientColor = acolor;
        
        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &ecolor))
            meshHelper.emissiveColor = ecolor;
        
        // Culling
        unsigned int max = 1;
        int two_sided;
        if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
            meshHelper.twoSided = true;
        else
            meshHelper.twoSided = false;
        
        // Create a VBO for our vertices
        GLuint vhandle;
        glGenBuffers(1, &vhandle);
        
        glBindBuffer(GL_ARRAY_BUFFER, vhandle);
        
        if(getAnimationCount())
            glBufferData(GL_ARRAY_BUFFER, sizeof(aiVertex) * mesh->mNumVertices, NULL, GL_STREAM_DRAW/*GL_STATIC_DRAW GL_STREAM_DRAW*/);
        else
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
           
            // No longer in aiVertex VBO structure
            /*          
            if (NULL == mesh->mTangents)
            {
                verts->vTangent = aiVector3D(0.0f,0.0f,0.0f);
                verts->vBitangent = aiVector3D(0.0f,0.0f,0.0f);
            }
            else
            {
                verts->vTangent = mesh->mTangents[x];
                verts->vBitangent = mesh->mBitangents[x];
            }
             
            if (mesh->HasTextureCoords(1))
                verts->vTextureUV2 = mesh->mTextureCoords[1][x];
            else 
                verts->vTextureUV2 = aiVector3D(0.5f,0.5f, 0.0f);

            if( mesh->HasBones()){
                unsigned char boneIndices[4] = { 0, 0, 0, 0 };
                unsigned char boneWeights[4] = { 0, 0, 0, 0 };
                ai_assert( weightsPerVertex[x].size() <= 4);

                for( unsigned int a = 0; a < weightsPerVertex[x].size(); a++){
                    boneIndices[a] = weightsPerVertex[x][a].mVertexId;
                    boneWeights[a] = (unsigned char) (weightsPerVertex[x][a].mWeight * 255.0f);
                }

                memcpy( verts->mBoneIndices, boneIndices, sizeof( boneIndices));
                memcpy( verts->mBoneWeights, boneWeights, sizeof( boneWeights));
            }
            else{
               // memset( verts->mBoneIndices, 0, sizeof( verts->mBoneIndices));
               // memset( verts->mBoneWeights, 0, sizeof( verts->mBoneWeights));
            }
 */           
            ++verts;
        }
        
        glUnmapBufferARB(GL_ARRAY_BUFFER_ARB); //invalidates verts
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // set the mesh vertex buffer handle to our new vertex buffer.
        meshHelper.vertexBuffer = vhandle;
        
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
        meshHelper.indexBuffer = ihandle;
        meshHelper.numIndices = mesh->mNumFaces * nidx;
        
        // create the normal buffer. Assimp View creates a second normal buffer. Unsure why. Using only the interleaved normals for now.
        // This is here for reference.
        
        /*
        GLuint nhandle;
        glGenBuffers(1, &nhandle);
        glBindBuffer(GL_ARRAY_BUFFER, nhandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D)* mesh->mNumVertices, NULL, GL_STATIC_DRAW);

        // populate normals
        aiVector3D* normals = (aiVector3D*)glMapBuffer(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);

        for (unsigned int x = 0; x < mesh->mNumVertices; ++x)
        {
        aiVector3D vNormal = mesh->mNormals[x];
        *normals = vNormal;
        ++normals;
        }

        glUnmapBufferARB(GL_ARRAY_BUFFER_ARB); //invalidates verts
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        meshHelper.normalBuffer = nhandle;
        */
        
        // Create VAO and populate it
        GLuint vaoHandle; 
        glGenVertexArrays(1, &vaoHandle);
            
        // TODO: equivalent PC call.
        glBindVertexArray(vaoHandle);
        
        glBindBuffer(GL_ARRAY_BUFFER, meshHelper.vertexBuffer);
        
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
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshHelper.indexBuffer);
        
        glBindVertexArray(0);
       
        // save the VAO handle into our mesh helper
        meshHelper.vao = vaoHandle;
        
        modelMeshes.push_back(meshHelper);
    }
    ofLog(OF_LOG_VERBOSE, "finished loading gl resources");
}

//-------------------------------------------
void ofxAssimpModelLoader::deleteGLResources(){

    ofLog(OF_LOG_VERBOSE, "deleting gl resources");

    for (unsigned int i = 0; i < modelMeshes.size(); ++i){
    
        ofxAssimpMeshHelper meshHelper = modelMeshes[i];
        
        const GLuint indexBuffer = meshHelper.indexBuffer;
        const GLuint vertexBuffer = meshHelper.vertexBuffer;
        const GLuint normalBuffer = meshHelper.normalBuffer;
        const GLuint vaoHandle = meshHelper.vao;
        
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &indexBuffer);
        glDeleteBuffers(1, &normalBuffer);
        glDeleteVertexArrays(1, &vaoHandle);
        
        meshHelper.indexBuffer = 0;
        meshHelper.vertexBuffer = 0;
        meshHelper.normalBuffer = 0;
        meshHelper.vao = 0; 
        meshHelper.mesh = NULL;
        
        meshHelper.textureIndex = -1;
    }
    
    // clear out our meshes array.
    modelMeshes.clear();
    
    // clear out our textures (ofImages)
    textures.clear();
}

//-------------------------------------------
ofxAssimpModelLoader::~ofxAssimpModelLoader(){
}


//-------------------------------------------
void ofxAssimpModelLoader::getBoundingBoxWithMinVector(struct aiVector3D* min, struct aiVector3D* max)
{
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);
    
	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
    
    this->getBoundingBoxForNode(scene->mRootNode, min, max, &trafo);
}

//-------------------------------------------
void ofxAssimpModelLoader::getBoundingBoxForNode(const struct aiNode* nd,  struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo)
{
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;
    
	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);
    
	for (; n < nd->mNumMeshes; ++n){
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t){
        	struct aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);
        
            
			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);
            
			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}
    
	for (n = 0; n < nd->mNumChildren; ++n){
		this->getBoundingBoxForNode(nd->mChildren[n], min, max, trafo);
	}
    
	*trafo = prev;
}

//-------------------------------------------
unsigned int ofxAssimpModelLoader::getAnimationCount(){
    if(scene)
        return scene->mNumAnimations; 
    else {
        ofLog(OF_LOG_WARNING, "No Model Loaded");
        return 0;
    }

}

//-------------------------------------------
void ofxAssimpModelLoader::setAnimation(int anim){
        currentAnimation = MIN(anim, scene->mNumAnimations);
}

//-------------------------------------------
float ofxAssimpModelLoader::getDuration(int animation){
    const aiAnimation* anim = scene->mAnimations[animation];
    return anim->mDuration;
}


//-------------------------------------------
void ofxAssimpModelLoader::setNormalizedTime(float t){ // 0 - 1
    
    if(getAnimationCount())
    {

        const aiAnimation* anim = scene->mAnimations[currentAnimation];
        float realT = ofMap(t, 0.0, 1.0, 0.0, (float)anim->mDuration, false);

        setTime(realT);
    }
}

void ofxAssimpModelLoader::setTime(float t){ // 0 - 1
    if(getAnimationCount()){
        
        // only evaluate if we have a delta t.
        if(animationTime != t){
            animationTime = t;
            updateAnimation(currentAnimation, animationTime);
        }
    }
}


//-------------------------------------------
void ofxAssimpModelLoader::setPosition(float x, float y, float z){
    pos.x = x;
    pos.y = y;
    pos.z = z;
}

//-------------------------------------------
void ofxAssimpModelLoader::setScale(float x, float y, float z){
    scale.x = x;
    scale.y = y;
    scale.z = z;
}

//-------------------------------------------
void ofxAssimpModelLoader::setScaleNomalization(bool normalize)
{
    normalizeScale = normalize;
}


//-------------------------------------------
void ofxAssimpModelLoader::setRotation(int which, float angle, float rot_x, float rot_y, float rot_z){
    if(which + 1 > numRotations){
        int diff = 1 + (which - numRotations);
        for(int i = 0; i < diff; i++){
            rotAngle.push_back(0);
            rotAxis.push_back(ofPoint());
        }
        numRotations = rotAngle.size();
    }
    
    rotAngle[which]  = angle;
    rotAxis[which].x = rot_x;
    rotAxis[which].y = rot_y;
    rotAxis[which].z = rot_z;
}

//-------------------------------------------
void ofxAssimpModelLoader::updateAnimation(unsigned int animationIndex, float currentTime){
    
    const aiAnimation* mAnim = scene->mAnimations[animationIndex];
  
    // calculate the transformations for each animation channel
	for( unsigned int a = 0; a < mAnim->mNumChannels; a++)
	{
		const aiNodeAnim* channel = mAnim->mChannels[a];
                
        aiNode* targetNode = scene->mRootNode->FindNode(channel->mNodeName);

        // ******** Position *****
        aiVector3D presentPosition( 0, 0, 0);
        if( channel->mNumPositionKeys > 0)
        {
            // Look for present frame number. Search from last position if time is after the last time, else from beginning
            // Should be much quicker than always looking from start for the average use case.
            unsigned int frame = 0;// (currentTime >= lastAnimationTime) ? lastFramePositionIndex : 0;
            while( frame < channel->mNumPositionKeys - 1)
            {
                if( currentTime < channel->mPositionKeys[frame+1].mTime)
                    break;
                frame++;
            }
            
            // interpolate between this frame's value and next frame's value
            unsigned int nextFrame = (frame + 1) % channel->mNumPositionKeys;
            const aiVectorKey& key = channel->mPositionKeys[frame];
            const aiVectorKey& nextKey = channel->mPositionKeys[nextFrame];
            double diffTime = nextKey.mTime - key.mTime;
            if( diffTime < 0.0)
                diffTime += mAnim->mDuration;
            if( diffTime > 0)
            {
                float factor = float( (currentTime - key.mTime) / diffTime);
                presentPosition = key.mValue + (nextKey.mValue - key.mValue) * factor;
            } else
            {
                presentPosition = key.mValue;
            }
        }
        
        // ******** Rotation *********
        aiQuaternion presentRotation( 1, 0, 0, 0);
        if( channel->mNumRotationKeys > 0)
        {
            unsigned int frame = 0;//(currentTime >= lastAnimationTime) ? lastFrameRotationIndex : 0;
            while( frame < channel->mNumRotationKeys - 1)
            {
                if( currentTime < channel->mRotationKeys[frame+1].mTime)
                    break;
                frame++;
            }
            
            // interpolate between this frame's value and next frame's value
            unsigned int nextFrame = (frame + 1) % channel->mNumRotationKeys;
            const aiQuatKey& key = channel->mRotationKeys[frame];
            const aiQuatKey& nextKey = channel->mRotationKeys[nextFrame];
            double diffTime = nextKey.mTime - key.mTime;
            if( diffTime < 0.0)
                diffTime += mAnim->mDuration;
            if( diffTime > 0)
            {
                float factor = float( (currentTime - key.mTime) / diffTime);
                aiQuaternion::Interpolate( presentRotation, key.mValue, nextKey.mValue, factor);
            } else
            {
                presentRotation = key.mValue;
            }
        }
        
        // ******** Scaling **********
        aiVector3D presentScaling( 1, 1, 1);
        if( channel->mNumScalingKeys > 0)
        {
            unsigned int frame = 0;//(currentTime >= lastAnimationTime) ? lastFrameScaleIndex : 0;
            while( frame < channel->mNumScalingKeys - 1)
            {
                if( currentTime < channel->mScalingKeys[frame+1].mTime)
                    break;
                frame++;
            }
            
            // TODO: (thom) interpolation maybe? This time maybe even logarithmic, not linear
            presentScaling = channel->mScalingKeys[frame].mValue;
        }
        
        // build a transformation matrix from it
        //aiMatrix4x4& mat;// = mTransforms[a];
        aiMatrix4x4 mat = aiMatrix4x4( presentRotation.GetMatrix());
        mat.a1 *= presentScaling.x; mat.b1 *= presentScaling.x; mat.c1 *= presentScaling.x;
        mat.a2 *= presentScaling.y; mat.b2 *= presentScaling.y; mat.c2 *= presentScaling.y;
        mat.a3 *= presentScaling.z; mat.b3 *= presentScaling.z; mat.c3 *= presentScaling.z;
        mat.a4 = presentPosition.x; mat.b4 = presentPosition.y; mat.c4 = presentPosition.z;
        //mat.Transpose();
        
        targetNode->mTransformation = mat;

    }

    lastAnimationTime = currentTime;

/*
 for( size_t a = 0; a < anim->mNumChannels; ++a)
 {
 const aiNodeAnim* channel = anim->mChannels[a];
        aiVector3D curPosition;
        aiQuaternion curRotation;
        // scaling purposefully left out 
        
        // find the node which the channel affects
        aiNode* targetNode = scene->mRootNode->FindNode(channel->mNodeName);
        
        // find current position
        // Should be much quicker than always looking from start for the average use case.
        unsigned int frame = (currentTime >= lastAnimationTime) ? lastFramePositionIndex : 0;
        while( frame < channel->mNumPositionKeys - 1)
        {
            if( currentTime < channel->mPositionKeys[frame+1].mTime)
                break;
            frame++;
        }        
                
        // interpolate between this frame's value and next frame's value
        unsigned int nextFrame = (frame + 1) % channel->mNumPositionKeys;
        const aiVectorKey& key = channel->mPositionKeys[frame];
        const aiVectorKey& nextKey = channel->mPositionKeys[nextFrame];
        double diffTime = nextKey.mTime - key.mTime;
        if( diffTime < 0.0)
            diffTime += anim->mDuration;
        if( diffTime > 0)
        {
            float factor = float( (currentTime - key.mTime) / diffTime);
            curPosition = key.mValue + (nextKey.mValue - key.mValue) * factor;
        } else
        {
            curPosition = key.mValue;
        }
        
        lastFramePositionIndex = frame;

        
        // maybe add a check here if the anim has any position keys at all
        //curPosition = channel->mPositionKeys[posIndex].mValue;
        
        // same goes for rotation, but I shorten it now
        size_t rotIndex = 0;
        while( 1 )
        {
            if( rotIndex+1 >= channel->mNumRotationKeys )
                break;
            
            if( channel->mRotationKeys[rotIndex+1].mTime > currentTime )
                break;
            rotIndex++;
        } 
        
        
        curRotation = channel->mRotationKeys[rotIndex].mValue;
        
        // now build a transformation matrix from it. First rotation, thenn push position in it as well. 
        aiMatrix4x4 trafo = (aiMatrix4x4) curRotation.GetMatrix();
        trafo.a4 = curPosition.x; trafo.b4 = curPosition.y; trafo.c4 = curPosition.z;
        
        // assign this transformation to the node
        targetNode->mTransformation = trafo;
    }
    
    lastAnimationTime = currentTime;
    */
}

//-------------------------------------------
void ofxAssimpModelLoader::updateGLResources(){
        
    // update mesh position for the animation
    for (unsigned int i = 0; i < modelMeshes.size(); ++i){

        // current mesh we are introspecting
        const aiMesh* mesh = modelMeshes[i].mesh;
        
        // calculate bone matrices
        std::vector<aiMatrix4x4> boneMatrices( mesh->mNumBones);
        for( size_t a = 0; a < mesh->mNumBones; ++a)
        {
            const aiBone* bone = mesh->mBones[a];
            
            // find the corresponding node by again looking recursively through the node hierarchy for the same name
            aiNode* node = scene->mRootNode->FindNode(bone->mName);
            
            // start with the mesh-to-bone matrix 
            boneMatrices[a] = bone->mOffsetMatrix;
            // and now append all node transformations down the parent chain until we're back at mesh coordinates again
            const aiNode* tempNode = node;
            while( tempNode)
            {
                // check your matrix multiplication order here!!!
                boneMatrices[a] = tempNode->mTransformation * boneMatrices[a];   
                // boneMatrices[a] = boneMatrices[a] * tempNode->mTransformation;
                tempNode = tempNode->mParent;
            }
        }
        
        // all using the results from the previous code snippet
        std::vector<aiVector3D> resultPos( mesh->mNumVertices); 
        std::vector<aiVector3D> resultNorm( mesh->mNumVertices);
        
        // loop through all vertex weights of all bones
        for( size_t a = 0; a < mesh->mNumBones; ++a)
        {
            const aiBone* bone = mesh->mBones[a];
            const aiMatrix4x4& posTrafo = boneMatrices[a];
            
            // 3x3 matrix, contains the bone matrix without the translation, only with rotation and possibly scaling
            aiMatrix3x3 normTrafo = aiMatrix3x3( posTrafo); 
            for( size_t b = 0; b < bone->mNumWeights; ++b)
            {
                const aiVertexWeight& weight = bone->mWeights[b];
                
                size_t vertexId = weight.mVertexId; 
                const aiVector3D& srcPos = mesh->mVertices[vertexId];
                const aiVector3D& srcNorm = mesh->mNormals[vertexId];
                
                resultPos[vertexId] += weight.mWeight * (posTrafo * srcPos);
                resultNorm[vertexId] += weight.mWeight * (normTrafo * srcNorm);
            }
        }
                
        // now upload the result position and normal along with the other vertex attributes into a dynamic vertex buffer, VBO or whatever
        
        // get mesh helper for this mesh;
        ofxAssimpMeshHelper meshHelper = modelMeshes[i];
        
        glBindBuffer(GL_ARRAY_BUFFER, meshHelper.vertexBuffer);
        aiVertex* verts = (aiVertex*)glMapBuffer(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
        
        for (unsigned int x = 0; x < mesh->mNumVertices; ++x)
        {
            //verts->vPosition = mesh->mVertices[x];
            verts->vPosition = resultPos[x];
            
            if (NULL == mesh->mNormals)
                verts->vNormal = aiVector3D(0.0f,0.0f,0.0f);
            else
                verts->vNormal = resultNorm[x];
            
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
            
            // No longer in aiVertex VBO structure
            /*          
             if (NULL == mesh->mTangents)
             {
             verts->vTangent = aiVector3D(0.0f,0.0f,0.0f);
             verts->vBitangent = aiVector3D(0.0f,0.0f,0.0f);
             }
             else
             {
             verts->vTangent = mesh->mTangents[x];
             verts->vBitangent = mesh->mBitangents[x];
             }
             
             if (mesh->HasTextureCoords(1))
             verts->vTextureUV2 = mesh->mTextureCoords[1][x];
             else 
             verts->vTextureUV2 = aiVector3D(0.5f,0.5f, 0.0f);
             
             if( mesh->HasBones()){
             unsigned char boneIndices[4] = { 0, 0, 0, 0 };
             unsigned char boneWeights[4] = { 0, 0, 0, 0 };
             ai_assert( weightsPerVertex[x].size() <= 4);
             
             for( unsigned int a = 0; a < weightsPerVertex[x].size(); a++){
             boneIndices[a] = weightsPerVertex[x][a].mVertexId;
             boneWeights[a] = (unsigned char) (weightsPerVertex[x][a].mWeight * 255.0f);
             }
             
             memcpy( verts->mBoneIndices, boneIndices, sizeof( boneIndices));
             memcpy( verts->mBoneWeights, boneWeights, sizeof( boneWeights));
             }
             else{
             // memset( verts->mBoneIndices, 0, sizeof( verts->mBoneIndices));
             // memset( verts->mBoneWeights, 0, sizeof( verts->mBoneWeights));
             }
             */           
            ++verts;
        }
        
        glUnmapBufferARB(GL_ARRAY_BUFFER_ARB); //invalidates verts
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}


void ofxAssimpModelLoader::draw()
{
    if(scene){
        
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        
        glEnable(GL_NORMALIZE);
        
        glPushMatrix();
            
        glTranslatef(pos.x, pos.y, pos.z);

        glRotatef(180, 0, 0, 1);
        glTranslated(-scene_center.x, -scene_center.y, scene_center.z);    

        if(normalizeScale)
        {
            glScaled(normalizedScale , normalizedScale, normalizedScale);
        }
            
        for(int i = 0; i < numRotations; i++){
            glRotatef(rotAngle[i], rotAxis[i].x, rotAxis[i].y, rotAxis[i].z);
        }
        
        glScalef(scale.x, scale.y, scale.z);
        
        if(getAnimationCount())
        {
            updateGLResources();
        }
        
        if(modelMeshes.size())
        {
            for(int i = 0; i < modelMeshes.size(); i++){
                ofxAssimpMeshHelper meshHelper = modelMeshes.at(i);
                
                // Texture Binding
                if(meshHelper.textureIndex!=-1){
                    textures[meshHelper.textureIndex].getTextureReference().bind();
                }
                // Set up meterial state.

                float dc[4];
                float sc[4];
                float ac[4];
                float emc[4];
                
                // Material colors and properties
                color4_to_float4(&meshHelper.diffuseColor, dc);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dc);
                
                color4_to_float4(&meshHelper.specularColor, sc);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sc);
                
                color4_to_float4(&meshHelper.ambientColor, ac);
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ac);
                
                color4_to_float4(&meshHelper.emissiveColor, emc);
                glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emc);
                

                // Culling
                if(meshHelper.twoSided)
                    glEnable(GL_CULL_FACE);
                else 
                    glDisable(GL_CULL_FACE);
                        
                // TODO: equivalent VAO callfor linux and windows.
                glBindVertexArray(meshHelper.vao);
                glDrawElements(GL_TRIANGLES, meshHelper.numIndices, GL_UNSIGNED_INT, 0);
                
                // Texture Binding
                if(meshHelper.textureIndex!=-1){
                    textures[meshHelper.textureIndex].getTextureReference().unbind();
                }
            }
        }        
            
        glPopMatrix();
        
        glPopClientAttrib();
        glPopAttrib();
    }
}
