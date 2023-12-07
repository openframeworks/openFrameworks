#include "ofxAssimpModelLoader.h"
#include "ofxAssimpUtils.h"
#include "ofLight.h"
#include "ofImage.h"
#include "ofPixels.h"
#include "ofGraphics.h"
#include "ofConstants.h"
#include "ofMatrix4x4.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/DefaultLogger.hpp>

using std::shared_ptr;
using std::vector;

ofxAssimpModelLoader::ofxAssimpModelLoader(){
	clear();
}

ofxAssimpModelLoader::~ofxAssimpModelLoader(){
	clear();
}


// DEPRECATED
bool ofxAssimpModelLoader::load(string modelName, bool optimize){
	int optimizeFlags = OPTIMIZE_DEFAULT;
	if( optimize ){
		optimizeFlags = OPTIMIZE_HIGH;
	}
	return load(modelName, optimizeFlags);
}

// DEPRECATED
bool ofxAssimpModelLoader::load(ofBuffer & buffer, bool optimize, const char * extension){
	int optimizeFlags = OPTIMIZE_DEFAULT;
	if( optimize ){
		optimizeFlags = OPTIMIZE_HIGH;
	}
	return load(buffer, optimizeFlags, extension);
}

// DEPRECATED
bool ofxAssimpModelLoader::loadModel(string modelName, bool optimize){
	return load(modelName, optimize);
}

// DEPRECATED
bool ofxAssimpModelLoader::loadModel(ofBuffer & buffer, bool optimize, const char * extension){
	return load(buffer, optimize, extension);
}

//------------------------------------------
bool ofxAssimpModelLoader::load(string modelName, int assimpOptimizeFlags){

	file.open(modelName, ofFile::ReadOnly, true); // Since it may be a binary file we should read it in binary -Ed
	if(!file.exists()) {
		ofLogVerbose("ofxAssimpModelLoader") << "load(): model does not exist: \"" << modelName << "\"";
		return false;
	}

	ofLogVerbose("ofxAssimpModelLoader") << "load(): loading \"" << file.getFileName()
	<< "\" from \"" << file.getEnclosingDirectory() << "\"";

	if(scene.get() != nullptr){
		clear();
		// we reset the shared_ptr explicitly here, to force the old
		// aiScene to be deleted **before** a new aiScene is created.
		scene.reset();
	}

	// sets various properties & flags to a default preference
	unsigned int flags = initImportProperties(assimpOptimizeFlags);

	//	//enable assimp logging based on ofGetLogLevel
	//	if( ofGetLogLevel() < OF_LOG_NOTICE ){
	//		auto logLevel = Assimp::DefaultLogger::LogSeverity::VERBOSE;
	//		Assimp::DefaultLogger::create(ASSIMP_DEFAULT_LOG_NAME, logLevel, aiDefaultLogStream_DEBUGGER | aiDefaultLogStream_STDOUT );
	//	}

	// loads scene from file
	std::string path = file.getAbsolutePath();
	const aiScene * scenePtr = importer.ReadFile(path.c_str(), flags);

	//this is funky but the scenePtr is managed by assimp and so we can't put it in our shared_ptr without disabling the deleter with: [](const aiScene*){}
	scene = shared_ptr<const aiScene>(scenePtr,[](const aiScene*){});

	bool bOk = processScene();
	return bOk;
}


bool ofxAssimpModelLoader::load(ofBuffer & buffer, int assimpOptimizeFlags, const char * extension){

	ofLogVerbose("ofxAssimpModelLoader") << "load(): loading from memory buffer \"." << extension << "\"";

	if(scene.get() != nullptr){
		clear();
		// we reset the shared_ptr explicitly here, to force the old
		// aiScene to be deleted **before** a new aiScene is created.
		scene.reset();
	}

	// sets various properties & flags to a default preference
	unsigned int flags = initImportProperties(assimpOptimizeFlags);

	// 	//enable assimp logging based on ofGetLogLevel
	//	if( ofGetLogLevel() < OF_LOG_NOTICE ){
	//		auto logLevel = Assimp::DefaultLogger::LogSeverity::VERBOSE;
	//		Assimp::DefaultLogger::create(ASSIMP_DEFAULT_LOG_NAME, logLevel, aiDefaultLogStream_DEBUGGER | aiDefaultLogStream_STDOUT );
	//	}

	// loads scene from memory buffer - note this will not work for multipart files (obj, md3, etc)
	const aiScene * scenePtr = importer.ReadFileFromMemory(buffer.getData(), buffer.size(), flags, extension);

	// this is funky but the scenePtr is managed by assimp and so we can't put it in our shared_ptr without disabling the deleter with: [](const aiScene*){}
	scene = shared_ptr<const aiScene>(scenePtr,[](const aiScene*){});

	bool bOk = processScene();
	return bOk;
}

