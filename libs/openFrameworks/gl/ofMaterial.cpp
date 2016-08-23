#include "ofMaterial.h"
#include "ofConstants.h"
#include "ofGLUtils.h"
#include "ofLight.h"
#include "ofGLProgrammableRenderer.h"
#include "ofAppRunner.h"

std::map<ofGLProgrammableRenderer*, std::map<std::string, std::weak_ptr<ofMaterial::Shaders>>> ofMaterial::shadersMap;

namespace{
string vertexSource(string defaultHeader, int maxLights, bool hasTexture, bool hasColor);
string fragmentSource(string defaultHeader, string postFragment, int maxLights, bool hasTexture, bool hasColor);
}

ofMaterial::Data::Data()
:diffuse(0.8f, 0.8f, 0.8f, 1.0f)
,ambient(0.2f, 0.2f, 0.2f, 1.0f)
,specular(0.0f, 0.0f, 0.0f, 1.0f)
,emissive(0.0f, 0.0f, 0.0f, 1.0f)
,shininess(0.2f){

}

ofMaterial::ofMaterial() {
}

void ofMaterial::setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor oEmissive) {
	setDiffuseColor(oDiffuse);
	setAmbientColor(oAmbient);
	setSpecularColor(oSpecular);
	setEmissiveColor(oEmissive);
}

void ofMaterial::setDiffuseColor(ofFloatColor oDiffuse) {
	data.diffuse = oDiffuse;
}

void ofMaterial::setAmbientColor(ofFloatColor oAmbient) {
	data.ambient = oAmbient;
}

void ofMaterial::setSpecularColor(ofFloatColor oSpecular) {
	data.specular = oSpecular;
}

void ofMaterial::setEmissiveColor(ofFloatColor oEmissive) {
	data.emissive = oEmissive;
}

void ofMaterial::setShininess(float nShininess) {
	data.shininess = nShininess;
}

void ofMaterial::setData(const ofMaterial::Data &data){
    this->data = data;
}

float ofMaterial::getShininess()const{
	return data.shininess;
}

ofFloatColor ofMaterial::getDiffuseColor()const {
	return data.diffuse;
}

ofFloatColor ofMaterial::getAmbientColor()const {
	return data.ambient;
}

ofFloatColor ofMaterial::getSpecularColor()const {
	return data.specular;
}

ofFloatColor ofMaterial::getEmissiveColor()const {
	return data.emissive;
}

ofMaterial::Data ofMaterial::getData()const{
    return data;
}

void ofMaterial::begin() const{
	if(ofGetGLRenderer()){
		ofGetGLRenderer()->bind(*this);
	}
}

void ofMaterial::end() const{
	if(ofGetGLRenderer()){
		ofGetGLRenderer()->unbind(*this);
	}
}

void ofMaterial::initShaders(ofGLProgrammableRenderer & renderer) const{
    auto rendererShaders = shaders.find(&renderer);
    if(rendererShaders == shaders.end() || rendererShaders->second->numLights != ofLightsData().size()){
        if(shadersMap[&renderer].find(postFragment)!=shadersMap[&renderer].end()){
            auto newShaders = shadersMap[&renderer][postFragment].lock();
            if(newShaders == nullptr || newShaders->numLights != ofLightsData().size()){
                shadersMap[&renderer].erase(postFragment);
                shaders[&renderer] = nullptr;
            }else{
                shaders[&renderer] = newShaders;
            }
        }
    }

    if(shaders[&renderer] == nullptr){
        #ifndef TARGET_OPENGLES
            string vertexRectHeader = renderer.defaultVertexShaderHeader(GL_TEXTURE_RECTANGLE);
            string fragmentRectHeader = renderer.defaultFragmentShaderHeader(GL_TEXTURE_RECTANGLE);
        #endif
        string vertex2DHeader = renderer.defaultVertexShaderHeader(GL_TEXTURE_2D);
        string fragment2DHeader = renderer.defaultFragmentShaderHeader(GL_TEXTURE_2D);
        auto numLights = ofLightsData().size();
        shaders[&renderer].reset(new Shaders);
        shaders[&renderer]->numLights = numLights;
        shaders[&renderer]->noTexture.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,numLights,false,false));
        shaders[&renderer]->noTexture.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader,postFragment,numLights,false,false));
        shaders[&renderer]->noTexture.bindDefaults();
        shaders[&renderer]->noTexture.linkProgram();

        shaders[&renderer]->texture2D.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,numLights,true,false));
        shaders[&renderer]->texture2D.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader,postFragment,numLights,true,false));
        shaders[&renderer]->texture2D.bindDefaults();
        shaders[&renderer]->texture2D.linkProgram();

        #ifndef TARGET_OPENGLES
            shaders[&renderer]->textureRect.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertexRectHeader,numLights,true,false));
            shaders[&renderer]->textureRect.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragmentRectHeader,postFragment,numLights,true,false));
            shaders[&renderer]->textureRect.bindDefaults();
            shaders[&renderer]->textureRect.linkProgram();
        #endif

        shaders[&renderer]->color.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,numLights,false,true));
        shaders[&renderer]->color.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader,postFragment,numLights,false,true));
        shaders[&renderer]->color.bindDefaults();
        shaders[&renderer]->color.linkProgram();


        shaders[&renderer]->texture2DColor.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,numLights,true,true));
        shaders[&renderer]->texture2DColor.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader,postFragment,numLights,true,true));
        shaders[&renderer]->texture2DColor.bindDefaults();
        shaders[&renderer]->texture2DColor.linkProgram();

        #ifndef TARGET_OPENGLES
            shaders[&renderer]->textureRectColor.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertexRectHeader,numLights,true,true));
            shaders[&renderer]->textureRectColor.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragmentRectHeader,postFragment,numLights,true,true));
            shaders[&renderer]->textureRectColor.bindDefaults();
            shaders[&renderer]->textureRectColor.linkProgram();
        #endif

        shadersMap[&renderer][postFragment] = shaders[&renderer];
    }

}

