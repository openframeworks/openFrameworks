#include "ofTessellator.h"


#ifndef TARGET_WIN32
#define CALLBACK
#endif




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

#include <Poco/ScopedLock.h>
//ofMutex ofTessellator::mutex;


//---------------------------- for combine callback:
std::vector <double*> ofTessellator::newVertices;
//---------------------------- store all the polygon vertices:
std::vector <double> ofTessellator::ofShapePolyVertexs;

GLint ofTessellator::currentTriType; // GL_TRIANGLES, GL_TRIANGLE_FAN or GL_TRIANGLE_STRIP
int ofTessellator::numElements;
bool ofTessellator::initialized=false;


vector<ofPolyline> * ofTessellator::resultOutline=NULL;
vector<ofMesh> * ofTessellator::resultMesh=NULL;

GLUtesselator * ofTessellator::ofShapeTobj=NULL;



//----------------------------------------------------------
void CALLBACK ofTessellator::error(GLenum errCode){
	const GLubyte* estring;
	estring = gluErrorString( errCode);
	ofLog(OF_LOG_ERROR, "tessError: %s", estring);
}


//----------------------------------------------------------
void CALLBACK ofTessellator::begin(GLint type){
	// type can be GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP, or GL_TRIANGLES
	// or GL_LINE_LOOP if GLU_TESS_BOUNDARY_ONLY was set to TRUE
		
	currentTriType=type;
	numElements++;
	if(type==GL_LINE_LOOP){
		if(int(resultOutline->size())<numElements){
			resultOutline->push_back(ofPolyline());
		}
		resultOutline->at(numElements-1).clear();
		resultOutline->at(numElements-1).setClosed(true);
	}else{
		if(int(resultMesh->size())<numElements){
			//cout << "adding mesh size: " <<resultMesh->size()<< endl;
			resultMesh->push_back(ofMesh());
		}
		resultMesh->at(numElements-1).clear();
		resultMesh->at(numElements-1).setMode(ofGetOFPrimitiveMode(type));
	}
	
}

//----------------------------------------------------------
void CALLBACK ofTessellator::end(){

	
}


//----------------------------------------------------------
void CALLBACK ofTessellator::vertex( void* data){

	ofPoint p( ( (double *)data)[0], ( (double *)data)[1], ( (double *)data)[2] );
	if(currentTriType==GL_LINE_LOOP){
		resultOutline->at(numElements-1).addVertex( p );
	}else{
		resultMesh->at(numElements-1).addVertex(p);
	}
}


//----------------------------------------------------------
void CALLBACK ofTessellator::combine( GLdouble coords[3], void* vertex_data[4], GLfloat weight[4], void** outData){
	// make storage space for a new vertex
	double * vertex = new double[3];
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
    newVertices.push_back(vertex);
    *outData = vertex;
}

//----------------------------------------------------------
void ofTessellator::clear(){

    // combine callback also makes new vertices, let's clear them:
    for(vector<double*>::iterator itr=newVertices.begin();
        itr!=newVertices.end();
        ++itr){
        delete [] (*itr);
    }
    newVertices.clear();
}



//----------------------------------------------------------
void ofTessellator::tessellateToOutline( const vector<ofPolyline>& src, int polyWindingMode,  vector<ofPolyline> & dst, bool bIs2D ) {
	//Poco::ScopedLock<ofMutex> lock(mutex);
	dst.clear();
	//clear();
	resultOutline = &dst;
	
	performTessellation( src, polyWindingMode, false /* filled */, bIs2D );
}

void ofTessellator::tessellateToOutline( const ofPolyline & src, int polyWindingMode, vector<ofPolyline> & dst, bool bIs2D ) {
	vector<ofPolyline> tmpVector;
	tmpVector.push_back(src);
	return tessellateToOutline(tmpVector,polyWindingMode, dst, bIs2D);
}


//----------------------------------------------------------
void ofTessellator::tessellateToMesh( const ofPolyline& src,  int polyWindingMode, vector<ofMesh>& dstmesh, bool bIs2D){
	vector<ofPolyline> tempPolylinesVector;
	tempPolylinesVector.push_back( src );
	tessellateToMesh( tempPolylinesVector, polyWindingMode, dstmesh, bIs2D );
}

	
//----------------------------------------------------------
void ofTessellator::tessellateToMesh( const vector<ofPolyline>& src, int polyWindingMode, vector<ofMesh> & dstmesh, bool bIs2D ) {
	//Poco::ScopedLock<ofMutex> lock(mutex);
	dstmesh.clear();
	//clear();
	resultMesh = &dstmesh;

	performTessellation( src, polyWindingMode, true /* filled */,bIs2D );
}