unsigned int ofxAssimpModelLoader::initImportProperties(int assimpOptimizeFlags) {
	store.reset(aiCreatePropertyStore(), aiReleasePropertyStore);

	// only ever give us triangles.
	aiSetImportPropertyInteger(store.get(), AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
	aiSetImportPropertyInteger(store.get(), AI_CONFIG_PP_PTV_NORMALIZE, true);

	unsigned int flags = assimpOptimizeFlags;

	if( flags <= OPTIMIZE_HIGH ){

		if( flags == OPTIMIZE_NONE ){
			flags = 0;
		}else if( flags == OPTIMIZE_DEFAULT || flags == OPTIMIZE_HIGH ){
			flags =     aiProcess_CalcTangentSpace              |  \
			aiProcess_GenSmoothNormals              |  \
			aiProcess_JoinIdenticalVertices         |  \
			aiProcess_ImproveCacheLocality          |  \
			aiProcess_LimitBoneWeights              |  \
			aiProcess_RemoveRedundantMaterials      |  \
			aiProcess_SplitLargeMeshes              |  \
			aiProcess_Triangulate                   |  \
			aiProcess_GenUVCoords                   |  \
			aiProcess_SortByPType                   |  \
			aiProcess_FindDegenerates               |  \
			aiProcess_FindInstances                 |  \
			aiProcess_OptimizeMeshes;
		}

		if( flags == OPTIMIZE_HIGH ){
			flags |= aiProcess_OptimizeGraph |  \
			aiProcess_FindInstances |  \
			aiProcess_ValidateDataStructure;
		}

		// this fixes things for OF both tex uvs and model not flipped in z
		flags |= aiProcess_ConvertToLeftHanded;
	}

	return flags;
}

bool ofxAssimpModelLoader::processScene() {

	normalizeFactor = ofGetWidth() / 2.0;

	if(scene){
		loadGLResources();
		update();
		calculateDimensions();

		if(getAnimationCount())
			ofLogVerbose("ofxAssimpModelLoader") << "load(): scene has " << getAnimationCount() << "animations";
		else {
			ofLogVerbose("ofxAssimpModelLoader") << "load(): no animations";
		}

		return true;
	}else{
		ofLogError("ofxAssimpModelLoader") << "load(): " + (string) aiGetErrorString();
		clear();
		return false;
	}

	return false;
}


//-------------------------------------------
void ofxAssimpModelLoader::createEmptyModel(){
	if(scene){
		clear();
		scene.reset();
	}
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
	normalizedScale = std::max(double(scene_max.y - scene_min.y), normalizedScale);
	normalizedScale = std::max(double(scene_max.z - scene_min.z), normalizedScale);
	if (fabs(normalizedScale) < std::numeric_limits<float>::epsilon()){
		ofLogWarning("ofxAssimpModelLoader") << "Error calculating normalized scale of scene" << std::endl;
		normalizedScale = 1.0;
	} else {
		normalizedScale = 1.f / normalizedScale;
		normalizedScale *= normalizeFactor;
	}

	updateModelMatrix();
}

//-------------------------------------------
static GLint getGLFormatFromAiFormat(const char * aiFormat){

	std::string formatStr(aiFormat);

	if( formatStr.size() >= 8 ){
		if( formatStr.substr(0, 4) == "rgba" ){
			if(formatStr.substr(4,4) == "8888"){
				return GL_RGBA;
			}else if(formatStr.substr(4,4) == "8880"){
				return GL_RGB;
			}
		}else{
			ofLogError("getGLFormatFromAiFormat") << " can't parse format " << formatStr;
		}
	}

	ofLogWarning("getGLFormatFromAiFormat") << " can't parse format " << formatStr << " returning GL_RGB";
	return GL_RGB;
}

//-------------------------------------------
void ofxAssimpModelLoader::createLightsFromAiModel(){
	lights.clear();
	lights.resize(scene->mNumLights);
	for(unsigned int i = 0; i < scene->mNumLights; i++){
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

//-------------------------------------------
void ofxAssimpModelLoader::optimizeScene(){
	aiApplyPostProcessing(scene.get(),aiProcess_ImproveCacheLocality | aiProcess_OptimizeGraph |
						  aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices |
						  aiProcess_RemoveRedundantMaterials);
}

#ifndef TARGET_WIN32
//this is a hack to allow for weak definations of functions that might not exist in older assimp versions
const char *aiTextureTypeToString(enum aiTextureType in)__attribute__((weak));
#endif

//-------------------------------------------
void ofxAssimpModelLoader::loadGLResources(){

	ofLogVerbose("ofxAssimpModelLoader") << "loadGLResources(): starting";

	// we do this as we have textures and vbos and in meshHelper and we don't want to copy them
	// this should really be a vector of shared_ptr's but keeping it as objects for legacy reasons
	modelMeshes.clear();
	modelMeshes.reserve(scene->mNumMeshes);

	// create OpenGL buffers and populate them based on each meshes pertinant info.
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i){
		ofLogVerbose("ofxAssimpModelLoader") << "loadGLResources(): loading mesh " << i;
		// current mesh we are introspecting
		aiMesh* mesh = scene->mMeshes[i];

		// the current meshHelper we will be populating data into.
		modelMeshes.push_back(ofxAssimpMeshHelper());
		ofxAssimpMeshHelper & meshHelper = modelMeshes[i];
		//ofxAssimpMeshHelper meshHelper;

		//meshHelper.texture = NULL;

		// Handle material info
		aiMaterial* mtl = scene->mMaterials[mesh->mMaterialIndex];
		aiColor4D tcolor;

		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &tcolor)){
			auto col = aiColorToOfColor(tcolor);
			meshHelper.material.setDiffuseColor(col);
		}

		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &tcolor)){
			auto col = aiColorToOfColor(tcolor);
			meshHelper.material.setSpecularColor(col);
		}

		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &tcolor)){
			auto col = aiColorToOfColor(tcolor);
			meshHelper.material.setAmbientColor(col);
		}

		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &tcolor)){
			auto col = aiColorToOfColor(tcolor);
			meshHelper.material.setEmissiveColor(col);
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
		int two_sided=0;
		if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided){
			meshHelper.twoSided = true;
			ofLogVerbose("ofxAssimpModelLoader::loadGLResources") <<": mesh is two sided";
		}else{
			meshHelper.twoSided = false;
			ofLogVerbose("ofxAssimpModelLoader::loadGLResources") <<": mesh is one sided";
		}

		// Load Textures
		int texIndex = 0;
		aiString texPath;

		//this gets the wrapping in u and v coodinates
		//we don't support different wrapping in OF so just read u
		aiTextureMapMode texMapMode[2];

		for(int d = 0; d <= AI_TEXTURE_TYPE_MAX; d++){
			if(AI_SUCCESS == mtl->GetTexture((aiTextureType)d, texIndex, &texPath, NULL, NULL, NULL, NULL, &texMapMode[0])){

				//this is a solution to support older versions of assimp. see the weak defination above
				if( aiTextureTypeToString ){
					ofLogVerbose("ofxAssimpModelLoader") << "loadGLResource(): loading " <<  aiTextureTypeToString((aiTextureType)d) << " image from \"" << texPath.data << "\"";
				}

				bool bWrap = (texMapMode[0]==aiTextureMapMode_Wrap);

				std::string texPathStr = texPath.C_Str();

				//deal with Blender putting "//" in front of local file paths
				if( texPathStr.size() > 2 && texPathStr.substr(0, 2) == "//" ){
					texPathStr = texPathStr.substr(2, texPathStr.size()-2);
				}

				//glb embedded texture file starts with *0
				bool bTryEmbed = false;
				if( texPathStr.size() >= 2 && texPathStr[0] == '*'){
					bTryEmbed = true;
				}

				//stuff for embedded textures
				auto ogPath = texPathStr;
				bool bHasEmbeddedTexture = false;

				auto modelFolder = ofFilePath::getEnclosingDirectory( file.path() );
				auto relTexPath = ofFilePath::getEnclosingDirectory(texPathStr,false);
				auto realPath = modelFolder / of::filesystem::path{ texPathStr };
				

#ifndef TARGET_LINUX_ARM
				if(bTryEmbed || ofFile::doesFileExist(realPath) == false) {
					auto embeddedTexture = scene->GetEmbeddedTexture(ogPath.c_str());
					if( embeddedTexture ){
						bHasEmbeddedTexture = true;
						ofLogVerbose("ofxAssimpModelLoader") << "loadGLResource() texture " << realPath.filename() << " is embedded ";
					}else{
						ofLogError("ofxAssimpModelLoader") << "loadGLResource(): texture doesn't exist: \""
						<< file.getFileName() + "\" in \"" << realPath.string() << "\"";
					}
				}
#endif

				bool bTextureAlreadyExists = false;
				if(textures.count(realPath)){
					bTextureAlreadyExists = true;
				}

				if(bTextureAlreadyExists) {
					ofxAssimpTexture assimpTexture;

					assimpTexture.setup(*textures[realPath].get(), realPath, bWrap);
					assimpTexture.setTextureType((aiTextureType)d);
					meshHelper.addTexture(assimpTexture);

					ofLogVerbose("ofxAssimpModelLoader") << "loadGLResource(): texture already loaded: \""
					<< file.getFileName() + "\" from \"" << realPath.string() << "\"" << " adding texture as " << assimpTexture.getTextureTypeAsString() ;
				} else {

					shared_ptr<ofTexture> texture = std::make_shared<ofTexture>();

					if( bHasEmbeddedTexture ){

#ifndef TARGET_LINUX_ARM
						auto embeddedTexture = scene->GetEmbeddedTexture(ogPath.c_str());

						//compressed texture
						if( embeddedTexture->mHeight == 0 && embeddedTexture->mWidth > 0){
							ofImage tmp;
							ofBuffer buffer;
							buffer.set((char *)embeddedTexture->pcData, embeddedTexture->mWidth);

							tmp.setUseTexture(false);
							tmp.load(buffer);

							ofLogVerbose("ofxAssimpModelLoader") << "loadGLResource() texture size is " << tmp.getWidth() << "x" << tmp.getHeight();

							texture->loadData(tmp.getPixels());
						}else{
							//uncompressed texture - might need swizzling from argb to rgba?
							auto glFormat = getGLFormatFromAiFormat(embeddedTexture->achFormatHint);
							texture->loadData((const uint8_t *)embeddedTexture->pcData, embeddedTexture->mWidth, embeddedTexture->mHeight, glFormat);
						}
#endif
					}else{
						ofLoadImage(*texture.get(), realPath);
					}

					if(texture && texture->isAllocated()){
						ofxAssimpTexture tmpTex;
						tmpTex.setup(*texture.get(), realPath, bWrap);
						tmpTex.setTextureType((aiTextureType)d);
						textures[realPath] = texture;

						tmpTex.setTextureType((aiTextureType)d);
						meshHelper.addTexture( tmpTex );

						ofLogVerbose("ofxAssimpModelLoader") << "loadGLResource(): texture " << tmpTex.getTextureTypeAsString() << " loaded, dimensions: " << texture->getWidth() << "x" << texture->getHeight();
					}else{
						ofLogError("ofxAssimpModelLoader") << "loadGLResource(): couldn't load texture: \""
						<< file.getFileName() + "\" from \"" << realPath.string() << "\"";
					}
				}
			}
		}

		meshHelper.mesh = mesh;
		aiMeshToOfMesh(mesh, meshHelper.cachedMesh, &meshHelper);
		meshHelper.cachedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
		meshHelper.validCache = true;
		meshHelper.hasChanged = false;

		int numOfAnimations = scene->mNumAnimations;
		for (int i = 0; i<numOfAnimations; i++) {
			aiAnimation * animation = scene->mAnimations[i];
			animations.push_back(ofxAssimpAnimation(scene, animation));
		}

		if(hasAnimations()){
			meshHelper.animatedPos.resize(mesh->mNumVertices);
			if(mesh->HasNormals()){
				meshHelper.animatedNorm.resize(mesh->mNumVertices);
			}
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
			meshHelper.vbo.setTexCoordData(&meshHelper.cachedMesh.getTexCoords()[0].x, mesh->mNumVertices,GL_STATIC_DRAW,sizeof(glm::vec2));
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
	ofLogVerbose("ofxAssimpModelLoader") << "loadGLResource(): finished";
}

