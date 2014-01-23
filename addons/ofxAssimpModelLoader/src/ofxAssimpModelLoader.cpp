#include "ofxAssimpModelLoader.h"
#include "ofxAssimpUtils.h"

#include "assimp.h"
#include "aiScene.h"
#include "aiConfig.h"
#include "aiPostProcess.h"

ofxAssimpModelLoader::ofxAssimpModelLoader(){
	scene = NULL;
	clear();
}

ofxAssimpModelLoader::~ofxAssimpModelLoader(){
    clear();
}

//------------------------------------------
bool ofxAssimpModelLoader::loadModel(string modelName, bool optimize){
    file.open(modelName);
    if(!file.exists()) {
        ofLogVerbose("ofxAssimpModelLoader") << "loadModel(): model does not exist: \"" << modelName << "\"";
        return false;
    }

    ofLogVerbose("ofxAssimpModelLoader") << "loadModel(): loading \"" << file.getFileName()
		<< "\" from \"" << file.getEnclosingDirectory() << "\"";
    
    ofBuffer buffer = file.readToBuffer();
    
    bool bOk = loadModel(buffer, optimize, file.getExtension().c_str());

    return bOk;
}

//-------------------------------------------
bool ofxAssimpModelLoader::loadModel(ofBuffer & buffer, bool optimize, const char * extension){
	normalizeFactor = ofGetWidth() / 2.0;
    
    if(scene != NULL){
        clear();
    }
    
	// only ever give us triangles.
	aiSetImportPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
	aiSetImportPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, true);

	// aiProcess_FlipUVs is for VAR code. Not needed otherwise. Not sure why.
	unsigned int flags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_FlipUVs;
	if(optimize) flags |=  aiProcess_ImproveCacheLocality | aiProcess_OptimizeGraph |
			aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices |
			aiProcess_RemoveRedundantMaterials;

	scene = aiImportFileFromMemory(buffer.getBinaryBuffer(), buffer.size(), flags, extension);
    
	if(scene){
		calculateDimensions();
		loadGLResources();
        update();

		if(getAnimationCount())
			ofLogVerbose("ofxAssimpModelLoader") << "loadModel(): scene has " << getAnimationCount() << "animations";
		else {
			ofLogVerbose("ofxAssimpModelLoader") << "loadMode(): no animations";
		}

		ofAddListener(ofEvents().exit,this,&ofxAssimpModelLoader::onAppExit);


		return true;
	}else{
		ofLogError("ofxAssimpModelLoader") << "loadModel(): " + (string) aiGetErrorString();
		clear();
		return false;
	}
}

// automatic destruction on app exit makes the app crash because of some bug in assimp
// this is a hack to clear every object on the exit callback of the application
// FIXME: review when there's an update of assimp
//-------------------------------------------
void ofxAssimpModelLoader::onAppExit(ofEventArgs & args){
	clear();
}

//-------------------------------------------
void ofxAssimpModelLoader::createEmptyModel(){
	if(scene){
		clear();
	}
	scene = new aiScene;
}


void ofxAssimpModelLoader::setNormalizationFactor(float factor){
	normalizeFactor = factor;
}

