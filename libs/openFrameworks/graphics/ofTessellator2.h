#pragma once

#include "ofConstants.h"
#include "ofMesh.h"
#include "ofTypes.h"
#include "ofPath.h"
#include "tesselator.h"


/** ofTessellator
 
 Tessellates a polyline into a number of meshes, conforming to the given winding rule.
 
 @author Adapted by damian from Theo's ofxShape.
 */



class ofTessellator2
{
public:	
	
	/// tessellate polyline and return a mesh. if bIs2D==true, do a 10% more efficient normal calculation.
	static void tessellateToMesh( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, ofMesh& dstmesh, bool bIs2D=false );
	static void tessellateToMesh( const ofPolyline& src,  ofPolyWindingMode polyWindingMode, ofMesh& dstmesh, bool bIs2D=false );

	/// tessellate polyline and return an outline.
	static void tessellateToOutline( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, vector<ofPolyline> & dst, bool bIs2D=false );
	static void tessellateToOutline( const ofPolyline& src, ofPolyWindingMode polyWindingMode, vector<ofPolyline> & dst, bool bIs2D=false );
	
	/// tessellate to ofPath internal cache, used from ofPath for performace
	//static void tessellateToCache( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, ofPath::tessCache & cache, bool bIs2D=false );

private:
	
	static void performTessellation( const vector<ofPolyline>& polylines, ofPolyWindingMode polyWindingMode, bool bFilled, bool bIs2D );
	static void init();
	/// clear out everything
	static void clear();
	

	static ofMesh * resultMesh;
	
	
	static vector<ofPolyline> * resultOutline;
	
	static int  numElements;

	static bool initialized;

	static TESStesselator * cacheTess;
	static TESSalloc tessAllocator;
};


