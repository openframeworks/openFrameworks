#include "ofMaterial.h"
#include "ofLight.h"
#include "ofShadow.h"
#include "ofCubeMap.h"
#include "ofImage.h"
#include "ofGLProgrammableRenderer.h"

using std::shared_ptr;
using std::string;

std::map<ofGLProgrammableRenderer*, std::map<std::string, std::weak_ptr<ofMaterial::Shaders>>> ofMaterial::shadersMap;

namespace{
string vertexSource(bool bPBR, string defaultHeader, int maxLights, bool hasTexture, bool hasColor, std::string addShaderSrc,const ofMaterialSettings& adata);
string fragmentSource(bool bPBR, string defaultHeader, string customUniforms, const ofMaterialSettings& adata, int maxLights, bool hasTexture, bool hasColor, std::string definesString="");
}

//----------------------------------------------------------
ofMaterial::ofMaterial() {
}

//----------------------------------------------------------
std::string ofMaterial::getUniformName( const ofMaterialTextureType& aMaterialTextureType ) {
	switch(aMaterialTextureType) {
		case OF_MATERIAL_TEXTURE_DIFFUSE:
			return "tex0";
		case OF_MATERIAL_TEXTURE_SPECULAR:
			return "tex_specular";
		case OF_MATERIAL_TEXTURE_AMBIENT:
			return "tex_ambient";
		case OF_MATERIAL_TEXTURE_EMISSIVE:
			return "tex_emissive";
		case OF_MATERIAL_TEXTURE_NORMAL:
			return "tex_normal";
		case OF_MATERIAL_TEXTURE_OCCLUSION:
			return "tex_occlusion";
		case OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC:
			return "tex_ao_roughness_metallic";
		case OF_MATERIAL_TEXTURE_ROUGHNESS_METALLIC:
			return "tex_roughness_metallic";
		case OF_MATERIAL_TEXTURE_ROUGHNESS:
			return "tex_roughness";
		case OF_MATERIAL_TEXTURE_METALLIC:
			return "tex_metallic";
		case OF_MATERIAL_TEXTURE_DISPLACEMENT:
			return "tex_displacement";
		case OF_MATERIAL_TEXTURE_CLEARCOAT:
			return "tex_clearcoat_intensity";
		case OF_MATERIAL_TEXTURE_CLEARCOAT_ROUGHNESS:
			return "tex_clearcoat_roughness";
		case OF_MATERIAL_TEXTURE_CLEARCOAT_INTENSITY_ROUGHNESS:
			return "tex_clearcoat_intensity_roughness";
		case OF_MATERIAL_TEXTURE_CLEARCOAT_NORMAL:
			return "tex_clearcoat_normal";
		default:
			break;
	}
	return "";
}

//----------------------------------------------------------
void ofMaterial::setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor oEmissive) {
	setDiffuseColor(oDiffuse);
	setAmbientColor(oAmbient);
	setSpecularColor(oSpecular);
	setEmissiveColor(oEmissive);
}

//----------------------------------------------------------
void ofMaterial::setup(const ofMaterialSettings & settings){
	if(settings.customUniforms != data.customUniforms || settings.postFragment != data.postFragment || 
		settings.mainVertexKey != data.mainVertexKey || settings.mainFragmentKey != data.mainFragmentKey || 
		settings.isPbr != data.isPbr){
		shaders.clear();
		uniforms1f.clear();
		uniforms2f.clear();
		uniforms3f.clear();
		uniforms4f.clear();
		uniforms1i.clear();
		uniforms2i.clear();
		uniforms3i.clear();
		uniforms4i.clear();
		mCustomUniforms.clear();
		mDefines.clear();
		mBDefinesDirty = true;
	}
	data = settings;
	setClearCoatEnabled(data.clearCoatEnabled);
}

//----------------------------------------------------------
void ofMaterial::setShaderMain(std::string aShaderSrc, GLenum atype, std::string skey) {
	if(!isPBR()) {
		ofLogWarning("ofMaterial::setShaderMain") << "only available on PBR materials.";
		return;
	}
	
	if(atype == GL_VERTEX_SHADER) {
		// we would like to replace the current shader at key 
		// using a skey instead of shadersrc as key so we can easily overwrite
		if( data.mainVertexKey == skey) {
			// delete previous shader here, whether frag shader has same key or not
			std::string sid = getShaderStringId();
			mShaderIdsToRemove[sid]++;
		}
		data.mainVertex = aShaderSrc;
		data.mainVertexKey = skey;
	} else if( atype == GL_FRAGMENT_SHADER ) {
		if( data.mainFragmentKey == skey) {
			// delete previous shader here, whether frag shader has same key or not
			std::string sid = getShaderStringId();
			mShaderIdsToRemove[sid]++;
		}
		data.mainFragment = aShaderSrc;
		data.mainFragmentKey = skey;
	}
}

//----------------------------------------------------------
void ofMaterial::setDiffuseColor(ofFloatColor oDiffuse) {
	data.diffuse = oDiffuse;
	if( isBound() && currentRenderShader) {
		currentRenderShader->setUniform4fv("mat_diffuse", &data.diffuse.r );
	}
}

//----------------------------------------------------------
void ofMaterial::setAmbientColor(ofFloatColor oAmbient) {
	data.ambient = oAmbient;
	if( isBound() && !isPBR() && currentRenderShader) {
		currentRenderShader->setUniform4fv("mat_ambient", &data.ambient.r);
	}
}

//----------------------------------------------------------
void ofMaterial::setSpecularColor(ofFloatColor oSpecular) {
	data.specular = oSpecular;
	if( isBound() && !isPBR() && currentRenderShader) {
		currentRenderShader->setUniform4fv("mat_specular", &data.specular.r);
	}
}

//----------------------------------------------------------
void ofMaterial::setEmissiveColor(ofFloatColor oEmissive) {
	data.emissive = oEmissive;
	if( isBound() && currentRenderShader) {
		currentRenderShader->setUniform4fv("mat_emissive", &data.emissive.r);
	}
}

//----------------------------------------------------------
void ofMaterial::setShininess(float nShininess) {
	data.shininess = nShininess;
	if( isBound() && !isPBR() && currentRenderShader) {
		currentRenderShader->setUniform1f("mat_shininess",data.shininess);
	}
}

