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
	
	void setup(string shaderName);
	void setup(string vertexName, string fragmentName);
	void setupInline(string vertexShaderSource, string fragmentShaderSource);
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
	
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;
	
protected:
	GLint getUniformLocation(const char* name);
	
	void compileShader(GLuint shader, string source, string type);
	void checkProgramInfoLog(GLuint program);
	bool checkShaderLinkStatus(GLuint shader, string type);
	bool checkShaderCompileStatus(GLuint shader, string type);
	void checkShaderInfoLog(GLuint shader, string type);
	
	bool bLoaded;
};