#include "ofxShader.h"

ofxShader::ofxShader() :
bLoaded(false) {
}

ofxShader::~ofxShader() {
	unload();
}

void ofxShader::setup(string shaderName) {
	setup(shaderName + ".vert", shaderName + ".frag");
}

void ofxShader::setup(string vertexName, string fragmentName) {
	ofBuffer vertShaderBuffer, fragShaderBuffer;
	ofReadFile(vertexName, vertShaderBuffer);
	ofReadFile(fragmentName, fragShaderBuffer);
	setupInline(vertShaderBuffer.getBuffer(), fragShaderBuffer.getBuffer());
}

void ofxShader::compileShader(GLuint shader, string source, string type) {
	const char* sptr = source.c_str();
	int ssize = source.size();
	glShaderSource(shader, 1, &sptr, &ssize);
	glCompileShader(shader);
}

bool ofxShader::checkShaderCompileStatus(GLuint shader, string type) {
	GLint status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_TRUE)
		ofLog(OF_LOG_VERBOSE, type + " shader compiled.");
	else if (status == GL_FALSE) {
		ofLog(OF_LOG_ERROR, type + " shader failed to compile.");
		checkShaderInfoLog(shader, type);
		return false;
	}
	return true;
}

bool ofxShader::checkShaderLinkStatus(GLuint shader, string type) {
	GLint status;
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	if(status == GL_TRUE)
		ofLog(OF_LOG_VERBOSE, type + " shader linked.");
	else if (status == GL_FALSE) {
		ofLog(OF_LOG_ERROR, type + " shader failed to link.");
		checkShaderInfoLog(shader, type);
		return false;
	}
	return true;
}

void ofxShader::checkShaderInfoLog(GLuint shader, string type) {
	GLsizei infoLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		GLchar* infoBuffer = new GLchar[infoLength];
		glGetShaderInfoLog(shader, infoLength, &infoLength, infoBuffer);
		ofLog(OF_LOG_ERROR, type + " shader reports:\n" + infoBuffer);
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

void ofxShader::setupInline(string vertexShaderSource, string fragmentShaderSource) {
	unload();
	if (GL_ARB_shader_objects) {
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		compileShader(vertexShader, vertexShaderSource, "Vertex");
		bool vertexCompiled = checkShaderCompileStatus(vertexShader, "Vertex");
		
		compileShader(fragmentShader, fragmentShaderSource, "Fragment");
		bool fragmentCompiled = checkShaderCompileStatus(fragmentShader, "Fragment");
		
		if(vertexCompiled && fragmentCompiled) {
			program = glCreateProgram();
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
			glLinkProgram(program);
			
			checkShaderLinkStatus(vertexShader, "Vertex");
			checkShaderLinkStatus(fragmentShader, "Fragment");
			checkProgramInfoLog(program);
			
			bLoaded = true;
		}
	} else {
		cout << "Sorry, it looks like you can't run 'ARB_shader_objects'." << endl;
		cout << "Please check the capabilites of your graphics card." << endl;
		cout << "http://www.ozone3d.net/gpu_caps_viewer/" << endl;
	}
}

void ofxShader::unload() {
	if(bLoaded) {
		if (vertexShader) {
			glDetachShader(program, vertexShader);
			glDeleteShader(vertexShader);
			vertexShader = 0;
		}
		if (fragmentShader) {
			glDetachShader(program, fragmentShader);
			glDeleteShader(fragmentShader);
			fragmentShader = 0;
		}
		if (program) {
			glDeleteProgram(program);
			program = 0;
		}
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
