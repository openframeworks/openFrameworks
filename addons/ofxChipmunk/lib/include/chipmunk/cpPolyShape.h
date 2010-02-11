/* Copyright (c) 2007 Scott Lembcke
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Axis structure used by cpPolyShape.
typedef struct cpPolyShapeAxis{
	// normal
	cpVect n;
	// distance from origin
	cpFloat d;
} cpPolyShapeAxis;

// Convex polygon shape structure.
typedef struct cpPolyShape{
	cpShape shape;
	
	// Vertex and axis lists.
	int numVerts;
	cpVect *verts;
	cpPolyShapeAxis *axes;

	// Transformed vertex and axis lists.
	cpVect *tVerts;
	cpPolyShapeAxis *tAxes;
} cpPolyShape;

// Basic allocation functions.
cpPolyShape *cpPolyShapeAlloc(void);
cpPolyShape *cpPolyShapeInit(cpPolyShape *poly, cpBody *body, int numVerts, cpVect *verts, cpVect offset);
cpShape *cpPolyShapeNew(cpBody *body, int numVerts, cpVect *verts, cpVect offset);

// Check that a set of vertexes has a correct winding and that they are convex
int cpPolyValidate(cpVect *verts, int numVerts);

int cpPolyShapeGetNumVerts(cpShape *shape);
cpVect cpPolyShapeGetVert(cpShape *shape, int idx);

// *** inlined utility functions

// Returns the minimum distance of the polygon to the axis.
static inline cpFloat
cpPolyShapeValueOnAxis(const cpPolyShape *poly, const cpVect n, const cpFloat d)
{
	cpVect *verts = poly->tVerts;
	cpFloat min = cpvdot(n, verts[0]);
	
	int i;
	for(i=1; i<poly->numVerts; i++)
		min = cpfmin(min, cpvdot(n, verts[i]));
	
	return min - d;
}

// Returns true if the polygon contains the vertex.
static inline int
cpPolyShapeContainsVert(cpPolyShape *poly, cpVect v)
{
	cpPolyShapeAxis *axes = poly->tAxes;
	
	int i;
	for(i=0; i<poly->numVerts; i++){
		cpFloat dist = cpvdot(axes[i].n, v) - axes[i].d;
		if(dist > 0.0f) return 0;
	}
	
	return 1;
}

// Same as cpPolyShapeContainsVert() but ignores faces pointing away from the normal.
static inline int
cpPolyShapeContainsVertPartial(cpPolyShape *poly, cpVect v, cpVect n)
{
	cpPolyShapeAxis *axes = poly->tAxes;
	
	int i;
	for(i=0; i<poly->numVerts; i++){
		if(cpvdot(axes[i].n, n) < 0.0f) continue;
		cpFloat dist = cpvdot(axes[i].n, v) - axes[i].d;
		if(dist > 0.0f) return 0;
	}
	
	return 1;
}
