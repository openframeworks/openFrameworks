#pragma once

#include "ofConstants.h"
#include "ofMesh.h"
#include "ofTypes.h"
#include "ofPolyline.h"
#include "tesselator.h"

/// \brief
/// ofTessellator exists for one purpose: to turn ofPolylines into ofMeshes so
/// that they can be more efficiently displayed using OpenGL. The ofPath class
/// uses tessellation to turn its paths into openGL-ready shapes under the hood,
/// so if you're interested in just having ofPolyline instances converted to
/// meshes, it's handled for you by ofPath. If you're interested in using the
/// tessellation multiple polylines at a time, then you'll find what you need in
/// the ofTessellator. Many shapes can't be drawn by OpenGL without tessellation,
/// but by breaking them into triangles or quads, they can be drawn easily.
/// 
/// An example: The fairly simple shape shown on the left needs to be broken
/// into triangles to be displayed, an example of how this might be done is
/// shown on the right.
/// 
/// ![tessellation](graphics/tessellation.jpg)
class ofTessellator
{
public:	
	ofTessellator();
	~ofTessellator();

	/// \brief Copy constructor to copy properties from one tessellator to another. 
	ofTessellator(const ofTessellator & mom);

	/// \brief Operator overloading to copy properties from one tessellator to another. 
	ofTessellator & operator=(const ofTessellator & mom);

	/// \brief Tessellates a vector of ofPolyline instances into a single
	/// ofMesh instance using the winding mode set in ofPolyWindingMode.
	void tessellateToMesh( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, ofMesh & dstmesh, bool bIs2D=false );
	
	/// \brief Tessellates a ofPolyline instance into a single ofMesh instance
	/// using the winding mode set in ofPolyWindingMode.
	void tessellateToMesh( const ofPolyline& src,  ofPolyWindingMode polyWindingMode, ofMesh& dstmesh, bool bIs2D=false );

	/// \brief Tessellates a vector of ofPolyline instances into vector of
	/// ofPolyline instances using the winding mode set in ofPolyWindingMode.	
	void tessellateToPolylines( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, vector<ofPolyline>& dstpoly, bool bIs2D=false );

	/// \brief Tessellate multiple polylines into a single polyline.
	void tessellateToPolylines( const ofPolyline & src, ofPolyWindingMode polyWindingMode, vector<ofPolyline>& dstpoly, bool bIs2D=false );

private:
	
	void performTessellation( ofPolyWindingMode polyWindingMode, ofMesh& dstmesh, bool bIs2D );
	void performTessellation(ofPolyWindingMode polyWindingMode, vector<ofPolyline>& dstpoly, bool bIs2D );
	void init();

	TESStesselator * cacheTess;
	TESSalloc tessAllocator;
};