//----------------------------------------------------------
void ofMaterial::setTexCoordScale( float xscale, float yscale ) {
	data.texCoordScale.x = xscale;
	data.texCoordScale.y = yscale;
	if( isBound() && currentRenderShader) {
		currentRenderShader->setUniform2f("mat_texcoord_scale", data.texCoordScale );
	}
}

//----------------------------------------------------------
bool ofMaterial::loadTexture( const ofMaterialTextureType& aMaterialTextureType, std::string apath ) {
	return loadTexture(aMaterialTextureType, apath, ofGetUsingArbTex(), false);
}

//----------------------------------------------------------
bool ofMaterial::loadTexture( const ofMaterialTextureType& aMaterialTextureType, std::string apath, bool bTex2d, bool mirrorY ) {
	ofPixels tpix;
	bool bWasUsingArb = ofGetUsingArbTex();
	bTex2d ? ofEnableArbTex() : ofDisableArbTex();
	
	bool bLoadOk = false;
	
	if( ofLoadImage( tpix, apath )) {
		if( mirrorY ) {
			tpix.mirror(mirrorY, false);
		}
		bLoadOk = true;
		// if there was a previous instance, then erase it, then replace it 
		if( mLocalTextures.find(aMaterialTextureType) != mLocalTextures.end() ) {
			if( mLocalTextures[aMaterialTextureType] ) {
				mLocalTextures[aMaterialTextureType].reset();
			}
			mLocalTextures.erase(aMaterialTextureType);
		}
		
		auto tex = std::make_shared<ofTexture>();
		tex->loadData(tpix);
		mLocalTextures[aMaterialTextureType] = tex;
		setTexture( aMaterialTextureType, *tex );
	} else {
		ofLogError("ofMaterial") << "loadTexture(): FAILED for " << getUniformName(aMaterialTextureType) << " at path: " << apath;
	}
	
	bWasUsingArb ? ofEnableArbTex() : ofDisableArbTex();
	return bLoadOk;
}

//----------------------------------------------------------
bool ofMaterial::isPBRTexture(const ofMaterialTextureType& aMaterialTextureType) {
	return aMaterialTextureType >= (int)OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC;
}

//----------------------------------------------------------
void ofMaterial::setTexture(const ofMaterialTextureType& aMaterialTextureType,const ofTexture & aTex) {
	if(isBound()) {
		ofLogWarning("ofMaterial::setTexture") << " must set texture when material is not bound.";
		return;
	}
	if( !isPBR() ) {
		if(isPBRTexture(aMaterialTextureType)) {
			ofLogVerbose("ofMaterial::setTexture") << " setting material to pbr.";
			setPBR(true);
		}
	}

	if(aMaterialTextureType == OF_MATERIAL_TEXTURE_CLEARCOAT ||
	   aMaterialTextureType == OF_MATERIAL_TEXTURE_CLEARCOAT_ROUGHNESS ||
	   aMaterialTextureType == OF_MATERIAL_TEXTURE_CLEARCOAT_INTENSITY_ROUGHNESS ) {
		if( aTex.isAllocated() ) {
			setClearCoatEnabled(true);
		}
	}
	setCustomUniformTexture(getUniformName(aMaterialTextureType), aTex);
	mergeCustomUniformTextures();
}

//----------------------------------------------------------
void ofMaterial::setDiffuseTexture(const ofTexture & aTex) {
	setTexture(OF_MATERIAL_TEXTURE_DIFFUSE, aTex);
}

//----------------------------------------------------------
void ofMaterial::setSpecularTexture(const ofTexture & aTex){
	setTexture(OF_MATERIAL_TEXTURE_SPECULAR, aTex);
}

//----------------------------------------------------------
void ofMaterial::setAmbientTexture(const ofTexture & aTex){
	setTexture(OF_MATERIAL_TEXTURE_AMBIENT, aTex);
}

//----------------------------------------------------------
void ofMaterial::setEmissiveTexture(const ofTexture & aTex){
	setTexture(OF_MATERIAL_TEXTURE_EMISSIVE, aTex);
}

//----------------------------------------------------------
void ofMaterial::setNormalTexture(const ofTexture & aTex){
	setTexture(OF_MATERIAL_TEXTURE_NORMAL, aTex);
}

//----------------------------------------------------------
void ofMaterial::setOcclusionTexture(const ofTexture & aTex){
	setTexture(OF_MATERIAL_TEXTURE_OCCLUSION, aTex);
}

//----------------------------------------------------
void ofMaterial::setAoRoughnessMetallicTexture(const ofTexture & aTex) {
	// r: occlusion, g: roughness, b: metallic
	setTexture(OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC, aTex );
}

//----------------------------------------------------
void ofMaterial::setRoughnessMetallicTexture(const ofTexture & aTex) {
	// r: n/a, g: roughness, b: metallic
	setTexture(OF_MATERIAL_TEXTURE_ROUGHNESS_METALLIC, aTex);
}

//----------------------------------------------------
void ofMaterial::setRoughnessTexture(const ofTexture & aTex) {
	setTexture(OF_MATERIAL_TEXTURE_ROUGHNESS, aTex);
}

//----------------------------------------------------
void ofMaterial::setMetallicTexture(const ofTexture & aTex) {
	setTexture(OF_MATERIAL_TEXTURE_METALLIC, aTex);
}

//----------------------------------------------------
void ofMaterial::setDisplacementTexture(const ofTexture & aTex) {
	setTexture(OF_MATERIAL_TEXTURE_DISPLACEMENT, aTex);
}

//----------------------------------------------------
void ofMaterial::setClearCoatTexture( const ofTexture& aTex ) {
	setTexture(OF_MATERIAL_TEXTURE_CLEARCOAT, aTex );
}

//----------------------------------------------------
void ofMaterial::setMetallic( const float& ametallic ) {
	data.metallic = ametallic;
	
	if( isBound() && isPBR() && currentRenderShader) {
		currentRenderShader->setUniform1f("mat_metallic", data.metallic );
	}
	if( !isPBR() ) {
		setPBR(true);
	}
}

//----------------------------------------------------
void ofMaterial::setRoughness( const float& aroughness ) {
	data.roughness = aroughness;
	if( isBound() && isPBR() && currentRenderShader) {
		currentRenderShader->setUniform1f("mat_roughness", data.roughness );
	}
	if( !isPBR() ) {
		setPBR(true);
	}
}

