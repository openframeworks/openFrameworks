#include "ofxAssimpModelLoader.h"

#include "aiConfig.h"
#include "aiPostProcess.h"
#include <assert.h>

//--------------------------------------------------------------
static inline ofColor aiColorToOfColor(const aiColor4D& c){
	return ofColor(255*c.r,255*c.g,255*c.b,255*c.a);
}

//--------------------------------------------------------------
static inline ofColor aiColorToOfColor(const aiColor3D& c){
	return ofColor(255*c.r,255*c.g,255*c.b,255);
}

//--------------------------------------------------------------
static inline ofVec3f aiVecToOfVec(const aiVector3D& v){
	return ofVec3f(v.x,v.y,v.z);
}

static inline vector<ofVec3f> aiVecVecToOfVecVec(const vector<aiVector3D>& v){
	vector<ofVec3f> ofv(v.size());
	if(sizeof(aiVector3D)==sizeof(ofVec3f)){
		memcpy(&ofv[0],&v[0],v.size()*sizeof(ofVec3f));
	}else{
		for(int i=0;i<(int)v.size();i++){
			ofv[i]=aiVecToOfVec(v[i]);
		}
	}
	return ofv;
}

//--------------------------------------------------------------
static void aiMeshToOfMesh(const aiMesh* aim, ofMesh& ofm){
	// default to triangle mode
	ofm.setMode(OF_TRIANGLES_MODE);

	// copy vertices
	for (int i=0; i < (int)aim->mNumVertices;i++){
		ofm.addVertex(ofVec3f(aim->mVertices[i].x,aim->mVertices[i].y,aim->mVertices[i].z));
	}

	if(aim->HasNormals()){
		for (int i=0; i < (int)aim->mNumVertices;i++){
			ofm.addNormal(ofVec3f(aim->mNormals[i].x,aim->mNormals[i].y,aim->mNormals[i].z));
		}
	}

	// aiVector3D * 	mTextureCoords [AI_MAX_NUMBER_OF_TEXTURECOORDS]
	// just one for now
	if(aim->GetNumUVChannels()>0){
		for (int i=0; i < (int)aim->mNumVertices;i++){
			ofm.addTexCoord(ofVec2f(aim->mTextureCoords[0][i].x ,aim->mTextureCoords[0][i].y));
		}
	}

	//aiColor4D * 	mColors [AI_MAX_NUMBER_OF_COLOR_SETS]
	// just one for now
	if(aim->GetNumColorChannels()>0){
		for (int i=0; i < (int)aim->mNumVertices;i++){
			ofm.addColor(aiColorToOfColor(aim->mColors[0][i]));
		}
	}

	for (int i=0; i <(int) aim->mNumFaces;i++){
		if(aim->mFaces[i].mNumIndices>3){
			ofLog(OF_LOG_WARNING,"non-triangular face found: model face # " + ofToString(i));
		}
		for (int j=0; j<(int)aim->mFaces[i].mNumIndices; j++){
			ofm.addIndex(aim->mFaces[i].mIndices[j]);
		}
	}

	ofm.setName(string(aim->mName.data));
	//	ofm.materialId = aim->mMaterialIndex;
}

