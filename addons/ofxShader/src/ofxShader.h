#pragma once

/*
 todo: add support for attachment of multiple shaders
 if a uniform or attribute isn't available, this will cause an error
 make sure to catch and report that error.
 */

#include "ofMain.h"
#include <fstream>

class ofxShader {
public:
	ofxShader();
	~ofxShader();
	
	bool setup(string shaderName);
	bool setup(string vertName, string fragName = "", string geomName = "");
	
	
	
	// these are essential to call before linking the program with geometry shaders
	void setGeometryInputType(GLenum type); // type: GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT
	void setGeometryOutputType(GLenum type); // type: GL_POINTS, GL_LINE_STRIP or GL_TRIANGLE_STRIP
	void setGeometryOutputCount(int count);	// set number of output vertices
	
	int getGeometryMaxOutputCount();		// returns maximum number of supported vertices


	void unload();
	
	void begin();
	void end();
	
	// set a texture reference
	void setUniformTexture(const char* name, ofBaseHasTexture& img, int textureLocation);
	void setUniformTexture(const char* name, ofTexture& img, int textureLocation);
	
	// set a single uniform value
	void setUniform1i(const char* name, int v1);
	void setUniform2i(const char* name, int v1, int v2);
	void setUniform3i(const char* name, int v1, int v2, int v3);
	void setUniform4i(const char* name, int v1, int v2, int v3, int v4);
	
	void setUniform1f(const char* name, float v1);
	void setUniform2f(const char* name, float v1, float v2);
	void setUniform3f(const char* name, float v1, float v2, float v3);
	void setUniform4f(const char* name, float v1, float v2, float v3, float v4);
	
	// set an array of uniform values
	void setUniform1iv(const char* name, int* v, int count = 1);
	void setUniform2iv(const char* name, int* v, int count = 1);
	void setUniform3iv(const char* name, int* v, int count = 1);
	void setUniform4iv(const char* name, int* v, int count = 1);
	
	void setUniform1fv(const char* name, float* v, int count = 1);
	void setUniform2fv(const char* name, float* v, int count = 1);
	void setUniform3fv(const char* name, float* v, int count = 1);
	void setUniform4fv(const char* name, float* v, int count = 1);
	
	// set attributes that vary per vertex (look up the location before glBegin)
	GLint getAttributeLocation(const char* name);
	
	void setAttribute1s(GLint location, short v1);
	void setAttribute2s(GLint location, short v1, short v2);
	void setAttribute3s(GLint location, short v1, short v2, short v3);
	void setAttribute4s(GLint location, short v1, short v2, short v3, short v4);
	
	void setAttribute1f(GLint location, float v1);
	void setAttribute2f(GLint location, float v1, float v2);
	void setAttribute3f(GLint location, float v1, float v2, float v3);
	void setAttribute4f(GLint location, float v1, float v2, float v3, float v4);
	
	void setAttribute1d(GLint location, double v1);
	void setAttribute2d(GLint location, double v1, double v2);
	void setAttribute3d(GLint location, double v1, double v2, double v3);
	void setAttribute4d(GLint location, double v1, double v2, double v3, double v4);
	
	void printActiveUniforms();
	void printActiveAttributes();
	

	// advanced use
	
	// these methods create and compile a shader from source or file
	// type: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER_EXT etc.
	bool setupShaderFromSource(GLenum type, string source);
	bool setupShaderFromFile(GLenum type, string filename);
	
	// links program with all compiled shaders
	bool linkProgram();

	GLuint& getProgram();
	GLuint& getShader(GLenum type);
	
protected:
	GLuint program;
	map<GLenum, GLuint> shaders;
	
	
	GLint getUniformLocation(const char* name);
	
	void checkProgramInfoLog(GLuint program);
	bool checkShaderLinkStatus(GLuint shader, GLenum type);
	void checkShaderInfoLog(GLuint shader, GLenum type);
	
	static string nameForType(GLenum type);
	
	void checkAndCreateProgram();
	
	bool bLoaded;
};