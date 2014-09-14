#include "ofMaterial.h"
#include "ofConstants.h"
#include "ofGLUtils.h"
#include "ofLight.h"
#include "ofGLProgrammableRenderer.h"
#include "ofAppRunner.h"

ofShader ofMaterial::shaderNoTexture;
ofShader ofMaterial::shaderTexture2D;
ofShader ofMaterial::shaderTextureRect;
bool ofMaterial::shadersInitialized = false;
size_t ofMaterial::shaderLights = 0;

ofMaterial::ofMaterial() {
    data.diffuse.set(0.8f, 0.8f, 0.8f, 1.0f);
    data.specular.set(0.0f, 0.0f, 0.0f, 1.0f);
    data.ambient.set(0.2f, 0.2f, 0.2f, 1.0f);
    data.emissive.set(0.0f, 0.0f, 0.0f, 1.0f);
    data.shininess = 0.2f;
    prev_data.shininess = 0.2f;
    prev_data_back.shininess = 0.2f;
    currentShader = NULL;
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

void ofMaterial::begin() {
#ifndef TARGET_PROGRAMMABLE_GL
	if(!ofIsGLProgrammableRenderer()){
	#ifndef TARGET_OPENGLES
		// save previous values, opengl es cannot use push/pop attrib
		glGetMaterialfv(GL_FRONT,GL_DIFFUSE,&prev_data.diffuse.r);
		glGetMaterialfv(GL_FRONT,GL_SPECULAR,&prev_data.specular.r);
		glGetMaterialfv(GL_FRONT,GL_AMBIENT,&prev_data.ambient.r);
		glGetMaterialfv(GL_FRONT,GL_EMISSION,&prev_data.emissive.r);
		glGetMaterialfv(GL_FRONT, GL_SHININESS, &prev_data.shininess);

		glGetMaterialfv(GL_BACK,GL_DIFFUSE,&prev_data_back.diffuse.r);
		glGetMaterialfv(GL_BACK,GL_SPECULAR,&prev_data_back.specular.r);
		glGetMaterialfv(GL_BACK,GL_AMBIENT,&prev_data_back.ambient.r);
		glGetMaterialfv(GL_BACK,GL_EMISSION,&prev_data_back.emissive.r);
		glGetMaterialfv(GL_BACK, GL_SHININESS, &prev_data_back.shininess);

		// Material colors and properties
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &data.diffuse.r);
		glMaterialfv(GL_FRONT, GL_SPECULAR, &data.specular.r);
		glMaterialfv(GL_FRONT, GL_AMBIENT, &data.ambient.r);
		glMaterialfv(GL_FRONT, GL_EMISSION, &data.emissive.r);
		glMaterialfv(GL_FRONT, GL_SHININESS, &data.shininess);

		glMaterialfv(GL_BACK, GL_DIFFUSE, &data.diffuse.r);
		glMaterialfv(GL_BACK, GL_SPECULAR, &data.specular.r);
		glMaterialfv(GL_BACK, GL_AMBIENT, &data.ambient.r);
		glMaterialfv(GL_BACK, GL_EMISSION, &data.emissive.r);
		glMaterialfv(GL_BACK, GL_SHININESS, &data.shininess);
	#elif !defined(TARGET_PROGRAMMABLE_GL)
		// opengl es 1.1 implementation must use GL_FRONT_AND_BACK.

		glGetMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &prev_data.diffuse.r);
		glGetMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &prev_data.specular.r);
		glGetMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &prev_data.ambient.r);
		glGetMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &prev_data.emissive.r);
		glGetMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &prev_data.shininess);

		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &data.diffuse.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &data.specular.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &data.ambient.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &data.emissive.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &data.shininess);
	#endif
	}
#endif
	if(ofIsGLProgrammableRenderer()){
		ofGetGLProgrammableRenderer()->setCurrentMaterial(this);
	}
}

