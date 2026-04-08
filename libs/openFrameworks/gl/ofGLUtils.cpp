#include "ofGLUtils.h"
#include "ofGLProgrammableRenderer.h"
#include "ofGraphics.h"
#include "ofShader.h"
#include "ofGraphicsBaseTypes.h"
#include "ofRendererCollection.h"
#include "ofGLRenderer.h"
#include "ofPixels.h"
#include "ofLog.h"
#include "ofGraphicsConstants.h"
#include <set>

using std::shared_ptr;
using std::vector;
using std::string;

//---------------------------------
// deprecations

int ofGetGlInternalFormat(const ofPixels& pixels) {
    return ofGetGLInternalFormat(pixels);
}

int ofGetGlInternalFormat(const ofShortPixels& pixels) {
    return ofGetGLInternalFormat(pixels);
}

int ofGetGlInternalFormat(const ofFloatPixels& pixels) {
    return ofGetGLInternalFormat(pixels);
}

std::string ofGetGlInternalFormatName(int glInternalFormat) {
    return ofGetGLInternalFormatName(glInternalFormat);
}

int ofGetGlTypeFromInternal(int glInternalFormat) {
    return ofGetGLTypeFromInternal(glInternalFormat);
}

int ofGetGlType(const ofPixels & pixels) {
    return ofGetGLType(pixels);
}

int ofGetGlType(const ofShortPixels & pixels) {
    return ofGetGLType(pixels);
}

int ofGetGlType(const ofFloatPixels & pixels) {
    return ofGetGLType(pixels);
}

//---------------------------------
int ofGetGLInternalFormat(const ofPixels& pixels) {
	return ofGetGLInternalFormatFromPixelFormat(pixels.getPixelFormat());
}

//---------------------------------
int ofGetGLInternalFormat(const ofShortPixels& pixels) {
#ifndef TARGET_OPENGLES
	switch(pixels.getNumChannels()) {
		case 3: return GL_RGB16;
		case 4: return GL_RGBA16;
		case 2:
			if(ofIsGLProgrammableRenderer()){
				return GL_RG16;
			}else{
				return GL_LUMINANCE16_ALPHA16;
			}
		default:
			if(ofIsGLProgrammableRenderer()){
				return GL_R16;
			}else{
				return GL_LUMINANCE16;
			}
	}
#else
	ofLogWarning("ofGLUtils") << "ofGetGLInternalFormat(): 16bit textures are not supported in OpenGL ES";
	switch(pixels.getNumChannels()) {
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		case 2:
			return GL_LUMINANCE_ALPHA;
		default:
			return GL_LUMINANCE;
	}
#endif
}

//---------------------------------
int ofGetGLInternalFormat(const ofFloatPixels& pixels) {
#if defined(TARGET_EMSCRIPTEN)
    // WebGL2 has unreliable / slower support for 32F float textures
    // → we force 16F for stability and performance (matches current master)
    switch(pixels.getNumChannels()) {
    case 3: return GL_RGB16F;
    case 4: return GL_RGBA16F;
    case 2:
        ofLogWarning("ofGLUtils") << "ofGetGLInternalFormat(): two channel float textures not supported on Emscripten.";
        return GL_RG16F;
    default:
        ofLogWarning("ofGLUtils") << "ofGetGLInternalFormat(): single channel float textures not supported on Emscripten.";
        return GL_R16F;
    }
#elif !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    // Desktop + native GLES 3.0+ → full 32F support
    switch(pixels.getNumChannels()) {
    case 3: return GL_RGB32F;
    case 4: return GL_RGBA32F;
    case 2:
        return GL_RG32F;
    default:
        return GL_R32F;
    }
#else
    // GLES 2.0 fallback
    ofLogWarning("ofGLUtils") << "ofGetGLInternalFormat(): float textures not supported in OpenGL ES < 3.0";
    switch(pixels.getNumChannels()) {
    case 3: return GL_RGB;
    case 4: return GL_RGBA;
    case 2: return GL_LUMINANCE_ALPHA;
    default: return GL_LUMINANCE;
    }
#endif
}

