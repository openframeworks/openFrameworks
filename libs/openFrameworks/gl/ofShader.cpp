#include "ofShader.h"
#include "ofUtils.h"
#include "ofFileUtils.h"
#include "ofGraphics.h"
#include "ofGLProgrammableRenderer.h"
#include "ofTexture.h"
#include "ofMatrix4x4.h"
#include "ofMatrix3x3.h"
#include "ofVec2f.h"
#include "ofVec3f.h"
#include "ofVec4f.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofBufferObject.h"
#include <regex>
#ifdef TARGET_ANDROID
#include "ofxAndroidUtils.h"
#endif

using namespace std;


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

#ifndef TARGET_OPENGLES
//--------------------------------------------------------------
ofShader::TransformFeedbackRangeBinding::TransformFeedbackRangeBinding(const ofBufferObject & buffer, GLuint offset, GLuint size)
	:offset(offset)
	,size(size)
	,buffer(buffer){}

//--------------------------------------------------------------
ofShader::TransformFeedbackBaseBinding::TransformFeedbackBaseBinding(const ofBufferObject & buffer)
	:buffer(buffer){}
#endif

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
ofShader::ofShader(const ofShader & mom)
	:program(mom.program)
	,bLoaded(mom.bLoaded)
	,shaders(mom.shaders)
	,uniformsCache(mom.uniformsCache)
	,attributesBindingsCache(mom.attributesBindingsCache)
  #ifndef TARGET_OPENGLES
	,uniformBlocksCache(mom.uniformBlocksCache)
  #endif
{
	if(mom.bLoaded){
		retainProgram(program);
		for(auto it: shaders){
			auto shader = it.second;
			retainShader(shader.id);
		}
#ifdef TARGET_ANDROID
		ofAddListener(ofxAndroidEvents().unloadGL,this,&ofShader::unloadGL);
#endif
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
	attributesBindingsCache = mom.attributesBindingsCache;
	uniformsCache = mom.uniformsCache;
	if(mom.bLoaded){
		retainProgram(program);
		for(auto it: shaders){
			auto shader = it.second;
			retainShader(shader.id);
		}
#ifdef TARGET_ANDROID
		ofAddListener(ofxAndroidEvents().unloadGL,this,&ofShader::unloadGL);
#endif
	}
	return *this;
}

ofShader::ofShader(ofShader && mom)
	:program(std::move(mom.program))
	,bLoaded(std::move(mom.bLoaded))
	,shaders(std::move(mom.shaders))
	,uniformsCache(std::move(mom.uniformsCache))
	,attributesBindingsCache(std::move(mom.attributesBindingsCache)){
	if(mom.bLoaded){
#ifdef TARGET_ANDROID
		ofAddListener(ofxAndroidEvents().unloadGL,this,&ofShader::unloadGL);
#endif
	}
	mom.bLoaded = false;
}

ofShader & ofShader::operator=(ofShader && mom){
	if(this == &mom) {
		return *this;
	}
	if(bLoaded){
		unload();
	}
	program = std::move(mom.program);
	bLoaded = std::move(mom.bLoaded);
	shaders = std::move(mom.shaders);
	attributesBindingsCache = std::move(mom.attributesBindingsCache);
	uniformsCache = std::move(mom.uniformsCache);
	if(mom.bLoaded){
#ifdef TARGET_ANDROID
		ofAddListener(ofxAndroidEvents().unloadGL,this,&ofShader::unloadGL);
#endif
	}
	mom.bLoaded = false;
	return *this;
}

//--------------------------------------------------------------
bool ofShader::load(const std::filesystem::path& shaderName) {
	return load(shaderName.string() + ".vert", shaderName.string() + ".frag");
}

//--------------------------------------------------------------
bool ofShader::load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, const std::filesystem::path& geomName) {
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

#if !defined(TARGET_OPENGLES) && defined(glDispatchCompute)
//--------------------------------------------------------------
bool ofShader::loadCompute(const std::filesystem::path& shaderName) {
	return setupShaderFromFile(GL_COMPUTE_SHADER, shaderName) && linkProgram();
}
#endif

//--------------------------------------------------------------
bool ofShader::setup(const ofShaderSettings & settings) {
	for (auto shader : settings.shaderFiles) {
		auto ty = shader.first;
		auto file = shader.second;
		auto shaderSource = sourceFromFile(ty, file);
		shaderSource.intDefines = settings.intDefines;
		shaderSource.floatDefines = settings.floatDefines;
		if (!setupShaderFromSource(std::move(shaderSource))) {
			return false;
		}
	}

	for (auto shader : settings.shaderSources) {
		auto ty = shader.first;
		auto source = shader.second;
		Source shaderSource{ty, source, settings.sourceDirectoryPath};
		shaderSource.intDefines = settings.intDefines;
		shaderSource.floatDefines = settings.floatDefines;
		if (!setupShaderFromSource(std::move(shaderSource))) {
			return false;
		}
	}

	if (ofIsGLProgrammableRenderer() && settings.bindDefaults) {
		bindDefaults();
	}

	return linkProgram();
}

#if !defined(TARGET_OPENGLES)
//--------------------------------------------------------------
bool ofShader::setup(const TransformFeedbackSettings & settings) {
	for (auto shader : settings.shaderFiles) {
		auto ty = shader.first;
		auto file = shader.second;
		auto shaderSource = sourceFromFile(ty, file);
		shaderSource.intDefines = settings.intDefines;
		shaderSource.floatDefines = settings.floatDefines;
		if (!setupShaderFromSource(std::move(shaderSource))) {
			return false;
		}
	}

	for (auto shader : settings.shaderSources) {
		auto ty = shader.first;
		auto source = shader.second;
		Source shaderSource{ty, source, settings.sourceDirectoryPath};
		shaderSource.intDefines = settings.intDefines;
		shaderSource.floatDefines = settings.floatDefines;
		if (!setupShaderFromSource(std::move(shaderSource))) {
			return false;
		}
	}

	if (ofIsGLProgrammableRenderer() && settings.bindDefaults) {
		bindDefaults();
	}

	if (!settings.varyingsToCapture.empty()) {
		std::vector<const char*> varyings(settings.varyingsToCapture.size());
		std::transform(settings.varyingsToCapture.begin(), settings.varyingsToCapture.end(), varyings.begin(), [](const std::string & str) {
			return str.c_str();
		});
		glTransformFeedbackVaryings(getProgram(), varyings.size(), varyings.data(), settings.bufferMode);
	}
	return linkProgram();
}
#endif

//--------------------------------------------------------------
bool ofShader::setupShaderFromFile(GLenum type, const std::filesystem::path& filename) {
	ofBuffer buffer = ofBufferFromFile(filename);
	// we need to make absolutely sure to have an absolute path here, so that any #includes
	// within the shader files have a root directory to traverse from.
	string absoluteFilePath = ofFilePath::getAbsolutePath(filename, true);
	string sourceDirectoryPath = ofFilePath::getEnclosingDirectory(absoluteFilePath,false);
	if(buffer.size()) {
		return setupShaderFromSource(type, buffer.getText(), sourceDirectoryPath);
	} else {
		ofLogError("ofShader") << "setupShaderFromFile(): couldn't load " << nameForType(type) << " shader " << " from \"" << absoluteFilePath << "\"";
		return false;
	}
}

//--------------------------------------------------------------
ofShader::Source ofShader::sourceFromFile(GLenum type, const std::filesystem::path& filename) {
	ofBuffer buffer = ofBufferFromFile(filename);
	// we need to make absolutely sure to have an absolute path here, so that any #includes
	// within the shader files have a root directory to traverse from.
	string absoluteFilePath = ofFilePath::getAbsolutePath(filename, true);
	string sourceDirectoryPath = ofFilePath::getEnclosingDirectory(absoluteFilePath,false);
	if(buffer.size()) {
		return Source{type, buffer.getText(), sourceDirectoryPath};
	} else {
		ofLogError("ofShader") << "setupShaderFromFile(): couldn't load " << nameForType(type) << " shader " << " from \"" << absoluteFilePath << "\"";
		return Source{};
	}
}

//--------------------------------------------------------------
bool ofShader::setupShaderFromSource(GLenum type, string source, string sourceDirectoryPath) {
	return setupShaderFromSource({type, source, sourceDirectoryPath});
}


//--------------------------------------------------------------
bool ofShader::setupShaderFromSource(ofShader::Source && source){
	unload();

	// create program if it doesn't exist already
	checkAndCreateProgram();
	GLuint clearErrors = glGetError(); //needed for some users to clear gl errors
	if( clearErrors != GL_NO_ERROR ){
		ofLogVerbose("ofShader") << "setupShaderFromSource(): OpenGL error after checkAndCreateProgram() (probably harmless): error " << clearErrors;
	}

	// create shader
	GLuint shaderId = glCreateShader(source.type);
	if(shaderId == 0) {
		ofLogError("ofShader") << "setupShaderFromSource(): failed creating " << nameForType(source.type) << " shader";
		return false;
	} else {
		// if the shader object has been allocated successfully on the GPU
		// we must retain it so that it can be de-allocated again, once
		// this ofShader object has been discarded, or re-allocated.
		// we need to do this at this point in the code path, since early
		// return statements might prevent us from retaining later.
		retainShader(shaderId);
	}

	// parse for includes
	source.expandedSource = parseForIncludes( source.source, source.directoryPath );

    // parse and set defines
    for(auto & define: source.intDefines){
        const auto & name = define.first;
        const auto & value = define.second;
		std::regex re_define("#define[ \t]+" + name + "[ \t]+([0-9]+)");
        source.expandedSource = std::regex_replace(source.expandedSource, re_define, "#define " + name + " " + std::to_string(value));
    }

    for(auto & define: source.floatDefines){
        const auto & name = define.first;
        const auto & value = define.second;
		std::regex re_define("#define[ \t]+" + name + "[ \t]+[0-9]*(\\.[0-9]*f?)?");
        source.expandedSource = std::regex_replace(source.expandedSource, re_define, "#define " + name + " " + std::to_string(value));
	}

	// store source code (that's the expanded source with all includes copied in)
	// we need to store this here, and before shader compilation,
	// so that any shader compilation errors can be
	// traced down to the correct shader source code line.
	shaders[source.type] = { shaderId, std::move(source) };
	auto & shader = shaders[source.type];

	// compile shader
	const char* sptr = shader.source.expandedSource.c_str();
	int ssize = shader.source.expandedSource.size();
	glShaderSource(shaderId, 1, &sptr, &ssize);
	glCompileShader(shaderId);

	// check compile status
	GLint status = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	GLuint err = glGetError();
	if (err != GL_NO_ERROR){
		ofLogError("ofShader") << "setupShaderFromSource(): OpenGL generated error " << err << " trying to get the compile status for a " << nameForType(shader.source.type) << " shader, does your video card support this?";
		return false;
	}

	if(status == GL_TRUE){
		ofLogVerbose("ofShader") << "setupShaderFromSource(): " << nameForType(shader.source.type) + " shader compiled";
#ifdef TARGET_EMSCRIPTEN
		checkShaderInfoLog(shaderId, source.type, OF_LOG_VERBOSE);
#else
		checkShaderInfoLog(shaderId, source.type, OF_LOG_WARNING);
#endif
	}else if (status == GL_FALSE) {
		ofLogError("ofShader") << "setupShaderFromSource(): " << nameForType(shader.source.type) + " shader failed to compile";
		checkShaderInfoLog(shaderId, source.type, OF_LOG_ERROR);
		return false;
	}
	return true;
}

/*
 * Parse for GLSL includes based on
 * https://www.opengl.org/discussion_boards/showthread.php/169209-include-in-glsl?p=1192415&viewfull=1#post1192415
 */

//--------------------------------------------------------------
string ofShader::parseForIncludes( const string& source, const std::filesystem::path& sourceDirectoryPath) {
	vector<string> included;
	return parseForIncludes( source, included, 0, sourceDirectoryPath);
}

//--------------------------------------------------------------
string ofShader::parseForIncludes( const string& source, vector<string>& included, int level, const std::filesystem::path& sourceDirectoryPath) {

	if ( level > 32 ) {
		ofLogError( "ofShader", "glsl header inclusion depth limit reached, might be caused by cyclic header inclusion" );
		return "";
	}

	stringstream output;
	stringstream input;
	input << source;

	auto match_pragma_include = [](const std::string& s_, std::string& filename_) -> bool {
		filename_ = "";
		std::istringstream s(s_);
		s >> std::ws; // eat up any leading whitespace.

		if (s.peek() != '#') return false;
		// -----| invariant: found '#'
		s.seekg(1, std::ios::cur); // move forward one character

		std::string p, i, f;

		// while skipping whitespace, read in tokens for: pragma, include, and filename
		s >> std::skipws >> p >> i >> f;

		if (p.empty() || i.empty() || (f.size() < 2) ) return false;
		// -----| invariant: all tokens have values

		if (p != "pragma") return false;
		if (i != "include") return false;

		// first and last character of filename token must match and be either
		// '<' and '>', or '"

		if (f[0] == '<' && f[f.size()-1] != '>') return false; //< mismatching brackets

		if ((f[0] == '"' || f[0] == '\'') && (f[0] != f[f.size()-1])) return false; // mismatching quotes

		// invariant: filename properly quoted.

		filename_ = f.substr(1,f.size()-2);

		return true;
	};

	// once std::regex is available across the board, use this regex in favour of the above lambda:
	// std::regex re("^\\s*#\\s*pragma\\s+include\\s+[\"<](.*)[\">].*");

	string line;
	while( std::getline( input, line ) ) {

		string include;

		if (!match_pragma_include(line, include)){
			output << line << endl;
			continue;
		};

		// --------| invariant: '#pragma include' has been requested

		if ( std::find( included.begin(), included.end(), include ) != included.end() ) {
			ofLogVerbose("ofShader") << include << " already included";
			continue;
		}

		// we store the absolute paths so as have (more) unique file identifiers.

		include = ofFile(ofFilePath::join(sourceDirectoryPath, include)).getAbsolutePath();
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
	auto source = shaders.find(type);
	if ( source != shaders.end()) {
		return source->second.source.expandedSource;
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
bool ofShader::checkProgramLinkStatus() {
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
		checkProgramInfoLog();
		return false;
	}
	return true;
}

//--------------------------------------------------------------
void ofShader::checkShaderInfoLog(GLuint shader, GLenum type, ofLogLevel logLevel) {
	GLsizei infoLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		ofBuffer infoBuffer;
		infoBuffer.allocate(infoLength);
		glGetShaderInfoLog(shader, infoLength, &infoLength, infoBuffer.getData());
		ofLog(logLevel, "ofShader: %s shader reports:\n%s", nameForType(type).c_str(), infoBuffer.getText().c_str());
#if (!defined(TARGET_LINUX) || defined(GCC_HAS_REGEX))
		if (shaders.find(type) != shaders.end()) {
			// The following regexp should match shader compiler error messages by Nvidia and ATI.
			// Unfortunately, each vendor's driver formats error messages slightly different.
			std::regex nvidia_ati("^.*[(:]{1}(\\d+)[:)]{1}.*");
			std::regex intel("^[0-9]+:([0-9]+)\\([0-9]+\\):.*$");
			std::smatch matches;
			string infoString = ofTrim(infoBuffer);
			if (std::regex_search(infoString, matches, intel) || std::regex_search(infoString, matches, nvidia_ati)){
				ofBuffer buf;
				buf.set(shaders[type].source.expandedSource);
				ofBuffer::Line line = buf.getLines().begin();
				int  offendingLineNumber = ofToInt(matches[1]);
				ostringstream msg;
				msg << "ofShader: " + nameForType(type) + ", offending line " << offendingLineNumber << " :"<< endl;
				for(int i=0; line != buf.getLines().end(); line++, i++ ){
					string s = *line;
					if ( i >= offendingLineNumber -3 && i < offendingLineNumber + 2 ){
						msg << "\t" << setw(5) << (i+1) << "\t" << s << endl;
					}
				}
				ofLog(logLevel) << msg.str();
			}else{
				ofLog(logLevel) << shaders[type].source.expandedSource;
			}
		}
#endif
	}
}

//--------------------------------------------------------------
void ofShader::checkProgramInfoLog() {
	GLsizei infoLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		ofBuffer infoBuffer;
		infoBuffer.allocate(infoLength);
		glGetProgramInfoLog(program, infoLength, &infoLength, infoBuffer.getData());
		// TODO: it appears that Raspberry Pi only reports shader errors whilst linking,
		// but then it becomes hard to figure out whether the fragment or the
		// vertex shader caused the error.
		// We need to find a robust way of extracing this information from
		// the log, and unfortunately can't use regex whilst gcc on RPi is assumed to
		// be < 4.9, which is the first version fully supporting this c++11 feature.
		string msg = "ofShader: program reports:\n";
		ofLogError("ofShader") << msg + infoBuffer.getText();
#ifdef TARGET_RAPSBERRY_PI
		for(auto it: shaders){
			ofLogNotice("ofShader") << it.second.expandedSource;
		}
#endif
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

		for(auto it: shaders){
			auto shader = it.second;
			if(shader.id>0) {
				ofLogVerbose("ofShader") << "linkProgram(): attaching " << nameForType(it.first) << " shader to program " << program;
				glAttachShader(program, shader.id);
			}
		}

		glLinkProgram(program);

		checkProgramLinkStatus();


		// Pre-cache all active uniforms
		GLint numUniforms = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

		GLint uniformMaxLength = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);

		GLint count = -1;
		GLenum type = 0;
		GLsizei length;
		GLint location;
		vector<GLchar> uniformName(uniformMaxLength);
		for(GLint i = 0; i < numUniforms; i++) {
			glGetActiveUniform(program, i, uniformMaxLength, &length, &count, &type, uniformName.data());
			string name(uniformName.begin(), uniformName.begin()+length);
			// some drivers return uniform_name[0] for array uniforms
			// instead of the real uniform name
			location = glGetUniformLocation(program, name.c_str());
			if (location == -1) continue; // ignore uniform blocks

			uniformsCache[name] = location;
			auto arrayPos = name.find('[');
			if(arrayPos!=std::string::npos){
				name = name.substr(0, arrayPos);
				uniformsCache[name] = location;
			}
		}

#ifndef TARGET_OPENGLES
#ifdef GLEW_ARB_uniform_buffer_object
		if(GLEW_ARB_uniform_buffer_object) {
			// Pre-cache all active uniforms blocks
			GLint numUniformBlocks = 0;
			glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);

			count = -1;
			type = 0;
			vector<GLchar> uniformBlockName(uniformMaxLength);
			for(GLint i = 0; i < numUniformBlocks; i++) {
				glGetActiveUniformBlockName(program, i, uniformMaxLength, &length, uniformBlockName.data() );
				string name(uniformBlockName.begin(), uniformBlockName.begin()+length);
				uniformBlocksCache[name] = glGetUniformBlockIndex(program, name.c_str());
			}
		}
#endif
#endif

#ifdef TARGET_ANDROID
		ofAddListener(ofxAndroidEvents().unloadGL,this,&ofShader::unloadGL);
#endif

		// bLoaded means we have loaded shaders onto the graphics card;
		// it doesn't necessarily mean that these shaders have compiled and linked successfully.
		bLoaded = true;
	}
	return bLoaded;
}