void ofMaterial::end() {
#ifndef TARGET_PROGRAMMABLE_GL
	if(!ofIsGLProgrammableRenderer()){
	#ifndef TARGET_OPENGLES
		// Set previous material colors and properties
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &prev_data.diffuse.r);
		glMaterialfv(GL_FRONT, GL_SPECULAR, &prev_data.specular.r);
		glMaterialfv(GL_FRONT, GL_AMBIENT, &prev_data.ambient.r);
		glMaterialfv(GL_FRONT, GL_EMISSION, &prev_data.emissive.r);
		glMaterialfv(GL_FRONT, GL_SHININESS, &prev_data.shininess);

		glMaterialfv(GL_BACK, GL_DIFFUSE, &prev_data_back.diffuse.r);
		glMaterialfv(GL_BACK, GL_SPECULAR, &prev_data_back.specular.r);
		glMaterialfv(GL_BACK, GL_AMBIENT, &prev_data_back.ambient.r);
		glMaterialfv(GL_BACK, GL_EMISSION, &prev_data_back.emissive.r);
		glMaterialfv(GL_BACK, GL_SHININESS, &prev_data_back.shininess);
	#else
		// opengl es 1.1 implementation must use GL_FRONT_AND_BACK.

		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &prev_data.diffuse.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &prev_data.specular.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &prev_data.ambient.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &prev_data.emissive.r);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &prev_data.shininess);
	#endif
	}
#endif
	if(ofIsGLProgrammableRenderer()){
		ofGetGLProgrammableRenderer()->setCurrentMaterial(NULL);
		if(currentShader) currentShader->end();
		currentShader = NULL;
	}
}

void ofMaterial::initShaders(){
	if(!shadersInitialized || ofLightsData().size()!=shaderLights){
		shaderLights = ofLightsData().size();
		shaderNoTexture.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(shaderLights,false,false));
		shaderNoTexture.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(shaderLights,false,false));
		shaderNoTexture.bindDefaults();
		shaderNoTexture.linkProgram();

		shaderTexture2D.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(shaderLights,true,false));
		shaderTexture2D.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(shaderLights,true,false));
		shaderTexture2D.bindDefaults();
		shaderTexture2D.linkProgram();

#ifndef TARGET_OPENGLES
		shaderTextureRect.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(shaderLights,true,true));
		shaderTextureRect.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(shaderLights,true,true));
		shaderTextureRect.bindDefaults();
		shaderTextureRect.linkProgram();
#endif

		shadersInitialized = true;
	}
}