//----------------------------------------------------------
void ofTessellator::tessellateToCache( const vector<ofPolyline>& src, int polyWindingMode, ofPath::tessCache & cache, bool bIs2D){
	//Poco::ScopedLock<ofMutex> lock(mutex);
	//clear();
	resultMesh = &cache.meshes;

	performTessellation( src, polyWindingMode, true /* filled */,bIs2D );

	cache.numElements = numElements;
}

//----------------------------------------------------------
void ofTessellator::init(){
	// now get the tesselator object up and ready:
	ofShapeTobj = gluNewTess();

	#if defined( TARGET_OSX)
	#ifndef MAC_OS_X_VERSION_10_5
	#define OF_NEED_GLU_FIX
	#endif
	#endif

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// MAC - XCODE USERS PLEASE NOTE - some machines will not be able to compile the code below
		// if this happens uncomment the "OF_NEED_GLU_FIX" line below and it
		// should compile also please post to the forums with the error message, you OS X version,
		// Xcode verison and the CPU type - PPC or Intel. Thanks!
		// (note: this is known problem based on different version of glu.h, we are working on a fix)
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		//#define OF_NEED_GLU_FIX

	#ifdef OF_NEED_GLU_FIX
	#define OF_GLU_CALLBACK_HACK (void(CALLBACK*)(...))
	#else
	#define OF_GLU_CALLBACK_HACK (void(CALLBACK*)())
	#endif
	gluTessCallback( ofShapeTobj, GLU_TESS_BEGIN, OF_GLU_CALLBACK_HACK &ofTessellator::begin);
	gluTessCallback( ofShapeTobj, GLU_TESS_VERTEX, OF_GLU_CALLBACK_HACK &ofTessellator::vertex);
	gluTessCallback( ofShapeTobj, GLU_TESS_COMBINE, OF_GLU_CALLBACK_HACK &ofTessellator::combine);
	gluTessCallback( ofShapeTobj, GLU_TESS_END, OF_GLU_CALLBACK_HACK &ofTessellator::end);
	gluTessCallback( ofShapeTobj, GLU_TESS_ERROR, OF_GLU_CALLBACK_HACK &ofTessellator::error);
	gluTessProperty( ofShapeTobj, GLU_TESS_TOLERANCE, 0);
	initialized=true;
}

	
//----------------------------------------------------------
void ofTessellator::performTessellation(const vector<ofPolyline>& polylines, int polyWindingMode, bool bFilled, bool bIs2D ) {
	if(!initialized)init();

	gluTessProperty( ofShapeTobj, GLU_TESS_WINDING_RULE, polyWindingMode );
	if (!bFilled){
		gluTessProperty( ofShapeTobj, GLU_TESS_BOUNDARY_ONLY, true);
	} else {
		gluTessProperty( ofShapeTobj, GLU_TESS_BOUNDARY_ONLY, false);
	}
	
	/* ------------------------------------------
	 for 2d, this next call (normal) likely helps speed up ....
	 quote : The computation of the normal represents about 10% of
	 the computation time. For example, if all polygons lie in
	 the x-y plane, you can provide the normal by using the
	 -------------------------------------------  */
	/*if( bIs2D) {
		gluTessNormal(ofShapeTobj, 0.0, 0.0, 1.0);
	}*/

	gluTessBeginPolygon( ofShapeTobj, NULL);

	numElements = 0;
	int n=0;
	
	// allocate all the memory we need as doubles GLU doesn't support floats :S
	// we need to allocate everything in one go to avoid vector reallocation
	int totalVerts = 0;
	for ( int j=0; j<(int)polylines.size(); j++ ) {
		const ofPolyline& polyline = polylines[j];
		totalVerts+=polyline.size();
	}
	ofShapePolyVertexs.resize(totalVerts*3);

	// pass vertex pointers to GLU tessellator
	for ( int j=0; j<(int)polylines.size(); j++ ) {
		const ofPolyline& polyline = polylines[j];
		int firstVertex = n;
		gluTessBeginContour( ofShapeTobj );

		for ( int i=0; i<(int)polyline.size(); i++,n+=3) {
			ofShapePolyVertexs[n+0] = polyline[i].x;
			ofShapePolyVertexs[n+1] = polyline[i].y;
			ofShapePolyVertexs[n+2] = polyline[i].z;
			gluTessVertex( ofShapeTobj, &ofShapePolyVertexs[n], &ofShapePolyVertexs[n]);
		}

		if(polyline.isClosed()){
			gluTessVertex( ofShapeTobj, &ofShapePolyVertexs[firstVertex], &ofShapePolyVertexs[firstVertex]);
		}

		gluTessEndContour( ofShapeTobj );
	}
	
	gluTessEndPolygon( ofShapeTobj);
	//gluDeleteTess( ofShapeTobj);
	//ofShapeTobj = NULL;
	
   	// now clear the vertices on the dynamically allocated data
	clear();

}
