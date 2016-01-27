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
class ofTexture;

enum ofPrimitiveMode{
	OF_PRIMITIVE_TRIANGLES,
	OF_PRIMITIVE_TRIANGLE_STRIP,
	OF_PRIMITIVE_TRIANGLE_FAN,
	OF_PRIMITIVE_LINES,
	OF_PRIMITIVE_LINE_STRIP,
	OF_PRIMITIVE_LINE_LOOP,
	OF_PRIMITIVE_POINTS,
#ifndef TARGET_OPENGLES
    OF_PRIMITIVE_LINES_ADJACENCY,
    OF_PRIMITIVE_LINE_STRIP_ADJACENCY,
    OF_PRIMITIVE_TRIANGLES_ADJACENCY,
    OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY,
    OF_PRIMITIVE_PATCHES
#endif
};

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

shared_ptr<ofBaseGLRenderer> ofGetGLRenderer();


int ofGetGlType(const ofPixels & pixels);
int ofGetGlType(const ofShortPixels & pixels);
int ofGetGlType(const ofFloatPixels & pixels);

ofImageType ofGetImageTypeFromGLType(int glType);

GLuint ofGetGLPolyMode(ofPolyRenderMode m);

ofPolyRenderMode ofGetOFPolyMode(GLuint m);


GLuint ofGetGLPrimitiveMode(ofPrimitiveMode mode);

ofPrimitiveMode ofGetOFPrimitiveMode(GLuint mode);

int ofGetGLInternalFormatFromPixelFormat(ofPixelFormat pixelFormat);
int ofGetGLFormatFromPixelFormat(ofPixelFormat pixelFormat);
int ofGetBytesPerChannelFromGLType(int glType);
int ofGetNumChannelsFromGLFormat(int glFormat);
void ofSetPixelStoreiAlignment(GLenum pname, int w, int bpc, int numChannels);
void ofSetPixelStoreiAlignment(GLenum panme, int stride);

vector<string> ofGLSupportedExtensions();
bool ofGLCheckExtension(string searchName);
bool ofGLSupportsNPOTTextures();

bool ofIsGLProgrammableRenderer();

template<class T>
int ofGetGlFormat(const ofPixels_<T> & pixels) {
	return ofGetGLFormatFromPixelFormat(pixels.getPixelFormat());
}

string ofGLSLVersionFromGL(int major, int minor);


#ifndef TARGET_OPENGLES
	#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
	#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT

	#ifndef GL_UNSIGNED_INT_24_8
		#define GL_UNSIGNED_INT_24_8						GL_UNSIGNED_INT_24_8_EXT
	#endif
#else
    // ES1 - check if GL_FRAMEBUFFER is defined, if not assume ES1 is running.
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

    // ES2 + ES3 - GL_STENCIL_INDEX has been removed from gl header, and now replaced with GL_STENCIL_INDEX8.
    #ifndef GL_STENCIL_INDEX
        #ifdef GL_STENCIL_INDEX8
            #define GL_STENCIL_INDEX                        GL_STENCIL_INDEX8
        #endif
    #endif

	#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES
	#define GL_UNSIGNED_INT_24_8							GL_UNSIGNED_INT_24_8_OES

	#define GL_DEPTH24_STENCIL8								GL_DEPTH24_STENCIL8_OES
	#define GL_DEPTH_STENCIL								GL_DEPTH24_STENCIL8_OES
	#define GL_DEPTH_COMPONENT24							GL_DEPTH_COMPONENT24_OES
	#ifdef GL_DEPTH_COMPONENT32_OES
        #define GL_DEPTH_COMPONENT32						GL_DEPTH_COMPONENT32_OES
    #endif
    #ifdef TARGET_OPENGLES
        #ifndef GL_UNSIGNED_INT
            #define GL_UNSIGNED_INT                         GL_UNSIGNED_INT_OES
        #endif
        #ifndef GL_HALF_FLOAT
            #define GL_HALF_FLOAT                           GL_HALF_FLOAT_OES
        #endif
    #endif
#endif

#endif /* OFGLUTILS_H_ */
