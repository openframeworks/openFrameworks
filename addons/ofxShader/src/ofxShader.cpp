#include "ofxShader.h"

ofxShader::ofxShader() :
bLoaded(false),
program(0)
{
}

ofxShader::~ofxShader() {
	unload();
}

bool ofxShader::setup(string shaderName) {
	return setup(shaderName + ".vert", shaderName + ".frag");
}

bool ofxShader::setup(string vertName, string fragName, string geomName) {
	unload();

	if(vertName.empty() == false) setupShaderFromFile(GL_VERTEX_SHADER, vertName);
	if(fragName.empty() == false) setupShaderFromFile(GL_FRAGMENT_SHADER, fragName);
	if(geomName.empty() == false) setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, geomName);
	
	return linkProgram();
}


bool ofxShader::setupShaderFromFile(GLenum type, string filename) {
	ofBuffer buffer;
	if(ofReadFile(filename, buffer)) {
		return setupShaderFromSource(type, buffer.getBuffer());
	} else {
		ofLog(OF_LOG_ERROR, "Could not load shader of type " + nameForType(type) + " from file " + filename);
		return false;
	}
}

bool ofxShader::setupShaderFromSource(GLenum type, string source) {
	// create program if it doesn't exist already
	checkAndCreateProgram();

	
	// create shader
	GLuint shader = glCreateShader(type);
	if(shader == 0) {
		ofLog(OF_LOG_ERROR, "Failed creating shader of type " + nameForType(type));
		return false;
	}
	
	// compile shader
	const char* sptr = source.c_str();
	int ssize = source.size();
	glShaderSource(shader, 1, &sptr, &ssize);
	glCompileShader(shader);
	
	// check compile status
	GLint status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_TRUE)
		ofLog(OF_LOG_VERBOSE, nameForType(type) + " shader compiled.");
	
	else if (status == GL_FALSE) {
		ofLog(OF_LOG_ERROR, nameForType(type) + " shader failed to compile");
		checkShaderInfoLog(shader, type);
		return false;
	}
	
	shaders[type] = shader;
	
	return true;
}


void ofxShader::setGeometryInputType(GLenum type) {
	checkAndCreateProgram();
	glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, type);
}

void ofxShader::setGeometryOutputType(GLenum type) {
	checkAndCreateProgram();
	glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, type);
}

void ofxShader::setGeometryOutputCount(int count) {
	checkAndCreateProgram();
	glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, count);
}

int ofxShader::getGeometryMaxOutputCount() {
	int temp;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
	return temp;
}


bool ofxShader::checkShaderLinkStatus(GLuint shader, GLenum type) {
	GLint status;
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	if(status == GL_TRUE)
		ofLog(OF_LOG_VERBOSE, nameForType(type) + " shader linked.");
	else if (status == GL_FALSE) {
		ofLog(OF_LOG_ERROR, nameForType(type) + " shader failed to link.");
		checkShaderInfoLog(shader, type);
		return false;
	}
	return true;
}

void ofxShader::checkShaderInfoLog(GLuint shader, GLenum type) {
	GLsizei infoLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		GLchar* infoBuffer = new GLchar[infoLength];
		glGetShaderInfoLog(shader, infoLength, &infoLength, infoBuffer);
		ofLog(OF_LOG_ERROR, nameForType(type) + " shader reports:\n" + infoBuffer);
		delete [] infoBuffer;
	}
}

void ofxShader::checkProgramInfoLog(GLuint program) {
	GLsizei infoLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		GLchar* infoBuffer = new GLchar[infoLength];
		glGetProgramInfoLog(program, infoLength, &infoLength, infoBuffer);
		string msg = "Shader program reports:\n";
		ofLog(OF_LOG_ERROR, msg + infoBuffer);
		delete [] infoBuffer;
	}
}

void ofxShader::checkAndCreateProgram() {
	if(GL_ARB_shader_objects) {
		if(program == 0) {
			ofLog(OF_LOG_VERBOSE, "Creating GLSL Program");
			program = glCreateProgram();
		}
	} else {
		ofLog(OF_LOG_ERROR, "Sorry, it looks like you can't run 'ARB_shader_objects'.\nPlease check the capabilites of your graphics card.\nhttp://www.ozone3d.net/gpu_caps_viewer/");
	}
}