//-------------------------------------------
void ofxAssimpModelLoader::calculateDimensions(){
	if(!scene) return;
	ofLogVerbose("ofxAssimpModelLoader") << "calculateDimensions(): inited scene with "
		<< scene->mNumMeshes << " meshes & " << scene->mNumAnimations << " animations";

	getBoundingBoxWithMinVector(&scene_min, &scene_max);
	scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
	scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
	scene_center.z = (scene_min.z + scene_max.z) / 2.0f;

	// optional normalized scaling
	normalizedScale = scene_max.x-scene_min.x;
	normalizedScale = MAX(scene_max.y - scene_min.y,normalizedScale);
	normalizedScale = MAX(scene_max.z - scene_min.z,normalizedScale);
	normalizedScale = 1.f / normalizedScale;
	normalizedScale *= normalizeFactor;
    
    updateModelMatrix();
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

	ofLogVerbose("ofxAssimpModelLoader") << "loadGLResources(): starting";

    // create new mesh helpers for each mesh, will populate their data later.
    modelMeshes.resize(scene->mNumMeshes,ofxAssimpMeshHelper());

    // create OpenGL buffers and populate them based on each meshes pertinant info.
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
        ofLogVerbose("ofxAssimpModelLoader") << "loadGLResources(): loading mesh " << i;
        // current mesh we are introspecting
        aiMesh* mesh = scene->mMeshes[i];

        // the current meshHelper we will be populating data into.
        ofxAssimpMeshHelper & meshHelper = modelMeshes[i];
        //ofxAssimpMeshHelper meshHelper;
		
        //meshHelper.texture = NULL;

        // Handle material info
        aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];
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

        // Load Textures
        int texIndex = 0;
        aiString texPath;

        // TODO: handle other aiTextureTypes
        if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath)){
            ofLogVerbose("ofxAssimpModelLoader") << "loadGLResource(): loading image from \"" << texPath.data << "\"";
            string modelFolder = file.getEnclosingDirectory();
            string relTexPath = ofFilePath::getEnclosingDirectory(texPath.data,false);
            string texFile = ofFilePath::getFileName(texPath.data);
            string realPath = modelFolder + relTexPath  + texFile;
            
            if(ofFile::doesFileExist(realPath) == false) {
                ofLogError("ofxAssimpModelLoader") << "loadGLResource(): texture doesn't exist: \""
					<< file.getFileName() + "\" in \"" << realPath << "\"";
            }
            
            ofxAssimpTexture * assimpTexture = NULL;
            bool bTextureAlreadyExists = false;
            for(int j=0; j<textures.size(); j++) {
                assimpTexture = textures[j];
                if(assimpTexture->getTexturePath() == realPath) {
                    bTextureAlreadyExists = true;
                    break;
                }
            }
            if(bTextureAlreadyExists) {
                meshHelper.assimpTexture = assimpTexture;
                ofLogVerbose("ofxAssimpModelLoader") << "loadGLResource(): texture already loaded: \""
					<< file.getFileName() + "\" from \"" << realPath << "\"";
            } else {
                ofTexture * texture = new ofTexture();
                bool bTextureLoadedOk = ofLoadImage(*texture, realPath);
                if(bTextureLoadedOk) {
                    textures.push_back(new ofxAssimpTexture(texture, realPath));
                    assimpTexture = textures.back();
                    meshHelper.assimpTexture = assimpTexture;
                    ofLogVerbose("ofxAssimpModelLoader") << "loadGLResource(): texture loaded, dimensions: "
						<< texture->getWidth() << "x" << texture->getHeight();
                } else {
                    delete texture;
                    ofLogError("ofxAssimpModelLoader") << "loadGLResource(): couldn't load texture: \""
						<< file.getFileName() + "\" from \"" << realPath << "\"";
                }
            }
        }

        meshHelper.mesh = mesh;
        aiMeshToOfMesh(mesh, meshHelper.cachedMesh, &meshHelper);
        meshHelper.cachedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
        meshHelper.validCache = true;
        meshHelper.hasChanged = false;

        meshHelper.animatedPos.resize(mesh->mNumVertices);
        if(mesh->HasNormals()){
        	meshHelper.animatedNorm.resize(mesh->mNumVertices);
        }


        int usage;
        if(getAnimationCount()){
#ifndef TARGET_OPENGLES
        	if(!ofIsGLProgrammableRenderer()){
        		usage = GL_STATIC_DRAW;
        	}else{
        		usage = GL_STREAM_DRAW;
        	}
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
        if (meshHelper.cachedMesh.hasTexCoords()){			
        	meshHelper.vbo.setTexCoordData(meshHelper.cachedMesh.getTexCoordsPointer()[0].getPtr(),mesh->mNumVertices,GL_STATIC_DRAW,sizeof(ofVec2f));
        }

        meshHelper.indices.resize(mesh->mNumFaces * 3);
        int j=0;
        for (unsigned int x = 0; x < mesh->mNumFaces; ++x){
			for (unsigned int a = 0; a < mesh->mFaces[x].mNumIndices; ++a){
				meshHelper.indices[j++]=mesh->mFaces[x].mIndices[a];
			}
		}

        meshHelper.vbo.setIndexData(&meshHelper.indices[0],meshHelper.indices.size(),GL_STATIC_DRAW);

        //modelMeshes.push_back(meshHelper);
    }
    
    int numOfAnimations = scene->mNumAnimations;
    for(int i=0; i<numOfAnimations; i++) {
        aiAnimation * animation = scene->mAnimations[i];
        animations.push_back(ofxAssimpAnimation(scene, animation));
    }

    ofLogVerbose("ofxAssimpModelLoader") << "loadGLResource(): finished";
}

