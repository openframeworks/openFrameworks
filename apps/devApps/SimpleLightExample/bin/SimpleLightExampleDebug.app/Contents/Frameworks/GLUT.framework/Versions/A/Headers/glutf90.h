#ifndef __glutf90_h__
#define __glutf90_h__

/* Copyright (c) Mark J. Kilgard & Willam F. Mitchell, 1998. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

/* This header provides the binding interface for William Mitchell's
   f90gl Fortran 90 GLUT binding.  Other GLUT language bindings
   can and should use this interace. */

/* I appreciate the guidance from William Mitchell
   (mitchell@cam.nist.gov) in developing this friend interface
   for use by the f90gl package.  See ../../README.fortran */

#include <GLUT/glut.h>

#ifndef GLUTCALLBACK
	#define GLUTCALLBACK
#endif
#ifndef APIENTRY
	#define APIENTRY
#endif

/* Which callback enumerants for the __glutSetFCB/__glutGetFCB routines. */
/* NOTE These values are part of a binary interface for the f90gl Fortran
   90 binding and so must NOT changes (additions are allowed). */

/* GLUTwindow callbacks. */
#define GLUT_FCB_DISPLAY         0    /* GLUTdisplayFCB */
#define GLUT_FCB_RESHAPE         1    /* GLUTreshapeFCB */
#define GLUT_FCB_MOUSE           2    /* GLUTmouseFCB */
#define GLUT_FCB_MOTION          3    /* GLUTmotionFCB */
#define GLUT_FCB_PASSIVE         4    /* GLUTpassiveFCB */
#define GLUT_FCB_ENTRY           5    /* GLUTentryFCB */
#define GLUT_FCB_KEYBOARD        6    /* GLUTkeyboardFCB */
#define GLUT_FCB_KEYBOARD_UP     7    /* GLUTkeyboardFCB */
#define GLUT_FCB_WINDOW_STATUS   8    /* GLUTwindowStatusFCB */
#define GLUT_FCB_VISIBILITY      9    /* GLUTvisibilityFCB */
#define GLUT_FCB_SPECIAL         10   /* GLUTspecialFCB */
#define GLUT_FCB_SPECIAL_UP      11   /* GLUTspecialFCB */
#define GLUT_FCB_BUTTON_BOX      12   /* GLUTbuttonBoxFCB */
#define GLUT_FCB_DIALS           13   /* GLUTdialsFCB */
#define GLUT_FCB_SPACE_MOTION    14   /* GLUTspaceMotionFCB */
#define GLUT_FCB_SPACE_ROTATE    15   /* GLUTspaceRotateFCB */
#define GLUT_FCB_SPACE_BUTTON    16   /* GLUTspaceButtonFCB */
#define GLUT_FCB_TABLET_MOTION   17   /* GLUTtabletMotionFCB */
#define GLUT_FCB_TABLET_BUTTON   18   /* GLUTtabletButtonFCB */
#define GLUT_FCB_JOYSTICK        19   /* GLUTjoystickFCB */
#define GLUT_FCB_WMCLOSE         20   /* GLUTwmcloseFCB */
/* Non-GLUTwindow callbacks. */
#define GLUT_FCB_OVERLAY_DISPLAY 100  /* GLUTdisplayFCB */
#define GLUT_FCB_SELECT          101  /* GLUTselectFCB */
#define GLUT_FCB_TIMER           102  /* GLUTtimerFCB */

/* GLUT Fortran callback function types. */
typedef void (GLUTCALLBACK *GLUTdisplayFCB) (void);
typedef void (GLUTCALLBACK *GLUTwmcloseFCB) (void);
typedef void (GLUTCALLBACK *GLUTreshapeFCB) (int *, int *);
/* NOTE the pressed key is int, not unsigned char for Fortran! */
typedef void (GLUTCALLBACK *GLUTkeyboardFCB) (int *, int *, int *);
typedef void (GLUTCALLBACK *GLUTmouseFCB) (int *, int *, int *, int *);
typedef void (GLUTCALLBACK *GLUTmotionFCB) (int *, int *);
typedef void (GLUTCALLBACK *GLUTpassiveFCB) (int *, int *);
typedef void (GLUTCALLBACK *GLUTentryFCB) (int *);
typedef void (GLUTCALLBACK *GLUTwindowStatusFCB) (int *);
typedef void (GLUTCALLBACK *GLUTvisibilityFCB) (int *);
typedef void (GLUTCALLBACK *GLUTspecialFCB) (int *, int *, int *);
typedef void (GLUTCALLBACK *GLUTbuttonBoxFCB) (int *, int *);
typedef void (GLUTCALLBACK *GLUTdialsFCB) (int *, int *);
typedef void (GLUTCALLBACK *GLUTspaceMotionFCB) (int *, int *, int *);
typedef void (GLUTCALLBACK *GLUTspaceRotateFCB) (int *, int *, int *);
typedef void (GLUTCALLBACK *GLUTspaceButtonFCB) (int *, int *);
typedef void (GLUTCALLBACK *GLUTtabletMotionFCB) (int *, int *);
typedef void (GLUTCALLBACK *GLUTtabletButtonFCB) (int *, int *, int *, int *);
typedef void (GLUTCALLBACK *GLUTjoystickFCB) (unsigned int *buttonMask, int *x, int *y, int *z);

typedef void (GLUTCALLBACK *GLUTselectFCB) (int *);
typedef void (GLUTCALLBACK *GLUTtimerFCB) (int *);
typedef void (GLUTCALLBACK *GLUTmenuStateFCB) (int *);  /* DEPRICATED. */
typedef void (GLUTCALLBACK *GLUTmenuStatusFCB) (int *, int *, int *);
typedef void (GLUTCALLBACK *GLUTidleFCB) (void);

/* Functions that set and return Fortran callback functions. */
extern void* APIENTRY __glutGetFCB(int which);
extern void APIENTRY __glutSetFCB(int which, void *func);

#endif  /* __glutf90_h__ */
