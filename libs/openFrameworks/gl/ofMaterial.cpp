#include "ofMaterial.h"
#include "ofConstants.h"
#include "ofLight.h"
#include "ofGLProgrammableRenderer.h"

using namespace std;

std::map<ofGLProgrammableRenderer*, std::map<std::string, std::weak_ptr<ofMaterial::Shaders>>> ofMaterial::shadersMap;

namespace{
string vertexSource(string defaultHeader, int maxLights, bool hasTexture, bool hasColor);
string fragmentSource(string defaultHeader, string customUniforms, string postFragment, int maxLights, bool hasTexture, bool hasColor);
}


ofMaterial::ofMaterial() {
}

void ofMaterial::setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor oEmissive) {
	setDiffuseColor(oDiffuse);
	setAmbientColor(oAmbient);
	setSpecularColor(oSpecular);
	setEmissiveColor(oEmissive);
}


void ofMaterial::setup(const ofMaterialSettings & settings){
	if(settings.customUniforms != data.customUniforms || settings.postFragment != data.postFragment){
		shaders.clear();
		uniforms1f.clear();
		uniforms2f.clear();
		uniforms3f.clear();
		uniforms4f.clear();
		uniforms1i.clear();
		uniforms2i.clear();
		uniforms3i.clear();
		uniforms4i.clear();
	}
	data = settings;
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
	setup(data);
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

ofMaterialSettings ofMaterial::getSettings() const{
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
        if(shadersMap[&renderer].find(data.postFragment)!=shadersMap[&renderer].end()){
            auto newShaders = shadersMap[&renderer][data.postFragment].lock();
            if(newShaders == nullptr || newShaders->numLights != ofLightsData().size()){
                shadersMap[&renderer].erase(data.postFragment);
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
        shaders[&renderer]->noTexture.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader, data.customUniforms, data.postFragment,numLights,false,false));
        shaders[&renderer]->noTexture.bindDefaults();
        shaders[&renderer]->noTexture.linkProgram();

        shaders[&renderer]->texture2D.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,numLights,true,false));
        shaders[&renderer]->texture2D.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader, data.customUniforms, data.postFragment,numLights,true,false));
        shaders[&renderer]->texture2D.bindDefaults();
        shaders[&renderer]->texture2D.linkProgram();

        #ifndef TARGET_OPENGLES
            shaders[&renderer]->textureRect.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertexRectHeader,numLights,true,false));
            shaders[&renderer]->textureRect.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragmentRectHeader, data.customUniforms, data.postFragment,numLights,true,false));
            shaders[&renderer]->textureRect.bindDefaults();
            shaders[&renderer]->textureRect.linkProgram();
        #endif

        shaders[&renderer]->color.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,numLights,false,true));
        shaders[&renderer]->color.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader, data.customUniforms, data.postFragment,numLights,false,true));
        shaders[&renderer]->color.bindDefaults();
        shaders[&renderer]->color.linkProgram();


        shaders[&renderer]->texture2DColor.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,numLights,true,true));
        shaders[&renderer]->texture2DColor.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader, data.customUniforms, data.postFragment,numLights,true,true));
        shaders[&renderer]->texture2DColor.bindDefaults();
        shaders[&renderer]->texture2DColor.linkProgram();

        #ifndef TARGET_OPENGLES
            shaders[&renderer]->textureRectColor.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertexRectHeader,numLights,true,true));
            shaders[&renderer]->textureRectColor.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragmentRectHeader, data.customUniforms, data.postFragment,numLights,true,true));
            shaders[&renderer]->textureRectColor.bindDefaults();
            shaders[&renderer]->textureRectColor.linkProgram();
        #endif

        shadersMap[&renderer][data.postFragment] = shaders[&renderer];
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
			glm::vec3 direction = glm::vec3(light->position) + light->direction;
			glm::vec4 direction4 = renderer.getCurrentViewMatrix() * glm::vec4(direction,1.0);
			direction = glm::vec3(direction4) / direction4.w;
			direction = direction - glm::vec3(lightEyePosition);
			shader.setUniform3f("lights["+idx+"].spotDirection", glm::normalize(direction));
			shader.setUniform1f("lights["+idx+"].spotExponent", light->exponent);
			shader.setUniform1f("lights["+idx+"].spotCutoff", light->spotCutOff);
			shader.setUniform1f("lights["+idx+"].spotCosCutoff", cos(ofDegToRad(light->spotCutOff)));
		}else if(light->lightType==OF_LIGHT_DIRECTIONAL){
			glm::vec3 halfVector(glm::normalize(glm::vec4(0.f, 0.f, 1.f, 0.f) + lightEyePosition));
			shader.setUniform3f("lights["+idx+"].halfVector", halfVector);
		}else if(light->lightType==OF_LIGHT_AREA){
			shader.setUniform1f("lights["+idx+"].width", light->width);
			shader.setUniform1f("lights["+idx+"].height", light->height);
			glm::vec3 direction = glm::vec3(light->position) + light->direction;
			glm::vec4 direction4 = renderer.getCurrentViewMatrix() * glm::vec4(direction, 1.0);
			direction = glm::vec3(direction4) / direction4.w;
			direction = direction - glm::vec3(lightEyePosition);
			shader.setUniform3f("lights["+idx+"].spotDirection", glm::normalize(direction));
			glm::vec3 right = glm::vec3(light->position) + light->right;
			glm::vec4 right4 = renderer.getCurrentViewMatrix() * glm::vec4(right, 1.0);
			right = glm::vec3(right4) / right4.w;
			right = right - glm::vec3(lightEyePosition);
			auto up = glm::cross(right, direction);
			shader.setUniform3f("lights["+idx+"].right", glm::normalize(toGlm(right)));
			shader.setUniform3f("lights["+idx+"].up", glm::normalize(up));
		}
	}
}

