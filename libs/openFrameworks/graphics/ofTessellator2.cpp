#include "ofTessellator2.h"





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

int ofTessellator2::numElements;
bool ofTessellator2::initialized=false;


vector<ofPolyline> * ofTessellator2::resultOutline=NULL;
vector<ofMesh> * ofTessellator2::resultMesh=NULL;

TESStesselator * ofTessellator2::cacheTess;
TESSalloc ofTessellator2::tessAllocator;

void * cacheAllocator( void *userData, unsigned int size ){
	return malloc(size);
}

void * cacheReallocator( void *userData, void* ptr, unsigned int size ){
	return realloc(ptr,size);
}

void cacheFree( void *userData, void *ptr ){
	free (ptr);
}


//----------------------------------------------------------

//----------------------------------------------------------
void ofTessellator2::clear(){

}



//----------------------------------------------------------
void ofTessellator2::tessellateToOutline( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode,  vector<ofPolyline> & dst, bool bIs2D ) {
	//Poco::ScopedLock<ofMutex> lock(mutex);
	dst.clear();
	//clear();
	resultOutline = &dst;
	
	performTessellation( src, polyWindingMode, false /* filled */, bIs2D );
}

void ofTessellator2::tessellateToOutline( const ofPolyline & src, ofPolyWindingMode polyWindingMode, vector<ofPolyline> & dst, bool bIs2D ) {
	vector<ofPolyline> tmpVector;
	tmpVector.push_back(src);
	return tessellateToOutline(tmpVector,polyWindingMode, dst, bIs2D);
}


//----------------------------------------------------------
void ofTessellator2::tessellateToMesh( const ofPolyline& src,  ofPolyWindingMode polyWindingMode, vector<ofMesh>& dstmesh, bool bIs2D){
	vector<ofPolyline> tempPolylinesVector;
	tempPolylinesVector.push_back( src );
	tessellateToMesh( tempPolylinesVector, polyWindingMode, dstmesh, bIs2D );
}

	
//----------------------------------------------------------
void ofTessellator2::tessellateToMesh( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, vector<ofMesh> & dstmesh, bool bIs2D ) {
	//Poco::ScopedLock<ofMutex> lock(mutex);
	dstmesh.clear();
	//clear();
	resultMesh = &dstmesh;

	performTessellation( src, polyWindingMode, true /* filled */,bIs2D );
}

//----------------------------------------------------------
void ofTessellator2::tessellateToCache( const vector<ofPolyline>& src, ofPolyWindingMode polyWindingMode, ofPath::tessCache & cache, bool bIs2D){
	//Poco::ScopedLock<ofMutex> lock(mutex);
	//clear();
	resultMesh = &cache.meshes;

	performTessellation( src, polyWindingMode, true /* filled */,bIs2D );

	cache.numElements = 1;
}

//----------------------------------------------------------
void ofTessellator2::init(){
	// now get the tesselator object up and ready:
	tessAllocator.memalloc = cacheAllocator;
	tessAllocator.memrealloc = cacheReallocator;
	tessAllocator.memfree = cacheFree;
	tessAllocator.meshEdgeBucketSize=0;
	tessAllocator.meshVertexBucketSize=0;
	tessAllocator.meshFaceBucketSize=0;
	tessAllocator.dictNodeBucketSize=0;
	tessAllocator.regionBucketSize=0;
	tessAllocator.extraVertices=0;
	cacheTess = tessNewTess( &tessAllocator );
	initialized=true;
}

	
//----------------------------------------------------------
void ofTessellator2::performTessellation(const vector<ofPolyline>& polylines, ofPolyWindingMode polyWindingMode, bool bFilled, bool bIs2D ) {
	if(!initialized)init();

	numElements = 0;
	

	// pass vertex pointers to GLU tessellator
	for ( int j=0; j<(int)polylines.size(); j++ ) {
		ofPolyline& polyline = const_cast<ofPolyline&>(polylines[j]);

		tessAddContour( cacheTess, bIs2D?2:3, &polyline.getVertices()[0], sizeof(ofPoint), polyline.size());
	}
	
	tessTesselate( cacheTess, polyWindingMode, TESS_POLYGONS, /*polySize*/ 3,  3, NULL );
	
	int numVertexes = tessGetVertexCount( cacheTess );
	int numIndices = tessGetElementCount( cacheTess )*3;
	resultMesh->resize(1);
	resultMesh->at(0).clear();
	resultMesh->at(0).addVertices((ofVec3f*)tessGetVertices(cacheTess),numVertexes);
	resultMesh->at(0).addIndices(tessGetElements(cacheTess),numIndices);
	resultMesh->at(0).setMode(OF_TRIANGLES_MODE);

	//cout << resultMesh->at(0).getNumVertices() << " " << resultMesh->at(0).getNumIndices() << endl;
   	// now clear the vertices on the dynamically allocated data
	clear();

}