bool ofxShader::linkProgram() {
		if(shaders.empty()) {
			ofLog(OF_LOG_ERROR, "Trying to link GLSL program, but no shaders created yet");
		} else {
			checkAndCreateProgram();
			
			for(map<GLenum, GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it){
				GLuint shader = it->second;
				if(shader) {
					ofLog(OF_LOG_VERBOSE, "Attaching shader of type " + nameForType(it->first));
					glAttachShader(program, shader);
				}
			}
			
			glLinkProgram(program);

			for(map<GLenum, GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it){
				GLuint shader = it->second;
				if(shader) {
					checkShaderLinkStatus(shader, it->first);
				}
			}
			
			checkProgramInfoLog(program);
			
			bLoaded = true;
		}
}



void ofxShader::unload() {
	if(bLoaded) {
		for(map<GLenum, GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it) {
			GLuint shader = it->second;
			if(shader) {
				ofLog(OF_LOG_VERBOSE, "Detaching and deleting shader of type " + nameForType(it->first));
				glDetachShader(program, shader);
				glDeleteShader(shader);
			}
		}

		if (program) {
			glDeleteProgram(program);
			program = 0;
		}
		
		shaders.clear();
	}
	bLoaded = false;
}

void ofxShader::begin() {
	if (bLoaded == true)
		glUseProgram(program);
}

void ofxShader::end() {
	if (bLoaded == true)
		glUseProgram(0);
}

void ofxShader::setUniformTexture(const char* name, ofBaseHasTexture& img, int textureLocation) {
	setUniformTexture(name, img.getTextureReference(), textureLocation);
}

void ofxShader::setUniformTexture(const char* name, ofTexture& tex, int textureLocation) {
	if(bLoaded) {
		ofTextureData texData = tex.getTextureData();
		glActiveTexture(GL_TEXTURE0 + textureLocation);
		glEnable(texData.textureTarget);
		glBindTexture(texData.textureTarget, texData.textureID);
		glDisable(texData.textureTarget);
		setUniform1i(name, textureLocation);
		glActiveTexture(GL_TEXTURE0);
	}
}

void ofxShader::setUniform1i(const char* name, int v1) {
	if(bLoaded)
		glUniform1i(getUniformLocation(name), v1);
}

void ofxShader::setUniform2i(const char* name, int v1, int v2) {
	if(bLoaded)
		glUniform2i(getUniformLocation(name), v1, v2);
}

void ofxShader::setUniform3i(const char* name, int v1, int v2, int v3) {
	if(bLoaded)
		glUniform3i(getUniformLocation(name), v1, v2, v3);
}

void ofxShader::setUniform4i(const char* name, int v1, int v2, int v3, int v4) {
	if(bLoaded)
		glUniform4i(getUniformLocation(name), v1, v2, v3, v4);
}

void ofxShader::setUniform1f(const char* name, float v1) {
	if(bLoaded)
		glUniform1f(getUniformLocation(name), v1);
}

void ofxShader::setUniform2f(const char* name, float v1, float v2) {
	if(bLoaded)
		glUniform2f(getUniformLocation(name), v1, v2);
}

void ofxShader::setUniform3f(const char* name, float v1, float v2, float v3) {
	if(bLoaded)
		glUniform3f(getUniformLocation(name), v1, v2, v3);
}

void ofxShader::setUniform4f(const char* name, float v1, float v2, float v3, float v4) {
	if(bLoaded)
		glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
}

void ofxShader::setUniform1iv(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform1iv(getUniformLocation(name), count, v);
}

void ofxShader::setUniform2iv(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform2iv(getUniformLocation(name), count, v);
}

void ofxShader::setUniform3iv(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform3iv(getUniformLocation(name), count, v);
}

void ofxShader::setUniform4iv(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform4iv(getUniformLocation(name), count, v);
}

void ofxShader::setUniform1fv(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform1fv(getUniformLocation(name), count, v);
}

void ofxShader::setUniform2fv(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform2fv(getUniformLocation(name), count, v);
}

void ofxShader::setUniform3fv(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform3fv(getUniformLocation(name), count, v);
}