void ofMaterial::beginShader(int texType){
	initShaders();
	switch(texType){
	case OF_NO_TEXTURE:
		currentShader = &shaderNoTexture;
		break;
	case GL_TEXTURE_2D:
		currentShader = &shaderTexture2D;
		break;
	default:
		currentShader = &shaderTextureRect;
		break;
	}

	const ofMatrix4x4 & normalMatrix = ofGetCurrentNormalMatrix();
	currentShader->begin();
	currentShader->setUniformMatrix4f("normalMatrix",normalMatrix);
	currentShader->setUniform4fv("mat_ambient", &data.ambient.r);
	currentShader->setUniform4fv("mat_diffuse", &data.diffuse.r);
	currentShader->setUniform4fv("mat_specular", &data.specular.r);
	currentShader->setUniform4fv("mat_emissive", &data.emissive.r);
	currentShader->setUniform4fv("global_ambient", &ofGetGlobalAmbientColor().r);
	currentShader->setUniform1f("mat_shininess",data.shininess);

	for(size_t i=0;i<ofLightsData().size();i++){
		string idx = ofToString(i);
		if(ofLightsData()[i].expired() || !ofLightsData()[i].lock()->isEnabled){
			currentShader->setUniform1f("lights["+idx+"].enabled",0);
			continue;
		}

		shared_ptr<ofLight::Data> light = ofLightsData()[i].lock();
		ofVec4f lightEyePosition = light->position * ofGetCurrentViewMatrix();
		currentShader->setUniform1f("lights["+idx+"].enabled",1);
		currentShader->setUniform1f("lights["+idx+"].type", light->lightType);
		currentShader->setUniform4fv("lights["+idx+"].position", &lightEyePosition.x);
		currentShader->setUniform4fv("lights["+idx+"].ambient", &light->ambientColor.r);
		currentShader->setUniform4fv("lights["+idx+"].specular", &light->specularColor.r);
		currentShader->setUniform4fv("lights["+idx+"].diffuse", &light->diffuseColor.r);

		if(light->lightType==OF_LIGHT_POINT || light->lightType==OF_LIGHT_AREA){
			currentShader->setUniform1f("lights["+idx+"].constantAttenuation", light->attenuation_constant);
			currentShader->setUniform1f("lights["+idx+"].linearAttenuation", light->attenuation_linear);
			currentShader->setUniform1f("lights["+idx+"].quadraticAttenuation", light->attenuation_quadratic);
		}

		if(light->lightType==OF_LIGHT_SPOT){
			ofVec3f direction = light->position + light->direction;
			direction = direction * ofGetCurrentViewMatrix();
			direction = direction - lightEyePosition;
			currentShader->setUniform3fv("lights["+idx+"].spotDirection", &direction.x);
			currentShader->setUniform1f("lights["+idx+"].spotExponent", light->exponent);
			currentShader->setUniform1f("lights["+idx+"].spotCutoff", light->spotCutOff);
			currentShader->setUniform1f("lights["+idx+"].spotCosCutoff", cos(ofDegToRad(light->spotCutOff)));
		}else if(light->lightType==OF_LIGHT_DIRECTIONAL){
			ofVec3f halfVector = (ofVec3f(0,0,1) + lightEyePosition).getNormalized();
			currentShader->setUniform3fv("lights["+idx+"].halfVector", &halfVector.x);
		}else if(light->lightType==OF_LIGHT_AREA){
			currentShader->setUniform1f("lights["+idx+"].width", light->width);
			currentShader->setUniform1f("lights["+idx+"].height", light->height);
			ofVec3f direction = light->position + light->direction;
			direction = direction * ofGetCurrentViewMatrix();
			direction = direction - lightEyePosition;
			ofVec3f right = light->position + light->right;
			right = right * ofGetCurrentViewMatrix();
			right = right - lightEyePosition;
			ofVec3f up = right.getCrossed(direction);
			currentShader->setUniform3fv("lights["+idx+"].spotDirection", &direction.x);
			currentShader->setUniform3fv("lights["+idx+"].right", &right.x);
			currentShader->setUniform3fv("lights["+idx+"].up", &up.x);
		}
	}
}

#define STRINGIFY(x) #x

#ifdef TARGET_OPENGLES
static string vertex_shader_header =
		"precision mediump float;\n"
		"#define IN attribute\n"
		"#define OUT varying\n"
		"#define TEXTURE texture2D\n"
		"#define TARGET_OPENGLES\n"
		"#define MAX_LIGHTS %max_lights%\n";
static string fragment_shader_header =
		"precision mediump float;\n"
		"#define IN varying\n"
		"#define OUT\n"
		"#define TEXTURE texture2D\n"
		"#define FRAG_COLOR gl_FragColor\n"
		"#define TARGET_OPENGLES\n"
		"#define MAX_LIGHTS %max_lights%\n";
#else
static string vertex_shader_header =
		"#version %glsl_version%\n"
		"%extensions%\n"
		"#define IN in\n"
		"#define OUT out\n"
		"#define TEXTURE texture\n"
		"#define MAX_LIGHTS %max_lights%\n";
static string fragment_shader_header =
		"#version %glsl_version%\n"
		"%extensions%\n"
		"#define IN in\n"
		"#define OUT out\n"
		"#define TEXTURE texture\n"
		"#define FRAG_COLOR fragColor\n"
		"out vec4 fragColor;\n"
		"#define MAX_LIGHTS %max_lights%\n";
