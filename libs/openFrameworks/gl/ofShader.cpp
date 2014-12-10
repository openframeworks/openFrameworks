#include "ofShader.h"
#include "ofUtils.h"
#include "ofFileUtils.h"
#include "ofGraphics.h"
#include "ofGLProgrammableRenderer.h"
#include "Poco/RegularExpression.h"
#include "ofTexture.h"
#include "ofMatrix4x4.h"
#include "ofMatrix3x3.h"
#include "ofVec2f.h"
#include "ofVec3f.h"
#include "ofVec4f.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"

static const string COLOR_ATTRIBUTE="color";
static const string POSITION_ATTRIBUTE="position";
static const string NORMAL_ATTRIBUTE="normal";
static const string TEXCOORD_ATTRIBUTE="texcoord";

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
		ofLogWarning("ofShader") << "releaseShader(): something's wrong here, releasing unknown shader id " << id << " from program " << program;
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
		ofLogWarning("ofShader") << "releaseProgram(): something's wrong here, releasing unknown program id " << id;
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
ofShader::ofShader(const ofShader & mom) :
program(mom.program),
bLoaded(mom.bLoaded),
shaders(mom.shaders){
	if(mom.bLoaded){
		retainProgram(program);
		for(unordered_map<GLenum, GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it){
			GLuint shader = it->second;
			retainShader(shader);
		}
	}
}

//--------------------------------------------------------------
ofShader & ofShader::operator=(const ofShader & mom){
    if(this == &mom) {
        return *this;
    }
	if(bLoaded){
		unload();
	}
	program = mom.program;
	bLoaded = mom.bLoaded;
	shaders = mom.shaders;
	if(mom.bLoaded){
		retainProgram(program);
		for(unordered_map<GLenum, GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it){
			GLuint shader = it->second;
			retainShader(shader);
		}
	}
	return *this;
}

//--------------------------------------------------------------
bool ofShader::load(string shaderName) {
	return load(shaderName + ".vert", shaderName + ".frag");
}

//--------------------------------------------------------------
bool ofShader::load(string vertName, string fragName, string geomName) {
	if(vertName.empty() == false) setupShaderFromFile(GL_VERTEX_SHADER, vertName);
	if(fragName.empty() == false) setupShaderFromFile(GL_FRAGMENT_SHADER, fragName);
#ifndef TARGET_OPENGLES
	if(geomName.empty() == false) setupShaderFromFile(GL_GEOMETRY_SHADER_EXT, geomName);
#endif
	if(ofIsGLProgrammableRenderer()){
		bindDefaults();
	}
	return linkProgram();
}

//--------------------------------------------------------------
bool ofShader::setupShaderFromFile(GLenum type, string filename) {
	ofBuffer buffer = ofBufferFromFile(filename);
	// we need to make absolutely sure to have an absolute path here, so that any #includes
	// within the shader files have a root directory to traverse from.
	string absoluteFilePath = ofFilePath::getAbsolutePath(filename, true);
	string sourceDirectoryPath = ofFilePath::getEnclosingDirectory(absoluteFilePath,false);
	if(buffer.size()) {
		return setupShaderFromSource(type, buffer.getText(), sourceDirectoryPath);
	} else {
		ofLogError("ofShader") << "setupShaderFromFile(): couldn't load " << nameForType(type) << " shader " << " from \"" << filename << "\"";
		return false;
	}
}

