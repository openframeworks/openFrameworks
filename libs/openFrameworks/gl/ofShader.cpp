#include "ofShader.h"
#include "ofUtils.h"
#include "ofFileUtils.h"
#include <map>

#ifndef TARGET_OPENGLES


static map<GLuint,int> & getShaderIds(){
	static map<GLuint,int> * ids = new map<GLuint,int>;
	return *ids;
}
static map<GLuint,int> & getProgramIds(){
	static map<GLuint,int> * ids = new map<GLuint,int>;
	return *ids;
}

//--------------------------------------------------------------
static void retainShader(GLuint id){
	if(id==0) return;
	if(getShaderIds().find(id)!=getShaderIds().end()){
		getShaderIds()[id]++;
	}else{
		getShaderIds()[id]=1;
	}
}

//--------------------------------------------------------------
static void releaseShader(GLuint program, GLuint id){
	if(getShaderIds().find(id)!=getShaderIds().end()){
		getShaderIds()[id]--;
		if(getShaderIds()[id]==0){
			glDetachShader(program, id);
			glDeleteShader(id);
			getShaderIds().erase(id);
		}
	}else{
		ofLogWarning("ofShader") << "Releasing id not found. This shouldn't be happening. Releasing anyway.";
		glDetachShader(program, id);
		glDeleteShader(id);
	}
}

//--------------------------------------------------------------
static void retainProgram(GLuint id){
	if(id==0) return;
	if(getProgramIds().find(id)!=getProgramIds().end()){
		getProgramIds()[id]++;
	}else{
		getProgramIds()[id]=1;
	}
}

//--------------------------------------------------------------
static void releaseProgram(GLuint id){
	if(getProgramIds().find(id)!=getProgramIds().end()){
		getProgramIds()[id]--;
		if(getProgramIds()[id]==0){
			glDeleteProgram(id);
			getProgramIds().erase(id);
		}
	}else{
		ofLogWarning("ofShader") << "Releasing program not found. This shouldn't be happening. Releasing anyway.";
		glDeleteProgram(id);
	}
}

//--------------------------------------------------------------
ofShader::ofShader() :
program(0),
bLoaded(false)
{
}

//--------------------------------------------------------------
ofShader::~ofShader() {
	unload();
}

//--------------------------------------------------------------
bool ofShader::load(string shaderName) {
	return load(shaderName + ".vert", shaderName + ".frag");
}

//--------------------------------------------------------------
bool ofShader::load(string vertName, string fragName, string geomName) {
	unload();

	if(vertName.empty() == false) setupShaderFromFile(GL_VERTEX_SHADER, vertName);
	if(fragName.empty() == false) setupShaderFromFile(GL_FRAGMENT_SHADER, fragName);
	if(geomName.empty() == false) setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, geomName);
	
	return linkProgram();
}

//--------------------------------------------------------------
bool ofShader::setupShaderFromFile(GLenum type, string filename) {
	ofBuffer buffer = ofBufferFromFile(filename);
	if(buffer.size()) {
		return setupShaderFromSource(type, buffer.getText());
	} else {
		ofLogError("ofShader") << "Could not load shader of type " << nameForType(type) << " from file " << filename << ".";
		return false;
	}
}

//--------------------------------------------------------------
bool ofShader::setupShaderFromSource(GLenum type, string source) {
	// create program if it doesn't exist already
	checkAndCreateProgram();

	
	// create shader
	GLuint shader = glCreateShader(type);
	if(shader == 0) {
		ofLogError("ofShader") << "Failed creating shader of type " << nameForType(type) << ".";
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
    GLuint err = glGetError();
    if (err != GL_NO_ERROR){
        ofLogError("ofShader") << "OpenGL generated error " << err << " trying to get the compile status for " << nameForType(type) << " shader. Does your video card support this?";
        return false;
    }
    
	if(status == GL_TRUE) {
		ofLogVerbose("ofShader") << nameForType(type) << " shader compiled.";
	} else if (status == GL_FALSE) {
		ofLogError("ofShader") <<  nameForType(type) << " shader failed to compile.";
		checkShaderInfoLog(shader, type);
		return false;
	}
	
	shaders[type] = shader;
	retainShader(shader);

	return true;
}

//--------------------------------------------------------------
void ofShader::setGeometryInputType(GLenum type) {
	checkAndCreateProgram();
	glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, type);
}

//--------------------------------------------------------------
void ofShader::setGeometryOutputType(GLenum type) {
	checkAndCreateProgram();
	glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, type);
}

//--------------------------------------------------------------
void ofShader::setGeometryOutputCount(int count) {
	checkAndCreateProgram();
	glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, count);
}

//--------------------------------------------------------------
int ofShader::getGeometryMaxOutputCount() {
	int temp;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
	return temp;
}

