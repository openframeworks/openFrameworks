/*
 * ofGLUtils.h
 *
 *  Created on: 02/03/2011
 *      Author: arturo
 */

#ifndef OFGLUTILS_H_
#define OFGLUTILS_H_

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


inline GLuint ofGetGLPolyMode(ofPolyRenderMode m){
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

inline ofPolyRenderMode ofGetOFPolyMode(GLuint m){
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


inline GLuint ofGetGLPrimitiveMode(ofPrimitiveMode mode){
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

inline ofPrimitiveMode ofGetOFPrimitiveMode(GLuint mode){
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

inline int ofGetGLTypeFromPixelFormat(ofPixelFormat pixelFormat){
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
#ifdef TARGET_ANDROID
    	return GL_RGB;
#else
        return GL_RGB5;
#endif
	}
}
#endif /* OFGLUTILS_H_ */
