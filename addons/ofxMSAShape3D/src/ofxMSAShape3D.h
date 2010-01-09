/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 

#include "ofMain.h"

#define DEFAULT_RESERVE_AMOUNT		5000

#define SIZEOF_FLOAT		4

//#define USE_IMMEDIATE_MODE

class ofxMSAShape3D {
public:
	ofxMSAShape3D() {
		vertexArray = normalArray = colorArray = texCoordArray = 0;
		colorEnabled = normalEnabled = texCoordEnabled = false;
		setSafeMode(true);
		reserve(DEFAULT_RESERVE_AMOUNT);
	}
	
	~ofxMSAShape3D() {
		if(vertexArray) free(vertexArray);
		if(normalArray) free(normalArray);
		if(colorArray) free(colorArray);
		if(texCoordArray) free(texCoordArray);	
	}
	
	// reserve space for this many vertices
	// Not actually nessecary, arrays are resized automatically...
	// ... but reserving the size upfront improves performance
	inline void reserve(int reservedSize) {
		ofLog(OF_LOG_VERBOSE, "ofxMSAShape3D::reserve: " + ofToString(reservedSize));
		
		this->reservedSize = reservedSize;
		
		vertexArray		= (float*)realloc(vertexArray, 3 * reservedSize * SIZEOF_FLOAT);
		normalArray		= (float*)realloc(normalArray, 3 * reservedSize * SIZEOF_FLOAT);
		colorArray		= (float*)realloc(colorArray, 4 * reservedSize * SIZEOF_FLOAT);
		texCoordArray	= (float*)realloc(texCoordArray, 2 * reservedSize * SIZEOF_FLOAT);
		
		if(safeMode == false) setClientStates();
		
		reset();
	}
	
	// similar to OpenGL glBegin
	// starts primitive draw mode
	inline void begin(GLenum drawMode) {
#ifndef USE_IMMEDIATE_MODE
		this->drawMode = drawMode;
		reset();
#else	
		glBegin(drawMode);
#endif	
	}
	
	// similar to OpenGL glEnd()
	// sends all data to server to be drawn
	inline void end() {
#ifndef USE_IMMEDIATE_MODE
		if(safeMode) setClientStates();
		glDrawArrays(drawMode, 0, numVertices);
		if(safeMode) restoreClientStates();
#else
		glEnd();
#endif	
	}
	
	// redraws currently cached shape
	inline void draw() {
		this->end();
	}
	
	
	// vertex position methods
	// x,y,z coordinates (if z is omitted, assumed 0)	
	inline void addVertex(float x, float y, float z = 0) {
#ifndef USE_IMMEDIATE_MODE
		if(safeMode) {
			if(numVertices >= reservedSize) reserve(reservedSize * 1.1);      // if we hit limit, increase reserve by 10%
			memcpy(normalArray + numVertices*3, curNormal, 3*SIZEOF_FLOAT);
			memcpy(colorArray + numVertices*4, curColor, 4*SIZEOF_FLOAT);
			memcpy(texCoordArray + numVertices*2, curTexCoord, 2*SIZEOF_FLOAT);
		} else {
			if(normalEnabled) memcpy(normalArray + numVertices*3, curNormal, 3*SIZEOF_FLOAT);
			if(colorEnabled) memcpy(colorArray + numVertices*4, curColor, 4*SIZEOF_FLOAT);
			if(texCoordEnabled) memcpy(texCoordArray + numVertices*2, curTexCoord, 2*SIZEOF_FLOAT);
		}
		
		vertexArray[vertexIndex++]      = x;
		vertexArray[vertexIndex++]      = y;
		vertexArray[vertexIndex++]      = z;
		
		numVertices++;
#else
		glVertex3f(x, y, z);
#endif   
	}

	// pointer to x,y,z coordinates
	inline void addVertex3v(float *v) {
			this->addVertex(v[0], v[1], v[2]);
	}					
	
	// pointer to x,y coordinates. z is assumed 0
	inline void addVertex2v(float *v) {
			this->addVertex(v[0], v[1]);
	}						
	
	
	
	// normal methods
	// x,y,z components of normal
	inline void setNormal(float x, float y, float z) {
#ifndef USE_IMMEDIATE_MODE
		if(safeMode) normalEnabled = true;
		curNormal[0] = x;
		curNormal[1] = y;
		curNormal[2] = z;
#endif
		glNormal3f(x, y, z);
	}
	
	
	inline void setNormal3v(float *v) {
		this->setNormal(v[0], v[1], v[2]);
	}						// pointer to x,y,z components of normal
	
	// color methods
	// r,g,b,a color components (if a is omitted, assumed 0)	
	inline void setColor(float r, float g, float b, float a = 1) {
#ifndef USE_IMMEDIATE_MODE
		if(safeMode) colorEnabled = true;
		curColor[0] = r;
		curColor[1] = g;
		curColor[2] = b;
		curColor[3] = a;
#endif	
		glColor4f(r, g, b, a);
	}
	