//--------------------------------------------------------------
bool ofShader::checkProgramLinkStatus(GLuint program) {
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
    GLuint err = glGetError();
    if (err != GL_NO_ERROR){
        ofLogError("ofShader") << "OpenGL generated error " << err << " trying to get the program link status. Does your video card support shader programs?";
        return false;
    }
	if(status == GL_TRUE) {
		ofLogVerbose("ofShader") << "Program linked.";
	} else if (status == GL_FALSE) {
		ofLogError("ofShader") << "Program failed to link.";
		checkProgramInfoLog(program);
		return false;
	}
	return true;
}

//--------------------------------------------------------------
void ofShader::checkShaderInfoLog(GLuint shader, GLenum type) {
	GLsizei infoLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		GLchar* infoBuffer = new GLchar[infoLength];
		glGetShaderInfoLog(shader, infoLength, &infoLength, infoBuffer);
		ofLogError("ofShader") << nameForType(type) << " shader reports: " << endl << infoBuffer;
		delete [] infoBuffer;
	}
}

//--------------------------------------------------------------
void ofShader::checkProgramInfoLog(GLuint program) {
	GLsizei infoLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		GLchar* infoBuffer = new GLchar[infoLength];
		glGetProgramInfoLog(program, infoLength, &infoLength, infoBuffer);
		ofLogError("ofShader") << "Shader program reports: " << endl << infoBuffer;
		delete [] infoBuffer;
	}
}

//--------------------------------------------------------------
void ofShader::checkAndCreateProgram() {
	if(GL_ARB_shader_objects) {
		if(program == 0) {
            ofLogVerbose("ofShader") << "Creating GLSL Program.";
			program = glCreateProgram();
			retainProgram(program);
		}
	} else {
        ofLogError("ofShader") << "Sorry, it looks like you can't run 'ARB_shader_objects'." << endl
                               << "Please check the capabilites of your graphics card." << endl
                               << "http://www.ozone3d.net/gpu_caps_viewer/.";
	}
}

//--------------------------------------------------------------
bool ofShader::linkProgram() {
    if(shaders.empty()) {
        ofLogError("ofShader") << "Trying to link GLSL program, but no shaders created yet.";
    } else {
        checkAndCreateProgram();
        
        for(map<GLenum, GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it){
            GLuint shader = it->second;
            if(shader) {
                ofLogVerbose("ofShader") << "Attaching shader of type " << nameForType(it->first) << ".";
                glAttachShader(program, shader);
            }
        }
        
        glLinkProgram(program);
        
        checkProgramLinkStatus(program);

        // bLoaded means we have loaded shaders onto the graphics card;
        // it doesn't necessarily mean that these shaders have compiled and linked successfully.
        bLoaded = true;
    }
    return bLoaded;
}

//--------------------------------------------------------------
void ofShader::unload() {
	if(bLoaded) {
		for(map<GLenum, GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it) {
			GLuint shader = it->second;
			if(shader) {
				ofLogVerbose("ofShader") << "Detaching and deleting shader of type " + nameForType(it->first) << ".";
				releaseShader(program,shader);
			}
		}

		if (program) {
			releaseProgram(program);
			program = 0;
		}
		
		shaders.clear();
	}
	bLoaded = false;
}

//--------------------------------------------------------------
void ofShader::begin() {
	if (bLoaded == true)
		glUseProgram(program);
}

//--------------------------------------------------------------
void ofShader::end() {
	if (bLoaded == true)
		glUseProgram(0);
}

//--------------------------------------------------------------
void ofShader::setUniformTexture(const char* name, ofBaseHasTexture& img, int textureLocation) {
	setUniformTexture(name, img.getTextureReference(), textureLocation);
}

//--------------------------------------------------------------
void ofShader::setUniformTexture(const char* name, int textureTarget, GLint textureID, int textureLocation){
	if(bLoaded) {
		glActiveTexture(GL_TEXTURE0 + textureLocation);
		glEnable(textureTarget);
		glBindTexture(textureTarget, textureID);
		glDisable(textureTarget);
		setUniform1i(name, textureLocation);
		glActiveTexture(GL_TEXTURE0);
	}
}

