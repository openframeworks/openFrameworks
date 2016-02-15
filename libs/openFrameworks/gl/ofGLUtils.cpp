#include "ofGLUtils.h"

#include <set>
#include "ofGLProgrammableRenderer.h"
#include "ofGraphics.h"
#include "ofShader.h"
#include "ofBaseTypes.h"
#include "ofRendererCollection.h"

//---------------------------------
int ofGetGlInternalFormat(const ofPixels& pix) {
	return ofGetGLInternalFormatFromPixelFormat(pix.getPixelFormat());
}

//---------------------------------
int ofGetGlInternalFormat(const ofShortPixels& pix) {
#ifndef TARGET_OPENGLES
	switch(pix.getNumChannels()) {
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
	ofLogWarning("ofGLUtils") << "ofGetGlInternalFormat(): 16bit textures are not supported in OpenGL ES";
	switch(pix.getNumChannels()) {
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
int ofGetGlInternalFormat(const ofFloatPixels& pix) {
#ifndef TARGET_OPENGLES
	switch(pix.getNumChannels()) {
		case 3: return GL_RGB32F;
		case 4: return GL_RGBA32F;
		case 2:
			if(ofIsGLProgrammableRenderer()){
				return GL_RG32F;
			}else{
				return GL_LUMINANCE_ALPHA32F_ARB;
			}
		default:
			if(ofIsGLProgrammableRenderer()){
				return GL_R32F;
			}else{
				return GL_LUMINANCE32F_ARB;
			}
	}
#else
	ofLogWarning("ofGLUtils") << "ofGetGlInternalFormat(): float textures not supported in OpenGL ES";
	switch(pix.getNumChannels()) {
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
// this is helpful for debugging ofTexture
string ofGetGlInternalFormatName(int glInternalFormat) {
	switch(glInternalFormat) {
		case GL_RGBA: return "GL_RGBA";
#ifndef TARGET_OPENGLES
		case GL_RGBA8: return "GL_RGBA8";
#endif
		case GL_RGB: return "GL_RGB";
#ifndef TARGET_OPENGLES
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
	#ifndef TARGET_OPENGLES
			case GL_RGBA8:
			case GL_RGBA16:
			case GL_RGBA16F:
			case GL_RGBA32F_ARB:
	#endif
				 return GL_RGBA;


			case GL_RGB:
	#ifndef TARGET_OPENGLES
			case GL_RGB8:
			case GL_RGB16:
			case GL_RGB32F_ARB:
	#endif
				return GL_RGB;


			case GL_LUMINANCE:
	#ifndef TARGET_OPENGLES
			case GL_LUMINANCE8:
			case GL_LUMINANCE16:
			case GL_LUMINANCE32F_ARB:
	#endif
				 return GL_LUMINANCE;

			case GL_LUMINANCE_ALPHA:
	#ifndef TARGET_OPENGLES
			case GL_LUMINANCE8_ALPHA8:
			case GL_LUMINANCE16_ALPHA16:
			case GL_LUMINANCE_ALPHA32F_ARB:
	#endif
				return GL_LUMINANCE_ALPHA;


			case GL_DEPTH_STENCIL:
				 return GL_DEPTH_STENCIL;

			case GL_DEPTH_COMPONENT:
#ifndef TARGET_OPENGLES
			case GL_DEPTH_COMPONENT16:
			case GL_DEPTH_COMPONENT24:
			case GL_DEPTH_COMPONENT32:
#endif
				return GL_DEPTH_COMPONENT;

			case GL_STENCIL_INDEX:
				return GL_STENCIL_INDEX;

#ifndef TARGET_OPENGLES
			case GL_R8:
			case GL_R16:
			case GL_R16F:
			case GL_R32F:
				return GL_RED;

			case GL_RG8:
			case GL_RG16:
			case GL_RG16F:
			case GL_RG32F:
				return GL_RG;
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

int ofGetGlTypeFromInternal(int glInternalFormat){

	switch(glInternalFormat) {
		case GL_RGB:
		case GL_RGBA:
		case GL_LUMINANCE:
		case GL_LUMINANCE_ALPHA:
		case GL_ALPHA:
#ifndef TARGET_OPENGLES
		case GL_LUMINANCE8:
		case GL_LUMINANCE8_ALPHA8:
		case GL_R8:
		case GL_RG8:
		case GL_RGB8:
		case GL_RGBA8:
		case GL_ALPHA8:
#endif
			 return GL_UNSIGNED_BYTE;


#ifndef TARGET_OPENGLES
		case GL_LUMINANCE16:
		case GL_LUMINANCE16_ALPHA16:
		case GL_R16:
		case GL_RG16:
		case GL_RGB16:
		case GL_RGBA16:
#endif
			return GL_UNSIGNED_SHORT;

#if !defined(TARGET_OPENGLES) && defined(GL_RGB565)
		case GL_RGB565:
			return GL_UNSIGNED_SHORT_5_6_5;
		break;
#endif

#ifndef TARGET_OPENGLES
		case GL_LUMINANCE32F_ARB:
		case GL_LUMINANCE_ALPHA32F_ARB:
		case GL_R16F:
		case GL_RG16F:
		case GL_RGB16F:
		case GL_RGBA16F:
		case GL_R32F:
		case GL_RG32F:
		case GL_RGB32F:
		case GL_RGBA32F:
#endif
			return GL_FLOAT;


		case GL_DEPTH_STENCIL:
			 return GL_UNSIGNED_INT_24_8;

		case GL_DEPTH_COMPONENT:
#ifndef TARGET_OPENGLES
		case GL_DEPTH_COMPONENT16:
#endif
			return GL_UNSIGNED_SHORT;

#ifndef TARGET_OPENGLES
		case GL_DEPTH_COMPONENT24:
		case GL_DEPTH_COMPONENT32:
			return GL_UNSIGNED_INT;
#endif

		case GL_STENCIL_INDEX:
			return GL_UNSIGNED_BYTE;

		default:
			ofLogError("ofGLUtils") << "ofGetGlTypeFromInternal(): unknown internal format " << glInternalFormat << ", returning GL_UNSIGNED_BYTE";
			return GL_UNSIGNED_BYTE;

	}
}

//---------------------------------
int ofGetGlType(const ofPixels & pixels) {
#ifndef TARGET_OPENGLES
	if(pixels.getPixelFormat()==OF_PIXELS_RGB565){
		return GL_UNSIGNED_SHORT_5_6_5;
	}else{
#endif
		return GL_UNSIGNED_BYTE;
#ifndef TARGET_OPENGLES
	}
#endif
}

//---------------------------------
int ofGetGlType(const ofShortPixels & pixels) {
	return GL_UNSIGNED_SHORT;
}

//---------------------------------
int ofGetGlType(const ofFloatPixels & pixels) {
	return GL_FLOAT;
}

//---------------------------------
ofImageType ofGetImageTypeFromGLType(int glType){
	switch(glType){
	case GL_LUMINANCE:
#ifndef TARGET_OPENGLES
	case GL_LUMINANCE8:
	case GL_LUMINANCE16:
	case GL_LUMINANCE32F_ARB:
	case GL_R16:
	case GL_R16F:
	case GL_R16I:
	case GL_R16UI:
	case GL_R32F:
	case GL_R32I:
	case GL_R32UI:
	case GL_DEPTH_COMPONENT32F:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT16:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT:
#endif
		return OF_IMAGE_GRAYSCALE;


	case GL_RGB:
#ifndef TARGET_OPENGLES
	case GL_RGB8:
	case GL_RGB16:
	case GL_RGB16F:
	case GL_RGB16I:
	case GL_RGB16UI:
	case GL_RGB32F:
	case GL_RGB32I:
	case GL_RGB32UI:
#endif
		return OF_IMAGE_COLOR;


	case GL_RGBA:
#ifndef TARGET_OPENGLES
	case GL_RGBA8:
	case GL_RGBA16:
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

GLuint ofGetGLPolyMode(ofPolyRenderMode m){
#ifndef TARGET_OPENGLES
	switch(m){
		case(OF_MESH_POINTS):
			return GL_POINT;
			break;
		case(OF_MESH_WIREFRAME):
			return GL_LINE;
			break;
		case(OF_MESH_FILL):
			return GL_FILL;
			break;
		default:
			ofLogError("ofGLUtils") << "ofGetGLPolyMode(): unknown OF poly mode " << ofToString(m) << ", returning GL_FILL";
			return GL_FILL;
			break;
	}
#else
	return 0;
#endif
}

ofPolyRenderMode ofGetOFPolyMode(GLuint m){
#ifndef TARGET_OPENGLES
	switch(m){
		case(GL_POINT):
			return OF_MESH_POINTS;
			break;
		case(GL_LINE):
			return OF_MESH_WIREFRAME;
			break;
		case(GL_FILL):
			return OF_MESH_FILL;
			break;
		default:
			ofLogError("ofGLUtils") << "ofGetOFPolyMode(): unknown GL poly mode " << ofToString(m) << ", returning OF_MESH_FILL";
			return OF_MESH_FILL;
			break;
	}
#else
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
#ifndef TARGET_OPENGLES
		return GL_RGBA8;
#else
		return GL_RGBA;
#endif
	case OF_PIXELS_RGB:
	case OF_PIXELS_BGR:
#ifndef TARGET_OPENGLES
		return GL_RGB8;
#else
		return GL_RGB;
#endif
    case OF_PIXELS_RGB565:
	#if defined(TARGET_ANDROID) || defined(TARGET_RASPBERRY_PI)
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
		ofLogError("ofGLUtils") << "ofGetGLInternalFormatFromPixelFormat(): unknown OF pixel format" << pixelFormat << ", returning GL_RGBA";
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
    	return GL_RGB;
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
		ofLogError("ofGLUtils") << "ofGetGLFormatFromPixelFormat(): unknown OF pixel format" << pixelFormat << ", returning GL_LUMINANCE";
#ifndef TARGET_OPENGLES
		if(ofIsGLProgrammableRenderer()){
			return GL_RED;
		}else{
#endif
			return GL_LUMINANCE;
#ifndef TARGET_OPENGLES
		}
#endif
	}
}


int ofGetNumChannelsFromGLFormat(int glFormat){
	switch(glFormat){
	case GL_RGB:
		return 3;
	case GL_RGBA:
		return 4;
	case GL_LUMINANCE:
		return 1;
	case GL_LUMINANCE_ALPHA:
		return 2;
#ifndef TARGET_OPENGLES
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

#ifndef TARGET_OPENGLES
		case GL_FLOAT:
			return 4;
#endif
		case GL_UNSIGNED_INT_24_8:
			 return 4;


#ifndef TARGET_OPENGLES
		case GL_UNSIGNED_INT:
			return 4;
#endif
		case GL_HALF_FLOAT:
			return 2;

		default:
			ofLogError("ofGetBytesPerChannelFromGLType") << "unknown type returning 1";
			return 1;

	}
}

// Rounds an integer value up to the next multiple of 2,4 and 8.
#define OF_ROUND_UP_2(num)  (((num)+1)&~1)
#define OF_ROUND_UP_4(num)  (((num)+3)&~3)
#define OF_ROUND_UP_8(num)  (((num)+7)&~7)

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
	set<string> extensionsSet;
	extensionsSet.insert(extensionsList.begin(),extensionsList.end());
	return extensionsSet.find(searchName)!=extensionsSet.end();
#else
	return glewGetExtension(searchName.c_str());
#endif
}

bool ofGLSupportsNPOTTextures(){
#ifndef TARGET_OPENGLES
	return GL_ARB_texture_rectangle;
#elif !defined(TARGET_EMSCRIPTEN)
	static bool npotChecked = false;
	static bool npotSupported = false;
	if(!npotChecked){
		vector<string> extensionsList = ofGLSupportedExtensions();
		set<string> extensionsSet;
		extensionsSet.insert(extensionsList.begin(),extensionsList.end());

		npotSupported = extensionsSet.find("GL_OES_texture_npot")!=extensionsSet.end() ||
				extensionsSet.find("APPLE_texture_2D_limited_npot")!=extensionsSet.end() ||
				extensionsSet.find("GL_NV_texture_npot_2D_mipmap")!=extensionsSet.end() ||
				extensionsSet.find("GL_IMG_texture_npot")!=extensionsSet.end() ||
				extensionsSet.find("GL_ARB_texture_non_power_of_two")!=extensionsSet.end();
		npotChecked = true;
	}

	return npotSupported;
#else
	return true;
#endif
}

string ofGLSLVersionFromGL(int major, int minor){
#ifdef TARGET_OPENGLES
	return "ES1";
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

#if defined(TARGET_ANDROID)
void ofRegenerateAllVbos();
void ofRegenerateAllTextures();
void ofReloadAllImageTextures();
void ofReloadAllFontTextures();

void ofReloadGLResources(){
	ofRegenerateAllTextures();
	ofRegenerateAllVbos();
	ofReloadAllImageTextures();
	ofReloadAllFontTextures();
}
#endif