//--------------------------------------------------------------
#ifdef TARGET_ANDROID
void ofShader::unloadGL(){
	for(auto it: shaders) {
		auto shader = it.second;
		if(shader.id) {
			releaseShader(program,shader.id);
		}
	}

	if (program) {
		releaseProgram(program);
		program = 0;
	}
	bLoaded = false;
	ofAddListener(ofxAndroidEvents().reloadGL,this,&ofShader::reloadGL);
}

void ofShader::reloadGL(){
	auto source = shaders;
	auto bindings = attributesBindingsCache;
	shaders.clear();
	uniformsCache.clear();
#ifndef TARGET_OPENGLES
#ifdef GLEW_ARB_uniform_buffer_object // Core in OpenGL 3.1
	uniformBlocksCache.clear();
#endif
#endif
	attributesBindingsCache.clear();
	for(auto & shader: source){
		auto source = shader.second.source;
		setupShaderFromSource(std::move(shader.second.source));
	}
	for(auto binding: bindings){
		bindAttribute(binding.second, binding.first);
	}
	linkProgram();
}
#endif

//--------------------------------------------------------------
void ofShader::bindAttribute(GLuint location, const string & name) const{
	attributesBindingsCache[name] = location;
	glBindAttribLocation(program,location,name.c_str());
}