	// 0xFFFFFF hex color, alpha is assumed 1	
	inline void setColor(int hexColor) {
		float r = ((hexColor >> 16) & 0xff) * 1.0f/255;
		float g = ((hexColor >> 8) & 0xff)  * 1.0f/255;
		float b = ((hexColor >> 0) & 0xff)  * 1.0f/255;
		this->setColor(r, g, b);
	}		
	
	// pointer to r,g,b components. alpha is assumed 1
	inline void setColor3v(float *v) {
		this->setColor(v[0], v[1], v[2]);
	}						
	
	// pointer to r,g,b,a components
	inline void setColor4v(float *v) {
		this->setColor(v[0], v[1], v[2], v[3]);		
	}						
	
	
	// texture coordinate methods
	// u,v texture coordinates
	inline void setTexCoord(float u, float v) {
#ifndef USE_IMMEDIATE_MODE
		if(safeMode) texCoordEnabled = true;
		curTexCoord[0] = u;
		curTexCoord[1] = v;
#else		
		glTexCoord2f(u, v);
#endif	
	}				
	
	// pointer to u,v texture coordinates
	inline void setTexCoord2v(float *v) {
		this->setTexCoord(v[0], v[1]);
	}					
	
	
	// rectangle from (x1, y1) to (x2, y2)
	inline void drawRect(float x1, float y1, float x2, float y2) {
		this->begin(GL_TRIANGLE_STRIP);
		this->addVertex(x1, y1);
		this->addVertex(x2, y1);
		this->addVertex(x1, y2);
		this->addVertex(x2, y2);
		this->end();
	}
	

	
	/********* Advanced use ***********/
	
	// safe mode is TRUE by default
	// if safe mode is on, all clientstates are set as needed in end()
	// this may not be efficient if lots of shapes are drawn
	// by disabling safe mode, you can manually set the clientstates (using the below methods)
	// and avoid this overhead
	// if you don't know what this means, don't touch it
	inline void setSafeMode(bool b) {
		safeMode = b;
	}
	
	// set whether the shape will be using any of the below
	inline void enableNormal(bool b) {
		if(safeMode) normalWasEnabled = glIsEnabled(GL_NORMAL_ARRAY);
		if(normalEnabled = b) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, &normalArray[0]);
		}
		else glDisableClientState(GL_NORMAL_ARRAY);
	}
	
	inline void enableColor(bool b) {
		if(safeMode) colorWasEnabled = glIsEnabled(GL_COLOR_ARRAY);
		if(colorEnabled = b) {
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, 0, &colorArray[0]);
		}
		else glDisableClientState(GL_COLOR_ARRAY);	
	}
	
	inline void enableTexCoord(bool b) {
		if(safeMode) texCoordWasEnabled = glIsEnabled(GL_TEXTURE_COORD_ARRAY);
		if(texCoordEnabled = b) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, &texCoordArray[0]);
		}
		else glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	// enables or disables all client states based on information provided in above methods
	inline void setClientStates() {
		enableColor(colorEnabled);
		enableNormal(normalEnabled);
		enableTexCoord(texCoordEnabled);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &vertexArray[0]);
	}
	
	inline void restoreClientStates() {
		restoreState(GL_NORMAL_ARRAY, normalEnabled, normalWasEnabled);
		restoreState(GL_COLOR_ARRAY, colorEnabled, colorWasEnabled);
		restoreState(GL_TEXTURE_COORD_ARRAY, texCoordEnabled, texCoordWasEnabled);
	}
	
	
protected:
	inline void reset() {
		if(safeMode) {
			colorEnabled		= false;
			normalEnabled		= false;
			texCoordEnabled		= false;
		}
		numVertices			= 0;
		
		vertexIndex = 0;
//		normalIndex = colorIndex = texCoordIndex = 0;
	}
	
	inline void restoreState(int flag, bool curState, bool oldState) {
		if(curState == oldState) return;
		if(oldState) glEnableClientState(flag);
		else glDisableClientState(flag);
		curState = oldState;
	}
	
	float	*vertexArray;	// 3
	float	*normalArray;	// 3
	float	*colorArray;	// 4
	float	*texCoordArray;	// 2
	int		sizeOfFloat;

	
	float	curNormal[3];
	float	curColor[4];
	float	curTexCoord[2];
	
	bool	normalEnabled;
	bool	colorEnabled;
	bool	texCoordEnabled;
	
	bool	normalWasEnabled;
	bool	colorWasEnabled;
	bool	texCoordWasEnabled;
	
	bool	safeMode;
	
	int		numVertices;
	int		vertexIndex;
//	int		normalIndex;
//	int		colorIndex;
//	int		texCoordIndex;	

	int		reservedSize;
	GLenum	drawMode;
	
	
};
