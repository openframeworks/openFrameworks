#include "ofTessellator.h"





//-------------- polygons ----------------------------------
//
// to do polygons, we need tesselation
// to do tesselation, we need glu and callbacks....
// ------------------------------------
// one of the callbacks creates new vertices (on intersections, etc),
// and there is a potential for memory leaks
// if we don't clean up properly
// ------------------------------------
// also the easiest system, using beginShape
// vertex(), endShape, will also use dynamically
// allocated memory
// ------------------------------------
// so, therefore, we will be using a static vector here
// for two things:
//
// a) collecting vertices
// b) new vertices on combine callback
//
// important note!
//
// this assumes single threaded polygon creation
// you can have big problems if creating polygons in
// multiple threads... please be careful
//
// (but also be aware that alot of opengl code
// is single threaded anyway, so you will have problems
// with many things opengl related across threads)
//
// ------------------------------------
// (note: this implementation is based on code from ftgl)
// ------------------------------------


void * memAllocator( void *userData, unsigned int size ){
	return malloc(size);
}

void * memReallocator( void *userData, void* ptr, unsigned int size ){
	return realloc(ptr,size);
}

void memFree( void *userData, void *ptr ){
	free (ptr);
}

//----------------------------------------------------------
ofTessellator::ofTessellator()
  : cacheTess(nullptr)
{
	init();
}

//----------------------------------------------------------
ofTessellator::~ofTessellator(){
	tessDeleteTess(cacheTess);
}

//----------------------------------------------------------
ofTessellator::ofTessellator(const ofTessellator & mom)
  : cacheTess(nullptr)
{
	if(&mom != this){
		if(cacheTess) tessDeleteTess(cacheTess);
		init();
	}
}

//----------------------------------------------------------
ofTessellator & ofTessellator::operator=(const ofTessellator & mom){
	if(&mom != this){
		if(cacheTess) tessDeleteTess(cacheTess);
		init();
	}
	return *this;
}

//----------------------------------------------------------
void ofTessellator::init(){
	tessAllocator.memalloc = memAllocator;
	tessAllocator.memrealloc = memReallocator;
	tessAllocator.memfree = memFree;
	tessAllocator.meshEdgeBucketSize=0;
	tessAllocator.meshVertexBucketSize=0;
	tessAllocator.meshFaceBucketSize=0;
	tessAllocator.dictNodeBucketSize=0;
	tessAllocator.regionBucketSize=0;
	tessAllocator.extraVertices=0;
	cacheTess = tessNewTess( &tessAllocator );
}

//----------------------------------------------------------
void ofTessellator::tessellateToMesh( const ofPolyline& src,  ofPolyWindingMode polyWindingMode, ofMesh& dstmesh, bool bIs2D){

	ofPolyline& polyline = const_cast<ofPolyline&>(src);
	tessAddContour( cacheTess, bIs2D?2:3, &polyline.getVertices()[0], sizeof(ofPoint), polyline.size());

	performTessellation( polyWindingMode, dstmesh, bIs2D );
}

	
//----------------------------------------------------------
void ofTessellator::tessellateToMesh( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, ofMesh & dstmesh, bool bIs2D ) {


	// pass vertex pointers to GLU tessellator
	for ( int i=0; i<(int)src.size(); ++i ) {
		if (src[i].size() > 0) {
			ofPolyline& polyline = const_cast<ofPolyline&>(src[i]);

			tessAddContour(cacheTess, bIs2D ? 2 : 3, &polyline.getVertices()[0].x, sizeof(ofPoint), polyline.size());
		}
	}

	performTessellation( polyWindingMode, dstmesh, bIs2D );
}

//----------------------------------------------------------
void ofTessellator::tessellateToPolylines( const ofPolyline& src,  ofPolyWindingMode polyWindingMode, vector<ofPolyline>& dstpoly, bool bIs2D){

	if (src.size() > 0) {
		ofPolyline& polyline = const_cast<ofPolyline&>(src);
		tessAddContour(cacheTess, bIs2D ? 2 : 3, &polyline.getVertices()[0], sizeof(ofPoint), polyline.size());
	}
	performTessellation( polyWindingMode, dstpoly, bIs2D );
}


//----------------------------------------------------------
void ofTessellator::tessellateToPolylines( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, vector<ofPolyline>& dstpoly, bool bIs2D ) {

	// pass vertex pointers to GLU tessellator
	for ( int i=0; i<(int)src.size(); ++i ) {
		if (src[i].size() > 0) {
			ofPolyline& polyline = const_cast<ofPolyline&>(src[i]);

			tessAddContour(cacheTess, bIs2D ? 2 : 3, &polyline.getVertices()[0].x, sizeof(ofPoint), polyline.size());
		}
	}

	performTessellation( polyWindingMode, dstpoly, bIs2D );
}

	
//----------------------------------------------------------
void ofTessellator::performTessellation(ofPolyWindingMode polyWindingMode, ofMesh& dstmesh, bool bIs2D ) {

	if (!tessTesselate(cacheTess, polyWindingMode, TESS_POLYGONS, 3, 3, 0)){
		ofLogError("ofTessellator") << "performTessellation(): mesh polygon tessellation failed, winding mode " << polyWindingMode;
		return;
	}

	int numVertices = tessGetVertexCount( cacheTess );
	int numIndices = tessGetElementCount( cacheTess )*3;

	dstmesh.clear();
	dstmesh.addVertices((ofVec3f*)tessGetVertices(cacheTess),numVertices);
	dstmesh.addIndices((ofIndexType*)tessGetElements(cacheTess),numIndices);
	/*ofIndexType * tessElements = (ofIndexType *)tessGetElements(cacheTess);
	for(int i=0;i<numIndices;i++){
		if(tessElements[i]!=TESS_UNDEF)
			dstmesh.addIndex(tessElements[i]);
	}*/
	dstmesh.setMode(OF_PRIMITIVE_TRIANGLES);

}


//----------------------------------------------------------
void ofTessellator::performTessellation(ofPolyWindingMode polyWindingMode, vector<ofPolyline>& dstpoly, bool bIs2D ) {
	if (!tessTesselate(cacheTess, polyWindingMode, TESS_BOUNDARY_CONTOURS, 0, 3, 0)){
		ofLogError("ofTessellator") << "performTesselation(): polyline boundary contours tessellation failed, winding mode " << polyWindingMode;
		return;
	}

	const ofPoint* verts = (ofPoint*)tessGetVertices(cacheTess);
	const TESSindex* elems = tessGetElements(cacheTess);
	const int nelems = tessGetElementCount(cacheTess);
	dstpoly.resize(nelems);
	for (int i = 0; i < nelems; ++i)
	{
			int b = elems[i*2];
			int n = elems[i*2+1];
			dstpoly[i].clear();
			dstpoly[i].addVertices(&verts[b],n);
			dstpoly[i].setClosed(true);
	}
}
