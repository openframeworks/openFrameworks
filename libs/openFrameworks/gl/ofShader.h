#pragma once


/*
 todo: add support for attachment of multiple shaders
 if a uniform or attribute isn't available, this will cause an error
 make sure to catch and report that error.
 */

#include "ofConstants.h"
#include "ofFileUtils.h"
#include "glm/fwd.hpp"
#include <unordered_map>
#include <map>

class ofTexture;
class ofMatrix3x3;
class ofParameterGroup;
class ofBufferObject;
class ofBaseHasTexture;

template<typename T>
class ofColor_;
typedef ofColor_<float> ofFloatColor;

enum ofLogLevel: short;

struct ofShaderSettings {
    std::map<GLuint, std::filesystem::path> shaderFiles;
    std::map<GLuint, std::string> shaderSources;
    std::map<std::string, int> intDefines;
    std::map<std::string, float> floatDefines;
    std::filesystem::path sourceDirectoryPath;
    bool bindDefaults = true;
};

class ofShader {

	struct Source{
		Source(GLuint type, const std::string & source, const std::filesystem::path & directoryPath)
			:type(type)
			,source(source)
			,directoryPath(directoryPath){}

		Source(){}

		GLuint type;
		std::string source;
		std::string expandedSource;
		std::filesystem::path directoryPath;
		std::map<std::string, int>   intDefines;
		std::map<std::string, float> floatDefines;
	};

public:
	ofShader();
	~ofShader();
	ofShader(const ofShader & shader);
	ofShader & operator=(const ofShader & shader);
	ofShader(ofShader && shader);
	ofShader & operator=(ofShader && shader);

	bool load(const std::filesystem::path& shaderName);
	bool load(const std::filesystem::path& vertName, const std::filesystem::path& fragName, const std::filesystem::path& geomName="");
#if !defined(TARGET_OPENGLES) && defined(glDispatchCompute)
	bool loadCompute(const std::filesystem::path& shaderName);
#endif

#if !defined(TARGET_OPENGLES)
	struct TransformFeedbackSettings {
		std::map<GLuint, std::filesystem::path> shaderFiles;
		std::map<GLuint, std::string> shaderSources;
		std::vector<std::string> varyingsToCapture;
		std::map<std::string, int> intDefines;
		std::map<std::string, float> floatDefines;
		std::filesystem::path sourceDirectoryPath;
		bool bindDefaults = true;
		GLuint bufferMode = GL_INTERLEAVED_ATTRIBS; // GL_INTERLEAVED_ATTRIBS or GL_SEPARATE_ATTRIBS
	};

	/// a range of the buffer will be bound with glBindBufferRange
	///
	/// @see: https://www.opengl.org/sdk/docs/man4/html/glBindBufferRange.xhtml
	struct TransformFeedbackRangeBinding {
		TransformFeedbackRangeBinding(const ofBufferObject & buffer, GLuint offset, GLuint size);

		GLuint index = 0;
		GLuint offset = 0;
		GLuint size;
	private:
		const ofBufferObject & buffer;
		friend class ofShader;
	};

	/// full buffer will be bound with glBindBufferBase
	///
	/// @see: https://www.opengl.org/sdk/docs/man4/html/glBindBufferBase.xhtml
	struct TransformFeedbackBaseBinding {
		TransformFeedbackBaseBinding(const ofBufferObject & buffer);

		GLuint index = 0;
	private:
		const ofBufferObject & buffer;
		friend class ofShader;
	};
#endif

	bool setup(const ofShaderSettings & settings);
#if !defined(TARGET_OPENGLES)
	bool setup(const TransformFeedbackSettings & settings);
#endif

	// these are essential to call before linking the program with geometry shaders
	void setGeometryInputType(GLenum type); // type: GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT
	void setGeometryOutputType(GLenum type); // type: GL_POINTS, GL_LINE_STRIP or GL_TRIANGLE_STRIP
	void setGeometryOutputCount(int count);	// set number of output vertices

	int getGeometryMaxOutputCount() const;		// returns maximum number of supported vertices


	void unload();

	bool isLoaded() const;