//--------------------------------------------------------------
bool ofShader::bindDefaults(){
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
		for(auto it: shaders) {
			auto shader = it.second;
			if(shader.id) {
				ofLogVerbose("ofShader") << "unload(): detaching and deleting " << nameForType(shader.source.type) << " shader from program " << program;
				releaseShader(program,shader.id);
			}
		}

		if (program) {
			releaseProgram(program);
			program = 0;
		}

		shaders.clear();
		uniformsCache.clear();
#ifndef TARGET_OPENGLES
#ifdef GLEW_ARB_uniform_buffer_object // Core in OpenGL 3.1
		uniformBlocksCache.clear();
#endif
#endif
		attributesBindingsCache.clear();
#ifdef TARGET_ANDROID
		ofRemoveListener(ofxAndroidEvents().reloadGL,this,&ofShader::reloadGL);
		ofRemoveListener(ofxAndroidEvents().unloadGL,this,&ofShader::unloadGL);
#endif
	}
	bLoaded = false;
}

//--------------------------------------------------------------
bool ofShader::isLoaded() const{
	return bLoaded;
}

//--------------------------------------------------------------
void ofShader::begin()  const{
	ofGetGLRenderer()->bind(*this);
}

//--------------------------------------------------------------
void ofShader::end()  const{
	ofGetGLRenderer()->unbind(*this);
}