//--------------------------------------------------------------
static void aiMatrix4x4ToOfMatrix4x4(const aiMatrix4x4& aim, ofNode& ofm){
	float m[16] = { aim.a1,aim.a2,aim.a3,aim.a4,
					aim.b1,aim.b2,aim.b3,aim.b4,
					aim.c1,aim.c2,aim.c3,aim.c4,
					aim.d1,aim.d2,aim.d3,aim.d4 };

	ofm.setTransformMatrix(	m);
}
/*
//--------------------------------------------------------------
static void aiNodeToOfNode(const aiNode* ain, ofNode& ofn, const ofModel& model){
	aiMatrix4x4ToOfMatrix4x4(ain->mTransformation, ofn);
//	for (int i =0; i < (int)ain->mNumMeshes;i++){
//		ofn.addMesh(model.meshes.at(ain->mMeshes[i]));
//	}
//	ofn.setName(string(ain->mName.data));
}

//--------------------------------------------------------------
static int createNodes(const aiNode* curNode, ofModel& model){
	//lets only make nodes that have meshes for now
	if(curNode->mNumMeshes){
		model.nodes.push_back(ofNode());
		aiNodeToOfNode(curNode, model.nodes.back(), model);
	}

	if (curNode->mNumChildren>0){
		for (int i =0; i<(int)curNode->mNumChildren;i++){
			createNodes(curNode->mChildren[i], model);
		}
	}else return 0;

#warning "this is returning nothing ig nNumChildren i>0  fix it"
}

//--------------------------------------------------------------
void createBones(const aiScene* scene, ofModel& model){
	for (int i =0; i < (int)scene->mNumMeshes;i++){
		aiMesh& curMesh = *scene->mMeshes[i];
		if(curMesh.HasBones()){
			for (int j=0; j < (int)curMesh.mNumBones;j++){
				aiNode* boneNode = scene->mRootNode->FindNode(curMesh.mBones[j]->mName);
			}
		}
	}
}*/


ofxAssimpModelLoader::ofxAssimpModelLoader(){
	scene = NULL;
	clear();
}

//------------------------------------------
bool ofxAssimpModelLoader::loadModel(string modelName, bool optimize){


    // if we have a model loaded, unload the fucker.
    if(scene != NULL){
        clear();
    }


    // Load our new path.
    filepath = ofToDataPath(modelName);

	//theo added - so we can have models and their textures in sub folders
	modelFolder = ofFilePath::getEnclosingDirectory(filepath);

    ofLog(OF_LOG_VERBOSE, "loading model %s", filepath.c_str());
    ofLog(OF_LOG_VERBOSE, "loading from folder %s", modelFolder.c_str());

    // only ever give us triangles.
    aiSetImportPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
    aiSetImportPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);

    // aiProcess_FlipUVs is for VAR code. Not needed otherwise. Not sure why.
    unsigned int flags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_FlipUVs;
    if(optimize) flags |=  aiProcess_ImproveCacheLocality | aiProcess_OptimizeGraph |
			aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices |
			aiProcess_RemoveRedundantMaterials;

    scene = aiImportFile(filepath.c_str(), flags);

    if(scene){
        calculateDimensions();
        loadGLResources();

        if(getAnimationCount())
            ofLog(OF_LOG_VERBOSE, "scene has animations");
        else {
            ofLog(OF_LOG_VERBOSE, "no animations");

        }
        return true;
    }else{
    	ofLog(OF_LOG_ERROR,string("ofxAssimpModelLoader: ") + aiGetErrorString());
    	return false;
    }
}

//-------------------------------------------
bool ofxAssimpModelLoader::loadModel(ofBuffer & buffer, bool optimize, const char * extension){
	// only ever give us triangles.
	aiSetImportPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
	aiSetImportPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);

	// aiProcess_FlipUVs is for VAR code. Not needed otherwise. Not sure why.
	unsigned int flags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_FlipUVs;
	if(optimize) flags |=  aiProcess_ImproveCacheLocality | aiProcess_OptimizeGraph |
			aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices |
			aiProcess_RemoveRedundantMaterials;

	if(scene){
		clear();
	}
	scene = aiImportFileFromMemory(buffer.getBinaryBuffer(), buffer.size(), flags, extension);
	if(scene){
		calculateDimensions();
		loadGLResources();

		if(getAnimationCount())
			ofLog(OF_LOG_VERBOSE, "scene has animations");
		else {
			ofLog(OF_LOG_VERBOSE, "no animations");

		}
		return true;
	}else{
		ofLog(OF_LOG_ERROR,string("ofxAssimpModelLoader: ") + aiGetErrorString());
		return false;
	}

}

//-------------------------------------------
void ofxAssimpModelLoader::createEmptyModel(){
	if(scene){
		clear();
	}
	scene = new aiScene;
}