	void begin() const;
	void end() const;

#if !defined(TARGET_OPENGLES)
	void beginTransformFeedback(GLenum mode) const;
	void beginTransformFeedback(GLenum mode, const TransformFeedbackRangeBinding & binding) const;
	void beginTransformFeedback(GLenum mode, const std::vector<TransformFeedbackRangeBinding> & binding) const;
	void beginTransformFeedback(GLenum mode, const TransformFeedbackBaseBinding & binding) const;
	void beginTransformFeedback(GLenum mode, const std::vector<TransformFeedbackBaseBinding> & binding) const;
	void endTransformFeedback() const;
	void endTransformFeedback(const TransformFeedbackRangeBinding & binding) const;
	void endTransformFeedback(const std::vector<TransformFeedbackRangeBinding> & binding) const;
	void endTransformFeedback(const TransformFeedbackBaseBinding & binding) const;
	void endTransformFeedback(const std::vector<TransformFeedbackBaseBinding> & binding) const;
#endif

#if !defined(TARGET_OPENGLES) && defined(glDispatchCompute)
	void dispatchCompute(GLuint x, GLuint y, GLuint z) const;
#endif

	// set a texture reference
	void setUniformTexture(const std::string & name, const ofBaseHasTexture& img, int textureLocation) const;
	void setUniformTexture(const std::string & name, const ofTexture& img, int textureLocation) const;
	void setUniformTexture(const std::string & name, int textureTarget, GLint textureID, int textureLocation) const;

	// set a single uniform value
	void setUniform1i(const std::string & name, int v1) const;
	void setUniform2i(const std::string & name, int v1, int v2) const;
	void setUniform3i(const std::string & name, int v1, int v2, int v3) const;
	void setUniform4i(const std::string & name, int v1, int v2, int v3, int v4) const;

	void setUniform1f(const std::string & name, float v1) const;
	void setUniform2f(const std::string & name, float v1, float v2) const;
	void setUniform3f(const std::string & name, float v1, float v2, float v3) const;
	void setUniform4f(const std::string & name, float v1, float v2, float v3, float v4) const;

	void setUniform2f(const std::string & name, const glm::vec2 & v) const;
	void setUniform3f(const std::string & name, const glm::vec3 & v) const;
	void setUniform4f(const std::string & name, const glm::vec4 & v) const;
	void setUniform4f(const std::string & name, const ofFloatColor & v) const;

	// set an array of uniform values
	void setUniform1iv(const std::string & name, const int* v, int count = 1) const;
	void setUniform2iv(const std::string & name, const int* v, int count = 1) const;
	void setUniform3iv(const std::string & name, const int* v, int count = 1) const;
	void setUniform4iv(const std::string & name, const int* v, int count = 1) const;

	void setUniform1fv(const std::string & name, const float* v, int count = 1) const;
	void setUniform2fv(const std::string & name, const float* v, int count = 1) const;
	void setUniform3fv(const std::string & name, const float* v, int count = 1) const;
	void setUniform4fv(const std::string & name, const float* v, int count = 1) const;

	void setUniforms(const ofParameterGroup & parameters) const;

	// note: it may be more optimal to use a 4x4 matrix than a 3x3 matrix, if possible
	void setUniformMatrix3f(const std::string & name, const glm::mat3 & m, int count = 1) const;
	void setUniformMatrix4f(const std::string & name, const glm::mat4 & m, int count = 1) const;

	GLint getUniformLocation(const std::string & name) const;

	// set attributes that vary per vertex (look up the location before glBegin)
	GLint getAttributeLocation(const std::string & name) const;

#ifndef TARGET_OPENGLES
#ifdef GLEW_ARB_uniform_buffer_object
	GLint getUniformBlockIndex(const std::string & name) const;
	GLint getUniformBlockBinding(const std::string & name) const;
	void bindUniformBlock(GLuint bindind, const std::string & name) const;
	void printActiveUniformBlocks() const;
#endif
#endif

#ifndef TARGET_OPENGLES
	void setAttribute1s(GLint location, short v1) const;
	void setAttribute2s(GLint location, short v1, short v2) const;
	void setAttribute3s(GLint location, short v1, short v2, short v3) const;
	void setAttribute4s(GLint location, short v1, short v2, short v3, short v4) const;
#endif