#if !defined(TARGET_OPENGLES)
//--------------------------------------------------------------
void ofShader::beginTransformFeedback(GLenum mode) const {
	begin();
	glBeginTransformFeedback(mode);
}

//--------------------------------------------------------------
void ofShader::beginTransformFeedback(GLenum mode, const TransformFeedbackRangeBinding & binding) const {
	binding.buffer.bindRange(GL_TRANSFORM_FEEDBACK_BUFFER, binding.index, binding.offset, binding.size);
	beginTransformFeedback(mode);
}

//--------------------------------------------------------------
void ofShader::beginTransformFeedback(GLenum mode, const std::vector<TransformFeedbackRangeBinding> & bindings) const {
	for (auto & binding : bindings) {
		binding.buffer.bindRange(GL_TRANSFORM_FEEDBACK_BUFFER, binding.index, binding.offset, binding.size);
	}
	beginTransformFeedback(mode);
}

//--------------------------------------------------------------
void ofShader::beginTransformFeedback(GLenum mode, const TransformFeedbackBaseBinding & binding) const {
	binding.buffer.bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, binding.index);
	beginTransformFeedback(mode);
}

//--------------------------------------------------------------
void ofShader::beginTransformFeedback(GLenum mode, const std::vector<TransformFeedbackBaseBinding> & bindings) const {
	for (auto & binding : bindings) {
		binding.buffer.bindBase(GL_TRANSFORM_FEEDBACK_BUFFER, binding.index);
	}
	beginTransformFeedback(mode);
}

