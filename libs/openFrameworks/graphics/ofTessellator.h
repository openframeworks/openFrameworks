#pragma once

#include "ofConstants.h"
#include "ofMesh.h"
#include "ofTypes.h"
#include "ofPath.h"

struct GLUtesselator;

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
	static void tessellateToMesh( const vector<ofPolyline>& src, int polyWindingMode, vector<ofMesh> & dstmesh, bool bIs2D=false );
	static void tessellateToMesh( const ofPolyline& src,  int polyWindingMode, vector<ofMesh>& dstmesh, bool bIs2D=false );

	/// tessellate polyline and return an outline.
	static void tessellateToOutline( const vector<ofPolyline>& src, int polyWindingMode, vector<ofPolyline> & dst, bool bIs2D=false );
	static void tessellateToOutline( const ofPolyline& src, int polyWindingMode, vector<ofPolyline> & dst, bool bIs2D=false );
	
	/// tessellate to ofPath internal cache, used from ofPath for performace
	static void tessellateToCache( const vector<ofPolyline>& src, int polyWindingMode, ofPath::tessCache & cache, bool bIs2D=false );

private:
	
	static void performTessellation( const vector<ofPolyline>& polylines, int polyWindingMode, bool bFilled, bool bIs2D );
	static void init();
	/// clear out everything
	static void clear();

	/// callbacks for GLU tesselation
	static void CALLBACK error(GLenum);
	static void CALLBACK vertex( void* data);
	static void CALLBACK combine( GLdouble coords[3], void* vertex_data[4], GLfloat weight[4], void** outData);
	
	static void CALLBACK begin(GLint type);
	static void CALLBACK end();
		
	/// ensure thread-safety
	//static ofMutex mutex;

	
	// filled during tessellation
	static GLint currentTriType; // GL_TRIANGLES, GL_TRIANGLE_FAN or GL_TRIANGLE_STRIP
	
	//---------------------------- for combine callback:
	static std::vector <double*> newVertices;
	//---------------------------- store all the polygon vertices:
	static std::vector <double> ofShapePolyVertexs;
	

	static vector<ofMesh> * resultMesh;
	
	
	static vector<ofPolyline> * resultOutline;
	
	static int  numElements;

	static bool initialized;

	static GLUtesselator * ofShapeTobj;

};