//----------------------------------------------------
void ofMaterial::setReflectance( const float& areflectance ) {
	data.reflectance = areflectance;
	if( isBound() && isPBR() && currentRenderShader) {
		currentRenderShader->setUniform1f("mat_reflectance", data.reflectance );
	}
	if( !isPBR() ) {
		setPBR(true);
	}
}

//----------------------------------------------------
void ofMaterial::setClearCoatEnabled( bool ab ) {
	if(isBound()) {
		ofLogWarning("ofMaterial::setClearCoatEnabled") << " must be called when material is not bound.";
		return;
	}
	if( !isPBR() ) {
		setPBR(true);
	}
	data.clearCoatEnabled = ab;
	if(ab) {
		addShaderDefine( "HAS_CLEAR_COAT", "1" );
	} else {
		removeShaderDefine( "HAS_CLEAR_COAT" );
	}
}

//----------------------------------------------------
void ofMaterial::setClearCoatStrength( const float& astrength ) {
	data.clearCoatStrength = astrength;
	if( isClearCoatEnabled() && isBound() && isPBR() && currentRenderShader ) {
		currentRenderShader->setUniform2f("mat_clearcoat", data.clearCoatStrength, data.clearCoatRoughness );
	}
	if( !isPBR() ) {
		setPBR(true);
	}
}

//----------------------------------------------------
void ofMaterial::setClearCoatRoughness( const float& aroughness ) {
	data.clearCoatRoughness = aroughness;
	if( isClearCoatEnabled() && isBound() && isPBR() && currentRenderShader ) {
		currentRenderShader->setUniform2f("mat_clearcoat", data.clearCoatStrength, data.clearCoatRoughness );
	}
	if( !isPBR() ) {
		setPBR(true);
	}
}

//----------------------------------------------------------
void ofMaterial::setDisplacementStrength( const float& astrength ) {
	data.displacementStrength = astrength;
	if(isBound() && isPBR() && currentRenderShader ) {
		if( hasTexture(OF_MATERIAL_TEXTURE_DISPLACEMENT) ) {
			currentRenderShader->setUniform1f("mat_displacement_strength", data.displacementStrength );
		}
	}
	if( !isPBR() ) {
		setPBR(true);
	}
}

//----------------------------------------------------------
void ofMaterial::setDisplacementNormalsStrength( const float& astrength ) {
	data.displacementNormalsStrength = astrength;
	if(isBound() && isPBR() && currentRenderShader ) {
		if( hasTexture(OF_MATERIAL_TEXTURE_DISPLACEMENT) ) {
			currentRenderShader->setUniform1f("mat_displacement_normals_strength", data.displacementNormalsStrength );
		}
	}
}

//----------------------------------------------------------
void ofMaterial::setNormalGeomToNormalMapMix( const float& astrength ) {
	data.normalGeomToNormalMapMix = astrength;
	if(isBound() && isPBR() && currentRenderShader ) {
		if( hasTexture(OF_MATERIAL_TEXTURE_NORMAL) || hasTexture(OF_MATERIAL_TEXTURE_DISPLACEMENT) ) {
			currentRenderShader->setUniform1f("mat_normal_mix", data.normalGeomToNormalMapMix );
		}
	}
}

//----------------------------------------------------------
void ofMaterial::setData(const ofMaterial::Data &data){
	setup(data);
}

//----------------------------------------------------------
float ofMaterial::getShininess()const{
	return data.shininess;
}

//----------------------------------------------------
float ofMaterial::getMetallic() const {
	return data.metallic;
}

//----------------------------------------------------
float ofMaterial::getRoughness() const {
	return data.roughness;
}

//----------------------------------------------------
float ofMaterial::getReflectance() const {
	return data.reflectance;
}

//----------------------------------------------------
bool ofMaterial::isClearCoatEnabled() const {
	return data.clearCoatEnabled;
}

//----------------------------------------------------
float ofMaterial::getClearCoatStrength() const {
	return data.clearCoatStrength;
}

//----------------------------------------------------
float ofMaterial::getClearCoatRoughness() const {
	return data.clearCoatRoughness;
}

//----------------------------------------------------
float ofMaterial::getDisplacementStrength() const {
	return data.displacementStrength;
}

//----------------------------------------------------
float ofMaterial::getDisplacementNormalsStrength() const {
	return data.displacementNormalsStrength;
}

//----------------------------------------------------
float ofMaterial::getNormalGeomToNormalMapMix() const {
	return data.normalGeomToNormalMapMix;
}

//----------------------------------------------------------
ofFloatColor ofMaterial::getDiffuseColor()const {
	return data.diffuse;
}

//----------------------------------------------------------
ofFloatColor ofMaterial::getAmbientColor()const {
	return data.ambient;
}

//----------------------------------------------------------
ofFloatColor ofMaterial::getSpecularColor()const {
	return data.specular;
}

//----------------------------------------------------------
ofFloatColor ofMaterial::getEmissiveColor()const {
	return data.emissive;
}

//----------------------------------------------------------
ofMaterialSettings ofMaterial::getSettings() const{
    return data;
}

//-----------------------------------------------------------
void ofMaterial::begin() const{
	if(ofGetGLRenderer()){
		ofGetGLRenderer()->bind(*this);
	}
}

//----------------------------------------------------------
void ofMaterial::end() const{
	if(ofGetGLRenderer()){
		ofGetGLRenderer()->unbind(*this);
	}
}

//----------------------------------------------------------
void ofMaterial::uploadMatrices(const ofShader & shader,ofGLProgrammableRenderer & renderer) const {
	if(!isPBR()) {
		ofBaseMaterial::uploadMatrices(shader, renderer);
	}
}

//-----------------------------------------------------------
void ofMaterial::mergeCustomUniformTextures() {
	mergeCustomUniformTextures(OF_MATERIAL_TEXTURE_ROUGHNESS_METALLIC, {
		OF_MATERIAL_TEXTURE_ROUGHNESS,
		OF_MATERIAL_TEXTURE_METALLIC
	} );
	
	mergeCustomUniformTextures(OF_MATERIAL_TEXTURE_ROUGHNESS_METALLIC, {
		OF_MATERIAL_TEXTURE_OCCLUSION,
		OF_MATERIAL_TEXTURE_ROUGHNESS_METALLIC
	} );
	
	mergeCustomUniformTextures(OF_MATERIAL_TEXTURE_AO_ROUGHNESS_METALLIC, {
		OF_MATERIAL_TEXTURE_OCCLUSION,
		OF_MATERIAL_TEXTURE_ROUGHNESS,
		OF_MATERIAL_TEXTURE_METALLIC
	} );
}

