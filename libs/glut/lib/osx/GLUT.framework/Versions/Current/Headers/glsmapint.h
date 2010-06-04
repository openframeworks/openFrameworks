#ifndef __glsmapint_h__
#define __glsmapint_h__

/* Copyright (c) Mark J. Kilgard, 1998. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

#include "glsmap.h"

enum { X = 0, Y = 1, Z = 2 };

#define INITFACE(mesh) \
	int steps = mesh->steps; \
	int sqsteps = mesh->steps * mesh->steps

#define FACE(side,y,x) \
	mesh->face[(side)*sqsteps + (y)*steps + (x)]

#define FACExy(side,i,j) \
	(&FACE(side,i,j).x)

#define FACEst(side,i,j) \
	(&FACE(side,i,j).s)

#define INITBACK(mesh) \
	int allrings = mesh->rings + mesh->edgeExtend; \
	int ringedspokes = allrings * mesh->steps

#define BACK(edge,ring,spoke) \
	mesh->back[(edge)*ringedspokes + (ring)*mesh->steps + (spoke)]

#define BACKxy(edge,ring,spoke) \
	(&BACK(edge,ring,spoke).x)

#define BACKst(edge,ring,spoke) \
	(&BACK(edge,ring,spoke).s)

typedef struct _STXY {
	GLfloat s, t;
	GLfloat x, y;
} STXY;

typedef struct _SphereMapMesh {

	int refcnt;

	int steps;
	int rings;
	int edgeExtend;

	STXY *face;
	STXY *back;

} SphereMapMesh;

struct _SphereMap {

	/* Shared sphere map mesh vertex data. */
	SphereMapMesh *mesh;

	/* Texture object ids. */
	GLuint smapTexObj;
	GLuint viewTexObjs[6];
	GLuint viewTexObj;

        /* Flags */
        SphereMapFlags flags;

	/* Texture dimensions must be a power of two. */
	int viewTexDim;  /* view texture dimension */
	int smapTexDim;  /* sphere map texture dimension */

	/* Viewport origins for view and sphere map rendering. */
	int viewOrigin[2];
	int smapOrigin[2];

	/* Viewing vectors. */
	GLfloat eye[3];
	GLfloat up[3];
	GLfloat obj[3];

	/* Projection parameters. */
	GLfloat viewNear;
	GLfloat viewFar;

	/* Rendering callbacks. */
	void (*positionLights)(int view, void *context);
	void (*drawView)(int view, void *context);

	/* Application specified callback data. */
	void *context;

};

/* Library internal routines. */
extern void __smapDrawSphereMapMeshSide(SphereMapMesh *mesh, int side);
extern void __smapDrawSphereMapMeshBack(SphereMapMesh *mesh);
extern void __smapValidateSphereMapMesh(SphereMapMesh *mesh);

#endif /* __glsmapint_h__ */