//-------------------------------------------
void ofxAssimpModelLoader::clear(){

    ofLogVerbose("ofxAssimpModelLoader") << "clear(): deleting GL resources";

    // clear out everything.
    modelMeshes.clear();
    animations.clear();
    pos.set(0,0,0);
    scale.set(1,1,1);
    rotAngle.clear();
    rotAxis.clear();
    lights.clear();

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
    
    currentAnimation = -1;
    
    for(int i=0; i<textures.size(); i++) {
        if(textures[i]->hasTexture()) {
            ofTexture * tex = textures[i]->getTexturePtr();
            delete tex;
        }
    }
    textures.clear();
    
    updateModelMatrix();
    ofRemoveListener(ofEvents().exit,this,&ofxAssimpModelLoader::onAppExit);
}

//------------------------------------------- update.
void ofxAssimpModelLoader::update() {
	if(!scene) return;
    updateAnimations();
    updateMeshes(scene->mRootNode, ofMatrix4x4());
    if(hasAnimations() == false) {
        return;
    }
    updateBones();
    updateGLResources();
}

void ofxAssimpModelLoader::updateAnimations() {
    for(unsigned int i=0; i<animations.size(); i++) {
        animations[i].update();
    }
}

void ofxAssimpModelLoader::updateMeshes(aiNode * node, ofMatrix4x4 parentMatrix) {
    
    aiMatrix4x4 m = node->mTransformation;
    m.Transpose();
	ofMatrix4x4 matrix(m.a1, m.a2, m.a3, m.a4,
                       m.b1, m.b2, m.b3, m.b4,
                       m.c1, m.c2, m.c3, m.c4,
                       m.d1, m.d2, m.d3, m.d4);
    matrix *= parentMatrix;
    
    for(unsigned int i=0; i<node->mNumMeshes; i++) {
        int meshIndex = node->mMeshes[i];
        ofxAssimpMeshHelper & mesh = modelMeshes[meshIndex];
        mesh.matrix = matrix;
    }
    
    for(unsigned int i=0; i<node->mNumChildren; i++) {
        updateMeshes(node->mChildren[i], matrix);
    }
}