//-----------------------------------------------------------
void ofMaterial::mergeCustomUniformTextures(ofMaterialTextureType mainType, std::vector<ofMaterialTextureType> mergeTypes) {
	size_t mtsSize = mergeTypes.size();
	if(!hasTexture(mainType) && mtsSize > 1){
		bool bHasAllMergeTypes = true;
		GLint texID;
		int texTarget;
		bool bMatchingTextures = true;
		int minTexLocation = 99999;
		
		for(int i = 0; i < mtsSize; i++ ) {
			if(!hasTexture(mergeTypes[i])){
				bHasAllMergeTypes = false;
				bMatchingTextures = false;
				break;
			} else {
				auto rtex = getCustomUniformTexture(mergeTypes[i]);
				if(i == 0 ) {
					texID = rtex.textureID;
					texTarget = rtex.textureTarget;
				}
				if( rtex.textureTarget != texTarget || rtex.textureID != texID ) {
					bMatchingTextures = false;
					break;
				} else {
					if( rtex.textureLocation < minTexLocation ) {
						minTexLocation = rtex.textureLocation;
					}
				}
			}
		}
		
		if(bHasAllMergeTypes && bMatchingTextures && minTexLocation < 1000){
			for(int i = 0; i < mtsSize; i++ ) {
				removeCustomUniformTexture(mergeTypes[i]);
			}
			setCustomUniformTexture( getUniformName(mainType), texTarget, texID, minTexLocation );
		}
	}
}

//-----------------------------------------------------------
ofMaterial::TextureUnifom ofMaterial::getCustomUniformTexture(const ofMaterialTextureType& aMaterialTextureType){
	return getCustomUniformTexture(getUniformName(aMaterialTextureType));
}

//-----------------------------------------------------------
ofMaterial::TextureUnifom ofMaterial::getCustomUniformTexture(const std::string & name){
	if(uniformstex.find(name) != uniformstex.end()){
		return uniformstex[name];
	}
	TextureUnifom tu;
	return tu;
}

// called from ofGLProgrammableRenderer
//-----------------------------------------------------------
void ofMaterial::unbind(ofGLProgrammableRenderer & renderer) const {
	mBound = false;
	currentRenderShader = nullptr;
}

//-----------------------------------------------------------
const std::string ofMaterial::getShaderStringId() const {
	std::string pbrStr = "pbr-yes";
	if(!isPBR()) {
		pbrStr = "pbr-no";
	}
	return pbrStr+data.uniqueIdString+data.postFragment+data.mainVertexKey+data.mainFragmentKey;
}

//-----------------------------------------------------------
void ofMaterial::initShaders(ofGLProgrammableRenderer & renderer) const{
	// remove any shaders that have their main source remove 
	{
		if( mShaderIdsToRemove.size() ) {
			for( auto& sids : mShaderIdsToRemove ) {
				if(shadersMap[&renderer].find(sids.first)!=shadersMap[&renderer].end()){
					auto newShaders = shadersMap[&renderer][sids.first].lock();
					if( newShaders ) {
						newShaders.reset();
					}
					shadersMap[&renderer].erase(sids.first);
				}
			}

			ofLogNotice("ofMaterial :: initShaders") << shadersMap.size() << " | " << ofGetFrameNum();

			auto trendererShaders = shaders.find(&renderer);
			if( trendererShaders != shaders.end() ) {
				if(trendererShaders->second) {
					trendererShaders->second.reset();
				}
				shaders.erase(&renderer);
			}
			mShaderIdsToRemove.clear();
		}
	}

    auto rendererShaders = shaders.find(&renderer);
	
	size_t numLights = ofLightsData().size();
	// only support for a single cube map at a time
	size_t numCubeMaps = ofCubeMapsData().size() > 0 ? 1 : 0;
	const std::string shaderId = getShaderStringId();//data.uniqueIdString+data.postFragment+data.mainVertexKey+data.mainFragmentKey;
	
	if(rendererShaders == shaders.end() || rendererShaders->second->numLights != numLights || rendererShaders->second->numCubeMaps != numCubeMaps ){
		if(shadersMap[&renderer].find(shaderId)!=shadersMap[&renderer].end()){
			auto newShaders = shadersMap[&renderer][shaderId].lock();
			if(newShaders == nullptr || newShaders->numLights != numLights || newShaders->numCubeMaps != numCubeMaps ){
				shadersMap[&renderer].erase(shaderId);
				shaders[&renderer] = nullptr;
			}else{
				shaders[&renderer] = newShaders;
			}
		}
	}

    if(shaders[&renderer] == nullptr){
		ofLogNotice("ofMaterial") << "initShaders : allocating shaders again | " << ofGetFrameNum();
        //add the custom uniforms to the shader header
        auto customUniforms = data.customUniforms;
        for( auto & custom : mCustomUniforms ){
        	customUniforms += custom.second + " " + custom.first + ";\n";
        }

		std::string definesString = getDefinesString();
		
		ofLogVerbose("ofMaterial") << " defines--------------- " << std::endl;
		ofLogVerbose("ofMaterial") << definesString;
//		ofLogNotice("ofMaterial") << "custom uniforms ---------- "
		ofLogVerbose("ofMaterial") << "textures --------------- " << uniformstex.size();
		for (auto & uniform : uniformstex) {
			ofLogVerbose() << uniform.first << ", " << uniform.second.textureTarget <<", " << uniform.second.textureID << ", " << uniform.second.textureLocation << std::endl;
		}
		
		std::string extraVertString = definesString;
		if( hasTexture(OF_MATERIAL_TEXTURE_DISPLACEMENT) ) {
			extraVertString += "\nuniform SAMPLER "+getUniformName(OF_MATERIAL_TEXTURE_DISPLACEMENT)+";\n";
		}
    
        #ifndef TARGET_OPENGLES
            string vertexRectHeader = renderer.defaultVertexShaderHeader(GL_TEXTURE_RECTANGLE);
            string fragmentRectHeader = renderer.defaultFragmentShaderHeader(GL_TEXTURE_RECTANGLE);
        #endif
        string vertex2DHeader = renderer.defaultVertexShaderHeader(GL_TEXTURE_2D);
        string fragment2DHeader = renderer.defaultFragmentShaderHeader(GL_TEXTURE_2D);

        shaders[&renderer].reset(new Shaders);
        shaders[&renderer]->numLights = numLights;
		shaders[&renderer]->numCubeMaps = numCubeMaps;
		
		//std::cout << "ofMaterial:: Shader source ------ " << std::endl;
		//std::cout << vertexSource(isPBR(),vertex2DHeader,numLights,true,false,extraVertString,data) << std::endl;
//		std::cout << fragmentSource(isPBR(),fragmentRectHeader, customUniforms, data.postFragment,numLights,true,false,definesString);
//		std::cout << fragmentSource(isPBR(),fragment2DHeader, customUniforms, data.postFragment,numLights,false,false, definesString) << std::endl;
		
        shaders[&renderer]->noTexture.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(isPBR(),vertex2DHeader,numLights,false,false,extraVertString,data));
        shaders[&renderer]->noTexture.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(isPBR(),fragment2DHeader, customUniforms, data,numLights,false,false, definesString));
        shaders[&renderer]->noTexture.bindDefaults();
        shaders[&renderer]->noTexture.linkProgram();

        shaders[&renderer]->texture2D.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(isPBR(),vertex2DHeader,numLights,true,false,extraVertString,data));
        shaders[&renderer]->texture2D.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(isPBR(),fragment2DHeader, customUniforms, data,numLights,true,false,definesString));
        shaders[&renderer]->texture2D.bindDefaults();
        shaders[&renderer]->texture2D.linkProgram();

        #ifndef TARGET_OPENGLES
            shaders[&renderer]->textureRect.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(isPBR(),vertexRectHeader,numLights,true,false,extraVertString,data));
            shaders[&renderer]->textureRect.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(isPBR(),fragmentRectHeader, customUniforms, data,numLights,true,false,definesString));
            shaders[&renderer]->textureRect.bindDefaults();
            shaders[&renderer]->textureRect.linkProgram();
        #endif

        shaders[&renderer]->color.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(isPBR(),vertex2DHeader,numLights,false,true,extraVertString,data));
        shaders[&renderer]->color.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(isPBR(),fragment2DHeader, customUniforms, data,numLights,false,true, definesString));
        shaders[&renderer]->color.bindDefaults();
        shaders[&renderer]->color.linkProgram();


        shaders[&renderer]->texture2DColor.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(isPBR(),vertex2DHeader,numLights,true,true,extraVertString,data));
        shaders[&renderer]->texture2DColor.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(isPBR(),fragment2DHeader, customUniforms, data,numLights,true,true,definesString));
        shaders[&renderer]->texture2DColor.bindDefaults();
        shaders[&renderer]->texture2DColor.linkProgram();

        #ifndef TARGET_OPENGLES
            shaders[&renderer]->textureRectColor.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(isPBR(),vertexRectHeader,numLights,true,true,extraVertString,data));
            shaders[&renderer]->textureRectColor.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(isPBR(),fragmentRectHeader, customUniforms, data,numLights,true,true,definesString));
            shaders[&renderer]->textureRectColor.bindDefaults();
            shaders[&renderer]->textureRectColor.linkProgram();
        #endif