//---------------------------------
// this is helpful for debugging ofTexture
string ofGetGLInternalFormatName(int glInternalFormat) {
	switch(glInternalFormat) {
		case GL_RGBA: return "GL_RGBA";
#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
		case GL_RGBA8: return "GL_RGBA8";
#endif
		case GL_RGB: return "GL_RGB";
#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
		case GL_RGB8: return "GL_RGB8";
#endif
		case GL_LUMINANCE: return "GL_LUMINANCE";
#ifndef TARGET_OPENGLES
		case GL_LUMINANCE8: return "GL_LUMINANCE8";
		case GL_RGBA16: return "GL_RGBA16";
		case GL_RGB16: return "GL_RGB16";
		case GL_LUMINANCE16: return "GL_LUMINANCE16";
		case GL_RGBA32F_ARB: return "GL_RGBA32F_ARB";
		case GL_RGB32F_ARB: return "GL_RGB32F_ARB";
		case GL_LUMINANCE32F_ARB: return "GL_LUMINANCE32F_ARB";
#endif
		case GL_LUMINANCE_ALPHA: return "GL_LUMINANCE_ALPHA";
#ifndef TARGET_OPENGLES
		case GL_LUMINANCE8_ALPHA8: return "GL_LUMINANCE8_ALPHA8";
#endif
		default: return "unknown glInternalFormat";
	}
}

int ofGetGLFormatFromInternal(int glInternalFormat){
    switch(glInternalFormat) {
    case GL_RGBA:
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    case GL_RGBA8:
    case GL_RGBA16F:
    #endif
    #ifndef TARGET_OPENGLES
    case GL_RGBA16:
    case GL_RGBA32F_ARB:
    case GL_RGBA16I:
    case GL_RGBA16UI:
    case GL_RGBA32I:
    case GL_RGBA32UI:
    #endif
        return GL_RGBA;

#ifdef TARGET_OF_IOS
    case GL_BGRA:
        return GL_BGRA;
#endif

    case GL_RGB:
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    case GL_RGB8:
    case GL_RGB16F:
    #endif
    #ifndef TARGET_OPENGLES
    case GL_RGB16:
    case GL_RGB32F_ARB:
    #endif
        return GL_RGB;

    case GL_LUMINANCE:
    #if !defined(TARGET_OPENGLES)
    case GL_LUMINANCE8:
    case GL_LUMINANCE16:
    case GL_LUMINANCE32F_ARB:
    #endif
        return GL_LUMINANCE;

    case GL_LUMINANCE_ALPHA:
    #if !defined(TARGET_OPENGLES)
    case GL_LUMINANCE8_ALPHA8:
    case GL_LUMINANCE16_ALPHA16:
    case GL_LUMINANCE_ALPHA32F_ARB:
    #endif
        return GL_LUMINANCE_ALPHA;

    case GL_DEPTH_STENCIL:
        return GL_DEPTH_STENCIL;

    case GL_DEPTH_COMPONENT:
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    case GL_DEPTH_COMPONENT16:
    case GL_DEPTH_COMPONENT24:
    #endif
    #if defined(TARGET_OPENGLES) && defined(GL_ES_VERSION_3_0)
    case GL_DEPTH_COMPONENT32F:
    #endif
    #ifndef TARGET_OPENGLES
    case GL_DEPTH_COMPONENT32:
    #endif
        return GL_DEPTH_COMPONENT;

    case GL_STENCIL_INDEX:
        return GL_STENCIL_INDEX;

    // modern red / RG formats (used for float + half-float textures)
	#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
	case GL_R8:
	case GL_R16F:
	case GL_R32F:
	case GL_RG8:
	case GL_RG16F:
	case GL_RG32F:
	#endif
	#ifndef TARGET_OPENGLES
	case GL_R16:
	case GL_RG16:
		return (glInternalFormat == GL_R8 || glInternalFormat == GL_R16F || glInternalFormat == GL_R32F) ? GL_RED : GL_RG;
	#endif

	#if !defined(TARGET_OPENGLES) && defined(GL_RGB565)
    case GL_RGB565:
        return GL_UNSIGNED_SHORT_5_6_5;
    #endif

    #ifndef TARGET_OPENGLES
    case GL_ALPHA8:
    #endif
    case GL_ALPHA:
        return GL_ALPHA;

    default:
        ofLogError("ofGLUtils") << "ofGetGLFormatFromInternal(): unknown internal format " << glInternalFormat << ", returning GL_RGBA";
        return GL_RGBA;
    }
}