//-------------------------------------------
void ofxAssimpModelLoader::clear(){

	if( scene ){
		scene.reset();
		importer.FreeScene();
	}

	ofLogVerbose("ofxAssimpModelLoader") << "clear(): deleting GL resources";

	// clear out everything.
	modelMeshes.clear();
	animations.clear();
	pos = glm::vec3(0,0,0);
	scale = glm::vec3(1,1,1);
	rotAngle.clear();
	rotAxis.clear();
	lights.clear();

	scale = glm::vec3(1, 1, 1);
	normalizeScale = true;
	bUsingMaterials = true;
	bUsingNormals = true;
	bUsingTextures = true;
	bUsingColors = true;

	currentAnimation = -1;

	textures.clear();

	updateModelMatrix();
}

//------------------------------------------- update.
void ofxAssimpModelLoader::update() {
	if(!scene) return;
	updateAnimations();
	updateMeshes(scene->mRootNode, glm::mat4());
	if(hasAnimations() == false) {
		return;
	}
	updateBones();
	updateGLResources();
}

void ofxAssimpModelLoader::updateAnimations() {
	for(size_t i = 0; i < animations.size(); i++) {
		animations[i].update();
	}
}

void ofxAssimpModelLoader::updateMeshes(aiNode * node, glm::mat4 parentMatrix) {

	aiMatrix4x4 m = node->mTransformation;
	m.Transpose();
	ofMatrix4x4 matrix(m.a1, m.a2, m.a3, m.a4,
					   m.b1, m.b2, m.b3, m.b4,
					   m.c1, m.c2, m.c3, m.c4,
					   m.d1, m.d2, m.d3, m.d4);
	matrix *= parentMatrix;

	for(unsigned int i = 0; i < node->mNumMeshes; i++) {
		int meshIndex = node->mMeshes[i];
		ofxAssimpMeshHelper & mesh = modelMeshes[meshIndex];
		mesh.matrix = matrix;
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++) {
		updateMeshes(node->mChildren[i], matrix);
	}
}