//        shadersMap[&renderer][data.postFragment] = shaders[&renderer];
		shadersMap[&renderer][shaderId] = shaders[&renderer];
    }

}

const ofShader & ofMaterial::getShader(int textureTarget, bool geometryHasColor, ofGLProgrammableRenderer & renderer) const{
    initShaders(renderer);
	
	if(bHasCustomShader && customShader){
		return *customShader;
	}
	
//	std::cout << "ofMaterial::getShader : textureTarget: " << textureTarget << " GL_TEXTURE_2D: " << GL_TEXTURE_2D << " geometryHasColor: " << geometryHasColor << " diffuse: " << hasTexture(OF_MATERIAL_TEXTURE_DIFFUSE) << std::endl;
	
	// override the textureTarget argument coming from the programmable renderer
	// the renderer is passing the textureTarget based on if there is a texture that is bound
	// if there is no texture bound, then go ahead and switch to the diffuse texture
	if(textureTarget == OF_NO_TEXTURE && hasTexture(OF_MATERIAL_TEXTURE_DIFFUSE) ) {
		const std::string loc = getUniformName(OF_MATERIAL_TEXTURE_DIFFUSE);
		const auto& dt = uniformstex.at(loc);
		textureTarget = dt.textureTarget;
	}
	
//	if(ofGetFrameNum() % 600 == 0) {
//		std::cout << shaders[&renderer]->texture2D.getShaderSource( GL_VERTEX_SHADER );
//	}
	
	switch(textureTarget){
	case OF_NO_TEXTURE:
        if(geometryHasColor){
            return shaders[&renderer]->color;
        }else{
            return shaders[&renderer]->noTexture;
        }
		break;
	case GL_TEXTURE_2D:
        if(geometryHasColor){
            return shaders[&renderer]->texture2DColor;
        }else{
            return shaders[&renderer]->texture2D;
        }
		break;
    default:
        if(geometryHasColor){
            return shaders[&renderer]->textureRectColor;
        }else{
            return shaders[&renderer]->textureRect;
        }
		break;
	}
}

