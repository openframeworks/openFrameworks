#include "ofGLUtils.h"

#include <set>
#include "ofProgrammableGLRenderer.h"
#include "ofGraphics.h"
#include "ofShader.h"
#include "ofBaseTypes.h"
#include "ofRendererCollection.h"

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
			ofLog(OF_LOG_ERROR,"asked for unsupported or non existant poly mode " + ofToString(m) + " returning GL_FILL");
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
			ofLog(OF_LOG_ERROR,"asked for non existant glPolygonMode " + ofToString(m) + " returning OF_MESH_FILL");
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
			ofLog(OF_LOG_ERROR,"asked for unsupported or non existant primitive mode " + ofToString(mode) + " returning GL_TRIANGLES");
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
			ofLog(OF_LOG_ERROR,"asked for non existant primitive mode " + ofToString(mode) + " returning OF_PRIMITIVE_TRIANGLES");
			return OF_PRIMITIVE_TRIANGLES;
			break;
	}
}

int ofGetGLTypeFromPixelFormat(ofPixelFormat pixelFormat){
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
    	return GL_RGB;
#else
        return GL_RGB5;
#endif
	default:
		ofLogError("ofGLUtils") << "Unknown GL type for this ofPixelFormat" << pixelFormat << "returning GL_LUMINANCE";
		return GL_LUMINANCE;
	}
}


bool ofCheckGLExtension(string searchName){
#ifdef TARGET_OPENGLES
	string extensions = (char*)glGetString(GL_EXTENSIONS);
	vector<string> extensionsList = ofSplitString(extensions," ");
	set<string> extensionsSet;
	extensionsSet.insert(extensionsList.begin(),extensionsList.end());
	return extensionsSet.find(searchName)!=extensionsSet.end();
#else
	return glewGetExtension(searchName.c_str());
#endif
}

bool ofGLIsFixedPipeline(){
	return ofGetCurrentRenderer() && ofGetCurrentRenderer()->getType()!="ProgrammableGL";
}

ofPtr<ofProgrammableGLRenderer> ofGetProgrammableGLRenderer(){
	if(ofGetCurrentRenderer()->getType()=="ProgrammableGL"){
		return (ofPtr<ofProgrammableGLRenderer>&)ofGetCurrentRenderer();
	}else{
		return ofPtr<ofProgrammableGLRenderer>();
	}
}

GLint ofGetAttrLocationPosition(){
	if(ofGLIsFixedPipeline()) return 0;
	return ofGetProgrammableGLRenderer()->getAttrLocationPosition();
}

GLint ofGetAttrLocationColor(){
	if(ofGLIsFixedPipeline()) return 0;
	return ofGetProgrammableGLRenderer()->getAttrLocationColor();
}

GLint ofGetAttrLocationNormal(){
	if(ofGLIsFixedPipeline()) return 0;
	return ofGetProgrammableGLRenderer()->getAttrLocationNormal();
}

GLint ofGetAttrLocationTexCoord(){
	if(ofGLIsFixedPipeline()) return 0;
	return ofGetProgrammableGLRenderer()->getAttrLocationTexCoord();
}

ofPtr<ofBaseGLRenderer> ofGetGLRenderer(){
	if(ofGetCurrentRenderer()->getType()=="GL" || ofGetCurrentRenderer()->getType()=="ProgrammableGL"){
		return (ofPtr<ofBaseGLRenderer>&)ofGetCurrentRenderer();
	}else if(ofGetCurrentRenderer()->getType()=="collection"){
		return ((ofPtr<ofRendererCollection>&)ofGetCurrentRenderer())->getGLRenderer();
	}else{
		return ofPtr<ofGLRenderer>();
	}
}

void ofEnableVertices(){
	ofPtr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->enableVertices();
	}
}

void ofEnableTexCoords(){
	ofPtr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->enableTexCoords();
	}
}

void ofEnableColorCoords(){
	ofPtr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->enableColors();
	}
}

void ofEnableNormals(){
	ofPtr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->enableNormals();
	}
}

void ofDisableVertices(){
	ofPtr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->disableVertices();
	}
}

void ofDisableTexCoords(){
	ofPtr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->disableTexCoords();
	}
}

void ofDisableColorCoords(){
	ofPtr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->disableColors();
	}
}

void ofDisableNormals(){
	ofPtr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->disableNormals();
	}
}