//-------------------------------------------
void ofxAssimpModelLoader::calculateDimensions(){
	if(!scene) return;
	ofLog(OF_LOG_VERBOSE, "initted scene with %i meshes & %i animations", scene->mNumMeshes, scene->mNumAnimations);

	getBoundingBoxWithMinVector(&scene_min, &scene_max);
	scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
	scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
	scene_center.z = (scene_min.z + scene_max.z) / 2.0f;

	// optional normalized scaling
	normalizedScale = scene_max.x-scene_min.x;
	normalizedScale = MAX(scene_max.y - scene_min.y,normalizedScale);
	normalizedScale = MAX(scene_max.z - scene_min.z,normalizedScale);
	normalizedScale = 1.f / normalizedScale;
	normalizedScale *= ofGetWidth() / 2.0;
}

//-------------------------------------------
void ofxAssimpModelLoader::createLightsFromAiModel(){
	lights.clear();
	lights.resize(scene->mNumLights);
	for(int i=0; i<(int)scene->mNumLights; i++){
		lights[i].enable();
		if(scene->mLights[i]->mType==aiLightSource_DIRECTIONAL){
			lights[i].setDirectional();
			lights[i].setOrientation(aiVecToOfVec(scene->mLights[i]->mDirection));
		}
		if(scene->mLights[i]->mType!=aiLightSource_POINT){
			lights[i].setSpotlight();
			lights[i].setPosition(aiVecToOfVec(scene->mLights[i]->mPosition));
		}
		lights[i].setAmbientColor(aiColorToOfColor(scene->mLights[i]->mColorAmbient));
		lights[i].setDiffuseColor(aiColorToOfColor(scene->mLights[i]->mColorDiffuse));
		lights[i].setSpecularColor(aiColorToOfColor(scene->mLights[i]->mColorSpecular));
	}
}

void ofxAssimpModelLoader::optimizeScene(){
	aiApplyPostProcessing(scene,aiProcess_ImproveCacheLocality | aiProcess_OptimizeGraph |
			aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices |
			aiProcess_RemoveRedundantMaterials);
}