//--------------------------------------------------------------
bool ofShader::setupShaderFromSource(GLenum type, string source, string sourceDirectoryPath) {
    unload();
    
	// create program if it doesn't exist already
	checkAndCreateProgram();
	GLuint clearErrors = glGetError(); //needed for some users to clear gl errors
    if( clearErrors != GL_NO_ERROR ){
        ofLogVerbose("ofShader") << "setupShaderFromSource(): OpenGL error after checkAndCreateProgram() (probably harmless): error " << clearErrors;
    }

	// create shader
	GLuint shader = glCreateShader(type);
	if(shader == 0) {
		ofLogError("ofShader") << "setupShaderFromSource(): failed creating " << nameForType(type) << " shader";
		return false;
	}

	// parse for includes
	string src = parseForIncludes( source , sourceDirectoryPath);
	
	// store source code (that's the expanded source with all includes copied in)
	shaderSource[type] = src;
	
	// compile shader
	const char* sptr = src.c_str();
	int ssize = src.size();
	glShaderSource(shader, 1, &sptr, &ssize);
	glCompileShader(shader);
	
	// check compile status
	GLint status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    GLuint err = glGetError();
    if (err != GL_NO_ERROR){
        ofLogError("ofShader") << "setupShaderFromSource(): OpenGL generated error " << err << " trying to get the compile status for a " << nameForType(type) << " shader, does your video card support this?";
        return false;
    }
    
	if(status == GL_TRUE){
		ofLogVerbose("ofShader") << "setupShaderFromSource(): " << nameForType(type) + " shader compiled";
		checkShaderInfoLog(shader, type, OF_LOG_WARNING);
	}
	
	else if (status == GL_FALSE) {
		ofLogError("ofShader") << "setupShaderFromSource(): " << nameForType(type) + " shader failed to compile";
		checkShaderInfoLog(shader, type, OF_LOG_ERROR);
		return false;
	}
	
	shaders[type] = shader;
	retainShader(shader);

	return true;
}

/*
 * Parse for GLSL includes based on
 * https://www.opengl.org/discussion_boards/showthread.php/169209-include-in-glsl?p=1192415&viewfull=1#post1192415
 */

string ofShader::parseForIncludes( const string& source, const string& sourceDirectoryPath) {
	vector<string> included;
	return parseForIncludes( source, included, 0, sourceDirectoryPath);
}

string ofShader::parseForIncludes( const string& source, vector<string>& included, int level, const string& sourceDirectoryPath) {
    
	if ( level > 32 ) {
		ofLogError( "ofShader", "glsl header inclusion depth limit reached, might be caused by cyclic header inclusion" );
		return "";
	}
	
	stringstream output;
	stringstream input;
	input << source;
	
	Poco::RegularExpression re("^\\s*#\\s*pragma\\s+include\\s+[\"<](.*)[\">].*");
	Poco::RegularExpression::MatchVec matches;
	
	string line;
	while( std::getline( input, line ) ) {
		
		if ( re.match( line, 0, matches ) < 2 ) {
			output << line << endl;
			continue;
		}
		
		string include = line.substr(matches[1].offset, matches[1].length);
		
		if ( std::find( included.begin(), included.end(), include ) != included.end() ) {
			ofLogVerbose("ofShader") << include << " already included";
			continue;
		}
		
		// we store the absolute paths so as have (more) unique file identifiers.
		
		include = ofFile(sourceDirectoryPath + include).getAbsolutePath();
		included.push_back( include );
		
		
		ofBuffer buffer = ofBufferFromFile( include );
		if ( !buffer.size() ) {
			ofLogError("ofShader") <<"Could not open glsl include file " << include;
			continue;
		}
		
		string currentDir = ofFile(include).getEnclosingDirectory();
		output << parseForIncludes( buffer.getText(), included, level + 1, currentDir ) << endl;
	}
	
	return output.str();
}

//--------------------------------------------------------------
string ofShader::getShaderSource(GLenum type)  const{
	if (shaderSource.find(type) != shaderSource.end()) {
		return shaderSource[type];
	} else {
		ofLogError("ofShader") << "No shader source for shader of type: " << nameForType(type);
		return "";
	}
}

//--------------------------------------------------------------
void ofShader::setGeometryInputType(GLenum type) {
#ifndef TARGET_OPENGLES
	checkAndCreateProgram();
	glProgramParameteri(program, GL_GEOMETRY_INPUT_TYPE_EXT, type);
#endif
}