void ofMaterial::updateMaterial(const ofShader & shader,ofGLProgrammableRenderer & renderer) const{
	currentRenderShader = &shader;
	
	shader.setUniform4fv("mat_emissive", &data.emissive.r);
	shader.setUniform2f("mat_texcoord_scale", data.texCoordScale );
	
	if( isPBR() ) {
		shader.setUniform3f("uCameraPos", renderer.getCurrentEyePosition());
		shader.setUniform4fv("mat_diffuse", &data.diffuse.r );
		shader.setUniform1f("mat_roughness", data.roughness );
		shader.setUniform1f("mat_metallic", data.metallic );
		shader.setUniform1f("mat_reflectance", data.reflectance );
		if( isClearCoatEnabled() ) {
			shader.setUniform2f("mat_clearcoat", data.clearCoatStrength, data.clearCoatRoughness );
		}
		
		if( hasTexture(OF_MATERIAL_TEXTURE_DISPLACEMENT) ) {
			shader.setUniform1f("mat_displacement_strength", data.displacementStrength );
			shader.setUniform1f("mat_displacement_normals_strength", data.displacementNormalsStrength );
		}
		if( hasTexture(OF_MATERIAL_TEXTURE_NORMAL) || hasTexture(OF_MATERIAL_TEXTURE_DISPLACEMENT) ) {
			shader.setUniform1f("mat_normal_mix", data.normalGeomToNormalMapMix );
		}
		
		std::shared_ptr<ofCubeMap::Data> cubeMapData = ofCubeMap::getActiveData();
		if( cubeMapData ) {
			shader.setUniform1f("mat_ibl_exposure", cubeMapData->exposure );
		} else {
			shader.setUniform1f("mat_ibl_exposure", 1.0 );
		}
		
	} else {
		shader.setUniform4fv("mat_ambient", &data.ambient.r);
		shader.setUniform4fv("mat_diffuse", &data.diffuse.r);
		shader.setUniform4fv("mat_specular", &data.specular.r);
		shader.setUniform4fv("global_ambient", &ofGetGlobalAmbientColor().r);
		shader.setUniform1f("mat_shininess",data.shininess);
	}
	for(auto & uniform: uniforms1f){
		shader.setUniform1f(uniform.first, uniform.second);
	}
	for (auto & uniform : uniforms2f) {
		shader.setUniform2f(uniform.first, uniform.second);
	}
	for (auto & uniform : uniforms3f) {
		shader.setUniform3f(uniform.first, uniform.second);
	}
	for (auto & uniform : uniforms4f) {
		shader.setUniform4f(uniform.first, uniform.second);
	}
	for (auto & uniform : uniforms1i) {
		shader.setUniform1i(uniform.first, uniform.second);
	}
	for (auto & uniform : uniforms2i) {
		shader.setUniform2i(uniform.first, uniform.second.x, uniform.second.y);
	}
	for (auto & uniform : uniforms3i) {
		shader.setUniform3i(uniform.first, uniform.second.x, uniform.second.y, uniform.second.z);
	}
	for (auto & uniform : uniforms4i) {
		shader.setUniform4i(uniform.first, uniform.second.x, uniform.second.y, uniform.second.z, uniform.second.w);
	}
	for (auto & uniform : uniforms4m) {
		shader.setUniformMatrix4f(uniform.first, uniform.second);
	}
	for (auto & uniform : uniforms3m) {
		shader.setUniformMatrix3f(uniform.first, uniform.second);
	}
	for (auto & uniform : uniformstex) {
		//std::cout << "uniform tex " << uniform.first << " " << std::endl;
		shader.setUniformTexture(uniform.first,
								 uniform.second.textureTarget,
								 uniform.second.textureID,
								 uniform.second.textureLocation);
	}
}

void ofMaterial::updateLights(const ofShader & shader,ofGLProgrammableRenderer & renderer) const{
	for(size_t i=0;i<ofLightsData().size();i++){
		string idx = ofToString(i);
		shared_ptr<ofLight::Data> light = ofLightsData()[i].lock();
		if(!light || !light->isEnabled){
			shader.setUniform1f("lights["+idx+"].enabled",0);
			continue;
		}
		glm::vec4 lightEyePosition = light->position;
		// pbr uses global positions
		if( !isPBR() ) {
			lightEyePosition = renderer.getCurrentViewMatrix() * light->position;
		}
		shader.setUniform1f("lights["+idx+"].enabled",1);
		shader.setUniform1f("lights["+idx+"].type", light->lightType);
		shader.setUniform4f("lights["+idx+"].position", lightEyePosition);
		if( !isPBR() ) {
			shader.setUniform4f("lights["+idx+"].ambient", light->ambientColor);
			shader.setUniform4f("lights["+idx+"].specular", light->specularColor);
		}
		shader.setUniform4f("lights["+idx+"].diffuse", light->diffuseColor);

		if(light->lightType!=OF_LIGHT_DIRECTIONAL){
			// TODO: add in light radius if pbr?
			shader.setUniform1f("lights["+idx+"].radius", 0.0f);
			shader.setUniform1f("lights["+idx+"].constantAttenuation", light->attenuation_constant);
			shader.setUniform1f("lights["+idx+"].linearAttenuation", light->attenuation_linear);
			shader.setUniform1f("lights["+idx+"].quadraticAttenuation", light->attenuation_quadratic);
		}

		if(light->lightType==OF_LIGHT_SPOT){
			// PBR light calcs are in world space
			glm::vec3 direction = light->direction;
			if( !isPBR() ) {
				direction = glm::vec3(light->position) + light->direction;
				glm::vec4 direction4 = renderer.getCurrentViewMatrix() * glm::vec4(direction,1.0);
				direction = glm::vec3(direction4) / direction4.w;
				direction = direction - glm::vec3(lightEyePosition);
			}
			shader.setUniform3f("lights["+idx+"].spotDirection", glm::normalize(direction));
			shader.setUniform1f("lights["+idx+"].spotExponent", light->exponent);
			shader.setUniform1f("lights["+idx+"].spotCutoff", light->spotCutOff);
			shader.setUniform1f("lights["+idx+"].spotCosCutoff", cos(ofDegToRad(light->spotCutOff)));
		}else if(light->lightType==OF_LIGHT_DIRECTIONAL){
			if( !isPBR() ) {
				glm::vec3 halfVector(glm::normalize(glm::vec4(0.f, 0.f, 1.f, 0.f) + lightEyePosition));
				shader.setUniform3f("lights["+idx+"].halfVector", halfVector);
			}
		}else if(light->lightType==OF_LIGHT_AREA){
			shader.setUniform1f("lights["+idx+"].width", light->width);
			shader.setUniform1f("lights["+idx+"].height", light->height);
			glm::vec3 direction = light->direction;
			if( !isPBR() ) {
				direction = glm::vec3(light->position) + light->direction;
				glm::vec4 direction4 = renderer.getCurrentViewMatrix() * glm::vec4(direction, 1.0);
				direction = glm::vec3(direction4) / direction4.w;
				direction = direction - glm::vec3(lightEyePosition);
			}
			shader.setUniform3f("lights["+idx+"].spotDirection", glm::normalize(direction));
			auto right = light->right;
			auto up = light->up;
			if( !isPBR() ) {
				right = glm::vec3(light->position) + light->right;
				glm::vec4 right4 = renderer.getCurrentViewMatrix() * glm::vec4(right, 1.0);
				right = glm::vec3(right4) / right4.w;
				right = right - glm::vec3(lightEyePosition);
				up = glm::cross(right, direction);
			}
			shader.setUniform3f("lights["+idx+"].right", glm::normalize(toGlm(right)));
			shader.setUniform3f("lights["+idx+"].up", glm::normalize(up));
		}
	}
}