//-------------------------------------------
void ofxAssimpModelLoader::loadGLResources(){

    ofLog(OF_LOG_VERBOSE, "loading gl resources");

    // create new mesh helpers for each mesh, will populate their data later.
   // modelMeshes.resize(scene->mNumMeshes,ofxAssimpMeshHelper());

    // create OpenGL buffers and populate them based on each meshes pertinant info.
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
        ofLog(OF_LOG_VERBOSE, "loading mesh %u", i);
        // current mesh we are introspecting
        aiMesh* mesh = scene->mMeshes[i];

        // the current meshHelper we will be populating data into.
        //ofxAssimpMeshHelper & meshHelper = modelMeshes[i];
        ofxAssimpMeshHelper meshHelper;

        meshHelper.mesh = mesh;
        aiMeshToOfMesh(mesh,meshHelper.cachedMesh);
        meshHelper.cachedMesh.setMode(OF_TRIANGLES_MODE);
        meshHelper.validCache = true;
        meshHelper.hasChanged = false;

        meshHelper.animatedPos.resize(mesh->mNumVertices);
        if(mesh->HasNormals()){
        	meshHelper.animatedNorm.resize(mesh->mNumVertices);
        }

        // Handle material info
        aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];

        // Load Textures
        int texIndex = 0;
        aiString texPath;

        //meshHelper.texture = NULL;

        // TODO: handle other aiTextureTypes
        if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath)){
            // This is magic. Thanks Kyle.

            ofLog(OF_LOG_VERBOSE, "loading image from %s", texPath.data);
            string modelFolder = ofFilePath::getEnclosingDirectory(filepath);

			if(ofFilePath::isAbsolute(texPath.data) && ofFile::doesFileExist(texPath.data)) {
				if(!ofLoadImage(meshHelper.texture,texPath.data)){
					ofLog(OF_LOG_ERROR,string("error loading image ") + texPath.data);
				}
			}
			else {
				if(!ofLoadImage(meshHelper.texture,modelFolder + texPath.data)){
					ofLog(OF_LOG_ERROR,"error loading image " + modelFolder + texPath.data);
				}
			}

            ofLog(OF_LOG_VERBOSE, "texture width: %f height %f", meshHelper.texture.getWidth(), meshHelper.texture.getHeight());

        }

        aiColor4D dcolor, scolor, acolor, ecolor;

        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &dcolor)){
            meshHelper.material.setDiffuseColor(aiColorToOfColor(dcolor));
        }

        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &scolor)){
        	meshHelper.material.setSpecularColor(aiColorToOfColor(scolor));
        }

        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &acolor)){
        	meshHelper.material.setAmbientColor(aiColorToOfColor(acolor));
        }

        if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &ecolor)){
        	meshHelper.material.setEmissiveColor(aiColorToOfColor(ecolor));
        }

        float shininess;
        if(AI_SUCCESS == aiGetMaterialFloat(mtl, AI_MATKEY_SHININESS, &shininess)){
			meshHelper.material.setShininess(shininess);
		}

        int blendMode;
		if(AI_SUCCESS == aiGetMaterialInteger(mtl, AI_MATKEY_BLEND_FUNC, &blendMode)){
			if(blendMode==aiBlendMode_Default){
				meshHelper.blendMode=OF_BLENDMODE_ALPHA;
			}else{
				meshHelper.blendMode=OF_BLENDMODE_ADD;
			}
		}


        // Culling
        unsigned int max = 1;
        int two_sided;
        if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
            meshHelper.twoSided = true;
        else
            meshHelper.twoSided = false;

        int usage;
        if(getAnimationCount()){
#ifndef TARGET_OPENGLES
        	usage = GL_STREAM_DRAW;
#else
        	usage = GL_DYNAMIC_DRAW;
#endif
        }else{
        	usage = GL_STATIC_DRAW;

        }
        meshHelper.vbo.setVertexData(&mesh->mVertices[0].x,3,mesh->mNumVertices,usage,sizeof(aiVector3D));
        if(mesh->HasVertexColors(0)){
        	meshHelper.vbo.setColorData(&mesh->mColors[0][0].r,mesh->mNumVertices,GL_STATIC_DRAW,sizeof(aiColor4D));
        }
        if(mesh->HasNormals()){
        	meshHelper.vbo.setNormalData(&mesh->mNormals[0].x,mesh->mNumVertices,usage,sizeof(aiVector3D));
        }
        if (mesh->HasTextureCoords(0)){
        	meshHelper.vbo.setTexCoordData(&mesh->mTextureCoords[0][0].x,mesh->mNumVertices,GL_STATIC_DRAW,sizeof(aiVector3D));
        }

        meshHelper.indices.resize(mesh->mNumFaces * 3);
        int i=0;
        for (unsigned int x = 0; x < mesh->mNumFaces; ++x){
			for (unsigned int a = 0; a < mesh->mFaces[x].mNumIndices; ++a){
				meshHelper.indices[i++]=mesh->mFaces[x].mIndices[a];
			}
		}

        meshHelper.vbo.setIndexData(&meshHelper.indices[0],meshHelper.indices.size(),GL_STATIC_DRAW);
        modelMeshes.push_back(meshHelper);
    }

    animationTime = -1;
    setNormalizedTime(0);

    ofLog(OF_LOG_VERBOSE, "finished loading gl resources");

}