#endif

string ofMaterial::vertexShader = STRINGIFY(
	OUT vec4 outColor; // this is the ultimate color for this vertex
	OUT vec2 outtexcoord; // pass the texCoord if needed
	OUT vec3 transformedNormal;
	OUT vec3 eyePosition3;

	IN vec4 position;
	IN vec4 color;
	IN vec4 normal;
	IN vec2 texcoord;

	// these are passed in from OF programmable renderer
	uniform mat4 modelViewMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 textureMatrix;
	uniform mat4 modelViewProjectionMatrix;
	uniform mat4 normalMatrix;


	void main (void){
		float alphaFade = 1.0;
		vec4 eyePosition = modelViewMatrix * position;
		vec3 tempNormal = (normalMatrix * normal).xyz;
		transformedNormal = normalize(tempNormal);
		eyePosition3 = (eyePosition.xyz) / eyePosition.w;

		outtexcoord = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy;
		gl_Position = modelViewProjectionMatrix * position;
	}
);


string ofMaterial::fragmentShader = STRINGIFY(
	IN vec4 outColor; // this is the ultimate color for this vertex
	IN vec2 outtexcoord; // pass the texCoord if needed
	IN vec3 transformedNormal;
	// Eye-coordinate position of vertex
	IN vec4 eyePosition;
	IN vec3 eyePosition3;


	struct lightData
	{
		float enabled;
		vec4 ambient;
		float type; // 0 = pointlight 1 = directionlight
		vec4 position; // where are we
		vec4 diffuse; // how diffuse
		vec4 specular; // what kinda specular stuff we got going on?
		// attenuation
		float constantAttenuation;
		float linearAttenuation;
		float quadraticAttenuation;
		// only for spot
		float spotCutoff;
		float spotCosCutoff;
		float spotExponent;
		// spot and area
		vec3 spotDirection;
		// only for directional
		vec3 halfVector;
		// only for area
		float width;
		float height;
		vec3 right;
		vec3 up;
	};

	uniform SAMPLER tex0;

	uniform vec4 mat_ambient;
	uniform vec4 mat_diffuse;
	uniform vec4 mat_specular;
	uniform vec4 mat_emissive;
	uniform float mat_shininess;
	uniform vec4 global_ambient;

	// these are passed in from OF programmable renderer
	uniform mat4 modelViewMatrix;
	uniform mat4 projectionMatrix;
	uniform mat4 textureMatrix;
	uniform mat4 modelViewProjectionMatrix;

	uniform lightData lights[MAX_LIGHTS];


	void pointLight( in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular ){
		float nDotVP;       // normal . light direction
		float nDotHV;       // normal . light half vector
		float pf;           // power factor
		float attenuation;  // computed attenuation factor
		float d;            // distance from surface to light source
		vec3  VP;           // direction from surface to light position
		vec3  halfVector;   // direction of maximum highlights
		vec3 eye = vec3 (0.0, 0.0, 1.0);

		// Compute vector from surface to light position
		VP = vec3 (light.position.xyz) - ecPosition3;

		// Compute distance between surface and light position
		d = length(VP);


		// Compute attenuation
		attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);

		// Normalize the vector from surface to light position
		VP = normalize(VP);
		halfVector = normalize(VP + eye);

		nDotVP = max(0.0, dot(normal, VP));
		nDotHV = max(0.0, dot(normal, halfVector));

		// ha! no branching :)
		// fresnel factor, produces artifacts on spot lights
		// http://en.wikibooks.org/wiki/GLSL_Programming/Unity/Specular_Highlights_at_Silhouettes
		/*float w = pow(1.0 - max(0.0, dot(halfVector, VP)), 5.0);
		vec3 specularReflection = attenuation * vec3(light.specular.rgb)
		  * mix(vec3(mat_specular.rgb), vec3(1.0), w)
		  * pow(nDotHV, mat_shininess);
		specular += mix(vec3(0.0), specularReflection, step(0.0000001, nDotVP));*/
		pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));

		ambient += light.ambient.rgb * attenuation;
		diffuse += light.diffuse.rgb * nDotVP * attenuation;
		specular += light.specular.rgb * pf * nDotVP * attenuation;
	}

	void directionalLight(in lightData light, in vec3 normal, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
		float nDotVP;         // normal . light direction
		float nDotHV;         // normal . light half vector
		float pf;             // power factor

		nDotVP = max(0.0, dot(normal, normalize(vec3(light.position))));
		nDotHV = max(0.0, dot(normal, light.halfVector));

		pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));

		ambient += light.ambient.rgb;
		diffuse += light.diffuse.rgb * nDotVP;
		specular += light.specular.rgb * pf * nDotVP;
	}

	void spotLight(in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
		float nDotVP;// = max(dot(normal,normalize(vec3(light.position))),0.0);
		float nDotHV;       // normal . light half vector
		float pf;
		float d;            // distance from surface to light source
		vec3  VP;           // direction from surface to light position
		vec3 eye = vec3 (0.0, 0.0, 1.0);
		float spotEffect;
		float attenuation;
		vec3  halfVector;   // direction of maximum highlights
		// Compute vector from surface to light position
		VP = vec3 (light.position.xyz) - ecPosition3;
		spotEffect = dot(normalize(light.spotDirection), -normalize(VP));

		if (spotEffect > light.spotCosCutoff) {
			// Compute distance between surface and light position
			d = length(VP);
			spotEffect = pow(spotEffect, light.spotExponent);
			attenuation = spotEffect / d;

			halfVector = normalize(VP + eye);
			nDotHV = max(0.0, dot(normal, halfVector));
			nDotVP = max(0.0, dot(normal, VP));

			pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));

			diffuse += light.diffuse.rgb * nDotVP * attenuation;
			specular += light.specular.rgb * pf * nDotVP * attenuation;
		}

		ambient += light.ambient.rgb * attenuation;
	}


	vec3 projectOnPlane(in vec3 point, in vec3 planeCenter, in vec3 planeNormal){
		return point - dot( point - planeCenter, planeNormal ) * planeNormal;
	}

	vec3 linePlaneIntersect(in vec3 lp, in vec3 lv, in vec3 pc, in vec3 pn){
	   return lp+lv*(dot(pn,pc-lp)/dot(pn,lv));
	}

	void areaLight(in lightData light, in vec3 N, in vec3 V, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
		vec3 right = normalize(light.right);
		vec3 pnormal = normalize(light.spotDirection);
		vec3 up = normalize(light.up);

		//width and height of the area light:
		float width = light.width*0.5;
		float height = light.height*0.5;
		float attenuation;

		//project onto plane and calculate direction from center to the projection.
		vec3 projection = projectOnPlane(V,light.position.xyz,pnormal);// projection in plane
		vec3 dir = projection-light.position.xyz;

		//calculate distance from area:
		vec2 diagonal = vec2(dot(dir,right),dot(dir,up));
		vec2 nearest2D = vec2(clamp( diagonal.x,-width,width  ),clamp(  diagonal.y,-height,height));
		vec3 nearestPointInside = vec3(light.position.xyz)+(right*nearest2D.x+up*nearest2D.y);
		float dist = distance(V,nearestPointInside);//real distance to area rectangle

		vec3 lightDir = normalize(nearestPointInside - V);
		attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * dist + light.quadraticAttenuation * dist * dist);

		float NdotL = max( dot( pnormal, -lightDir ), 0.0 );
		float NdotL2 = max( dot( N, lightDir ), 0.0 );
		if ( NdotL * NdotL2 > 0.0 ) {
			float diffuseFactor = sqrt( NdotL * NdotL2 );
			vec3 R = reflect( normalize( -V ), N );
			vec3 E = linePlaneIntersect( V, R, light.position.xyz, pnormal );
			float specAngle = dot( R, pnormal );
			if (specAngle > 0.0){
				vec3 dirSpec = E - light.position.xyz;
				vec2 dirSpec2D = vec2(dot(dirSpec,right),dot(dirSpec,up));
				vec2 nearestSpec2D = vec2(clamp( dirSpec2D.x,-width,width  ),clamp(  dirSpec2D.y,-height,height));
				float specFactor = 1.0-clamp(length(nearestSpec2D-dirSpec2D) * 0.05 * mat_shininess,0.0,1.0);
				specular += light.specular.rgb * specFactor * specAngle * diffuseFactor * attenuation;
			}
			diffuse  += light.diffuse.rgb  * diffuseFactor * attenuation;
		}
		ambient += light.ambient.rgb * attenuation;
	}




	//////////////////////////////////////////////////////
	// here's the main method
	//////////////////////////////////////////////////////


	void main (void){

		vec3 ambient = global_ambient.rgb;
		vec3 diffuse = vec3(0.0,0.0,0.0);
		vec3 specular = vec3(0.0,0.0,0.0);

		for( int i = 0; i < MAX_LIGHTS; i++ ){
			if(lights[i].enabled<0.5) continue;
			if(lights[i].type<0.5){
				pointLight(lights[i], transformedNormal, eyePosition3, ambient, diffuse, specular);
			}else if(lights[i].type<1.5){
				directionalLight(lights[i], transformedNormal, ambient, diffuse, specular);
			}else if(lights[i].type<2.5){
				spotLight(lights[i], transformedNormal, eyePosition3, ambient, diffuse, specular);
			}else{
				areaLight(lights[i], transformedNormal, eyePosition3, ambient, diffuse, specular);
			}
		}

		////////////////////////////////////////////////////////////
		// now add the material info
		\n#ifdef HAS_TEXTURE\n
			vec4 tex = TEXTURE(tex0, outtexcoord);
			vec4 localColor = vec4(ambient,1.0) * mat_ambient + vec4(diffuse,1.0) * tex + vec4(specular,1.0) * mat_specular + mat_emissive;
		\n#else\n
			vec4 localColor = vec4(ambient,1.0) * mat_ambient + vec4(diffuse,1.0) * mat_diffuse + vec4(specular,1.0) * mat_specular + mat_emissive;
		\n#endif\n
		FRAG_COLOR = clamp( localColor, 0.0, 1.0 );
	}
);