int ofGetGLTypeFromInternal(int glInternalFormat){
    switch(glInternalFormat) {
    // 8-bit unsigned byte formats (most common on all platforms)
    case GL_RGB:
    case GL_RGBA:
    case GL_LUMINANCE:
    case GL_LUMINANCE_ALPHA:
    case GL_ALPHA:
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    #ifndef TARGET_OPENGLES
    case GL_LUMINANCE8:
    case GL_LUMINANCE8_ALPHA8:
    case GL_ALPHA8:
    #endif
    case GL_R8:
    case GL_RG8:
    case GL_RGB8:
    case GL_RGBA8:
    #endif
        return GL_UNSIGNED_BYTE;

    // 16-bit integer formats (desktop only)
    #ifndef TARGET_OPENGLES
    case GL_LUMINANCE16:
    case GL_LUMINANCE16_ALPHA16:
    case GL_R16:
    case GL_RG16:
    case GL_RGB16:
    case GL_RGBA16:
        return GL_UNSIGNED_SHORT;
    #endif

    // 16-bit half-float formats (GLES 3.0 core + Emscripten)
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    case GL_R16F:
    case GL_RG16F:
    case GL_RGB16F:
    case GL_RGBA16F:
        #ifdef GL_HALF_FLOAT
            return GL_HALF_FLOAT;          // core in GLES 3.0+
        #else
            return GL_HALF_FLOAT_OES;      // safe fallback for older headers
        #endif
    #endif

    // 32-bit full float formats (native ES3 + desktop)
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    case GL_R32F:
    case GL_RG32F:
    case GL_RGB32F:
    case GL_RGBA32F:
        return GL_FLOAT;
    #endif

    #ifndef TARGET_OPENGLES
    // legacy desktop luminance float formats
    case GL_LUMINANCE32F_ARB:
    case GL_LUMINANCE_ALPHA32F_ARB:
        return GL_FLOAT;
    #endif

    // depth / stencil formats
    case GL_DEPTH_STENCIL:
        return GL_UNSIGNED_INT_24_8;

    case GL_DEPTH_COMPONENT:
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    case GL_DEPTH_COMPONENT16:
    case GL_R16UI:
    case GL_RG16UI:
    case GL_RGB16UI:
    case GL_RGBA16UI:
    #endif
        return GL_UNSIGNED_SHORT;

    #if defined(TARGET_OPENGLES) && defined(GL_ES_VERSION_3_0)
    case GL_DEPTH_COMPONENT24:
        return GL_UNSIGNED_INT;
    case GL_DEPTH_COMPONENT32F:
        return GL_FLOAT;
    #endif

    #ifndef TARGET_OPENGLES
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT32:
    case GL_R32UI:
    case GL_RG32UI:
    case GL_RGB32UI:
    case GL_RGBA32UI:
        return GL_UNSIGNED_INT;

    case GL_R16I:
    case GL_RG16I:
    case GL_RGB16I:
    case GL_RGBA16I:
        return GL_SHORT;

    case GL_R32I:
    case GL_RG32I:
    case GL_RGB32I:
    case GL_RGBA32I:
        return GL_INT;
    #endif

    case GL_STENCIL_INDEX:
        return GL_UNSIGNED_BYTE;

    // legacy RGB565 (very old desktop/Android)
    #if !defined(TARGET_OPENGLES) && defined(GL_RGB565)
    case GL_RGB565:
        return GL_UNSIGNED_SHORT_5_6_5;
    #endif

    default:
        ofLogError("ofGLUtils") << "ofGetGLTypeFromInternal(): unknown internal format " << glInternalFormat << ", returning GL_UNSIGNED_BYTE";
        return GL_UNSIGNED_BYTE;
    }
}

//---------------------------------
int ofGetGLType(const ofPixels & pixels) {
#ifndef TARGET_OPENGLES
	if(pixels.getPixelFormat() == OF_PIXELS_RGB565){
		return GL_UNSIGNED_SHORT_5_6_5;
	}else{
#endif
		return GL_UNSIGNED_BYTE;
#ifndef TARGET_OPENGLES
	}
#endif
}

//---------------------------------
int ofGetGLType(const ofShortPixels & pixels) {
	return GL_UNSIGNED_SHORT;
}