void ofxAssimpModelLoader::updateBones() {
    // update mesh position for the animation
	for(unsigned int i=0; i<modelMeshes.size(); ++i) {
		// current mesh we are introspecting
		const aiMesh* mesh = modelMeshes[i].mesh;
        
		// calculate bone matrices
		vector<aiMatrix4x4> boneMatrices(mesh->mNumBones);
		for(unsigned int a=0; a<mesh->mNumBones; ++a) {
			const aiBone* bone = mesh->mBones[a];
            
			// find the corresponding node by again looking recursively through the node hierarchy for the same name
			aiNode* node = scene->mRootNode->FindNode(bone->mName);
            
			// start with the mesh-to-bone matrix
			boneMatrices[a] = bone->mOffsetMatrix;
			// and now append all node transformations down the parent chain until we're back at mesh coordinates again
			const aiNode* tempNode = node;
			while(tempNode) {
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
		for(unsigned int a=0; a<mesh->mNumBones; ++a) {
			const aiBone* bone = mesh->mBones[a];
			const aiMatrix4x4& posTrafo = boneMatrices[a];
            
			for(unsigned int b=0; b<bone->mNumWeights; ++b) {
				const aiVertexWeight& weight = bone->mWeights[b];
                
				size_t vertexId = weight.mVertexId;
				const aiVector3D& srcPos = mesh->mVertices[vertexId];
                
				modelMeshes[i].animatedPos[vertexId] += weight.mWeight * (posTrafo * srcPos);
			}
			if(mesh->HasNormals()){
				// 3x3 matrix, contains the bone matrix without the translation, only with rotation and possibly scaling
				aiMatrix3x3 normTrafo = aiMatrix3x3( posTrafo);
				for(unsigned int b=0; b<bone->mNumWeights; ++b) {
					const aiVertexWeight& weight = bone->mWeights[b];
					size_t vertexId = weight.mVertexId;
                    
					const aiVector3D& srcNorm = mesh->mNormals[vertexId];
					modelMeshes[i].animatedNorm[vertexId] += weight.mWeight * (normTrafo * srcNorm);
				}
			}
		}
	}
}

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

void ofxAssimpModelLoader::updateModelMatrix() {
    modelMatrix.makeIdentityMatrix();
    modelMatrix.glTranslate(pos);
    modelMatrix.glRotate(180, 0, 0, 1);
    if(normalizeScale) {
        modelMatrix.glScale(normalizedScale , normalizedScale, normalizedScale);
    }
    for(int i = 0; i < (int)rotAngle.size(); i++){ // @julapy - not sure why rotAngle isn't a ofVec4f.
        modelMatrix.glRotate(rotAngle[i], rotAxis[i].x, rotAxis[i].y, rotAxis[i].z);
    }
    modelMatrix.glScale(scale.x, scale.y, scale.z);
}

//------------------------------------------- animations.
bool ofxAssimpModelLoader::hasAnimations() {
    return animations.size() > 0;
}

unsigned int ofxAssimpModelLoader::getAnimationCount(){
    return animations.size();
}

ofxAssimpAnimation & ofxAssimpModelLoader::getAnimation(int animationIndex) {
    animationIndex = ofClamp(animationIndex, 0, animations.size()-1);
    return animations[animationIndex];
}

void ofxAssimpModelLoader::playAllAnimations() {
    for(unsigned int i=0; i<animations.size(); i++) {
        animations[i].play();
    }
}

void ofxAssimpModelLoader::stopAllAnimations() {
    for(unsigned int i=0; i<animations.size(); i++) {
        animations[i].stop();
    }
}

void ofxAssimpModelLoader::resetAllAnimations() {
    for(unsigned int i=0; i<animations.size(); i++) {
        animations[i].reset();
    }
}

void ofxAssimpModelLoader::setPausedForAllAnimations(bool pause) {
    for(unsigned int i=0; i<animations.size(); i++) {
        animations[i].setPaused(pause);
    }
}

void ofxAssimpModelLoader::setLoopStateForAllAnimations(ofLoopType state) {
    for(unsigned int i=0; i<animations.size(); i++) {
        animations[i].setLoopState(state);
    }
}

void ofxAssimpModelLoader::setPositionForAllAnimations(float position) {
    for(unsigned int i=0; i<animations.size(); i++) {
        animations[i].setPosition(position);
    }
}

// DEPRECATED.
void ofxAssimpModelLoader::setAnimation(int animationIndex) {
    if(!hasAnimations()) {
        return;
    }
    currentAnimation = ofClamp(animationIndex, 0, getAnimationCount() - 1);
}

// DEPRECATED.
void ofxAssimpModelLoader::setNormalizedTime(float time) {
    if(!hasAnimations()) {
        return;
    }
    setAnimation(currentAnimation); // call this again to clamp animation index, in case the model is reloaded.
    ofxAssimpAnimation & animation = animations[currentAnimation];
    float realT = ofMap(time, 0.0, 1.0, 0.0, animation.getDurationInSeconds(), false);
    setTime(realT);
}

// DEPRECATED.
void ofxAssimpModelLoader::setTime(float time) {
    if(!hasAnimations()) {
        return;
    }
    setAnimation(currentAnimation); // call this again to clamp animation index, in case the model is reloaded.
    ofxAssimpAnimation & animation = animations[currentAnimation];
    animation.setPosition(time);
    update();
}

// DEPRECATED.
float ofxAssimpModelLoader::getDuration(int animationIndex) {
    if(!hasAnimations()) {
        return 0;
    }
    animationIndex = ofClamp(animationIndex, 0, getAnimationCount() - 1);
    float duration = animations[animationIndex].getDurationInSeconds();
    return duration;
}

//------------------------------------------- meshes.
bool ofxAssimpModelLoader::hasMeshes() {
    return modelMeshes.size() > 0;
}

unsigned int ofxAssimpModelLoader::getMeshCount() {
    return modelMeshes.size();
}

ofxAssimpMeshHelper & ofxAssimpModelLoader::getMeshHelper(int meshIndex) {
    meshIndex = ofClamp(meshIndex, 0, modelMeshes.size()-1);
    return modelMeshes[meshIndex];
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
void ofxAssimpModelLoader::setPosition(float x, float y, float z){
    pos.x = x;
    pos.y = y;
    pos.z = z;

    updateModelMatrix();
}

//-------------------------------------------
void ofxAssimpModelLoader::setScale(float x, float y, float z){
    scale.x = x;
    scale.y = y;
    scale.z = z;

    updateModelMatrix();
}

//-------------------------------------------
void ofxAssimpModelLoader::setScaleNomalization(bool normalize) {
    normalizeScale = normalize;

    updateModelMatrix();
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

    updateModelMatrix();
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
void ofxAssimpModelLoader::draw(ofPolyRenderMode renderType) {
    if(scene == NULL) {
        return;
    }
    
    ofPushStyle();
    
    if(!ofGetGLProgrammableRenderer()){
	#ifndef TARGET_OPENGLES
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
	#endif
		glEnable(GL_NORMALIZE);
    }
    
    ofPushMatrix();
    ofMultMatrix(modelMatrix);
    
    for(unsigned int i=0; i<modelMeshes.size(); i++) {
        ofxAssimpMeshHelper & mesh = modelMeshes[i];
        
        ofPushMatrix();
        ofMultMatrix(mesh.matrix);
        
        if(bUsingTextures){
            if(mesh.hasTexture()) {
                ofTexture * tex = mesh.getTexturePtr();
                if(tex->isAllocated()) {
                    tex->bind();
                }
            }
        }
        
        if(bUsingMaterials){
            mesh.material.begin();
        }
        
        if(mesh.twoSided) {
            glEnable(GL_CULL_FACE);
        }
        else {
            glDisable(GL_CULL_FACE);
        }
        
        ofEnableBlendMode(mesh.blendMode);
#ifndef TARGET_OPENGLES
        mesh.vbo.drawElements(GL_TRIANGLES,mesh.indices.size());
#else
        switch(renderType){
		    case OF_MESH_FILL:
		    	mesh.vbo.drawElements(GL_TRIANGLES,mesh.indices.size());
		    	break;
		    case OF_MESH_WIREFRAME:
		    	mesh.vbo.drawElements(GL_LINES,mesh.indices.size());
		    	break;
		    case OF_MESH_POINTS:
		    	mesh.vbo.drawElements(GL_POINTS,mesh.indices.size());
		    	break;
        }
#endif
        
        if(bUsingTextures){
            if(mesh.hasTexture()) {
                ofTexture * tex = mesh.getTexturePtr();
                if(tex->isAllocated()) {
                    tex->unbind();
                }
            }
        }
        
        if(bUsingMaterials){
            mesh.material.end();
        }
        
        ofPopMatrix();
    }
    
    ofPopMatrix();

    if(!ofGetGLProgrammableRenderer()){
	#ifndef TARGET_OPENGLES
		glPopClientAttrib();
		glPopAttrib();
	#endif
    }
    ofPopStyle();
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
	ofm.setMode(OF_PRIMITIVE_TRIANGLES);
	aiMesh * aim = NULL;
	for(int i=0; i<(int)scene->mNumMeshes; i++){
		if(string(scene->mMeshes[i]->mName.data)==name){
			aim = scene->mMeshes[i];
			break;
		}
	}

	if(!aim){
		ofLogError("ofxAssimpModelLoader") <<"getMesh(): couldn't find mesh: \"" << name << "\"";
		return ofm;
	}

	aiMeshToOfMesh(aim,ofm);
	return ofm;
}

//-------------------------------------------
ofMesh ofxAssimpModelLoader::getMesh(int num){
	ofMesh ofm;
	if((int)scene->mNumMeshes<=num){
		ofLogError("ofxAssimpModelLoader") << "getMesh(): mesh id " << num
		<< " out of range for total num meshes: " << scene->mNumMeshes;
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

	ofLogError("ofxAssimpModelLoader") << "getCurrentAnimatedMesh(): couldn't find mesh: \"" + name << "\"";
	return ofMesh();

}

//-------------------------------------------
ofMesh ofxAssimpModelLoader::getCurrentAnimatedMesh(int num){
	if((int)modelMeshes.size()<=num){
		ofLogError("ofxAssimpModelLoader") << "getCurrentAnimatedMesh(): mesh id: " << num
			<< "out of range for total num meshes: " << scene->mNumMeshes;
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
	ofLogError("ofxAssimpModelLoader") << "getMaterialForMesh(): couldn't find mesh: \"" + name << "\"";
	return ofMaterial();
}

//-------------------------------------------
ofMaterial ofxAssimpModelLoader::getMaterialForMesh(int num){
	if((int)modelMeshes.size()<=num){
		ofLogError("ofxAssimpModelLoader") << "getMaterialForMesh(): mesh id: " << num
			<< "out of range for total num meshes: " << scene->mNumMeshes;
		return ofMaterial();
	}
	return modelMeshes[num].material;
}

//-------------------------------------------
ofTexture ofxAssimpModelLoader::getTextureForMesh(string name){
	for(int i=0; i<(int)modelMeshes.size(); i++){
		if(string(modelMeshes[i].mesh->mName.data)==name){
            if(modelMeshes[i].hasTexture()) {
                ofTexture * tex = modelMeshes[i].getTexturePtr();
                return *tex;
            }
		}
	}
	ofLogError("ofxAssimpModelLoader") << "getTextureForMesh(): couldn't find mesh: \"" + name << "\"";
	return ofTexture();
}

//-------------------------------------------
ofTexture ofxAssimpModelLoader::getTextureForMesh(int i){
	if(i < modelMeshes.size()){
        if(modelMeshes[i].hasTexture()) {
            ofTexture * tex = modelMeshes[i].getTexturePtr();
            return *tex;
        }
	}
    ofLogError("ofxAssimpModelLoader") << "getTextureForMesh(): mesh id: " << i
			<< "out of range for total num meshes: " << scene->mNumMeshes;
    return ofTexture();
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
ofMatrix4x4 ofxAssimpModelLoader::getModelMatrix() {
    return modelMatrix;
}

//-------------------------------------------
ofPoint ofxAssimpModelLoader::getSceneMin(bool bScaled ){
	ofPoint sceneMin(scene_min.x, scene_min.y, scene_min.z);
	if( bScaled ){
		return sceneMin * scale;
	}else{
		return sceneMin;
	}
}

//-------------------------------------------
ofPoint	ofxAssimpModelLoader::getSceneMax(bool bScaled ){
	ofPoint sceneMax(scene_max.x, scene_max.y, scene_max.z);
	if( bScaled ){
		return sceneMax * scale;
	}else{
		return sceneMax;
	}
}

//-------------------------------------------
int ofxAssimpModelLoader::getNumRotations(){
	return rotAngle.size();
}

//-------------------------------------------
ofPoint ofxAssimpModelLoader::getRotationAxis(int which){
	if((int)rotAxis.size() > which){
		return rotAxis[which];
	}else{
		return ofPoint();
	}
}

//-------------------------------------------
float ofxAssimpModelLoader::getRotationAngle(int which){
	if((int)rotAngle.size() > which){
		return rotAngle[which];
	}else{
		return 0.0;
	}
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
