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
#include "ofPixels.h"

class ofShader;
class ofGLProgrammableRenderer;
class ofBaseGLRenderer;

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

int ofGetGlInternalFormat(const ofPixels& pix);
int ofGetGlInternalFormat(const ofShortPixels& pix);
int ofGetGlInternalFormat(const ofFloatPixels& pix);

//---------------------------------
// this is helpful for debugging ofTexture
string ofGetGlInternalFormatName(int glInternalFormat);
int ofGetGLFormatFromInternal(int glInternalFormat);
int ofGetGlTypeFromInternal(int glInternalFormat);

template<class T>
int ofGetGlFormat(const ofPixels_<T> & pixels) {
	switch(pixels.getNumChannels()) {
		case 4:
			return GL_RGBA;
			break;
		case 3:
			return GL_RGB;
			break;
		case 2:
			return GL_LUMINANCE_ALPHA;
			break;
		case 1:
			return GL_LUMINANCE;
			break;

		default:
			ofLogError() << "ofGetGlFormatAndType(): glInternalFormat not recognized returning glFormat as GL_RGBA";
			return GL_RGBA;
			break;
	}
}


int ofGetGlType(const ofPixels & pixels);
int ofGetGlType(const ofShortPixels & pixels);
int ofGetGlType(const ofFloatPixels & pixels);

ofImageType ofGetImageTypeFromGLType(int glType);

GLuint ofGetGLPolyMode(ofPolyRenderMode m);

ofPolyRenderMode ofGetOFPolyMode(GLuint m);


GLuint ofGetGLPrimitiveMode(ofPrimitiveMode mode);

ofPrimitiveMode ofGetOFPrimitiveMode(GLuint mode);

int ofGetGLInternalFormatFromPixelFormat(ofPixelFormat pixelFormat);
int ofGetGLTypeFromPixelFormat(ofPixelFormat pixelFormat);


bool ofCheckGLExtension(string searchName);

ofPtr<ofGLProgrammableRenderer> ofGetGLProgrammableRenderer();
ofPtr<ofBaseGLRenderer> ofGetGLRenderer();

void ofEnableVertices();
void ofEnableTexCoords();
void ofEnableColorCoords();
void ofEnableNormals();
void ofDisableVertices();
void ofDisableTexCoords();
void ofDisableColorCoords();
void ofDisableNormals();

#ifndef TARGET_OPENGLES
	#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT

	#ifndef GL_UNSIGNED_INT_24_8
		#define GL_UNSIGNED_INT_24_8						GL_UNSIGNED_INT_24_8_EXT
	#endif
#else
	#ifndef GL_FRAMEBUFFER
		#define GL_FRAMEBUFFER									GL_FRAMEBUFFER_OES
		#define GL_RENDERBUFFER									GL_RENDERBUFFER_OES
		#define GL_DEPTH_ATTACHMENT								GL_DEPTH_ATTACHMENT_OES
		#define GL_STENCIL_ATTACHMENT							GL_STENCIL_ATTACHMENT_OES
		//#define GL_DEPTH_STENCIL_ATTACHMENT					GL_DEPTH_STENCIL_ATTACHMENT_OES
		#define GL_DEPTH_COMPONENT								GL_DEPTH_COMPONENT16_OES
		#define GL_STENCIL_INDEX								GL_STENCIL_INDEX8_OES
		#define GL_FRAMEBUFFER_BINDING							GL_FRAMEBUFFER_BINDING_OES
		#define GL_MAX_COLOR_ATTACHMENTS						GL_MAX_COLOR_ATTACHMENTS_OES
		#define GL_MAX_SAMPLES									GL_MAX_SAMPLES_OES
		#define GL_READ_FRAMEBUFFER								GL_READ_FRAMEBUFFER_OES
		#define GL_DRAW_FRAMEBUFFER								GL_DRAW_FRAMEBUFFER_OES
		#define GL_WRITE_FRAMEBUFFER							GL_WRITE_FRAMEBUFFER_OES
		#define GL_COLOR_ATTACHMENT0							GL_COLOR_ATTACHMENT0_OES
		#define GL_FRAMEBUFFER_COMPLETE							GL_FRAMEBUFFER_COMPLETE_OES
		#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT			GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES
		#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES
		#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES
		#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER			GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_OES
		#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER			GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_OES
		#define GL_FRAMEBUFFER_UNSUPPORTED						GL_FRAMEBUFFER_UNSUPPORTED_OES
		#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE			GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_OES
		#define GL_DEPTH_COMPONENT16							GL_DEPTH_COMPONENT16_OES
	#endif
	#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES
	#define GL_UNSIGNED_INT_24_8							GL_UNSIGNED_INT_24_8_OES

	#define GL_DEPTH24_STENCIL8								GL_DEPTH24_STENCIL8_OES
	#define GL_DEPTH_STENCIL								GL_DEPTH24_STENCIL8_OES
	#define GL_DEPTH_COMPONENT24							GL_DEPTH_COMPONENT24_OES
	#ifdef GL_DEPTH_COMPONENT32_OES
        #define GL_DEPTH_COMPONENT32						GL_DEPTH_COMPONENT32_OES
    #endif
	#ifdef TARGET_OF_IPHONE
    	#define GL_UNSIGNED_INT                                 GL_UNSIGNED_INT_OES
	#endif
#endif

#endif /* OFGLUTILS_H_ */