//---------------------------------
int ofGetGLType(const ofFloatPixels & pixels) {
	return GL_FLOAT;
}

//---------------------------------
ofImageType ofGetImageTypeFromGLType(int glType){
    switch(glType){
    case GL_LUMINANCE:
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    #ifndef TARGET_OPENGLES
    case GL_LUMINANCE8:
    case GL_LUMINANCE16:
    case GL_LUMINANCE32F_ARB:
    case GL_R8:
    case GL_R16:
    #endif
    case GL_R16F:
    case GL_R16I:
    case GL_R16UI:
    case GL_R32F:
    case GL_R32I:
    case GL_R32UI:
    case GL_DEPTH_COMPONENT32F:
    #ifndef TARGET_OPENGLES
    case GL_DEPTH_COMPONENT32:
    #endif
    case GL_DEPTH_COMPONENT16:
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT:
    #endif
        return OF_IMAGE_GRAYSCALE;

    case GL_RGB:
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    case GL_RGB8:
    #ifndef TARGET_OPENGLES
    case GL_RGB16:
    #endif
    case GL_RGB16F:
    case GL_RGB16I:
    case GL_RGB16UI:
    case GL_RGB32F:
    case GL_RGB32I:
    case GL_RGB32UI:
    #endif
        return OF_IMAGE_COLOR;

    case GL_RGBA:
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
    case GL_RGBA8:
    #ifndef TARGET_OPENGLES
    case GL_RGBA16:
    #endif
    case GL_RGBA16F:
    case GL_RGBA16I:
    case GL_RGBA16UI:
    case GL_RGBA32F:
    case GL_RGBA32I:
    case GL_RGBA32UI:
    #endif
        return OF_IMAGE_COLOR_ALPHA;
    }
    return OF_IMAGE_UNDEFINED;
}

GLuint ofGetGLPolyMode(ofPolyRenderMode mode){
#if !defined(TARGET_OPENGLES)
	switch(mode){
		case OF_MESH_POINTS:
			return GL_POINT;
		case OF_MESH_WIREFRAME:
			return GL_LINE;
		case OF_MESH_FILL:
			return GL_FILL;
		default:
			ofLogError("ofGLUtils") << "ofGetGLPolyMode(): unknown OF poly mode " << ofToString(mode) << ", returning GL_FILL";
			return GL_FILL;
	}
#else
	ofLogError("ofGLUtils") << "ofGetGLPolyMode(): poly modes not supported in OpenGL ES < 3.0";
	return 0;
#endif
}

ofPolyRenderMode ofGetOFPolyMode(GLuint mode){
#if !defined(TARGET_OPENGLES)
	switch(mode){
		case GL_POINT:
			return OF_MESH_POINTS;
		case GL_LINE:
			return OF_MESH_WIREFRAME;
		case GL_FILL:
			return OF_MESH_FILL;
		default:
			ofLogError("ofGLUtils") << "ofGetOFPolyMode(): unknown GL poly mode " << ofToString(mode) << ", returning OF_MESH_FILL";
			return OF_MESH_FILL;
	}
#else
	ofLogError("ofGLUtils") << "ofGetOFPolyMode(): poly modes not supported in OpenGL ES < 3.0. Returning OF_MESH_FILL";
	return OF_MESH_FILL;
#endif
}

GLuint ofGetGLPrimitiveMode(ofPrimitiveMode mode){
	switch(mode){
		case OF_PRIMITIVE_TRIANGLES:
			return GL_TRIANGLES;
			break;
		case OF_PRIMITIVE_TRIANGLE_STRIP:
			return GL_TRIANGLE_STRIP;
			break;
		case OF_PRIMITIVE_TRIANGLE_FAN:
			return GL_TRIANGLE_FAN;
			break;
		case OF_PRIMITIVE_LINES:
			return GL_LINES;
			break;
		case OF_PRIMITIVE_LINE_STRIP:
			return GL_LINE_STRIP;
			break;
		case OF_PRIMITIVE_LINE_LOOP:
			return GL_LINE_LOOP;
			break;
		case OF_PRIMITIVE_POINTS:
			return GL_POINTS;
			break;
#ifndef TARGET_OPENGLES
		case OF_PRIMITIVE_LINES_ADJACENCY:
			return GL_LINES_ADJACENCY;
			break;
		case OF_PRIMITIVE_LINE_STRIP_ADJACENCY:
			return GL_LINE_STRIP_ADJACENCY;
			break;
		case OF_PRIMITIVE_TRIANGLES_ADJACENCY:
			return GL_TRIANGLES_ADJACENCY;
			break;
		case OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY:
			return GL_TRIANGLE_STRIP_ADJACENCY;
			break;
		case OF_PRIMITIVE_PATCHES:
			return GL_PATCHES;
			break;
#endif
		default:
			ofLogError("ofGLUtils") << "ofGetGLPrimitiveMode(): unknown OF primitive mode " << ofToString(mode) << ", returning GL_TRIANGLES";
			return GL_TRIANGLES;
			break;
	}
}