//--------------------------------------------------------------
void ofShader::setUniformTexture(const char* name, ofTexture& tex, int textureLocation) {
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

//--------------------------------------------------------------
void ofShader::setUniform1i(const char* name, int v1) {
	if(bLoaded)
		glUniform1i(getUniformLocation(name), v1);
}

//--------------------------------------------------------------
void ofShader::setUniform2i(const char* name, int v1, int v2) {
	if(bLoaded)
		glUniform2i(getUniformLocation(name), v1, v2);
}

//--------------------------------------------------------------
void ofShader::setUniform3i(const char* name, int v1, int v2, int v3) {
	if(bLoaded)
		glUniform3i(getUniformLocation(name), v1, v2, v3);
}

//--------------------------------------------------------------
void ofShader::setUniform4i(const char* name, int v1, int v2, int v3, int v4) {
	if(bLoaded)
		glUniform4i(getUniformLocation(name), v1, v2, v3, v4);
}

//--------------------------------------------------------------
void ofShader::setUniform1f(const char* name, float v1) {
	if(bLoaded)
		glUniform1f(getUniformLocation(name), v1);
}

//--------------------------------------------------------------
void ofShader::setUniform2f(const char* name, float v1, float v2) {
	if(bLoaded)
		glUniform2f(getUniformLocation(name), v1, v2);
}

//--------------------------------------------------------------
void ofShader::setUniform3f(const char* name, float v1, float v2, float v3) {
	if(bLoaded)
		glUniform3f(getUniformLocation(name), v1, v2, v3);
}

//--------------------------------------------------------------
void ofShader::setUniform4f(const char* name, float v1, float v2, float v3, float v4) {
	if(bLoaded)
		glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
}

//--------------------------------------------------------------
void ofShader::setUniform1iv(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform1iv(getUniformLocation(name), count, v);
}

//--------------------------------------------------------------
void ofShader::setUniform2iv(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform2iv(getUniformLocation(name), count, v);
}

//--------------------------------------------------------------
void ofShader::setUniform3iv(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform3iv(getUniformLocation(name), count, v);
}

//--------------------------------------------------------------
void ofShader::setUniform4iv(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform4iv(getUniformLocation(name), count, v);
}

//--------------------------------------------------------------
void ofShader::setUniform1fv(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform1fv(getUniformLocation(name), count, v);
}

//--------------------------------------------------------------
void ofShader::setUniform2fv(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform2fv(getUniformLocation(name), count, v);
}

//--------------------------------------------------------------
void ofShader::setUniform3fv(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform3fv(getUniformLocation(name), count, v);
}

//--------------------------------------------------------------
void ofShader::setUniform4fv(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform4fv(getUniformLocation(name), count, v);
}

//--------------------------------------------------------------
void ofShader::setUniformMatrix4f(const char* name, const ofMatrix4x4 & m) {
	if(bLoaded)
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, m.getPtr());
}

//--------------------------------------------------------------
void ofShader::setAttribute1s(GLint location, short v1) {
	if(bLoaded)
		glVertexAttrib1s(location, v1);
}

//--------------------------------------------------------------
void ofShader::setAttribute2s(GLint location, short v1, short v2) {
	if(bLoaded)
		glVertexAttrib2s(location, v1, v2);
}

//--------------------------------------------------------------
void ofShader::setAttribute3s(GLint location, short v1, short v2, short v3) {
	if(bLoaded)
		glVertexAttrib3s(location, v1, v2, v3);
}

//--------------------------------------------------------------
void ofShader::setAttribute4s(GLint location, short v1, short v2, short v3, short v4) {
	if(bLoaded)
		glVertexAttrib4s(location, v1, v2, v3, v4);
}

//--------------------------------------------------------------
void ofShader::setAttribute1f(GLint location, float v1) {
	if(bLoaded)
		glVertexAttrib1f(location, v1);
}

//--------------------------------------------------------------
void ofShader::setAttribute2f(GLint location, float v1, float v2) {
	if(bLoaded)
		glVertexAttrib2f(location, v1, v2);
}

//--------------------------------------------------------------
void ofShader::setAttribute3f(GLint location, float v1, float v2, float v3) {
	if(bLoaded)
		glVertexAttrib3f(location, v1, v2, v3);
}

//--------------------------------------------------------------
void ofShader::setAttribute4f(GLint location, float v1, float v2, float v3, float v4) {
	if(bLoaded)
		glVertexAttrib4f(location, v1, v2, v3, v4);
}

//--------------------------------------------------------------
void ofShader::setAttribute1d(GLint location, double v1) {
	if(bLoaded)
		glVertexAttrib1d(location, v1);
}

//--------------------------------------------------------------
void ofShader::setAttribute2d(GLint location, double v1, double v2) {
	if(bLoaded)
		glVertexAttrib2d(location, v1, v2);
}

//--------------------------------------------------------------
void ofShader::setAttribute3d(GLint location, double v1, double v2, double v3) {
	if(bLoaded)
		glVertexAttrib3d(location, v1, v2, v3);
}

//--------------------------------------------------------------
void ofShader::setAttribute4d(GLint location, double v1, double v2, double v3, double v4) {
	if(bLoaded)
		glVertexAttrib4d(location, v1, v2, v3, v4);
}

