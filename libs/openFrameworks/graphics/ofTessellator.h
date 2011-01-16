/*
 *  ofTessellator.h
 *  openFrameworks
 *
 *  Created by theo on 28/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofConstants.h"
#include "ofMain.h"
#include "ofShape.h"

#ifndef DRAW_WITH_MESHIES
#include "ofMesh.h"
#endif

#ifndef CALLBACK
#define CALLBACK
#endif


/** ofTessellator
 
 Tessellates a polyline into a number of meshes, conforming to the given winding rule.
 
 @author Adapted by damian from Theo's ofxShape.
 */



class ofTessellator
{
public:	
	
	/// tessellate polyline and return a mesh. if bIs2D==true, do a 10% more efficient normal calculation.
#ifdef DRAW_WITH_MESHIES
	static vector<meshy> tessellateToMesh( const vector<ofPolyline>& polylines, int polyWindingMode, bool bIs2D=false );
	static vector<meshy> tessellateToMesh( const ofPolyline& polyline, int polyWindingMode, bool bIs2D=false );
#else
	static ofMesh tessellateToMesh( const vector<ofPolyline>& polylines, int polyWindingMode, bool bIs2D=false );
	static ofMesh tessellateToMesh( const ofPolyline& polyline, bool bFilled, bool bIs2D=false );
#endif

	/// tessellate polyline and return an outline.
	static ofPolyline tessellateToOutline( const vector<ofPolyline>& polylines, int polyWindingMode, bool bIs2D=false );
	static ofPolyline tessellateToOutline( const ofPolyline& polyline, int polyWindingMode, bool bIs2D=false );
	
	
private:
	
	static void performTessellation( const vector<ofPolyline>& polylines, int polyWindingMode, bool bFilled, bool bIs2D );
	
	
	/// clear out everything
	static void clear();

	/// callbacks for GLU tesselation
	static void CALLBACK error(GLenum);
	static void CALLBACK vertex( void* data);
	static void CALLBACK combine( GLdouble coords[3], void* vertex_data[4], GLfloat weight[4], void** outData);
	
	static void CALLBACK begin(GLint type);
	static void CALLBACK end();
		
	/// ensure thread-safety
	static ofMutex mutex;

	
	// filled during tessellation
	static GLint currentTriType; // GL_TRIANGLES, GL_TRIANGLE_FAN or GL_TRIANGLE_STRIP
	static vector<ofPoint> vertices;
	
	//---------------------------- for combine callback:
	static std::vector <double*> newVertices;
	//---------------------------- store all the polygon vertices:
	static std::vector <double*> ofShapePolyVertexs;
	

#ifdef DRAW_WITH_MESHIES
	static vector<meshy> resultMeshies;
#else	
	static ofMesh resultMesh;
#endif
	
	
	static ofPolyline resultOutline;
	
};