ofPrimitiveMode ofGetOFPrimitiveMode(GLuint mode){
	switch(mode){
		case GL_TRIANGLES:
			return OF_PRIMITIVE_TRIANGLES;
			break;
		case GL_TRIANGLE_STRIP:
			return OF_PRIMITIVE_TRIANGLE_STRIP;
			break;
		case GL_TRIANGLE_FAN:
			return OF_PRIMITIVE_TRIANGLE_FAN;
			break;
		case GL_LINES:
			return OF_PRIMITIVE_LINES;
			break;
		case GL_LINE_STRIP:
			return OF_PRIMITIVE_LINE_STRIP;
			break;
		case GL_LINE_LOOP:
			return OF_PRIMITIVE_LINE_LOOP;
			break;
		case GL_POINTS:
			return OF_PRIMITIVE_POINTS;
			break;
#ifndef TARGET_OPENGLES
		case GL_LINES_ADJACENCY:
			return OF_PRIMITIVE_LINES_ADJACENCY;
			break;
		case GL_LINE_STRIP_ADJACENCY:
			return OF_PRIMITIVE_LINE_STRIP_ADJACENCY;
			break;
		case GL_TRIANGLES_ADJACENCY:
			return OF_PRIMITIVE_TRIANGLES_ADJACENCY;
			break;
		case GL_TRIANGLE_STRIP_ADJACENCY:
			return OF_PRIMITIVE_TRIANGLE_STRIP_ADJACENCY;
			break;
		case GL_PATCHES:
			return OF_PRIMITIVE_PATCHES;
			break;
#endif
		default:
			ofLogError("ofGLUtils") << "ofGetOFPrimitiveMode(): unknown GL primitive mode " << ofToString(mode) << ", returning OF_PRIMITIVE_TRIANGLES";
			return OF_PRIMITIVE_TRIANGLES;
			break;
	}
}


int ofGetGLInternalFormatFromPixelFormat(ofPixelFormat pixelFormat){
	switch(pixelFormat){
	case OF_PIXELS_BGRA:
	case OF_PIXELS_RGBA:
#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
		return GL_RGBA8;
#else
		return GL_RGBA;
#endif
	case OF_PIXELS_RGB:
	case OF_PIXELS_BGR:
#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
		return GL_RGB8;
#else
		return GL_RGB;
#endif
    case OF_PIXELS_RGB565:
	#if defined(TARGET_ANDROID) || defined(TARGET_RASPBERRY_PI_LEGACY)
		return GL_RGB565_OES;
	#elif defined(GL_RGB565)
		return GL_RGB565;
	#else
		return GL_RGB;
	#endif
    case OF_PIXELS_GRAY:
    case OF_PIXELS_NV12:
    case OF_PIXELS_NV21:
	case OF_PIXELS_YV12:
	case OF_PIXELS_I420:
	case OF_PIXELS_Y:
	case OF_PIXELS_U:
	case OF_PIXELS_V:
#ifndef TARGET_OPENGLES
		if(ofIsGLProgrammableRenderer()){
			return GL_R8;
		}else{
#endif
			return GL_LUMINANCE;
#ifndef TARGET_OPENGLES
		}
#endif
    case OF_PIXELS_GRAY_ALPHA:
	case OF_PIXELS_YUY2:
	case OF_PIXELS_UV:
	case OF_PIXELS_VU:
#ifndef TARGET_OPENGLES
		if(ofIsGLProgrammableRenderer()){
			return GL_RG8;
		}else{
#endif
			return GL_LUMINANCE_ALPHA;
#ifndef TARGET_OPENGLES
		}
#endif
	default:
		ofLogError("ofGLUtils") << "ofGetGLInternalFormatFromPixelFormat(): unknown OF pixel format "
						<< ofToString(pixelFormat) << ", returning GL_RGBA";
		return GL_RGBA;
	}
}