//--------------------------------------------------------------
void ofShader::setGeometryOutputType(GLenum type) {
#ifndef TARGET_OPENGLES
	checkAndCreateProgram();
	glProgramParameteri(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, type);
#endif
}

//--------------------------------------------------------------
void ofShader::setGeometryOutputCount(int count) {
#ifndef TARGET_OPENGLES
	checkAndCreateProgram();
	glProgramParameteri(program, GL_GEOMETRY_VERTICES_OUT_EXT, count);
#endif
}

//--------------------------------------------------------------
int ofShader::getGeometryMaxOutputCount()  const{
#ifndef TARGET_OPENGLES
	int temp;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
	return temp;
#else
	return 0;
#endif
}

//--------------------------------------------------------------
bool ofShader::checkProgramLinkStatus(GLuint program) {
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
    GLuint err = glGetError();
    if (err != GL_NO_ERROR){
        ofLogError("ofShader") << "checkProgramLinkStatus(): OpenGL generated error " << err << " trying to get the program link status, does your video card support shader programs?";
        return false;
    }
	if(status == GL_TRUE)
		ofLogVerbose("ofShader") << "checkProgramLinkStatus(): program " << program << " linked";
	else if (status == GL_FALSE) {
		ofLogError("ofShader") << "checkProgramLinkStatus(): program failed to link";
		checkProgramInfoLog(program);
		return false;
	}
	return true;
}

//--------------------------------------------------------------
void ofShader::checkShaderInfoLog(GLuint shader, GLenum type, ofLogLevel logLevel) {
	GLsizei infoLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		GLchar* infoBuffer = new GLchar[infoLength];
		glGetShaderInfoLog(shader, infoLength, &infoLength, infoBuffer);
		ofLog(logLevel, "ofShader: %s shader reports:\n%s", nameForType(type).c_str(), infoBuffer);
		if (shaderSource.find(type) != shaderSource.end()) {
			// The following regexp should match shader compiler error messages by Nvidia and ATI.
			// Unfortunately, each vendor's driver formats error messages slightly different.
			Poco::RegularExpression re("^.*[(:]{1}(\\d+)[:)]{1}.*");
			Poco::RegularExpression::MatchVec matches;
			string infoString = (infoBuffer != NULL) ? string(infoBuffer): "";
			re.match(infoString, 0, matches);
			ofBuffer buf = shaderSource[type];
			ofBuffer::Line line = buf.getLines().begin();
			if (!matches.empty()){
			int  offendingLineNumber = ofToInt(infoString.substr(matches[1].offset, matches[1].length));
				ostringstream msg;
				msg << "ofShader: " + nameForType(type) + ", offending line " << offendingLineNumber << " :"<< endl;
				for(int i=0; line != buf.getLines().end(); line++, i++ ){
					string s = *line;
					if ( i >= offendingLineNumber -3 && i < offendingLineNumber + 2 ){
						msg << "\t" << setw(5) << (i+1) << "\t" << s << endl;
					}
				}
				ofLog(logLevel) << msg.str();
			}
		}
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
		string msg = "ofShader: program reports:\n";
#ifdef TARGET_RASPBERRYPI
		if (shaderSource.find(GL_FRAGMENT_SHADER) != shaderSource.end()) {
			Poco::RegularExpression re(",.line.([^\\)]*)");
			Poco::RegularExpression::MatchVec matches;
			string infoString = (infoBuffer != NULL) ? string(infoBuffer): "";
			re.match(infoString, 0, matches);
			ofBuffer buf = shaderSource[GL_FRAGMENT_SHADER];
			ofBuffer::Line line = buf.getLines().begin();
			if (!matches.empty()){
			int  offendingLineNumber = ofToInt(infoString.substr(matches[1].offset, matches[1].length));
				ostringstream msg;
				msg << "ofShader: " + nameForType(GL_FRAGMENT_SHADER) + ", offending line " << offendingLineNumber << " :"<< endl;
				for(int i=0; line != buf.getLines().end(); line++, i++ ){
					string s = *line;
					if ( i >= offendingLineNumber -3 && i < offendingLineNumber + 2 ){
						msg << "\t" << setw(5) << (i+1) << "\t" << s << endl;
					}
				}
				ofLogError("ofShader") << msg.str();
			}
		}
#endif
		ofLogError("ofShader", msg + infoBuffer);
		delete [] infoBuffer;
	}
}