const ofShader & ofMaterial::getShader(int textureTarget, bool geometryHasColor, ofGLProgrammableRenderer & renderer) const{
    initShaders(renderer);
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
	shader.setUniform4fv("mat_ambient", &data.ambient.r);
	shader.setUniform4fv("mat_diffuse", &data.diffuse.r);
	shader.setUniform4fv("mat_specular", &data.specular.r);
	shader.setUniform4fv("mat_emissive", &data.emissive.r);
	shader.setUniform4fv("global_ambient", &ofGetGlobalAmbientColor().r);
	shader.setUniform1f("mat_shininess",data.shininess);
}

void ofMaterial::updateLights(const ofShader & shader,ofGLProgrammableRenderer & renderer) const{
	for(size_t i=0;i<ofLightsData().size();i++){
		string idx = ofToString(i);
		shared_ptr<ofLight::Data> light = ofLightsData()[i].lock();
		if(!light || !light->isEnabled){
			shader.setUniform1f("lights["+idx+"].enabled",0);
			continue;
		}
		auto lightEyePosition = renderer.getCurrentViewMatrix() * light->position;
		shader.setUniform1f("lights["+idx+"].enabled",1);
		shader.setUniform1f("lights["+idx+"].type", light->lightType);
		shader.setUniform4f("lights["+idx+"].position", lightEyePosition);
		shader.setUniform4f("lights["+idx+"].ambient", light->ambientColor);
		shader.setUniform4f("lights["+idx+"].specular", light->specularColor);
		shader.setUniform4f("lights["+idx+"].diffuse", light->diffuseColor);

		if(light->lightType!=OF_LIGHT_DIRECTIONAL){
			shader.setUniform1f("lights["+idx+"].constantAttenuation", light->attenuation_constant);
			shader.setUniform1f("lights["+idx+"].linearAttenuation", light->attenuation_linear);
			shader.setUniform1f("lights["+idx+"].quadraticAttenuation", light->attenuation_quadratic);
		}

		if(light->lightType==OF_LIGHT_SPOT){
			auto direction = toGlm(light->position).xyz() + light->direction;
			auto direction4 = renderer.getCurrentViewMatrix() * glm::vec4(direction,1.0);
			direction = direction4.xyz() / direction4.w;
			direction = direction - lightEyePosition.xyz();
			shader.setUniform3f("lights["+idx+"].spotDirection", glm::normalize(direction));
			shader.setUniform1f("lights["+idx+"].spotExponent", light->exponent);
			shader.setUniform1f("lights["+idx+"].spotCutoff", light->spotCutOff);
			shader.setUniform1f("lights["+idx+"].spotCosCutoff", cos(ofDegToRad(light->spotCutOff)));
		}else if(light->lightType==OF_LIGHT_DIRECTIONAL){
			auto halfVector = glm::normalize(glm::vec4(0.f, 0.f, 1.f, 0.f) + lightEyePosition);
			shader.setUniform3f("lights["+idx+"].halfVector", halfVector.xyz());
		}else if(light->lightType==OF_LIGHT_AREA){
			shader.setUniform1f("lights["+idx+"].width", light->width);
			shader.setUniform1f("lights["+idx+"].height", light->height);
			auto direction = light->position.xyz() + light->direction;
			auto direction4 = renderer.getCurrentViewMatrix() * glm::vec4(direction, 1.0);
			direction = direction4.xyz() / direction4.w;
			direction = direction - lightEyePosition.xyz();
			shader.setUniform3f("lights["+idx+"].spotDirection", glm::normalize(direction));
			auto right = toGlm(light->position).xyz() + light->right;
			auto right4 = renderer.getCurrentViewMatrix() * glm::vec4(right, 1.0);
			right = right4.xyz() / right4.w;
			right = right - lightEyePosition.xyz();
			auto up = glm::cross(toGlm(right), direction);
			shader.setUniform3f("lights["+idx+"].right", glm::normalize(toGlm(right)));
			shader.setUniform3f("lights["+idx+"].up", glm::normalize(up));
		}
	}
}

void ofMaterial::setPostFragment(const std::string & source){
    if(postFragment != source){
        shaders.clear();
        postFragment = source;
    }
}

#include "shaders/phong.vert"
#include "shaders/phong.frag"

namespace{
    string shaderHeader(string header, int maxLights, bool hasTexture, bool hasColor){
        header += "#define MAX_LIGHTS " + ofToString(max(1,maxLights)) + "\n";
        if(hasTexture){
            header += "#define HAS_TEXTURE 1\n";
        }
        if(hasColor){
            header += "#define HAS_COLOR 1\n";
        }
        return header;
    }

    string vertexSource(string defaultHeader, int maxLights, bool hasTexture, bool hasColor){
        return shaderHeader(defaultHeader, maxLights, hasTexture, hasColor) + vertexShader;
    }

    string fragmentSource(string defaultHeader, string postFragment, int maxLights, bool hasTexture, bool hasColor){
        auto source = fragmentShader;
        auto pos = source.find("%postFragment%");
        auto len = std::string("%postFragment%").length();
        if(postFragment.empty()){
            postFragment = "vec4 postFragment(vec4 localColor){ return localColor; }";
        }
        source = source.replace(pos, len, postFragment);
        source = shaderHeader(defaultHeader, maxLights, hasTexture, hasColor) + source;
        return source;
    }
}