int ofGetGLFormatFromPixelFormat(ofPixelFormat pixelFormat){
    switch(pixelFormat){
    case OF_PIXELS_BGRA:
    #ifdef TARGET_OPENGLES
        return GL_BGRA_EXT;
    #else
        return GL_BGRA;
    #endif

    case OF_PIXELS_RGB:
        return GL_RGB;

    case OF_PIXELS_BGR:
    #ifdef TARGET_OPENGLES
        return GL_RGB;          // ES does not support GL_BGR
    #else
        return GL_BGR;
    #endif

    case OF_PIXELS_RGBA:
        return GL_RGBA;

    case OF_PIXELS_RGB565:
        return GL_RGB;

    case OF_PIXELS_GRAY:
    case OF_PIXELS_NV12:
    case OF_PIXELS_NV21:
    case OF_PIXELS_YV12:
    case OF_PIXELS_I420:
    case OF_PIXELS_Y:
    case OF_PIXELS_U:
    case OF_PIXELS_V:
    #ifndef TARGET_OPENGLES
        if(ofIsGLProgrammableRenderer()){
            return GL_RED;
        }else{
    #endif
            return GL_LUMINANCE;
    #ifndef TARGET_OPENGLES
        }
    #endif

    case OF_PIXELS_GRAY_ALPHA:
    case OF_PIXELS_YUY2:
    case OF_PIXELS_UV:
    case OF_PIXELS_VU:
    #ifndef TARGET_OPENGLES
        if(ofIsGLProgrammableRenderer()){
            return GL_RG;
        }else{
    #endif
            return GL_LUMINANCE_ALPHA;
    #ifndef TARGET_OPENGLES
        }
    #endif

    default:
    #if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
        ofLogError("ofGLUtils") << "ofGetGLFormatFromPixelFormat(): unknown OF pixel format " << pixelFormat << ", returning GL_RED";
        return ofIsGLProgrammableRenderer() ? GL_RED : GL_LUMINANCE;
    #else
        ofLogError("ofGLUtils") << "ofGetGLFormatFromPixelFormat(): unknown OF pixel format " << pixelFormat << ", returning GL_LUMINANCE";
        return GL_LUMINANCE;
    #endif
    }
}


int ofGetNumChannelsFromGLFormat(int glFormat){
	switch(glFormat){
	case GL_RGB:
		return 3;
#ifdef TARGET_OF_IOS
	case GL_BGRA:
#endif
	case GL_RGBA:
		return 4;
	case GL_LUMINANCE:
		return 1;
	case GL_LUMINANCE_ALPHA:
		return 2;
#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
	case GL_RED:
		return 1;
	case GL_RG:
		return 2;
#endif
	default:
		return 1;
	}
}

int ofGetBytesPerChannelFromGLType(int glType){
	switch(glType) {
		case GL_UNSIGNED_BYTE:
			return 1;
		case GL_UNSIGNED_SHORT:
			return 2;

#if !defined(TARGET_OPENGLES) && defined(GL_RGB565)
		case GL_UNSIGNED_SHORT_5_6_5:
			return 2;
#endif

#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_2_0) || defined(GL_ES_VERSION_3_0)
		case GL_FLOAT:
			return 4;
#endif
#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
		case GL_UNSIGNED_INT_24_8:
			 return 4;
#endif


#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_2_0) || defined(GL_ES_VERSION_3_0)
		case GL_UNSIGNED_INT:
			return 4;
#endif
#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
		case GL_HALF_FLOAT:
			return 2;
#endif
		default:
			ofLogError("ofGetBytesPerChannelFromGLType") << "unknown type returning 1";
			return 1;

	}
}

void ofSetPixelStoreiAlignment(GLenum pname, int w, int bpc, int numChannels){
	int stride = w * numChannels * bpc;
	ofSetPixelStoreiAlignment(pname,stride);
}