//--------------------------------------------------------------
void ofShader::endTransformFeedback() const {
	glEndTransformFeedback();
	end();
}

//--------------------------------------------------------------
void ofShader::endTransformFeedback(const TransformFeedbackRangeBinding & binding) const {
	binding.buffer.unbindRange(GL_TRANSFORM_FEEDBACK_BUFFER, binding.index);
	endTransformFeedback();
}

//--------------------------------------------------------------
void ofShader::endTransformFeedback(const std::vector<TransformFeedbackRangeBinding> & bindings) const {
	for (auto & binding : bindings) {
		binding.buffer.unbindRange(GL_TRANSFORM_FEEDBACK_BUFFER, binding.index);
	}
	endTransformFeedback();
}

//--------------------------------------------------------------
void ofShader::endTransformFeedback(const TransformFeedbackBaseBinding & binding) const {
	binding.buffer.unbindBase(GL_TRANSFORM_FEEDBACK_BUFFER, binding.index);
	endTransformFeedback();
}

//--------------------------------------------------------------
void ofShader::endTransformFeedback(const std::vector<TransformFeedbackBaseBinding> & bindings) const {
	for (auto & binding : bindings) {
		binding.buffer.unbindBase(GL_TRANSFORM_FEEDBACK_BUFFER, binding.index);
	}
	endTransformFeedback();
}
#endif

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
#ifndef TARGET_OPENGLES
			if (texData.bufferId != 0) {
				glTexBuffer(GL_TEXTURE_BUFFER, texData.glInternalFormat, texData.bufferId);
			}