void ofxShader::setUniform4fv(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform4fv(getUniformLocation(name), count, v);
}

void ofxShader::setAttribute1s(GLint location, short v1) {
	if(bLoaded)
		glVertexAttrib1s(location, v1);
}

void ofxShader::setAttribute2s(GLint location, short v1, short v2) {
	if(bLoaded)
		glVertexAttrib2s(location, v1, v2);
}

void ofxShader::setAttribute3s(GLint location, short v1, short v2, short v3) {
	if(bLoaded)
		glVertexAttrib3s(location, v1, v2, v3);
}

void ofxShader::setAttribute4s(GLint location, short v1, short v2, short v3, short v4) {
	if(bLoaded)
		glVertexAttrib4s(location, v1, v2, v3, v4);
}

void ofxShader::setAttribute1f(GLint location, float v1) {
	if(bLoaded)
		glVertexAttrib1f(location, v1);
}

void ofxShader::setAttribute2f(GLint location, float v1, float v2) {
	if(bLoaded)
		glVertexAttrib2f(location, v1, v2);
}

void ofxShader::setAttribute3f(GLint location, float v1, float v2, float v3) {
	if(bLoaded)
		glVertexAttrib3f(location, v1, v2, v3);
}

void ofxShader::setAttribute4f(GLint location, float v1, float v2, float v3, float v4) {
	if(bLoaded)
		glVertexAttrib4f(location, v1, v2, v3, v4);
}

void ofxShader::setAttribute1d(GLint location, double v1) {
	if(bLoaded)
		glVertexAttrib1d(location, v1);
}

void ofxShader::setAttribute2d(GLint location, double v1, double v2) {
	if(bLoaded)
		glVertexAttrib2d(location, v1, v2);
}

void ofxShader::setAttribute3d(GLint location, double v1, double v2, double v3) {
	if(bLoaded)
		glVertexAttrib3d(location, v1, v2, v3);
}

void ofxShader::setAttribute4d(GLint location, double v1, double v2, double v3, double v4) {
	if(bLoaded)
		glVertexAttrib4d(location, v1, v2, v3, v4);
}

GLint ofxShader::getAttributeLocation(const char* name) {
	return glGetAttribLocation(program, name);
}

GLint ofxShader::getUniformLocation(const char* name) {
	return glGetUniformLocation(program, name);
}

void ofxShader::printActiveUniforms() {
	GLint numUniforms = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
	cout << numUniforms << " uniforms:" << endl;
	
	GLint uniformMaxLength = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);
	
	GLint count = -1;
	GLenum type = 0;
	GLchar* uniformName = new GLchar[uniformMaxLength];
	for(GLint i = 0; i < numUniforms; i++) {
		GLsizei length;
		glGetActiveUniform(program, i, uniformMaxLength, &length, &count, &type, uniformName);
		cout << " [" << i << "] ";
		for(int j = 0; j < length; j++)
			cout << uniformName[j];
		cout << " @ index " << getUniformLocation(uniformName) << endl;
	}
	delete [] uniformName;
}

void ofxShader::printActiveAttributes() {
	GLint numAttributes = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	cout << numAttributes << " attributes:" << endl;
	
	GLint attributeMaxLength = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributeMaxLength);
	
	GLint count = -1;
	GLenum type = 0;
	GLchar* attributeName = new GLchar[attributeMaxLength];
	for(GLint i = 0; i < numAttributes; i++) {
		GLsizei length;
		glGetActiveAttrib(program, i, attributeMaxLength, &length, &count, &type, attributeName);
		cout << " [" << i << "] ";
		for(int j = 0; j < length; j++)
			cout <<attributeName[j];
		cout << " @ index " << getAttributeLocation(attributeName) << endl;
	}
	delete [] attributeName;
}

GLuint& ofxShader::getProgram() {
	return program;
}

GLuint& ofxShader::getShader(GLenum type) {
	return shaders[type];
}



string ofxShader::nameForType(GLenum type) {
	switch(type) {
		case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		case GL_GEOMETRY_SHADER_EXT: return "GL_GEOMETRY_SHADER_EXT";
		default: return "UNKNOWN SHADER TYPE";
	}
}