void ofSetPixelStoreiAlignment(GLenum pname, int stride){
    if (OF_ROUND_UP_8 (stride) == stride) {
        glPixelStorei (pname, 8);
    } else if (OF_ROUND_UP_4 (stride) == stride) {
        glPixelStorei (pname, 4);
    } else if (OF_ROUND_UP_2 (stride) == stride) {
        glPixelStorei (pname, 2);
    } else {
        glPixelStorei (pname, 1);
    }
}

vector<string> ofGLSupportedExtensions(){
#ifdef TARGET_OPENGLES
	char* extensions = (char*)glGetString(GL_EXTENSIONS);
	if(extensions){
		string extensions_str = extensions;
		return ofSplitString(extensions_str," ");
	}else{
		return vector<string>();
	}
#else
	int numExtensions=0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	std::vector<std::string> extensions;
	for(int i=0;i<numExtensions;i++){
		char* extension = (char*)glGetStringi(GL_EXTENSIONS, i);
		if(extension){
			extensions.emplace_back(extension);
		}
	}
	return extensions;
#endif
}

bool ofGLCheckExtension(string searchName){
#if defined( TARGET_OPENGLES )
	vector<string> extensionsList = ofGLSupportedExtensions();
	std::set<string> extensionsSet;
	extensionsSet.insert(extensionsList.begin(),extensionsList.end());
	return extensionsSet.find(searchName)!=extensionsSet.end();
#else
	return glewIsSupported(searchName.c_str());
#endif
}

bool ofGLSupportsNPOTTextures(){
#if !defined(TARGET_OPENGLES) || defined(GL_ES_VERSION_3_0)
	// Desktop + GLES 3.0+ support NPOT textures natively (core feature)
	return true;
#else
	// Pure GLES 2.0 — check extensions at runtime
	static bool npotChecked = false;
	static bool npotSupported = false;
	if(!npotChecked){
		vector<string> extensionsList = ofGLSupportedExtensions();
		std::set<string> extensionsSet(extensionsList.begin(), extensionsList.end());

		npotSupported = extensionsSet.find("GL_OES_texture_npot") != extensionsSet.end() ||
						extensionsSet.find("APPLE_texture_2D_limited_npot") != extensionsSet.end() ||
						extensionsSet.find("GL_NV_texture_npot_2D_mipmap") != extensionsSet.end() ||
						extensionsSet.find("GL_IMG_texture_npot") != extensionsSet.end() ||
						extensionsSet.find("GL_ARB_texture_non_power_of_two") != extensionsSet.end();

		npotChecked = true;
	}

	return npotSupported;
#endif
}

string ofGLSLVersionFromGL(int major, int minor){
#ifdef TARGET_OPENGLES

	#ifdef TARGET_EMSCRIPTEN
	if(major >= 2) { // for emscripten major version refers to WEBGL version
        return "300";   // WebGL2 = GLSL ES 3.00 (header adds " es")
    } else {
        return "100";
    }
	#else
		if(major == 1){
			return "ES1";
		}else if(major == 2){
			return "100";
		}else if(major == 3){
			return "300";
		}else {
			return ofToString(major*100+minor*10);
		}
	#endif
#else
	switch(major){
	case 3:
		if(minor==0){
			return "130";
		}else if(minor==1){
			return "140";
		}else if(minor==2){
			return "150";
		}else{
			return ofToString(major*100+minor*10);
		}
	case 4:
		return ofToString(major*100+minor*10);
	default:
		return "120";
	}
#endif
}

string ofGLSLVersionFromGL(){
    int major = 0;
    int minor = 0;

    auto glRenderer = std::dynamic_pointer_cast<ofBaseGLRenderer>(ofGetCurrentRenderer());
    if( glRenderer ){
        major = glRenderer->getGLVersionMajor();
        minor = glRenderer->getGLVersionMinor();
    }
    return ofGLSLVersionFromGL(major, minor);
}