#endif
			glDisable(texData.textureTarget);
		} else {
			glBindTexture(texData.textureTarget, texData.textureID);
#ifndef TARGET_OPENGLES
			if (texData.bufferId != 0) {
				glTexBuffer(GL_TEXTURE_BUFFER, texData.glInternalFormat, texData.bufferId);
			}
#endif
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
void ofShader::setUniform2f(const string & name, const glm::vec2 & v) const{
	setUniform2f(name,v.x,v.y);
}

//--------------------------------------------------------------
void ofShader::setUniform3f(const string & name, const glm::vec3 & v) const{
	setUniform3f(name,v.x,v.y,v.z);
}

//--------------------------------------------------------------
void ofShader::setUniform4f(const string & name, const glm::vec4 & v) const{
	setUniform4f(name,v.x,v.y,v.z,v.w);
}

//--------------------------------------------------------------
void ofShader::setUniform4f(const string & name, const ofFloatColor & v) const{
	setUniform4f(name,v.r,v.g,v.b,v.a);
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
	for(std::size_t i=0;i<parameters.size();i++){
		if(parameters[i].type()==typeid(ofParameter<int>).name()){
			setUniform1i(parameters[i].getEscapedName(),parameters[i].cast<int>());
		}else if(parameters[i].type()==typeid(ofParameter<float>).name()){
			setUniform1f(parameters[i].getEscapedName(),parameters[i].cast<float>());
		}else if(parameters[i].type()==typeid(ofParameter<glm::vec2>).name()){
			setUniform2f(parameters[i].getEscapedName(),parameters[i].cast<glm::vec2>());
		}else if(parameters[i].type()==typeid(ofParameter<glm::vec3>).name()){
			setUniform3f(parameters[i].getEscapedName(),parameters[i].cast<glm::vec3>());
		}else if(parameters[i].type()==typeid(ofParameter<glm::vec4>).name()){
			setUniform4f(parameters[i].getEscapedName(),parameters[i].cast<glm::vec4>());
		}else if(parameters[i].type()==typeid(ofParameter<ofVec2f>).name()){
			setUniform2f(parameters[i].getEscapedName(),parameters[i].cast<glm::vec2>());
		}else if(parameters[i].type()==typeid(ofParameter<ofVec3f>).name()){
			setUniform3f(parameters[i].getEscapedName(),parameters[i].cast<glm::vec3>());
		}else if(parameters[i].type()==typeid(ofParameter<ofVec4f>).name()){
			setUniform4f(parameters[i].getEscapedName(),parameters[i].cast<glm::vec4>());
		}else if(parameters[i].type()==typeid(ofParameterGroup).name()){
			setUniforms((ofParameterGroup&)parameters[i]);
		}
	}
}

//--------------------------------------------------------------
void ofShader::setUniformMatrix3f(const string & name, const glm::mat3 & m, int count)  const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniformMatrix3fv(loc, count, GL_FALSE, glm::value_ptr(m));
	}
}