//--------------------------------------------------------------
GLint ofShader::getAttributeLocation(const char* name) {
	return glGetAttribLocation(program, name);
}

//--------------------------------------------------------------
GLint ofShader::getUniformLocation(const char* name) {
	return glGetUniformLocation(program, name);
}

//--------------------------------------------------------------
vector<ofShaderUniform> ofShader::getActiveUniforms() {

    vector<ofShaderUniform> activeUniforms;
    
	GLint numUniforms = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
	GLint uniformMaxLength = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);
	
	GLint count = -1;
	GLenum type = 0;
    GLsizei length;
	GLchar* uniformName = new GLchar[uniformMaxLength];
	for(GLint i = 0; i < numUniforms; i++) {
        glGetActiveUniform(program, i, uniformMaxLength, &length, &count, &type, uniformName);
        GLenum err = glGetError();
        if(err == GL_NO_ERROR) {
            ofShaderUniform uniformVar;
            string name(uniformName,length);
            uniformVar.name = name;
            uniformVar.location = getUniformLocation(name.c_str());
            uniformVar.index = i;
            uniformVar.size = count;
            uniformVar.type = type;
            uniformVar.isBuiltIn = (uniformVar.name.length() > 3 && uniformVar.name.substr(0,3) == "gl_");
            // add it
            activeUniforms.push_back(uniformVar);
        } else {
            const GLubyte* errString = gluErrorString(err);
            if(errString != NULL) {
                ofLogError("ofShader") << "Error reading uniform variable code " << err << " : " << errString << ".";
            } else {
                ofLogError("ofShader") << "Error reading uniform variable code " << err << " : unknown error type.";
            }
        }
        
	}
	delete [] uniformName;
    
    return activeUniforms;
}

//--------------------------------------------------------------
vector<ofShaderAttribute> ofShader::getActiveAttributes() {

    vector<ofShaderAttribute> activeAttributes;
    
	GLint numAttributes = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	GLint attributeMaxLength = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributeMaxLength);
	
	GLint count = -1;
	GLenum type = 0;
    GLsizei length;
	GLchar* attributeName = new GLchar[attributeMaxLength];
	for(GLint i = 0; i < numAttributes; i++) {
        glGetActiveUniform(program, i, attributeMaxLength, &length, &count, &type, attributeName);
        GLenum err = glGetError();
        if(err == GL_NO_ERROR) {
            ofShaderAttribute attributeVar;
            string name(attributeName,length);
            attributeVar.name = name;
            attributeVar.location = getUniformLocation(name.c_str());
            attributeVar.index = i;
            attributeVar.size = count;
            attributeVar.type = type;
            attributeVar.isBuiltIn = (attributeVar.name.length() > 3 && attributeVar.name.substr(0,3) == "gl_");
            // add it
            activeAttributes.push_back(attributeVar);
        } else {
            const GLubyte* errString = gluErrorString(err);
            if(errString != NULL) {
                ofLogError("ofShader") << "Error reading attribute variable code " << err << " : " << errString << ".";
            } else {
                ofLogError("ofShader") << "Error reading attribute variable code " << err << " : unknown error type.";
            }
        }
        
	}
	delete [] attributeName;
    
    return activeAttributes;
}

//--------------------------------------------------------------
string ofShader::printActiveUniforms() {
    vector<ofShaderUniform> activeUniforms = getActiveUniforms();
    stringstream ss;
    ss << activeUniforms.size() << " uniforms:" << endl;
    for(size_t i = 0; i < activeUniforms.size(); i++) {
        ofShaderUniform& value = activeUniforms[i];
        ss << " [" << value.index << "] ";
        ss << value.name << " @ index ";
        ss << value.location << endl;
    }

    cout << ss.str(); // to the console
    return ss.str();
}

//--------------------------------------------------------------
string ofShader::printActiveAttributes() {
    vector<ofShaderAttribute> activeAttributes = getActiveAttributes();
    stringstream ss;
    ss << activeAttributes.size() << " attributes:" << endl;
    for(size_t i = 0; i < activeAttributes.size(); i++) {
        ofShaderUniform& value = activeAttributes[i];
        ss << " [" << value.index << "] ";
        ss << value.name << " @ index ";
        ss << value.location << endl;
    }
    
    cout << ss.str(); // to the console
    return ss.str();
}

//--------------------------------------------------------------
GLuint& ofShader::getProgram() {
	return program;
}

//--------------------------------------------------------------
GLuint& ofShader::getShader(GLenum type) {
	return shaders[type];
}

//--------------------------------------------------------------
string ofShader::nameForType(GLenum type) {
	switch(type) {
		case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		case GL_GEOMETRY_SHADER_EXT: return "GL_GEOMETRY_SHADER_EXT";
		default: return "UNKNOWN SHADER TYPE";
	}
}

#endif