//TODO: unify this with similar in ofGLProgrammableRenderer and ofMaterial
string ofGLSLGetDefaultHeader(){
    string header = "";

    auto glRenderer = std::dynamic_pointer_cast<ofBaseGLRenderer>(ofGetCurrentRenderer());

    if( glRenderer ){
        string versionStr = ofGLSLVersionFromGL(glRenderer->getGLVersionMajor(), glRenderer->getGLVersionMinor());
        header = "#version "+versionStr+"\n";

        #ifdef TARGET_OPENGLES
            if( versionStr != "ES1" ){
                header += "#extension GL_OES_standard_derivatives : enable\n";
            }
            #ifdef TARGET_ANDROID
                header += "#extension GL_OES_EGL_image_external : require\n";
            #endif
            header += "precision mediump float;\n";
            header += "precision mediump int;\n";
            header += "#define TARGET_OPENGLES\n";
        #else
            if( ofGetUsingArbTex() ){
                if( glRenderer->getGLVersionMajor() < 4 && glRenderer->getGLVersionMinor() < 2 ){
                    header += "\n#extension GL_ARB_texture_rectangle : enable";
                }
            }
        #endif
    }
    return header;
}

#ifdef TARGET_OPENGLES
int ofGLESVersionFromGL(){
	const GLubyte * version = glGetString(GL_VERSION);
	if(version != nullptr){
		if(strstr((const char*)version, "ES 1.0")){
			return 100;
		}else if(strstr((const char*)version, "ES 1.1")){
			return 101;
		}else if(strstr((const char*)version, "ES-CM 1.1")){
			return 101;
		}else if(strstr((const char*)version, "ES 2.0")){
			return 200;
		}else if(strstr((const char*)version, "ES 3.0")){
			return 300;
		}else if(strstr((const char*)version, "ES 3.1")){
			return 301;
		}else if(strstr((const char*)version, "ES 3.2")){
			return 302;
		}else{
			return 101;
		}
	}else{
		return 101;
	}
}
#endif

#ifndef TARGET_PROGRAMMABLE_GL
shared_ptr<ofBaseGLRenderer> ofGetGLRenderer(){
	if(ofGetCurrentRenderer()->getType()==ofGLRenderer::TYPE || ofGetCurrentRenderer()->getType()==ofGLProgrammableRenderer::TYPE){
		return (shared_ptr<ofBaseGLRenderer>&)ofGetCurrentRenderer();
	}else if(ofGetCurrentRenderer()->getType()==ofRendererCollection::TYPE){
		return ((shared_ptr<ofRendererCollection>&)ofGetCurrentRenderer())->getGLRenderer();
	}else{
		return shared_ptr<ofGLRenderer>();
	}
}
#else
shared_ptr<ofBaseGLRenderer> ofGetGLRenderer(){
	if(ofGetCurrentRenderer()->getType()==ofGLProgrammableRenderer::TYPE){
		return (shared_ptr<ofBaseGLRenderer>&)ofGetCurrentRenderer();
	}else if(ofGetCurrentRenderer()->getType()==ofRendererCollection::TYPE){
		return ((shared_ptr<ofRendererCollection>&)ofGetCurrentRenderer())->getGLRenderer();
	}else{
		return shared_ptr<ofGLProgrammableRenderer>();
	}
}
#endif

#ifndef TARGET_OPENGLES
namespace{
	void gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void * user){
		std::ostringstream oss;
		oss << "GL Debug: ";

		ofLogLevel level;
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			level = OF_LOG_ERROR;
			oss << "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			level = OF_LOG_WARNING;
			oss << "DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			level = OF_LOG_WARNING;
			oss << "UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			level = OF_LOG_NOTICE;
			oss << "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			level = OF_LOG_NOTICE;
			oss << "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_OTHER:
		default:
			level = OF_LOG_VERBOSE;
			oss << "OTHER";
			break;
		}

		switch (severity) {
		case GL_DEBUG_SEVERITY_LOW:
			oss << " (LOW)";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			oss << " (MEDIUM)";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			oss << " (HIGH)";
			break;
		}

		oss << ", " << id << ": " << message;

		ofLog(level, oss.str());
	}
}

void ofEnableGLDebugLog(){
	if(ofGLCheckExtension("GL_KHR_debug") && ofGLCheckExtension("GL_ARB_debug_output")){
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback((GLDEBUGPROC)gl_debug_callback, nullptr);
	}
}

void ofDisableGLDebugLog(){
	if(ofGLCheckExtension("GL_KHR_debug") && ofGLCheckExtension("GL_ARB_debug_output")){
		glDisable(GL_DEBUG_OUTPUT);
	}
}
#endif