	void setAttribute1f(GLint location, float v1) const;
	void setAttribute2f(GLint location, float v1, float v2) const;
	void setAttribute3f(GLint location, float v1, float v2, float v3) const;
	void setAttribute4f(GLint location, float v1, float v2, float v3, float v4) const;

#ifndef TARGET_OPENGLES
	void setAttribute1d(GLint location, double v1) const;
	void setAttribute2d(GLint location, double v1, double v2) const;
	void setAttribute3d(GLint location, double v1, double v2, double v3) const;
	void setAttribute4d(GLint location, double v1, double v2, double v3, double v4) const;
#endif

	void setAttribute1fv(const std::string & name, const float* v, GLsizei stride=sizeof(float)) const;
	void setAttribute2fv(const std::string & name, const float* v, GLsizei stride=sizeof(float)*2) const;
	void setAttribute3fv(const std::string & name, const float* v, GLsizei stride=sizeof(float)*3) const;
	void setAttribute4fv(const std::string & name, const float* v, GLsizei stride=sizeof(float)*4) const;

	void bindAttribute(GLuint location, const std::string & name) const;

	void printActiveUniforms() const;
	void printActiveAttributes() const;


	// advanced use

	// these methods create and compile a shader from source or file
	// type: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER_EXT etc.
	bool setupShaderFromSource(GLenum type, std::string source, std::string sourceDirectoryPath = "");
	bool setupShaderFromFile(GLenum type, const std::filesystem::path& filename);

	// links program with all compiled shaders
	bool linkProgram();

	// binds default uniforms and attributes, only useful for
	// fixed pipeline simulation under programmable renderer
	// has to be called before linking
	bool bindDefaults();

	GLuint getProgram() const;
	GLuint getShader(GLenum type) const;

	bool operator==(const ofShader & other) const;
	bool operator!=(const ofShader & other) const;


	// these are used only for openGL ES2 or GL3/4 using the programmable GL renderer
	enum defaultAttributes{
		POSITION_ATTRIBUTE=0,  // tig: was =1, and BOY, what a performance hog this was!!! see: http://www.chromium.org/nativeclient/how-tos/3d-tips-and-best-practices
		COLOR_ATTRIBUTE,
		NORMAL_ATTRIBUTE,
		TEXCOORD_ATTRIBUTE,
		INDEX_ATTRIBUTE  // usually not used except for compute shades
	};

	/// @brief returns the shader source as it was passed to the GLSL compiler
	/// @param type (GL_VERTEX_SHADER | GL_FRAGMENT_SHADER | GL_GEOMETRY_SHADER_EXT) the shader source you'd like to inspect.
	std::string getShaderSource(GLenum type) const;


private:
	GLuint program = 0;
	bool bLoaded = false;

	struct Shader{
		GLuint id;
		Source source;
	};

	std::unordered_map<GLenum, Shader> shaders;
	std::unordered_map<std::string, GLint> uniformsCache;
	mutable std::unordered_map<std::string, GLint> attributesBindingsCache;

#ifndef TARGET_OPENGLES
	std::unordered_map<std::string, GLint> uniformBlocksCache;
#endif

	bool setupShaderFromSource(Source && source);
	ofShader::Source sourceFromFile(GLenum type, const std::filesystem::path& filename);
	void checkProgramInfoLog();
	bool checkProgramLinkStatus();
	void checkShaderInfoLog(GLuint shader, GLenum type, ofLogLevel logLevel);
	template<typename T>
	void setDefineConstantTemp(const std::string & name, T value);
	template<typename T>
	void setConstantTemp(const std::string & name, const std::string & type, T value);
	
	static std::string nameForType(GLenum type);

	/// @brief			Mimics the #include behaviour of the c preprocessor
	/// @description	Includes files specified using the
	///					'#pragma include <filepath>' directive.
	/// @note			Include paths are always specified _relative to the including file's current path_
	///	@note			Recursive #pragma include statements are possible
	/// @note			Includes will be processed up to 32 levels deep
	static std::string parseForIncludes( const std::string& source, const std::filesystem::path& sourceDirectoryPath = "");
	static std::string parseForIncludes( const std::string& source, std::vector<std::string>& included, int level = 0, const std::filesystem::path& sourceDirectoryPath = "");

	void checkAndCreateProgram();
#ifdef TARGET_ANDROID
	void unloadGL();
	void reloadGL();
#endif
};

