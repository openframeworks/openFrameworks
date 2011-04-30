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
ofTessellator::ofTessellator(){
	init();
}

//----------------------------------------------------------
ofTessellator::~ofTessellator(){
	tessDeleteTess(cacheTess);
}

//----------------------------------------------------------
ofTessellator::ofTessellator(const ofTessellator & mom){
	if(&mom != this) init();
}

//----------------------------------------------------------
ofTessellator & ofTessellator::operator=(const ofTessellator & mom){
	if(&mom != this) init();
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
	dstmesh.clear();

	ofPolyline& polyline = const_cast<ofPolyline&>(src);
	tessAddContour( cacheTess, bIs2D?2:3, &polyline.getVertices()[0], sizeof(ofPoint), polyline.size());

	performTessellation( polyWindingMode, dstmesh, bIs2D );
}

	
//----------------------------------------------------------
void ofTessellator::tessellateToMesh( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, ofMesh & dstmesh, bool bIs2D ) {
	//Poco::ScopedLock<ofMutex> lock(mutex);
	dstmesh.clear();


	// pass vertex pointers to GLU tessellator
	for ( int j=0; j<(int)src.size(); j++ ) {
		ofPolyline& polyline = const_cast<ofPolyline&>(src[j]);

		tessAddContour( cacheTess, bIs2D?2:3, &polyline.getVertices()[0], sizeof(ofPoint), polyline.size());
	}

	performTessellation( polyWindingMode, dstmesh, bIs2D );
}

	
//----------------------------------------------------------
void ofTessellator::performTessellation(ofPolyWindingMode polyWindingMode, ofMesh& dstmesh, bool bIs2D ) {
	
	tessTesselate( cacheTess, polyWindingMode, TESS_POLYGONS, /*polySize*/ 3,  3, NULL );
	
	int numVertexes = tessGetVertexCount( cacheTess );
	int numIndices = tessGetElementCount( cacheTess )*3;

	dstmesh.clear();
	dstmesh.addVertices((ofVec3f*)tessGetVertices(cacheTess),numVertexes);
	dstmesh.addIndices((ofIndexType *)tessGetElements(cacheTess),numIndices);
	dstmesh.setMode(OF_TRIANGLES_MODE);

}
