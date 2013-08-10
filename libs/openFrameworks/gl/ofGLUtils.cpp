#include "ofGLUtils.h"

#include <set>
#include "ofGLProgrammableRenderer.h"
#include "ofGraphics.h"
#include "ofShader.h"
#include "ofBaseTypes.h"
#include "ofRendererCollection.h"

//---------------------------------
int ofGetGlInternalFormat(const ofPixels& pix) {
#ifndef TARGET_OPENGLES
	switch(pix.getNumChannels()) {
		case 3: return GL_RGB8;
		case 4: return GL_RGBA8;
		case 2:
			if(ofIsGLProgrammableRenderer()){
				return GL_RG8;
			}else{
				return GL_LUMINANCE_ALPHA;
			}
		default:
			if(ofIsGLProgrammableRenderer()){
				return GL_R8;
			}else{
				return GL_LUMINANCE8;
			}
	}
#else
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
			if(ofGetGLProgrammableRenderer()){
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
			case GL_R32F:
				return GL_RED;

			case GL_RG8:
			case GL_RG16:
			case GL_RG32F:
				return GL_RG;
#endif

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
#ifndef TARGET_OPENGLES
		case GL_RGB8:
		case GL_RGBA8:
		case GL_LUMINANCE8:
		case GL_LUMINANCE8_ALPHA8:
		case GL_R8:
		case GL_RG8:
#endif
			 return GL_UNSIGNED_BYTE;


#ifndef TARGET_OPENGLES
		case GL_RGB16:
		case GL_RGBA16:
		case GL_LUMINANCE16:
		case GL_LUMINANCE16_ALPHA16:
		case GL_R16:
		case GL_RG16:
#endif
			return GL_UNSIGNED_SHORT;


#ifndef TARGET_OPENGLES
#endif
			 return GL_LUMINANCE;

#ifndef TARGET_OPENGLES
		case GL_LUMINANCE32F_ARB:
		case GL_LUMINANCE_ALPHA32F_ARB:
		case GL_RGB32F:
		case GL_RGBA32F:
		case GL_R32F:
		case GL_RG32F:
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
	return GL_UNSIGNED_BYTE;
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
		return OF_IMAGE_GRAYSCALE;
	case GL_RGB:
		return OF_IMAGE_COLOR;
	case GL_RGBA:
		return OF_IMAGE_COLOR_ALPHA;
#ifndef TARGET_OPENGLES
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT16:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT:
		return OF_IMAGE_GRAYSCALE;
#endif
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
		default:
			ofLogError("ofGLUtils") << "ofGetOFPrimitiveMode(): unknown GL primitive mode " << ofToString(mode) << ", returning OF_PRIMITIVE_TRIANGLES";
			return OF_PRIMITIVE_TRIANGLES;
			break;
	}
}


int ofGetGLInternalFormatFromPixelFormat(ofPixelFormat pixelFormat){
	switch(pixelFormat){
	case OF_PIXELS_BGRA:
		return GL_RGBA;
	case OF_PIXELS_MONO:
		return GL_LUMINANCE;
	case OF_PIXELS_RGB:
		return GL_RGB;
	case OF_PIXELS_RGBA:
		return GL_RGBA;
    case OF_PIXELS_RGB565:
	#ifdef TARGET_OPENGLES
		#if defined(TARGET_ANDROID) || defined(TARGET_RASPBERRY_PI)
			return GL_RGB565_OES;
		#else
			return GL_RGB;
		#endif
	#else
		return GL_RGB5;
	#endif
	default:
		ofLogError("ofGLUtils") << "ofGetGLInternalFormatFromPixelFormat(): unknown OF pixel format" << pixelFormat << ", returning GL_LUMINANCE";
		return GL_LUMINANCE;
	}
}

int ofGetGLTypeFromPixelFormat(ofPixelFormat pixelFormat){
	switch(pixelFormat){
	case OF_PIXELS_BGRA:
#ifdef TARGET_OPENGLES
    	return GL_BGRA_EXT;
#else
        return GL_BGRA;
#endif
	case OF_PIXELS_MONO:
		return GL_LUMINANCE;
	case OF_PIXELS_RGB:
		return GL_RGB;
	case OF_PIXELS_RGBA:
		return GL_RGBA;
    case OF_PIXELS_RGB565:
#ifdef TARGET_OPENGLES
    	return GL_RGB;
#else
        return GL_RGB5;
#endif
	default:
		ofLogError("ofGLUtils") << "ofGetGLTypeFromPixelFormat(): unknown OF pixel format" << pixelFormat << ", returning GL_LUMINANCE";
		return GL_LUMINANCE;
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
	default:
		return 1;
	}
}

// Rounds an integer value up to the next multiple of 2,4 and 8.
#define OF_ROUND_UP_2(num)  (((num)+1)&~1)
#define OF_ROUND_UP_4(num)  (((num)+3)&~3)
#define OF_ROUND_UP_8(num)  (((num)+7)&~7)

void ofSetPixelStorei(int w, int bpc, int numChannels){
	int stride = w * numChannels * bpc;
    if (OF_ROUND_UP_8 (stride) == stride) {
        glPixelStorei (GL_UNPACK_ALIGNMENT, 8);
    } else if (OF_ROUND_UP_4 (stride) == stride) {
        glPixelStorei (GL_UNPACK_ALIGNMENT, 4);
    } else if (OF_ROUND_UP_2 (stride) == stride) {
        glPixelStorei (GL_UNPACK_ALIGNMENT, 2);
    } else {
        glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    }
}

vector<string> ofGLSupportedExtensions(){
	string extensions = (char*)glGetString(GL_EXTENSIONS);
	return ofSplitString(extensions," ");
}

bool ofGLCheckExtension(string searchName){
#ifdef TARGET_OPENGLES
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
#else
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
#endif
}

ofPtr<ofGLProgrammableRenderer> ofGetGLProgrammableRenderer(){
	if(ofGetCurrentRenderer() && ofGetCurrentRenderer()->getType()==ofGLProgrammableRenderer::TYPE){
		return (ofPtr<ofGLProgrammableRenderer>&)ofGetCurrentRenderer();
	}else{
		return ofPtr<ofGLProgrammableRenderer>();
	}
}

bool ofIsGLProgrammableRenderer(){
	return ofGetCurrentRenderer() && ofGetCurrentRenderer()->getType()==ofGLProgrammableRenderer::TYPE;
}

ofPtr<ofBaseGLRenderer> ofGetGLRenderer(){
	if(ofGetCurrentRenderer()->getType()==ofGLRenderer::TYPE || ofGetCurrentRenderer()->getType()==ofGLProgrammableRenderer::TYPE){
		return (ofPtr<ofBaseGLRenderer>&)ofGetCurrentRenderer();
	}else if(ofGetCurrentRenderer()->getType()==ofRendererCollection::TYPE){
		return ((ofPtr<ofRendererCollection>&)ofGetCurrentRenderer())->getGLRenderer();
	}else{
		return ofPtr<ofGLRenderer>();
	}
}

#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
void ofUpdateBitmapCharacterTexture();
void ofReloadAllImageTextures();
void ofReloadAllFontTextures();
void ofRegenerateAllVbos();

void ofReloadGLResources(){
	ofUpdateBitmapCharacterTexture();
	ofReloadAllImageTextures();
	ofReloadAllFontTextures();
	ofRegenerateAllVbos();
}
#endif