//-------------------------------------------
void ofxAssimpModelLoader::clear(){

    ofLog(OF_LOG_VERBOSE, "deleting gl resources");

    // clear out everything.
    modelMeshes.clear();
    pos.set(0,0,0);
    scale.set(1,1,1);
    rotAngle.clear();
    rotAxis.clear();
    lights.clear();

    lastAnimationTime = 0;
    currentAnimation = 0;
    animationTime = 0;
    scale = ofPoint(1, 1, 1);
	if(scene){
		aiReleaseImport(scene);
		scene = NULL;
	}
    normalizeScale = true;
    bUsingMaterials = true;
    bUsingNormals = true;
    bUsingTextures = true;
    bUsingColors = true;
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


			min->x = MIN(min->x,tmp.x);
			min->y = MIN(min->y,tmp.y);
			min->z = MIN(min->z,tmp.z);

			max->x = MAX(max->x,tmp.x);
			max->y = MAX(max->y,tmp.y);
			max->z = MAX(max->z,tmp.z);
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
    currentAnimation = MIN(anim, (int)scene->mNumAnimations);
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
    if(which + 1 > (int)rotAngle.size()){
        int diff = 1 + (which - rotAngle.size());
        for(int i = 0; i < diff; i++){
            rotAngle.push_back(0);
            rotAxis.push_back(ofPoint());
        }
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
			modelMeshes[i].hasChanged = true;
			modelMeshes[i].validCache = false;
		}

		modelMeshes[i].animatedPos.assign(modelMeshes[i].animatedPos.size(),0);
		if(mesh->HasNormals()){
			modelMeshes[i].animatedNorm.assign(modelMeshes[i].animatedNorm.size(),0);
		}
		// loop through all vertex weights of all bones
		for( size_t a = 0; a < mesh->mNumBones; ++a)
		{
			const aiBone* bone = mesh->mBones[a];
			const aiMatrix4x4& posTrafo = boneMatrices[a];


			for( size_t b = 0; b < bone->mNumWeights; ++b)
			{
				const aiVertexWeight& weight = bone->mWeights[b];

				size_t vertexId = weight.mVertexId;
				const aiVector3D& srcPos = mesh->mVertices[vertexId];

				modelMeshes[i].animatedPos[vertexId] += weight.mWeight * (posTrafo * srcPos);
			}
			if(mesh->HasNormals()){
				// 3x3 matrix, contains the bone matrix without the translation, only with rotation and possibly scaling
				aiMatrix3x3 normTrafo = aiMatrix3x3( posTrafo);
				for( size_t b = 0; b < bone->mNumWeights; ++b)
				{
					const aiVertexWeight& weight = bone->mWeights[b];
					size_t vertexId = weight.mVertexId;

					const aiVector3D& srcNorm = mesh->mNormals[vertexId];
					modelMeshes[i].animatedNorm[vertexId] += weight.mWeight * (normTrafo * srcNorm);

				}
			}
		}
	}
}

//-------------------------------------------
void ofxAssimpModelLoader::updateGLResources(){
    // now upload the result position and normal along with the other vertex attributes into a dynamic vertex buffer, VBO or whatever
    for (unsigned int i = 0; i < modelMeshes.size(); ++i){
    	if(modelMeshes[i].hasChanged){
			const aiMesh* mesh = modelMeshes[i].mesh;
			modelMeshes[i].vbo.updateVertexData(&modelMeshes[i].animatedPos[0].x,mesh->mNumVertices);
			if(mesh->HasNormals()){
				 modelMeshes[i].vbo.updateNormalData(&modelMeshes[i].animatedNorm[0].x,mesh->mNumVertices);
			}
			modelMeshes[i].hasChanged = false;
    	}
    }
}


//--------------------------------------------------------------
void ofxAssimpModelLoader::drawWireframe(){
	draw(OF_MESH_WIREFRAME);
}

//--------------------------------------------------------------
void ofxAssimpModelLoader::drawFaces(){
	draw(OF_MESH_FILL);
}

//--------------------------------------------------------------
void ofxAssimpModelLoader::drawVertices(){
	draw(OF_MESH_POINTS);
}


