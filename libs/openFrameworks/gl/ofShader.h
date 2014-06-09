#pragma once


/*
 todo: add support for attachment of multiple shaders
 if a uniform or attribute isn't available, this will cause an error
 make sure to catch and report that error.
 */

#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofTexture.h"
#include "ofMatrix4x4.h"
#include "Poco/RegularExpression.h"
#include <map>
#include "ofAppBaseWindow.h"

class ofShader {
public:
	ofShader();
	~ofShader();
	ofShader(const ofShader & shader);
	ofShader & operator=(const ofShader & shader);
	
	bool load(string shaderName);
	bool load(string vertName, string fragName, string geomName="");
	
	
	
	// these are essential to call before linking the program with geometry shaders
	void setGeometryInputType(GLenum type); // type: GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT
	void setGeometryOutputType(GLenum type); // type: GL_POINTS, GL_LINE_STRIP or GL_TRIANGLE_STRIP
	void setGeometryOutputCount(int count);	// set number of output vertices
	
	int getGeometryMaxOutputCount();		// returns maximum number of supported vertices


	void unload();
	
	bool isLoaded();

	void begin();
	void end();
	
	// set a texture reference
	void setUniformTexture(const string & name, ofBaseHasTexture& img, int textureLocation);
	void setUniformTexture(const string & name, ofTexture& img, int textureLocation);
	void setUniformTexture(const string & name, int textureTarget, GLint textureID, int textureLocation);
	
	// set a single uniform value
	void setUniform1i(const string & name, int v1);
	void setUniform2i(const string & name, int v1, int v2);
	void setUniform3i(const string & name, int v1, int v2, int v3);
	void setUniform4i(const string & name, int v1, int v2, int v3, int v4);
	
	void setUniform1f(const string & name, float v1);
	void setUniform2f(const string & name, float v1, float v2);
	void setUniform3f(const string & name, float v1, float v2, float v3);
	void setUniform4f(const string & name, float v1, float v2, float v3, float v4);
	
	// set an array of uniform values
	void setUniform1iv(const string & name, int* v, int count = 1);
	void setUniform2iv(const string & name, int* v, int count = 1);
	void setUniform3iv(const string & name, int* v, int count = 1);
	void setUniform4iv(const string & name, int* v, int count = 1);
	
	void setUniform1fv(const string & name, float* v, int count = 1);
	void setUniform2fv(const string & name, float* v, int count = 1);
	void setUniform3fv(const string & name, float* v, int count = 1);
	void setUniform4fv(const string & name, float* v, int count = 1);
	
	void setUniformMatrix4f(const string & name, const ofMatrix4x4 & m);

	// set attributes that vary per vertex (look up the location before glBegin)
	GLint getAttributeLocation(const string & name);

#ifndef TARGET_OPENGLES
	void setAttribute1s(GLint location, short v1);
	void setAttribute2s(GLint location, short v1, short v2);
	void setAttribute3s(GLint location, short v1, short v2, short v3);
	void setAttribute4s(GLint location, short v1, short v2, short v3, short v4);
#endif
	
	void setAttribute1f(GLint location, float v1);
	void setAttribute2f(GLint location, float v1, float v2);
	void setAttribute3f(GLint location, float v1, float v2, float v3);
	void setAttribute4f(GLint location, float v1, float v2, float v3, float v4);

#ifndef TARGET_OPENGLES
	void setAttribute1d(GLint location, double v1);
	void setAttribute2d(GLint location, double v1, double v2);
	void setAttribute3d(GLint location, double v1, double v2, double v3);
	void setAttribute4d(GLint location, double v1, double v2, double v3, double v4);
#endif

	void setAttribute1fv(const string & name, float* v, GLsizei stride=sizeof(float));
	void setAttribute2fv(const string & name, float* v, GLsizei stride=sizeof(float)*2);
	void setAttribute3fv(const string & name, float* v, GLsizei stride=sizeof(float)*3);
	void setAttribute4fv(const string & name, float* v, GLsizei stride=sizeof(float)*4);
	
	void bindAttribute(GLuint location, const string & name);

	void printActiveUniforms();
	void printActiveAttributes();
	

	// advanced use
	
	// these methods create and compile a shader from source or file
	// type: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER_EXT etc.
	bool setupShaderFromSource(GLenum type, string source, string sourceDirectoryPath = "");
	bool setupShaderFromFile(GLenum type, string filename);
	
	// links program with all compiled shaders
	bool linkProgram();

	// binds default uniforms and attributes, only useful for
	// fixed pipeline simulation under programmable renderer
	// has to be called before linking
	bool bindDefaults();

	GLuint& getProgram();
	GLuint& getShader(GLenum type);
	
	bool operator==(const ofShader & other);
	bool operator!=(const ofShader & other);


	// these are used only for openGL ES2 or GL3/4 using the programmable GL renderer
	enum defaultAttributes{
		POSITION_ATTRIBUTE=0,  // tig: was =1, and BOY, what a performance hog this was!!! see: http://www.chromium.org/nativeclient/how-tos/3d-tips-and-best-practices
		COLOR_ATTRIBUTE,
		NORMAL_ATTRIBUTE,
		TEXCOORD_ATTRIBUTE
	};

	/// @brief returns the shader source as it was passed to the GLSL compiler
	/// @param type (GL_VERTEX_SHADER | GL_FRAGMENT_SHADER | GL_GEOMETRY_SHADER_EXT) the shader source you'd like to inspect.
	string getShaderSource(GLenum type);


private:
	GLuint program;
	bool bLoaded;
	map<GLenum, GLuint> shaders;
	map<string, GLint> uniformLocations;
	map<GLenum, string> shaderSource;
	
	GLint getUniformLocation(const string & name);
	
	void checkProgramInfoLog(GLuint program);
	bool checkProgramLinkStatus(GLuint program);
	void checkShaderInfoLog(GLuint shader, GLenum type, ofLogLevel logLevel);
	
	static string nameForType(GLenum type);
	
    /// @brief			Mimics the #include behaviour of the c preprocessor
	/// @description	Includes files specified using the
	///					'#pragma include <filepath>' directive.
	/// @note			Include paths are always specified _relative to the including file's current path_
	///	@note			Recursive #pragma include statements are possible
	/// @note			Includes will be processed up to 32 levels deep
    string parseForIncludes( const string& source, const string& sourceDirectoryPath = "");
    string parseForIncludes( const string& source, vector<string>& included, int level = 0, const string& sourceDirectoryPath = "");
	
	void checkAndCreateProgram();
	

};

