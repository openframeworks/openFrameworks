/*
 * ofGLUtils.h
 *
 *  Created on: 02/03/2011
 *      Author: arturo
 */

#ifndef OFGLUTILS_H_
#define OFGLUTILS_H_

#include "ofConstants.h"
#include "ofTypes.h"
class ofGLES2Renderer;

enum ofPrimitiveMode{
	OF_PRIMITIVE_TRIANGLES,
	OF_PRIMITIVE_TRIANGLE_STRIP,
	OF_PRIMITIVE_TRIANGLE_FAN,
	OF_PRIMITIVE_LINES,
	OF_PRIMITIVE_LINE_STRIP,
	OF_PRIMITIVE_LINE_LOOP,
	OF_PRIMITIVE_POINTS
};

//legacy - 
#define OF_TRIANGLES_MODE 0
#define OF_TRIANGLE_STRIP_MODE 1
#define OF_TRIANGLE_FAN_MODE 2
#define OF_LINES_MODE 3
#define OF_LINE_STRIP_MODE 4
#define OF_LINE_LOOP_MODE 5
#define OF_POINTS_MODE 6

enum ofPolyRenderMode{
	OF_MESH_POINTS,
	OF_MESH_WIREFRAME,
	OF_MESH_FILL
};


GLuint ofGetGLPolyMode(ofPolyRenderMode m);

ofPolyRenderMode ofGetOFPolyMode(GLuint m);


GLuint ofGetGLPrimitiveMode(ofPrimitiveMode mode);

ofPrimitiveMode ofGetOFPrimitiveMode(GLuint mode);

int ofGetGLTypeFromPixelFormat(ofPixelFormat pixelFormat);


bool ofCheckGLExtension(string searchName);

bool ofGLIsFixedPipeline();

ofPtr<ofGLES2Renderer> ofGetGLES2Renderer();

GLint ofGetAttrLocationPosition();
GLint ofGetAttrLocationColor();
GLint ofGetAttrLocationNormal();
GLint ofGetAttrLocationTexCoord();
#endif /* OFGLUTILS_H_ */