void ofMaterial::updateShadows(const ofShader & shader,ofGLProgrammableRenderer & renderer) const {
	// going to start above the highest tex location
	shader.setShadowUniforms(getHighestUniformTextureLocation()+1);
}

void ofMaterial::updateEnvironmentMaps(const ofShader & shader,ofGLProgrammableRenderer & renderer) const {
	if( isPBR() ) {
		// adding 4 to the offset to account for the shadows
		shader.setPbrEnvironmentMapUniforms(getHighestUniformTextureLocation()+1+4);
	}
}

void ofMaterial::setCustomShader( std::shared_ptr<ofShader> aCustomShader) {
	customShader = aCustomShader;
	if( customShader ) {
		bHasCustomShader = true;
	}
}

void ofMaterial::setCustomUniform1f(const std::string & name, float value){
	uniforms1f[name] = value;
	mCustomUniforms[name] = "uniform float";
}

void ofMaterial::setCustomUniform2f(const std::string & name, glm::vec2 value){
	uniforms2f[name] = value;
	mCustomUniforms[name] = "uniform vec2";
}

void ofMaterial::setCustomUniform3f(const std::string & name, glm::vec3 value) {
	uniforms3f[name] = value;
	mCustomUniforms[name] = "uniform vec3";
}

void ofMaterial::setCustomUniform4f(const std::string & name, glm::vec4 value) {
	uniforms4f[name] = value;
	mCustomUniforms[name] = "uniform vec4";
}

void ofMaterial::setCustomUniform1i(const std::string & name, int value) {
	uniforms1i[name] = value;
	mCustomUniforms[name] = "uniform int";
}

void ofMaterial::setCustomUniform2i(const std::string & name, glm::vec<2,int> value) {
	uniforms2i[name] = value;
	mCustomUniforms[name] = "uniform ivec2";
}

void ofMaterial::setCustomUniform3i(const std::string & name, glm::vec<3, int> value) {
	uniforms3i[name] = value;
	mCustomUniforms[name] = "uniform ivec3";
}

void ofMaterial::setCustomUniform4i(const std::string & name, glm::vec<4, int> value) {
	uniforms4i[name] = value;
	mCustomUniforms[name] = "uniform ivec4";
}

void ofMaterial::setCustomUniformMatrix4f(const std::string & name, glm::mat4 value){
	uniforms4m[name] = value;
	mCustomUniforms[name] = "uniform mat4";
}

void ofMaterial::setCustomUniformMatrix3f(const std::string & name, glm::mat3 value){
	uniforms3m[name] = value;
	mCustomUniforms[name] = "uniform mat3";
}

//--------------------------------------------------------
void ofMaterial::setCustomUniformTexture(const std::string & name, const ofTexture & value ) {
	setCustomUniformTexture(name, value.getTextureData().textureTarget, int(value.getTextureData().textureID) );
}

//--------------------------------------------------------
void ofMaterial::setCustomUniformTexture(const std::string & name, int textureTarget, GLint textureID){
	
	int textureLocation = -1;
	// if the texture uniform name is not registered, then try to find a new location //
	if( uniformstex.count(name) < 1 ) {
		// not detected, lets get the next location
		int newLoc = 1;
		// set a max of 20 texture locations //
		// going to start at 1 since OF will set the diffuse texture to 0
		for( int i = 2; i < 20; i++ ) {
			bool bAlreadyHas = false;
			newLoc = i;
			for( auto& iter : uniformstex ) {
				if( iter.second.textureLocation == i ) {
					bAlreadyHas = true;
					break;
				}
			}
			if( !bAlreadyHas ) {
				textureLocation = newLoc;
				break;
			}
		}
		if( textureLocation < 0 ) {
			ofLogWarning("ofMaterial") << "setCustomUniformTexture(): " << name << " auto textureLocation not detected";
		} else {
			ofLogVerbose("ofMaterial") << "setCustomUniformTexture(): add custom texture: " << name << " to textureLocation: " << textureLocation;
		}
	} else {
		textureLocation = uniformstex[name].textureLocation;
	}
	
	if( textureLocation > -1 ) {
		setCustomUniformTexture(name, textureTarget, textureID, textureLocation);
	}
}

//--------------------------------------------------------
void ofMaterial::setCustomUniformTexture(const std::string & name, const ofTexture & value, int textureLocation){
	setCustomUniformTexture(name, value.getTextureData().textureTarget, int(value.getTextureData().textureID), textureLocation);
}

//--------------------------------------------------------
void ofMaterial::setCustomUniformTexture(const std::string & name, int textureTarget, GLint textureID, int textureLocation){
	// only set the custom uniform and define if it is not the diffuse texture
	// the texture shaders have HAS_TEXTURE and uniform SAMPLER tex0 already included
	string shaderDefine = "";
	if( name != getUniformName(OF_MATERIAL_TEXTURE_DIFFUSE) ) {
		shaderDefine = "HAS_"+ofToUpper(name);
		mCustomUniforms[name] = "uniform SAMPLER";
		addShaderDefine(shaderDefine, "1" );
	}
	uniformstex[name] = {textureTarget, textureID, textureLocation, shaderDefine};
}

//--------------------------------------------------------
bool ofMaterial::removeCustomUniformTexture(const ofMaterialTextureType& aMaterialTextureType) {
	return removeCustomUniformTexture(getUniformName(aMaterialTextureType));
}

//--------------------------------------------------------
bool ofMaterial::removeCustomUniformTexture(const std::string & name){
	if( uniformstex.find(name) != uniformstex.end() ){
		removeShaderDefine(uniformstex[name].shaderDefine);
		uniformstex.erase(name);
		return true;
	}
	return false;
}

//--------------------------------------------------------
int ofMaterial::getHighestUniformTextureLocation() const {
	int max = 1;
	for( auto& iter : uniformstex ) {
		if( iter.second.textureLocation > max ) {
			max = iter.second.textureLocation;
		}
	}
	return max;
}

//--------------------------------------------------------
bool ofMaterial::hasTexture(const ofMaterialTextureType& aMaterialTextureType) const {
	auto name = getUniformName(aMaterialTextureType);
	if( uniformstex.find(name) != uniformstex.end() ){
		return true;
	}
	return false;
}

//--------------------------------------------------------
void ofMaterial::addShaderDefine( const std::string & aDefineName ) {
	addShaderDefine( aDefineName, "" );
}