void ofMaterial::setCustomUniform1f(const std::string & name, float value){
	uniforms1f[name] = value;
}

void ofMaterial::setCustomUniform2f(const std::string & name, glm::vec2 value){
	uniforms2f[name] = value;
}

void ofMaterial::setCustomUniform3f(const std::string & name, glm::vec3 value) {
	uniforms3f[name] = value;
}

void ofMaterial::setCustomUniform4f(const std::string & name, glm::vec4 value) {
	uniforms4f[name] = value;
}

void ofMaterial::setCustomUniform1i(const std::string & name, int value) {
	uniforms1i[name] = value;
}

void ofMaterial::setCustomUniform2i(const std::string & name, glm::vec<2,int> value) {
	uniforms2i[name] = value;
}

void ofMaterial::setCustomUniform3i(const std::string & name, glm::vec<3, int> value) {
	uniforms3i[name] = value;
}

void ofMaterial::setCustomUniform4i(const std::string & name, glm::vec<4, int> value) {
	uniforms4i[name] = value;
}

void ofMaterial::setCustomUniformMatrix4f(const std::string & name, glm::mat4 value){
	uniforms4m[name] = value;
}

void ofMaterial::setCustomUniformMatrix3f(const std::string & name, glm::mat3 value){
	uniforms3m[name] = value;
}

void ofMaterial::setCustomUniformTexture(const std::string & name, const ofTexture & value, int textureLocation){
	uniformstex[name] = {value.getTextureData().textureTarget, int(value.getTextureData().textureID), textureLocation};
}

void ofMaterial::setCustomUniformTexture(const string & name, int textureTarget, GLint textureID, int textureLocation){
	uniformstex[name] = {textureTarget, textureID, textureLocation};
}

#include "shaders/phong.vert"
#include "shaders/phong.frag"

namespace{
    string shaderHeader(string header, int maxLights, bool hasTexture, bool hasColor){
        header += "#define MAX_LIGHTS " + ofToString(max(1,maxLights)) + "\n";
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

    string vertexSource(string defaultHeader, int maxLights, bool hasTexture, bool hasColor){
        return shaderHeader(defaultHeader, maxLights, hasTexture, hasColor) + vertexShader;
    }

    string fragmentSource(string defaultHeader, string customUniforms,  string postFragment, int maxLights, bool hasTexture, bool hasColor){
        auto source = fragmentShader;
        if(postFragment.empty()){
            postFragment = "vec4 postFragment(vec4 localColor){ return localColor; }";
        }
		ofStringReplace(source, "%postFragment%", postFragment);
		ofStringReplace(source, "%custom_uniforms%", customUniforms);

        source = shaderHeader(defaultHeader, maxLights, hasTexture, hasColor) + source;
        return source;
    }
}