static string shaderHeader(string header, const string & glslVersion, int maxLights, bool hasTexture, bool textureRect){
	ofStringReplace(header,"%glsl_version%",glslVersion);
#ifndef TARGET_OPENGLES
	if(ofGetOpenGLVersionMajor()<4 && ofGetOpenGLVersionMinor()<2){
		ofStringReplace(header,"%extensions%","#extension GL_ARB_texture_rectangle : enable");
	}else{
		ofStringReplace(header,"%extensions%","");
	}
#endif
	ofStringReplace(header,"%max_lights%",ofToString(maxLights));
	if(hasTexture){
		header += "#define HAS_TEXTURE\n";
	}
	if(textureRect){
		header += "#define SAMPLER sampler2DRect\n";
	}else{
		header += "#define SAMPLER sampler2D\n";
	}
	return header;
}

string ofMaterial::vertexSource(int maxLights, bool hasTexture, bool textureRect){
	return shaderHeader(vertex_shader_header,ofGetGLSLVersion(),maxLights,hasTexture,textureRect) + vertexShader;
}

string ofMaterial::fragmentSource(int maxLights, bool hasTexture, bool textureRect){
	return shaderHeader(fragment_shader_header,ofGetGLSLVersion(),maxLights,hasTexture,textureRect) + fragmentShader;
}