//--------------------------------------------------------------
void ofShader::setUniformMatrix4f(const string & name, const glm::mat4 & m, int count) const{
	if(bLoaded) {
		int loc = getUniformLocation(name);
		if (loc != -1) glUniformMatrix4fv(loc, count, GL_FALSE, glm::value_ptr(m));
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

//--------------------------------------------------------------
void ofShader::setAttribute1fv(const string & name, const float* v, GLsizei stride) const{
	if(bLoaded){
		GLint location = getAttributeLocation(name);
		if (location != -1) {
			glVertexAttribPointer(location, 1, GL_FLOAT, GL_FALSE, stride, v);
			glEnableVertexAttribArray(location);
		}
	}
}

//--------------------------------------------------------------
void ofShader::setAttribute2fv(const string & name, const float* v, GLsizei stride) const{
	if(bLoaded){
		GLint location = getAttributeLocation(name);
		if (location != -1) {
			glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, v);
			glEnableVertexAttribArray(location);
		}
	}

}

//--------------------------------------------------------------
void ofShader::setAttribute3fv(const string & name, const float* v, GLsizei stride) const{
	if(bLoaded){
		GLint location = getAttributeLocation(name);
		if (location != -1) {
			glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, v);
			glEnableVertexAttribArray(location);
		}
	}
}