//-------------------------------------------
void ofxAssimpModelLoader::draw(ofPolyRenderMode renderType)
{
    if(scene){

        ofPushStyle();

#ifndef TARGET_OPENGLES
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
#endif
        glEnable(GL_NORMALIZE);

        ofPushMatrix();

        ofTranslate(pos);

        ofRotate(180, 0, 0, 1);
        ofTranslate(-scene_center.x, -scene_center.y, scene_center.z);

        if(normalizeScale)
        {
            ofScale(normalizedScale , normalizedScale, normalizedScale);
        }

        for(int i = 0; i < (int)rotAngle.size(); i++){
            ofRotate(rotAngle[i], rotAxis[i].x, rotAxis[i].y, rotAxis[i].z);
        }

        ofScale(scale.x, scale.y, scale.z);


        if(getAnimationCount())
        {
            updateGLResources();
        }

		for(int i = 0; i < (int)modelMeshes.size(); i++){
			ofxAssimpMeshHelper & meshHelper = modelMeshes.at(i);

			// Texture Binding
			if(bUsingTextures && meshHelper.texture.bAllocated()){
				meshHelper.texture.bind();
			}

			if(bUsingMaterials){
				meshHelper.material.begin();
			}


			// Culling
			if(meshHelper.twoSided)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);

			ofEnableBlendMode(meshHelper.blendMode);
#ifndef TARGET_OPENGLES
		    meshHelper.vbo.drawElements(GL_TRIANGLES,meshHelper.indices.size());
#else
		    switch(renderType){
		    case OF_MESH_FILL:
		    	meshHelper.vbo.drawElements(GL_TRIANGLES,meshHelper.indices.size());
		    	break;
		    case OF_MESH_WIREFRAME:
		    	meshHelper.vbo.drawElements(GL_LINES,meshHelper.indices.size());
		    	break;
		    case OF_MESH_POINTS:
		    	meshHelper.vbo.drawElements(GL_POINTS,meshHelper.indices.size());
		    	break;
		    }
#endif

			// Texture Binding
			if(bUsingTextures && meshHelper.texture.bAllocated()){
				meshHelper.texture.unbind();
			}

			if(bUsingMaterials){
				meshHelper.material.end();
			}
		}

        ofPopMatrix();

        ofPopStyle();
#ifndef TARGET_OPENGLES
        glPopClientAttrib();
        glPopAttrib();
#endif
    }
}

//-------------------------------------------
vector<string> ofxAssimpModelLoader::getMeshNames(){
	vector<string> names(scene->mNumMeshes);
	for(int i=0; i<(int)scene->mNumMeshes; i++){
		names[i] = scene->mMeshes[i]->mName.data;
	}
	return names;
}

//-------------------------------------------
int ofxAssimpModelLoader::getNumMeshes(){
	return scene->mNumMeshes;
}

//-------------------------------------------
ofMesh ofxAssimpModelLoader::getMesh(string name){
	ofMesh ofm;
	// default to triangle mode
	ofm.setMode(OF_TRIANGLES_MODE);
	aiMesh * aim = NULL;
	for(int i=0; i<(int)scene->mNumMeshes; i++){
		if(string(scene->mMeshes[i]->mName.data)==name){
			aim = scene->mMeshes[i];
			break;
		}
	}

	if(!aim){
		ofLog(OF_LOG_ERROR,"couldn't find mesh " + name);
		return ofm;
	}

	aiMeshToOfMesh(aim,ofm);
	return ofm;
}

//-------------------------------------------
ofMesh ofxAssimpModelLoader::getMesh(int num){
	ofMesh ofm;
	if((int)scene->mNumMeshes<=num){
		ofLog(OF_LOG_ERROR,"couldn't find mesh " + ofToString(num) + " there's only " + ofToString(scene->mNumMeshes));
		return ofm;
	}

	aiMeshToOfMesh(scene->mMeshes[num],ofm);
	return ofm;
}

//-------------------------------------------
ofMesh ofxAssimpModelLoader::getCurrentAnimatedMesh(string name){
	for(int i=0; i<(int)modelMeshes.size(); i++){
		if(string(modelMeshes[i].mesh->mName.data)==name){
			if(!modelMeshes[i].validCache){
				modelMeshes[i].cachedMesh.clearVertices();
				modelMeshes[i].cachedMesh.clearNormals();
				modelMeshes[i].cachedMesh.addVertices(aiVecVecToOfVecVec(modelMeshes[i].animatedPos));
				modelMeshes[i].cachedMesh.addNormals(aiVecVecToOfVecVec(modelMeshes[i].animatedNorm));
				modelMeshes[i].validCache = true;
			}
			return modelMeshes[i].cachedMesh;
		}
	}

	ofLog(OF_LOG_ERROR,"couldn't find mesh " + name);
	return ofMesh();

}