//--------------------------------------------------------
void ofMaterial::addShaderDefine( const std::string & aDefineName, const std::string & aDefineValue ) {
	if( aDefineName.empty() ) return;
	
	bool bUpdateDefines = false;
	if( mDefines.count(aDefineName) < 1 ) {
		bUpdateDefines = true;
		mBDefinesDirty = true;
	}
	mDefines[aDefineName] = aDefineValue;
	if( bUpdateDefines ) {
		// update the unique id using uniqueIdString string //
		data.uniqueIdString = "";
		for( auto& def : mDefines ) {
			data.uniqueIdString += def.first;
		}
	}
}

//--------------------------------------------------------
bool ofMaterial::removeShaderDefine( const std::string & aDefineName ) {
	if( aDefineName.empty() ) return false;
	
	if(mDefines.count(aDefineName) > 0 ) {
		mDefines.erase(aDefineName);
		
		// update the unique id using uniqueIdString string //
		data.uniqueIdString = "";
		for( auto& def : mDefines ) {
			data.uniqueIdString += def.first;
		}
		
		return true;
	}
	return false;
}

//--------------------------------------------------------
const std::string ofMaterial::getDefinesString() const {
	std::string definesString = "";
	for( auto& diter : mDefines ) {
		definesString += "#define "+diter.first+" "+diter.second+"\n";
	}
	
	if( isPBR() ) {
		#ifdef TARGET_OPENGLES
		definesString += "#define PBR_QUALITY_LEVEL_LOW 1 \n";
		#else
		definesString += "#define PBR_QUALITY_LEVEL_HIGH 1\n";
		#endif
	}
	
	if(isPBR() && ofCubeMapsData().size() > 0 && ofIsGLProgrammableRenderer() ) {
		const auto& cubeMapData = ofCubeMap::getActiveData();
		if( cubeMapData ) {
			if( cubeMapData->bIrradianceAllocated ) {
				definesString += "#define HAS_TEX_ENV_IRRADIANCE 1\n";
			}
			if(cubeMapData->bPreFilteredMapAllocated) {
				definesString += "#define HAS_TEX_ENV_PRE_FILTER 1\n";
			}
			if( cubeMapData->useLutTex && ofCubeMap::getBrdfLutTexture().isAllocated() ) {
				definesString += "#define HAS_TEX_ENV_BRDF_LUT 1\n";
			}
			definesString += "#define ENV_MAP_MAX_MIPS "+ofToString(cubeMapData->maxMipLevels,0)+"\n";
		}
	}
	
	definesString += ofShadow::getShaderDefinesAsString();
	return definesString;
}


#include "shaders/phong.vert"
#include "shaders/phong.frag"
#include "shaders/shadow.glsl"

#include "shaders/shaderUtils.glsl"
#include "shaders/pbrMaterial.glsl"
#include "shaders/pbrData.glsl"
#include "shaders/pbrLightingFuncs.glsl"
#include "shaders/pbrLighting.glsl"
#include "shaders/pbrLightIBL.glsl"

#include "shaders/pbr.vert"
#include "shaders/pbr.frag"


namespace{
    string shaderHeader(string header, int maxLights, bool hasTexture, bool hasColor){
        header += "#define MAX_LIGHTS " + ofToString(std::max(1,maxLights)) + "\n";
        if(hasTexture){
            header += "#define HAS_TEXTURE 1\n";
		} else {
			header += "#define HAS_TEXTURE 0\n";
		}
        if(hasColor){
            header += "#define HAS_COLOR 1\n";
		} else {
			header += "#define HAS_COLOR 0\n";
		}
        return header;
    }

	//string vertexSource(bool bPBR, string defaultHeader, int maxLights, bool hasTexture, bool hasColor, std::string addShaderSrc){
	string vertexSource(bool bPBR, string defaultHeader, int maxLights, bool hasTexture, bool hasColor, std::string addShaderSrc, const ofMaterialSettings& adata){
		auto source = bPBR ? shader_pbr_vert : vertexShader;
		if( bPBR ) {
			ofStringReplace(source, "%additional_includes%", addShaderSrc);
		} else {
			ofStringReplace(source, "%additional_includes%", "");
		}

		if( bPBR ) {
			string mainVertex = adata.mainVertex;
			if( mainVertex.empty() ) {
				mainVertex = shader_pbr_main_vert;
			}
			ofStringReplace(source, "%mainVertex%", mainVertex);
		}

        return shaderHeader(defaultHeader, maxLights, hasTexture, hasColor) + source;
    }
	// const ofMaterialSettings& adata
	string fragmentSource(bool bPBR, string defaultHeader, string customUniforms, const ofMaterialSettings& adata, int maxLights, bool hasTexture, bool hasColor, string definesString){
    // string fragmentSource(bool bPBR, string defaultHeader, string customUniforms, string postFragment, int maxLights, bool hasTexture, bool hasColor, string definesString){
        auto source = bPBR ? shader_pbr_frag : fragmentShader;
		
		string postFragment = adata.postFragment;
        if(postFragment.empty()){
            postFragment = "vec4 postFragment(vec4 localColor){ return localColor; }";
        }
        ofStringReplace(source, "%postFragment%", postFragment);
        ofStringReplace(source, "%custom_uniforms%", customUniforms);

		if( bPBR ) {
			string mainFrag = adata.mainFragment;
			if( mainFrag.empty() ) {
				if(bPBR) {
					mainFrag = shader_pbr_main_frag;
				}
			}
			ofStringReplace(source, "%mainFragment%", mainFrag);
		}
		
		if(bPBR) {
			string addIncludes = shader_utils;
			addIncludes += shader_pbr_material;
			addIncludes += shader_pbr_data;
			addIncludes += shader_pbr_lighting_funcs;
			addIncludes += shader_pbr_lighting;
			addIncludes += shader_pbr_lighting_ibl;
			// set PBR includes here
			ofStringReplace(source, "%additional_includes%", addIncludes);
		}
		
		
		#ifdef TARGET_OPENGLES
		ofStringReplace(source, "%shader_shadow_include%", "" );
		#else
		if( ofIsGLProgrammableRenderer() ) {
			ofStringReplace(source, "%shader_shadow_include%", shadow_shader_include );
		} else {
			ofStringReplace(source, "%shader_shadow_include%", "" );
		}
		#endif
				
        source = shaderHeader(defaultHeader, maxLights, hasTexture, hasColor) + definesString + source;
        return source;
    }
}
