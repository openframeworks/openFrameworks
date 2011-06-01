#pragma once

#include "ofConstants.h"
#include "ofMesh.h"
#include "ofTypes.h"
#include "ofPolyline.h"
#include "tesselator.h"


class ofTessellator
{
public:	
	
	ofTessellator();
	~ofTessellator();
	ofTessellator(const ofTessellator & mom);
	ofTessellator & operator=(const ofTessellator & mom);

	void tessellateToMesh( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, ofMesh & dstmesh, bool bIs2D=false );
	void tessellateToMesh( const ofPolyline& src,  ofPolyWindingMode polyWindingMode, ofMesh& dstmesh, bool bIs2D=false );

	void tessellateToPolylines( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, vector<ofPolyline>& dstpoly, bool bIs2D=false );
	void tessellateToPolylines( const ofPolyline & src, ofPolyWindingMode polyWindingMode, vector<ofPolyline>& dstpoly, bool bIs2D=false );

private:
	
	void performTessellation( ofPolyWindingMode polyWindingMode, ofMesh& dstmesh, bool bIs2D );
	void performTessellation(ofPolyWindingMode polyWindingMode, vector<ofPolyline>& dstpoly, bool bIs2D );
	void init();

	TESStesselator * cacheTess;
	TESSalloc tessAllocator;
};