//--------------------------------------------------------------
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
	if(!bLoaded) return -1;
	auto it = uniformsCache.find(name);
	if (it == uniformsCache.end()){
		return -1;
	} else {
		return it->second;
	}
}

#ifndef TARGET_OPENGLES
#ifdef GLEW_ARB_uniform_buffer_object
//--------------------------------------------------------------
GLint ofShader::getUniformBlockIndex(const string & name)  const{
	if(!bLoaded) return -1;

	if(GLEW_ARB_uniform_buffer_object) {
		auto it = uniformBlocksCache.find(name);
		if (it == uniformBlocksCache.end()){
			return -1;
		} else {
			return it->second;
		}
	} else {
		ofLogError("ofShader::getUniformBlockIndex") << "Sorry, it looks like you can't run 'ARB_uniform_buffer_object'";
		return -1;
	}
}

//--------------------------------------------------------------
GLint ofShader::getUniformBlockBinding( const string & name ) const{
	if(!bLoaded) return -1;

	if(GLEW_ARB_uniform_buffer_object) {
		GLint index = getUniformBlockIndex(name);
		if (index == -1) return -1;

		GLint blockBinding;
		glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_BINDING, &blockBinding);
		return blockBinding;
	} else {
		ofLogError("ofShader::getUniformBlockBinding") << "Sorry, it looks like you can't run 'ARB_uniform_buffer_object'";
		return -1;
	}
}

//--------------------------------------------------------------
void ofShader::printActiveUniformBlocks()  const{
	if(GLEW_ARB_uniform_buffer_object) {
		GLint numUniformBlocks = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);
		ofLogNotice("ofShader") << numUniformBlocks << " uniform blocks";

		GLint uniformBlockMaxLength = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformBlockMaxLength);

		GLchar* uniformBlockName = new GLchar[uniformBlockMaxLength];
		stringstream line;
		for(GLint i = 0; i < numUniformBlocks; i++) {
			GLsizei length;
			GLint blockBinding;
			GLsizei blockDataSize;
			glGetActiveUniformBlockName(program, i, uniformBlockMaxLength, &length, uniformBlockName );
			glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_BINDING, &blockBinding );
			glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockDataSize );

			line << " [" << i << "] ";
			for(int j = 0; j < length; j++) {
				line << uniformBlockName[j];
			}
			line << " @ index " << getUniformBlockIndex( uniformBlockName ) << ", binding point " << blockBinding << ", size " << blockDataSize << " bytes";
			ofLogNotice("ofShader") << line.str();
			line.str("");
		}
		delete [] uniformBlockName;
	} else {
		ofLogError("ofShader::printActiveUniformBlocks") << "Sorry, it looks like you can't run 'ARB_uniform_buffer_object'";
	}
}


void ofShader::bindUniformBlock(GLuint binding, const string & name) const{
	if(bLoaded){
		if(GLEW_ARB_uniform_buffer_object) {
			GLint index = getUniformBlockIndex(name);
			if (index != -1) {
				glUniformBlockBinding( program, index, binding );
			}
		} else {
			ofLogError("ofShader::bindUniformBlock") << "Sorry, it looks like you can't run 'ARB_uniform_buffer_object'";
		}
	}
}
#endif
#endif

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
	for(GLint i = 0; i < numUniforms; i++) {
		stringstream line;
		GLsizei length;
		GLint location;
		glGetActiveUniform(program, i, uniformMaxLength, &length, &count, &type, uniformName);
		location = glGetUniformLocation(program, uniformName);
		if (location == -1) continue; // ignore uniform blocks

		line << "[" << location << "] ";
		for(int j = 0; j < length; j++) {
			line << uniformName[j];
		}
		line << " @ index " << location;
		ofLogNotice("ofShader") << line.str();
	}
	delete[] uniformName;
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
	auto shader = shaders.find(type);
	if(shader!=shaders.end()){
		return shader->second.id;
	}else{
		return 0;
	}
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