void ofxAssimpModelLoader::updateBones() {
	if (!hasAnimations()){
		return;
	}
	// update mesh position for the animation
	for(size_t i = 0; i < modelMeshes.size(); ++i) {
		// current mesh we are introspecting
		const aiMesh* mesh = modelMeshes[i].mesh;

		// calculate bone matrices
		vector<aiMatrix4x4> boneMatrices(mesh->mNumBones);
		for(unsigned int a = 0; a < mesh->mNumBones; ++a) {
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

		modelMeshes[i].animatedPos.assign(modelMeshes[i].animatedPos.size(), aiVector3D(0.0f));
		if(mesh->HasNormals()){
			modelMeshes[i].animatedNorm.assign(modelMeshes[i].animatedNorm.size(), aiVector3D(0.0f));
		}
		// loop through all vertex weights of all bones
		for(unsigned int a = 0; a < mesh->mNumBones; ++a) {
			const aiBone* bone = mesh->mBones[a];
			const aiMatrix4x4& posTrafo = boneMatrices[a];

			for(unsigned int b = 0; b < bone->mNumWeights; ++b) {
				const aiVertexWeight& weight = bone->mWeights[b];

				size_t vertexId = weight.mVertexId;
				const aiVector3D& srcPos = mesh->mVertices[vertexId];

				modelMeshes[i].animatedPos[vertexId] += weight.mWeight * (posTrafo * srcPos);
			}
			if(mesh->HasNormals()){
				// 3x3 matrix, contains the bone matrix without the translation, only with rotation and possibly scaling
				aiMatrix3x3 normTrafo = aiMatrix3x3( posTrafo);
				for(unsigned int b = 0; b < bone->mNumWeights; ++b) {
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
			if(hasAnimations()){
				modelMeshes[i].vbo.updateVertexData(&modelMeshes[i].animatedPos[0].x,mesh->mNumVertices);
				if(mesh->HasNormals()){
					modelMeshes[i].vbo.updateNormalData(&modelMeshes[i].animatedNorm[0].x,mesh->mNumVertices);
				}
			}
			modelMeshes[i].hasChanged = false;
		}
	}
}

void ofxAssimpModelLoader::updateModelMatrix() {
	modelMatrix = glm::identity<glm::mat4>();
	modelMatrix = glm::translate(modelMatrix, toGlm(pos));
	modelMatrix = glm::rotate(modelMatrix, ofDegToRad(180), glm::vec3(0,0,1));

	if(normalizeScale) {
		modelMatrix = glm::scale(modelMatrix, glm::vec3(normalizedScale, normalizedScale, normalizedScale));
	}

	for(size_t i = 0; i < rotAngle.size(); i++){
		modelMatrix = glm::rotate(modelMatrix, ofDegToRad(rotAngle[i]), glm::vec3(rotAxis[i].x, rotAxis[i].y, rotAxis[i].z));
	}

	modelMatrix = glm::scale(modelMatrix, toGlm(scale));
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
	for(size_t i = 0; i < animations.size(); i++) {
		animations[i].play();
	}
}

void ofxAssimpModelLoader::stopAllAnimations() {
	for(size_t i = 0; i < animations.size(); i++) {
		animations[i].stop();
	}
}

void ofxAssimpModelLoader::resetAllAnimations() {
	for(size_t i = 0; i < animations.size(); i++) {
		animations[i].reset();
	}
}

void ofxAssimpModelLoader::setPausedForAllAnimations(bool pause) {
	for(size_t i = 0; i < animations.size(); i++) {
		animations[i].setPaused(pause);
	}
}

void ofxAssimpModelLoader::setLoopStateForAllAnimations(ofLoopType state) {
	for(size_t i = 0; i < animations.size(); i++) {
		animations[i].setLoopState(state);
	}
}

void ofxAssimpModelLoader::setPositionForAllAnimations(float position) {
	for(size_t i = 0; i < animations.size(); i++) {
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
	currentAnimation = ofClamp(currentAnimation, 0, getAnimationCount() - 1);
	ofxAssimpAnimation & animation = animations[currentAnimation];
	float realT = ofMap(time, 0.0, 1.0, 0.0, animation.getDurationInSeconds(), false);
	animation.setPosition(realT);
	update();
}

// DEPRECATED.
void ofxAssimpModelLoader::setTime(float time) {
	if(!hasAnimations()) {
		return;
	}
	currentAnimation = ofClamp(currentAnimation, 0, getAnimationCount() - 1);
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
void ofxAssimpModelLoader::getBoundingBoxWithMinVector( aiVector3D* min, aiVector3D* max )
{
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;

	for(auto & mesh: modelMeshes){
		this->getBoundingBoxForNode(mesh, min, max);
	}
}

//-------------------------------------------
void ofxAssimpModelLoader::getBoundingBoxForNode(const ofxAssimpMeshHelper & mesh, aiVector3D* min, aiVector3D* max){
	if (!hasAnimations()){
		for(unsigned int i = 0; i < mesh.mesh->mNumVertices; i++){
			auto vertex = mesh.mesh->mVertices[i];
			auto tmp = mesh.matrix * glm::vec4(vertex.x,vertex.y,vertex.z,1.0f);

			min->x = std::min(min->x,tmp.x);
			min->y = std::min(min->y,tmp.y);
			min->z = std::min(min->z,tmp.z);

			max->x = std::max(max->x,tmp.x);
			max->y = std::max(max->y,tmp.y);
			max->z = std::max(max->z,tmp.z);
		}
	} else {
		for (auto & animPos: mesh.animatedPos){
			auto tmp = mesh.matrix * glm::vec4(animPos.x,animPos.y,animPos.z,1.0f);

			min->x = std::min(min->x,tmp.x);
			min->y = std::min(min->y,tmp.y);
			min->z = std::min(min->z,tmp.z);

			max->x = std::max(max->x,tmp.x);
			max->y = std::max(max->y,tmp.y);
			max->z = std::max(max->z,tmp.z);
		}
	}
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
void ofxAssimpModelLoader::setScaleNormalization(bool normalize) {
	normalizeScale = normalize;

	updateModelMatrix();
}

//-------------------------------------------
void ofxAssimpModelLoader::setRotation(int which, float angle, float rot_x, float rot_y, float rot_z){
	if(which + 1 > (int)rotAngle.size()){
		int diff = 1 + (which - rotAngle.size());
		for(int i = 0; i < diff; i++){
			rotAngle.push_back(0);
			rotAxis.push_back(glm::vec3(0.0,0.0,0.0));
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
void ofxAssimpModelLoader::enableCulling(int glCullType){
	mCullType = glCullType;
}

//-------------------------------------------
void ofxAssimpModelLoader::disableCulling(){
	mCullType = -1;
}

//-------------------------------------------
void ofxAssimpModelLoader::draw(ofPolyRenderMode renderType) {
	if(scene == NULL) {
		return;
	}

	ofPushStyle();

	ofPushMatrix();
	ofMultMatrix(modelMatrix);

#ifndef TARGET_OPENGLES
	glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
#endif

	for(size_t i = 0; i < modelMeshes.size(); i++) {
		ofxAssimpMeshHelper & mesh = modelMeshes[i];

		ofPushMatrix();
		ofMultMatrix(mesh.matrix);

		if(bUsingTextures){
			if(mesh.hasTexture(aiTextureType_DIFFUSE)) {
				mesh.getTextureRef(aiTextureType_DIFFUSE).bind();
			}
		}

		if(bUsingMaterials){
			mesh.material.begin();
		}

		//		this was broken / backwards
		if(!mesh.twoSided && mCullType >= 0) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(mCullType);
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
				//note this won't look the same as on non ES renderers.
				//there is no easy way to convert GL_TRIANGLES to outlines for each triangle
				mesh.vbo.drawElements(GL_LINES,mesh.indices.size());
				break;
			case OF_MESH_POINTS:
				mesh.vbo.drawElements(GL_POINTS,mesh.indices.size());
				break;
		}
#endif

		if(bUsingTextures){
			if(mesh.hasTexture(aiTextureType_DIFFUSE)) {
				mesh.getTextureRef(aiTextureType_DIFFUSE).unbind();
			}
		}

		if(!mesh.twoSided) {
			glDisable(GL_CULL_FACE);
		}

		if(bUsingMaterials){
			mesh.material.end();
		}

		ofPopMatrix();
	}

#ifndef TARGET_OPENGLES
	//set the drawing mode back to FILL if its drawn the model with a different mode.
	if( renderType != OF_MESH_FILL ){
		glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(OF_MESH_FILL));
	}
#endif

	ofPopMatrix();
	ofPopStyle();
}

//-------------------------------------------
vector<string> ofxAssimpModelLoader::getMeshNames(){
	if(!scene)return vector<string>();

	vector<string> names(scene->mNumMeshes);
	for(unsigned int i=0; i< scene->mNumMeshes; i++){
		names[i] = scene->mMeshes[i]->mName.data;
	}
	return names;
}

//-------------------------------------------
unsigned int ofxAssimpModelLoader::getNumMeshes(){
	if( scene ){
		return scene->mNumMeshes;
	}
	return 0;
}

//-------------------------------------------
ofMesh ofxAssimpModelLoader::getMesh(string name){
	ofMesh ofm;
	if( scene ){
		// default to triangle mode
		ofm.setMode(OF_PRIMITIVE_TRIANGLES);
		aiMesh * aim = NULL;
		for(unsigned int i=0; i < scene->mNumMeshes; i++){
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
	}
	return ofm;
}

//-------------------------------------------
ofMesh ofxAssimpModelLoader::getMesh(unsigned int num){
	ofMesh ofm;
	if( scene ){
		if(scene->mNumMeshes <= num){
			ofLogError("ofxAssimpModelLoader") << "getMesh(): mesh id " << num
			<< " out of range for total num meshes: " << scene->mNumMeshes;
			return ofm;
		}
		aiMeshToOfMesh(scene->mMeshes[num],ofm);
	}
	return ofm;
}

//-------------------------------------------
ofMesh ofxAssimpModelLoader::getCurrentAnimatedMesh(string name){
	for(size_t i=0; i < modelMeshes.size(); i++){
		if(string(modelMeshes[i].mesh->mName.data)==name){
			if(!modelMeshes[i].validCache){
				modelMeshes[i].cachedMesh.clearVertices();
				modelMeshes[i].cachedMesh.clearNormals();
				if(hasAnimations()){
					modelMeshes[i].cachedMesh.addVertices(aiVecVecToOfVecVec(modelMeshes[i].animatedPos));
					modelMeshes[i].cachedMesh.addNormals(aiVecVecToOfVecVec(modelMeshes[i].animatedNorm));
				}
				modelMeshes[i].validCache = true;
			}
			return modelMeshes[i].cachedMesh;
		}
	}

	ofLogError("ofxAssimpModelLoader") << "getCurrentAnimatedMesh(): couldn't find mesh: \"" + name << "\"";
	return ofMesh();

}

//-------------------------------------------
ofMesh ofxAssimpModelLoader::getCurrentAnimatedMesh(unsigned int num){
	if(modelMeshes.size() <= num){
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
	for(size_t i = 0; i < modelMeshes.size(); i++){
		if(string(modelMeshes[i].mesh->mName.data)==name){
			return modelMeshes[i].material;
		}
	}
	ofLogError("ofxAssimpModelLoader") << "getMaterialForMesh(): couldn't find mesh: \"" + name << "\"";
	return ofMaterial();
}

//-------------------------------------------
ofMaterial ofxAssimpModelLoader::getMaterialForMesh(unsigned int num){
	if(modelMeshes.size() <= num){
		ofLogError("ofxAssimpModelLoader") << "getMaterialForMesh(): mesh id: " << num
		<< "out of range for total num meshes: " << scene->mNumMeshes;
		return ofMaterial();
	}
	return modelMeshes[num].material;
}

//-------------------------------------------
ofTexture ofxAssimpModelLoader::getTextureForMesh(string name){
	for(size_t i = 0; i < modelMeshes.size(); i++){
		if(string(modelMeshes[i].mesh->mName.data)==name){
			if(modelMeshes[i].hasTexture()) {
				return modelMeshes[i].getTextureRef();
			}
		}
	}
	ofLogError("ofxAssimpModelLoader") << "getTextureForMesh(): couldn't find mesh: \"" + name << "\"";
	return ofTexture();
}

//-------------------------------------------
ofTexture ofxAssimpModelLoader::getTextureForMesh(unsigned int i){
	if(i < modelMeshes.size()){
		if(modelMeshes[i].hasTexture()) {
			return modelMeshes[i].getTextureRef();
		}
	}
	ofLogError("ofxAssimpModelLoader") << "getTextureForMesh(): mesh id: " << i
	<< "out of range for total num meshes: " << scene->mNumMeshes;
	return ofTexture();
}

//-------------------------------------------
glm::vec3 ofxAssimpModelLoader::getPosition(){
	return pos;
}

//-------------------------------------------
glm::vec3 ofxAssimpModelLoader::getSceneCenter(){
	return aiVecToOfVec(scene_center);
}

//-------------------------------------------
float ofxAssimpModelLoader::getNormalizedScale(){
	return normalizedScale;
}

//-------------------------------------------
glm::vec3 ofxAssimpModelLoader::getScale(){
	return scale;
}

//-------------------------------------------
glm::mat4 ofxAssimpModelLoader::getModelMatrix() {
	return modelMatrix;
}

//-------------------------------------------
glm::vec3 ofxAssimpModelLoader::getSceneMin(bool bScaled ){
	glm::vec3 sceneMin(scene_min.x, scene_min.y, scene_min.z);
	if( bScaled ){
		return sceneMin * scale;
	}else{
		return sceneMin;
	}
}

//-------------------------------------------
glm::vec3 ofxAssimpModelLoader::getSceneMax(bool bScaled ){
	glm::vec3 sceneMax(scene_max.x, scene_max.y, scene_max.z);
	if( bScaled ){
		return sceneMax * scale;
	}else{
		return sceneMax;
	}
}

//-------------------------------------------
glm::vec3 ofxAssimpModelLoader::getSceneMinModelSpace(){
	glm::vec3 sceneMax(scene_min.x, scene_min.y, scene_min.z);
	return glm::vec3(modelMatrix * glm::vec4(scene_min.x, scene_min.y, scene_min.z, 1.0));
}

//-------------------------------------------
glm::vec3 ofxAssimpModelLoader::getSceneMaxModelSpace(){
	glm::vec3 sceneMax(scene_max.x, scene_max.y, scene_max.z);
	return glm::vec3(modelMatrix * glm::vec4(scene_max.x, scene_max.y, scene_max.z, 1.0));
}

//-------------------------------------------
glm::vec3 ofxAssimpModelLoader::getSceneCenterModelSpace(){
	glm::vec3 center = getSceneCenter();
	return glm::vec3(modelMatrix * glm::vec4(center.x, center.y, center.z, 1.0));
}

//-------------------------------------------
int ofxAssimpModelLoader::getNumRotations(){
	return rotAngle.size();
}

//-------------------------------------------
glm::vec3 ofxAssimpModelLoader::getRotationAxis(int which){
	if((int)rotAxis.size() > which){
		return rotAxis[which];
	}else{
		return glm::vec3(0.0,0.0,0.0);
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
	return scene.get();
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