//-------------------------------------------
ofMesh ofxAssimpModelLoader::getCurrentAnimatedMesh(int num){
	if((int)modelMeshes.size()<=num){
		ofLog(OF_LOG_ERROR,"couldn't find mesh " + ofToString(num) + " there's only " + ofToString(scene->mNumMeshes));
		return ofMesh();
	}
	if(!modelMeshes[num].validCache){
		modelMeshes[num].cachedMesh.clearVertices();
		modelMeshes[num].cachedMesh.clearNormals();
		modelMeshes[num].cachedMesh.addVertices(aiVecVecToOfVecVec(modelMeshes[num].animatedPos));
		modelMeshes[num].cachedMesh.addNormals(aiVecVecToOfVecVec(modelMeshes[num].animatedNorm));
		modelMeshes[num].validCache = true;
	}
	return modelMeshes[num].cachedMesh;
}

//-------------------------------------------
ofMaterial ofxAssimpModelLoader::getMaterialForMesh(string name){
	for(int i=0; i<(int)modelMeshes.size(); i++){
		if(string(modelMeshes[i].mesh->mName.data)==name){
			return modelMeshes[i].material;
		}
	}
	ofLog(OF_LOG_ERROR,"couldn't find mesh " + name);
	return ofMaterial();
}

//-------------------------------------------
ofMaterial ofxAssimpModelLoader::getMaterialForMesh(int num){
	if((int)modelMeshes.size()<=num){
		ofLog(OF_LOG_ERROR,"couldn't find mesh " + ofToString(num) + " there's only " + ofToString(scene->mNumMeshes));
		return ofMaterial();
	}
	return modelMeshes[num].material;
}

//-------------------------------------------
ofTexture ofxAssimpModelLoader::getTextureForMesh(string name){
	for(int i=0; i<(int)modelMeshes.size(); i++){
		if(string(modelMeshes[i].mesh->mName.data)==name){
			return modelMeshes[i].texture;
		}
	}
	ofLog(OF_LOG_ERROR,"couldn't find mesh " + name);
	return ofTexture();

}

//-------------------------------------------
ofTexture ofxAssimpModelLoader::getTextureForMesh(int num){
	if((int)modelMeshes.size()<=num){
		ofLog(OF_LOG_ERROR,"couldn't find mesh " + ofToString(num) + " there's only " + ofToString(scene->mNumMeshes));
		return ofTexture();
	}
	return modelMeshes[num].texture;
}

//-------------------------------------------
ofPoint ofxAssimpModelLoader::getPosition(){
	return pos;
}

//-------------------------------------------
ofPoint ofxAssimpModelLoader::getSceneCenter(){
	return aiVecToOfVec(scene_center);
}

//-------------------------------------------
float ofxAssimpModelLoader::getNormalizedScale(){
	return normalizedScale;
}

//-------------------------------------------
ofPoint ofxAssimpModelLoader::getScale(){
	return scale;
}

//-------------------------------------------
const aiScene* ofxAssimpModelLoader::getAssimpScene(){
	return scene;
}

//--------------------------------------------------------------
void ofxAssimpModelLoader::enableTextures(){
	bUsingTextures = true;
}

//--------------------------------------------------------------
void ofxAssimpModelLoader::enableNormals(){
	bUsingNormals = true;
}

//--------------------------------------------------------------
void ofxAssimpModelLoader::enableColors(){
	bUsingColors = true;
}

//--------------------------------------------------------------
void ofxAssimpModelLoader::enableMaterials(){
	bUsingMaterials = true;
}

//--------------------------------------------------------------
void ofxAssimpModelLoader::disableTextures(){
	bUsingTextures = false;
}

//--------------------------------------------------------------
void ofxAssimpModelLoader::disableNormals(){
	bUsingNormals = false;
}

//--------------------------------------------------------------
void ofxAssimpModelLoader::disableColors(){
	bUsingColors = false;
}

//--------------------------------------------------------------
void ofxAssimpModelLoader::disableMaterials(){
	bUsingMaterials = false;
}
