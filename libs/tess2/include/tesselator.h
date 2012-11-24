/*
** SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008) 
** Copyright (C) [dates of first publication] Silicon Graphics, Inc.
** All Rights Reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
** of the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
** 
** The above copyright notice including the dates of first publication and either this
** permission notice or a reference to http://oss.sgi.com/projects/FreeB/ shall be
** included in all copies or substantial portions of the Software. 
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
** INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
** PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL SILICON GRAPHICS, INC.
** BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
** OR OTHER DEALINGS IN THE SOFTWARE.
** 
** Except as contained in this notice, the name of Silicon Graphics, Inc. shall not
** be used in advertising or otherwise to promote the sale, use or other dealings in
** this Software without prior written authorization from Silicon Graphics, Inc.
*/
/*
** Author: Mikko Mononen, July 2009.
*/

#ifndef TESSELATOR_H
#define TESSELATOR_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined( __APPLE_CC__)
	#include <TargetConditionals.h>
#endif

enum TessWindingRule
{
	TESS_WINDING_ODD,
	TESS_WINDING_NONZERO,
	TESS_WINDING_POSITIVE,
	TESS_WINDING_NEGATIVE,
	TESS_WINDING_ABS_GEQ_TWO,
};

// Tesselation result element types:
// TESS_TRIANGLES
//   Each element is defined as array of 3 TESSindex.
//   The 3 values in the array defines indices to each vertex of a triangle.
// TESS_CONNECTED_TRIANGLES
//   Each element is defined as array of 6 TESSindex.
//   The first 3 values in the array defines indices to each vertex of a triangle.
//   The second 3 values in the array defines indices to each of neighbour element or -1 if there is no neighbour.
// TESS_BOUNDARY_CONTOURS
//   Each element is defined as array of 2 TESSindex.
//   The first value is index to first vertex in contour and the second value is number of vertices in the contour.
enum TessElementType
{
	TESS_POLYGONS,
	TESS_CONNECTED_POLYGONS,
	TESS_BOUNDARY_CONTOURS,
};

typedef float TESSreal;
//note this shouldn't be defined(TARGET_OS_IPHONE) as its always defined either 0 or 1
#if TARGET_OS_IPHONE || ANDROID || __ARMEL__
typedef unsigned short TESSindex;
#else
typedef unsigned int TESSindex;
#endif

typedef struct TESStesselator TESStesselator;
typedef struct TESSalloc TESSalloc;

#define TESS_UNDEF ((TESSindex)(~(TESSindex)0))

// Custom memory allocator interface.
// The internal memory allocator allocates mesh edges, vertices and faces
// as well as dictionary nodes and active regions in buckets and uses simple
// freelist to speed up the allocation. The bucket size should roughly match your
// expected input data. For example if you process only hundreds of vertices,
// a bucket size of 128 might be ok, where as when processing thousands of vertices
// bucket size of 1024 might be approproate. The bucket size is a compromise between
// how often to allocate memory from the system versus how much extra space the system
// should allocate. Reasonable defaults are show in commects below, they will be used if
// the bucket sizes are zero.
// 
// The use may left the memrealloc to be null. In that case, the tesselator will not try to
// dynamically grow int's internal arrays. The tesselator only needs the reallocation when it
// has found intersecting segments and needs to add new vertex. This defency can be cured by
// allocating some extra vertices beforehand. The 'extraVertices' variable allows to specify
// number of expected extra vertices.  
struct TESSalloc
{
	void *(*memalloc)( void *userData, unsigned int size );
	void *(*memrealloc)( void *userData, void* ptr, unsigned int size );
	void (*memfree)( void *userData, void *ptr );
	void* userData;				// User data passed to the allocator functions.
	int meshEdgeBucketSize;		// 512
	int meshVertexBucketSize;	// 512
	int meshFaceBucketSize;		// 256
	int dictNodeBucketSize;		// 512
	int regionBucketSize;		// 256
	int extraVertices;			// Number of extra vertices allocated for the priority queue.
};
	
// tessNewTess() - Creates a new tesselator.
// Use tessDeleteTess() to delete the tesselator.
// Returns:
//   new tesselator object.
TESStesselator* tessNewTess( TESSalloc* alloc );

// tessDeleteTess() - Deletes a tesselator.
// Parameters:
//   tess - pointer to tesselator object to be deleted.
void tessDeleteTess( TESStesselator *tess );

// tessAddContour() - Adds a contour to be tesselated.
// The type of the vertex coordinates is assumed to be TESSreal.
// Parameters:
//   tess - pointer to tesselator object.
//   size - number of coordinates per vertex. Must be 2 or 3.
//   pointer - pointer to the first coordinate of the first vertex in the array.
//   stride - defines offset in bytes between consecutive vertices.
//   count - number of vertices in contour.
void tessAddContour( TESStesselator *tess, int size, const void* pointer, int stride, int count );

// tessTesselate() - tesselate contours.
// Parameters:
//   tess - pointer to tesselator object.
//   windingRule - winding rules used for tesselation, must be one of TessWindingRule.
//   elementType - defines the tesselation result element type, must be one of TessElementType.
//   polySize - defines maximum vertices per polygons if output is polygons.
//   vertexSize - defines the number of coordinates in tesselation result vertex, must be 2 or 3.
//   normal - defines the normal of the input contours, of null the normal is calculated automatically.
// Returns:
//   1 if succeed, 0 if failed.
int tessTesselate( TESStesselator *tess, int windingRule, int elementType, int polySize, int vertexSize, const TESSreal* normal );

// tessGetVertexCount() - Returns number of vertices in the tesselated output.
int tessGetVertexCount( TESStesselator *tess );

// tessGetVertices() - Returns pointer to first coordinate of first vertex.
const TESSreal* tessGetVertices( TESStesselator *tess );

// tessGetVertexIndices() - Returns pointer to first vertex index.
// Vertex indices can be used to map the generated vertices to the original vertices.
// Every point added using tessAddContour() will get a new index starting at 0.
// New vertices generated at the intersections of segments are assigned value TESS_UNDEF.
const TESSindex* tessGetVertexIndices( TESStesselator *tess );
	
// tessGetElementCount() - Returns number of elements in the the tesselated output.
int tessGetElementCount( TESStesselator *tess );

// tessGetElements() - Returns pointer to the first element.
const TESSindex* tessGetElements( TESStesselator *tess );

#ifdef __cplusplus
};
#endif

#endif // TESSELATOR_H
