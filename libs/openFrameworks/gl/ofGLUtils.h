/*
 * ofGLUtils.h
 *
 *  Created on: 02/03/2011
 *      Author: arturo
 */

#ifndef OFGLUTILS_H_
#define OFGLUTILS_H_

enum ofPrimitiveMode{
	OF_TRIANGLES_MODE,
	OF_TRIANGLE_STRIP_MODE,
	OF_TRIANGLE_FAN_MODE,
	OF_LINES_MODE,
	OF_LINE_STRIP_MODE,
	OF_LINE_LOOP_MODE,
	OF_POINTS_MODE
};

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
		case OF_TRIANGLES_MODE:
			return GL_TRIANGLES;
			break;
		case OF_TRIANGLE_STRIP_MODE:
			return GL_TRIANGLE_STRIP;
			break;
		case OF_TRIANGLE_FAN_MODE:
			return GL_TRIANGLE_FAN;
			break;
		case OF_LINES_MODE:
			return GL_LINES;
			break;
		case OF_LINE_STRIP_MODE:
			return GL_LINE_STRIP;
			break;
		case OF_LINE_LOOP_MODE:
			return GL_LINE_LOOP;
			break;
		case OF_POINTS_MODE:
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
			return OF_TRIANGLES_MODE;
			break;
		case GL_TRIANGLE_STRIP:
			return OF_TRIANGLE_STRIP_MODE;
			break;
		case GL_TRIANGLE_FAN:
			return OF_TRIANGLE_FAN_MODE;
			break;
		case GL_LINES:
			return OF_LINES_MODE;
			break;
		case GL_LINE_STRIP:
			return OF_LINE_STRIP_MODE;
			break;
		case GL_LINE_LOOP:
			return OF_LINE_LOOP_MODE;
			break;
		case GL_POINTS:
			return OF_POINTS_MODE;
			break;
		default:
			ofLog(OF_LOG_ERROR,"asked for non existant primitive mode " + ofToString(mode) + " returning OF_TRIANGLES_MODE");
			return OF_TRIANGLES_MODE;
			break;
	}
}

#endif /* OFGLUTILS_H_ */