//--------------------------------------------------------------
void ofShader::checkAndCreateProgram() {
#ifndef TARGET_OPENGLES
	if(GL_ARB_shader_objects) {
#else
	if(ofIsGLProgrammableRenderer()){
#endif
		if(program == 0) {
			ofLogVerbose("ofShader") << "checkAndCreateProgram(): creating GLSL program";
			program = glCreateProgram();
			retainProgram(program);
		}
	} else {
		ofLogError("ofShader") << "sorry, it looks like you can't run 'ARB_shader_objects'";
		ofLogError("ofShader") << "please check the capabilites of your graphics card: http://www.ozone3d.net/gpu_caps_viewer";
	}
}

//--------------------------------------------------------------
bool ofShader::linkProgram() {
	if(shaders.empty()) {
		ofLogError("ofShader") << "linkProgram(): trying to link GLSL program, but no shaders created yet";
	} else {
		checkAndCreateProgram();

		for(unordered_map<GLenum, GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it){
			GLuint shader = it->second;
			if(shader) {
				ofLogVerbose("ofShader") << "linkProgram(): attaching " << nameForType(it->first) << " shader to program " << program;
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

void ofShader::bindAttribute(GLuint location, const string & name) const{
	glBindAttribLocation(program,location,name.c_str());
}

//--------------------------------------------------------------
bool ofShader::bindDefaults() const{
	if(shaders.empty()) {
		ofLogError("ofShader") << "bindDefaults(): trying to link GLSL program, but no shaders created yet";
		return false;
	} else {
		bindAttribute(ofShader::POSITION_ATTRIBUTE,::POSITION_ATTRIBUTE);
		bindAttribute(ofShader::COLOR_ATTRIBUTE,::COLOR_ATTRIBUTE);
		bindAttribute(ofShader::NORMAL_ATTRIBUTE,::NORMAL_ATTRIBUTE);
		bindAttribute(ofShader::TEXCOORD_ATTRIBUTE,::TEXCOORD_ATTRIBUTE);
		return true;
	}

}

//--------------------------------------------------------------
void ofShader::unload() {
	if(bLoaded) {
		for(unordered_map<GLenum, GLuint>::const_iterator it = shaders.begin(); it != shaders.end(); ++it) {
			GLuint shader = it->second;
			if(shader) {
				ofLogVerbose("ofShader") << "unload(): detaching and deleting " << nameForType(it->first) << " shader from program " << program;
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
bool ofShader::isLoaded() const{
	return bLoaded;
}

//--------------------------------------------------------------
void ofShader::begin()  const{
	if (bLoaded){
		glUseProgram(program);
		shared_ptr<ofGLProgrammableRenderer> renderer = ofGetGLProgrammableRenderer();
		if(renderer){
			renderer->beginCustomShader(*this);
		}
	}else{
		ofLogError("ofShader") << "begin(): couldn't begin, shader not loaded";
	}
}

//--------------------------------------------------------------
void ofShader::end()  const{
	if (bLoaded){
		shared_ptr<ofGLProgrammableRenderer> renderer = ofGetGLProgrammableRenderer();
		if(renderer){
			renderer->endCustomShader();
		}else{
			glUseProgram(0);
		}
	}
}

#if !defined(TARGET_OPENGLES) && defined(glDispatchCompute)
//--------------------------------------------------------------
void ofShader::dispatchCompute(GLuint x, GLuint y, GLuint z) const{
    glDispatchCompute(x,y,z);
}
#endif

//--------------------------------------------------------------
void ofShader::setUniformTexture(const string & name, const ofBaseHasTexture& img, int textureLocation)  const{
	setUniformTexture(name, img.getTexture(), textureLocation);
}

//--------------------------------------------------------------
void ofShader::setUniformTexture(const string & name, int textureTarget, GLint textureID, int textureLocation) const{
	if(bLoaded) {
		glActiveTexture(GL_TEXTURE0 + textureLocation);
		if (!ofIsGLProgrammableRenderer()){
			glEnable(textureTarget);
			glBindTexture(textureTarget, textureID);
			glDisable(textureTarget);
		} else {
			glBindTexture(textureTarget, textureID);
		}
		setUniform1i(name, textureLocation);
		glActiveTexture(GL_TEXTURE0);
	}
}

//--------------------------------------------------------------
void ofShader::setUniformTexture(const string & name, const ofTexture& tex, int textureLocation)  const{
	if(bLoaded) {
		ofTextureData texData = tex.getTextureData();
		glActiveTexture(GL_TEXTURE0 + textureLocation);
		if (!ofIsGLProgrammableRenderer()){
			glEnable(texData.textureTarget);
			glBindTexture(texData.textureTarget, texData.textureID);
			glDisable(texData.textureTarget);
		} else {
			glBindTexture(texData.textureTarget, texData.textureID);
		}
		setUniform1i(name, textureLocation);
		glActiveTexture(GL_TEXTURE0);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform1i(const string & name, int v1)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform1i(loc, v1);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform2i(const string & name, int v1, int v2)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform2i(loc, v1, v2);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform3i(const string & name, int v1, int v2, int v3)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform3i(loc, v1, v2, v3);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform4i(const string & name, int v1, int v2, int v3, int v4)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) 	glUniform4i(loc, v1, v2, v3, v4);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform1f(const string & name, float v1)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform1f(loc, v1);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform2f(const string & name, float v1, float v2)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform2f(loc, v1, v2);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform3f(const string & name, float v1, float v2, float v3)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform3f(loc, v1, v2, v3);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform4f(const string & name, float v1, float v2, float v3, float v4)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform4f(loc, v1, v2, v3, v4);
	}
}


//--------------------------------------------------------------
void ofShader::setUniform2f(const string & name, const ofVec2f & v) const{
	setUniform2f(name,v.x,v.y);
}

//--------------------------------------------------------------
void ofShader::setUniform3f(const string & name, const ofVec3f & v) const{
	setUniform3f(name,v.x,v.y,v.z);
}

//--------------------------------------------------------------
void ofShader::setUniform4f(const string & name, const ofVec4f & v) const{
	setUniform4f(name,v.x,v.y,v.z,v.w);
}

//--------------------------------------------------------------
void ofShader::setUniform1iv(const string & name, const int* v, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform1iv(loc, count, v);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform2iv(const string & name, const int* v, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform2iv(loc, count, v);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform3iv(const string & name, const int* v, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform3iv(loc, count, v);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform4iv(const string & name, const int* v, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform4iv(loc, count, v);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform1fv(const string & name, const float* v, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform1fv(loc, count, v);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform2fv(const string & name, const float* v, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform2fv(loc, count, v);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform3fv(const string & name, const float* v, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform3fv(loc, count, v);
	}
}

//--------------------------------------------------------------
void ofShader::setUniform4fv(const string & name, const float* v, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniform4fv(loc, count, v);
	}
}

//--------------------------------------------------------------
void ofShader::setUniforms(const ofParameterGroup & parameters) const{
	for(int i=0;i<parameters.size();i++){
		if(parameters[i].type()==typeid(ofParameter<int>).name()){
			setUniform1i(parameters[i].getEscapedName(),parameters[i].cast<int>());
		}else if(parameters[i].type()==typeid(ofParameter<float>).name()){
			setUniform1f(parameters[i].getEscapedName(),parameters[i].cast<float>());
		}else if(parameters[i].type()==typeid(ofParameter<ofVec2f>).name()){
			setUniform2f(parameters[i].getEscapedName(),parameters[i].cast<ofVec2f>());
		}else if(parameters[i].type()==typeid(ofParameter<ofVec3f>).name()){
			setUniform3f(parameters[i].getEscapedName(),parameters[i].cast<ofVec3f>());
		}else if(parameters[i].type()==typeid(ofParameter<ofVec4f>).name()){
			setUniform4f(parameters[i].getEscapedName(),parameters[i].cast<ofVec4f>());
		}else if(parameters[i].type()==typeid(ofParameterGroup).name()){
			setUniforms((ofParameterGroup&)parameters[i]);
		}
	}
}
	
//--------------------------------------------------------------
void ofShader::setUniformMatrix3f(const string & name, const ofMatrix3x3 & m, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniformMatrix3fv(loc, count, GL_FALSE, &m.a);
	}
}

//--------------------------------------------------------------
void ofShader::setUniformMatrix4f(const string & name, const ofMatrix4x4 & m, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniformMatrix4fv(loc, count, GL_FALSE, m.getPtr());
	}
}

#ifndef TARGET_OPENGLES
//--------------------------------------------------------------
void ofShader::setAttribute1s(GLint location, short v1)  const{
	if(bLoaded)
		glVertexAttrib1s(location, v1);
}

//--------------------------------------------------------------
void ofShader::setAttribute2s(GLint location, short v1, short v2)  const{
	if(bLoaded)
		glVertexAttrib2s(location, v1, v2);
}

//--------------------------------------------------------------
void ofShader::setAttribute3s(GLint location, short v1, short v2, short v3)  const{
	if(bLoaded)
		glVertexAttrib3s(location, v1, v2, v3);
}

//--------------------------------------------------------------
void ofShader::setAttribute4s(GLint location, short v1, short v2, short v3, short v4)  const{
	if(bLoaded)
		glVertexAttrib4s(location, v1, v2, v3, v4);
}
#endif

//--------------------------------------------------------------
void ofShader::setAttribute1f(GLint location, float v1)  const{
	if(bLoaded)
		glVertexAttrib1f(location, v1);
}

//--------------------------------------------------------------
void ofShader::setAttribute2f(GLint location, float v1, float v2)  const{
	if(bLoaded)
		glVertexAttrib2f(location, v1, v2);
}

//--------------------------------------------------------------
void ofShader::setAttribute3f(GLint location, float v1, float v2, float v3)  const{
	if(bLoaded)
		glVertexAttrib3f(location, v1, v2, v3);
}

//--------------------------------------------------------------
void ofShader::setAttribute4f(GLint location, float v1, float v2, float v3, float v4)  const{
	if(bLoaded)
		glVertexAttrib4f(location, v1, v2, v3, v4);
}

void ofShader::setAttribute1fv(const string & name, const float* v, GLsizei stride) const{
	if(bLoaded){
		GLint location = getAttributeLocation(name);
		if (location != -1) {
			glVertexAttribPointer(location, 1, GL_FLOAT, GL_FALSE, stride, v);
			glEnableVertexAttribArray(location);
		}
	}
}

void ofShader::setAttribute2fv(const string & name, const float* v, GLsizei stride) const{
	if(bLoaded){
		GLint location = getAttributeLocation(name);
		if (location != -1) {
			glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, v);
			glEnableVertexAttribArray(location);
		}
	}

}

void ofShader::setAttribute3fv(const string & name, const float* v, GLsizei stride) const{
	if(bLoaded){
		GLint location = getAttributeLocation(name);
		if (location != -1) {
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, v);
			glEnableVertexAttribArray(location);
		}
	}
}

void ofShader::setAttribute4fv(const string & name, const float* v, GLsizei stride) const{
	if(bLoaded){
		GLint location = getAttributeLocation(name);
		if (location != -1) {
			glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, stride, v);
			glEnableVertexAttribArray(location);
		}
	}
}

#ifndef TARGET_OPENGLES
//--------------------------------------------------------------
void ofShader::setAttribute1d(GLint location, double v1)  const{
	if(bLoaded)
		glVertexAttrib1d(location, v1);
}

//--------------------------------------------------------------
void ofShader::setAttribute2d(GLint location, double v1, double v2)  const{
	if(bLoaded)
		glVertexAttrib2d(location, v1, v2);
}

//--------------------------------------------------------------
void ofShader::setAttribute3d(GLint location, double v1, double v2, double v3)  const{
	if(bLoaded)
		glVertexAttrib3d(location, v1, v2, v3);
}

//--------------------------------------------------------------
void ofShader::setAttribute4d(GLint location, double v1, double v2, double v3, double v4)  const{
	if(bLoaded)
		glVertexAttrib4d(location, v1, v2, v3, v4);
}
#endif

//--------------------------------------------------------------
GLint ofShader::getAttributeLocation(const string & name)  const{
	return glGetAttribLocation(program, name.c_str());
}

//--------------------------------------------------------------
GLint ofShader::getUniformLocation(const string & name)  const{
	GLint loc = -1;

	// tig: caching uniform locations gives the RPi a 17% boost on average
	unordered_map<string, GLint>::iterator it = uniformLocations.find(name);
	if (it == uniformLocations.end()){
		loc = glGetUniformLocation(program, name.c_str());
		if (loc != -1) uniformLocations[name] = loc;
	} else {
		loc = it->second;
	}
	return loc;
}

//--------------------------------------------------------------
void ofShader::printActiveUniforms()  const{
	GLint numUniforms = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
	ofLogNotice("ofShader") << numUniforms << " uniforms";
	
	GLint uniformMaxLength = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);
	
	GLint count = -1;
	GLenum type = 0;
	GLchar* uniformName = new GLchar[uniformMaxLength];
	stringstream line;
	for(GLint i = 0; i < numUniforms; i++) {
		GLsizei length;
		glGetActiveUniform(program, i, uniformMaxLength, &length, &count, &type, uniformName);
		line << "[" << i << "] ";
		for(int j = 0; j < length; j++) {
			line << uniformName[j];
		}
		line << " @ index " << getUniformLocation(uniformName);
		ofLogNotice("ofShader") << line.str();
		line.str("");
	}
	delete [] uniformName;
}

//--------------------------------------------------------------
void ofShader::printActiveAttributes()  const{
	GLint numAttributes = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	ofLogNotice("ofShader") << numAttributes << " attributes";
	
	GLint attributeMaxLength = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributeMaxLength);
	
	GLint count = -1;
	GLenum type = 0;
	GLchar* attributeName = new GLchar[attributeMaxLength];
	stringstream line;
	for(GLint i = 0; i < numAttributes; i++) {
		GLsizei length;
		glGetActiveAttrib(program, i, attributeMaxLength, &length, &count, &type, attributeName);
		line << " [" << i << "] ";
		for(int j = 0; j < length; j++) {
			line << attributeName[j];
		}
		line << " @ index " << getAttributeLocation(attributeName);
		ofLogNotice("ofShader") << line.str();
		line.str("");
	}
	delete [] attributeName;
}

//--------------------------------------------------------------
GLuint ofShader::getProgram() const{
	return program;
}

//--------------------------------------------------------------
GLuint ofShader::getShader(GLenum type) const{
	return shaders[type];
}

//--------------------------------------------------------------
bool ofShader::operator==(const ofShader & other) const{
	return other.program==program;
}

//--------------------------------------------------------------
bool ofShader::operator!=(const ofShader & other) const{
	return other.program!=program;
}

//--------------------------------------------------------------
string ofShader::nameForType(GLenum type){
	switch(type) {
		case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		#ifndef TARGET_OPENGLES
		case GL_GEOMETRY_SHADER_EXT: return "GL_GEOMETRY_SHADER_EXT";
		#ifdef glDispatchCompute
		case GL_COMPUTE_SHADER: return "GL_COMPUTE_SHADER";
		#endif
		#endif
		default: return "UNKNOWN SHADER TYPE";
	}
}


