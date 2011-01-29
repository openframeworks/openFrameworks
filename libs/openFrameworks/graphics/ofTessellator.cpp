/*
 *  ofTessellator.cpp
 *  openFrameworks
 *
 *  Created by theo on 28/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

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
ofMutex ofTessellator::mutex;


//---------------------------- for combine callback:
std::vector <double*> ofTessellator::newVertices;
//---------------------------- store all the polygon vertices:
std::vector <double*> ofTessellator::ofShapePolyVertexs;

GLint ofTessellator::currentTriType; // GL_TRIANGLES, GL_TRIANGLE_FAN or GL_TRIANGLE_STRIP
vector<ofPoint> ofTessellator::vertices;


vector<ofPolyline> ofTessellator::resultOutline;
vector<ofVertexData> ofTessellator::resultMesh;



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
	
	vertices.clear();
	
}

//----------------------------------------------------------
void CALLBACK ofTessellator::end(){
	// here we take our big pile of vertices and push them at the mesh

	// deal with mesh elements (triangles, triangle fan, triangle strip)

	if(currentTriType!=GL_LINE_LOOP){
		resultMesh.push_back(ofVertexData(ofGetOFPrimitiveMode(currentTriType), vertices) );
	}else if ( currentTriType == GL_LINE_LOOP ){ // outline
		resultOutline.push_back( ofPolyline(vertices) );
		resultOutline.back().setClosed(true);
		// close the loop
		/*if ( vertices.size()>0 ) {
			resultOutline.addVertex( vertices[0] );
		}*/
	}
	else{
		//ofLog( OF_LOG_WARNING, "ofTessellate: unrecognized type '%i' (expected GL_TRIANGLES, GL_TRIANGLE_FAN or GL_TRIANGLE_STRIP)", currentTriType );
	}
	
}


//----------------------------------------------------------
void CALLBACK ofTessellator::vertex( void* data){
	
	ofPoint p( ( (double *)data)[0], ( (double *)data)[1], ( (double *)data)[2] );
	vertices.push_back( p );
}


//----------------------------------------------------------
void CALLBACK ofTessellator::combine( GLdouble coords[3], void* vertex_data[4], GLfloat weight[4], void** outData){
	// make storage space for a new vertex
    double* vertex = new double[3];
    newVertices.push_back(vertex);
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
    *outData = vertex;
}

//----------------------------------------------------------
void ofTessellator::clear(){
	// -------------------------------------------------
    // ---------------- delete newly created vertices !
     for(vector<double*>::iterator itr=ofShapePolyVertexs.begin();
        itr!=ofShapePolyVertexs.end();
        ++itr){
        delete [] (*itr);
    }
    ofShapePolyVertexs.clear();

    // combine callback also makes new vertices, let's clear them:
    for(vector<double*>::iterator itr=newVertices.begin();
        itr!=newVertices.end();
        ++itr){
        delete [] (*itr);
    }
    newVertices.clear();
}



//----------------------------------------------------------
vector<ofPolyline> ofTessellator::tessellateToOutline( const vector<ofPolyline>& polylines, int polyWindingMode, bool bIs2D ) {
	Poco::ScopedLock<ofMutex> lock(mutex);
	
	clear();
	resultOutline.clear();
	
	performTessellation( polylines, polyWindingMode, false /* filled */, bIs2D );
	

	return resultOutline;
}

vector<ofPolyline> ofTessellator::tessellateToOutline( const ofPolyline & polyline, int polyWindingMode, bool bIs2D ) {
	vector<ofPolyline> tmpVector;
	tmpVector.push_back(polyline);
	return tessellateToOutline(tmpVector,polyWindingMode, bIs2D);
}


//----------------------------------------------------------
vector<ofVertexData> ofTessellator::tessellateToMesh( const ofPolyline& polyline, int polyWindingMode, bool bIs2D ){
	vector<ofPolyline> tempPolylinesVector;
	tempPolylinesVector.push_back( polyline );
	return tessellateToMesh( tempPolylinesVector, polyWindingMode, bIs2D );
}

	
//----------------------------------------------------------
vector<ofVertexData> ofTessellator::tessellateToMesh( const vector<ofPolyline>& polylines, int polyWindingMode, bool bIs2D ) {

	Poco::ScopedLock<ofMutex> lock(mutex);
	
	clear();
	resultMesh.clear();

	performTessellation( polylines, polyWindingMode, true /* filled */, bIs2D );
	
	
	return resultMesh;
		
}


	
//----------------------------------------------------------
void ofTessellator::performTessellation(const vector<ofPolyline>& polylines, int polyWindingMode, bool bFilled, bool bIs2D ) {
	
	// now get the tesselator object up and ready:
	GLUtesselator * ofShapeTobj = gluNewTess();
	
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
	
	gluTessProperty( ofShapeTobj, GLU_TESS_WINDING_RULE, polyWindingMode );
	if (!bFilled){
		gluTessProperty( ofShapeTobj, GLU_TESS_BOUNDARY_ONLY, true);
	} else {
		gluTessProperty( ofShapeTobj, GLU_TESS_BOUNDARY_ONLY, false);
	}
	gluTessProperty( ofShapeTobj, GLU_TESS_TOLERANCE, 0);
	
	/* ------------------------------------------
	 for 2d, this next call (normal) likely helps speed up ....
	 quote : The computation of the normal represents about 10% of
	 the computation time. For example, if all polygons lie in
	 the x-y plane, you can provide the normal by using the
	 -------------------------------------------  */
	if( bIs2D) {
		gluTessNormal(ofShapeTobj, 0.0, 0.0, 1.0);
	}

	gluTessBeginPolygon( ofShapeTobj, NULL);

	int currentStartVertex = 0;
	
	for ( int j=0; j<(int)polylines.size(); j++ ) {
		const ofPolyline& polyline = polylines[j];
		for ( int i=0; i<(int)polyline.size(); i++ ) {
			double* point = new double[3];
			point[0] = polyline[i].x;
			point[1] = polyline[i].y;
			point[2] = polyline[i].z;
			ofShapePolyVertexs.push_back(point);
		}
		if ( polyline.isClosed() && polyline.size()>0 ) {
			double* point = new double[3];
			point[0] = polyline[0].x;
			point[1] = polyline[0].y;
			point[2] = polyline[0].z;
			ofShapePolyVertexs.push_back(point);
		}
		
		gluTessBeginContour( ofShapeTobj );
		for (int i=currentStartVertex; i<(int)ofShapePolyVertexs.size(); i++) {
			gluTessVertex( ofShapeTobj, ofShapePolyVertexs[i], ofShapePolyVertexs[i]);
		}
		gluTessEndContour( ofShapeTobj );
		
		currentStartVertex = (int)ofShapePolyVertexs.size();
		
	}
	
	
	// no matter what we did / do, we need to delete the tesselator object
	gluTessEndPolygon( ofShapeTobj);
	gluDeleteTess( ofShapeTobj);
	ofShapeTobj = NULL;
	
   	// now clear the vertices on the dynamically allocated data
   	clear();

}
