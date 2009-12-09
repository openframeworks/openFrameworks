/*
     File:       QD3DCamera.h
 
     Contains:   Generic camera routines
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DCAMERA__
#define __QD3DCAMERA__

#ifndef __QD3D__
#include <QD3D.h>
#endif




#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if PRAGMA_IMPORT
#pragma import on
#endif

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

#if PRAGMA_ENUM_ALWAYSINT
    #if defined(__fourbyteints__) && !__fourbyteints__ 
        #define __QD3DCAMERA__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DCAMERA__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                         Data Structure Definitions                       **
 **                                                                          **
 *****************************************************************************/
/*
 *  The placement of the camera.
 */

struct TQ3CameraPlacement {
    TQ3Point3D                      cameraLocation;             /*  Location point of the camera  */
    TQ3Point3D                      pointOfInterest;            /*  Point of interest           */
    TQ3Vector3D                     upVector;                   /*  "up" vector             */
};
typedef struct TQ3CameraPlacement       TQ3CameraPlacement;
/*
 *  The range of the camera.
 */

struct TQ3CameraRange {
    float                           hither;                     /*  Hither plane, measured from "from" towards "to"   */
    float                           yon;                        /*  Yon  plane, measured from "from" towards "to"     */
};
typedef struct TQ3CameraRange           TQ3CameraRange;
/*
 *  Viewport specification.  Origin is (-1, 1), and corresponds to the 
 *  upper left-hand corner; width and height maximum is (2.0, 2.0),
 *  corresponding to the lower left-hand corner of the window.  The
 *  TQ3Viewport specifies a part of the viewPlane that gets displayed 
 *  on the window that is to be drawn.
 *  Normally, it is set with an origin of (-1.0, 1.0), and a width and
 *  height of both 2.0, specifying that the entire window is to be
 *  drawn.  If, for example, an exposure event of the window exposed
 *  the right half of the window, you would set the origin to (0, 1),
 *  and the width and height to (1.0) and (2.0), respectively.
 *
 */

struct TQ3CameraViewPort {
    TQ3Point2D                      origin;
    float                           width;
    float                           height;
};
typedef struct TQ3CameraViewPort        TQ3CameraViewPort;

struct TQ3CameraData {
    TQ3CameraPlacement              placement;
    TQ3CameraRange                  range;
    TQ3CameraViewPort               viewPort;
};
typedef struct TQ3CameraData            TQ3CameraData;
/*
 *  An orthographic camera.
 *
 *  The lens characteristics are set with the dimensions of a
 *  rectangular viewPort in the frame of the camera.
 */

struct TQ3OrthographicCameraData {
    TQ3CameraData                   cameraData;
    float                           left;
    float                           top;
    float                           right;
    float                           bottom;
};
typedef struct TQ3OrthographicCameraData TQ3OrthographicCameraData;
/*
 *  A perspective camera specified in terms of an arbitrary view plane.
 *
 *  This is most useful when setting the camera to look at a particular
 *  object.  The viewPlane is set to distance from the camera to the object.
 *  The halfWidth is set to half the width of the cross section of the object,
 *  and the halfHeight equal to the halfWidth divided by the aspect ratio
 *  of the viewPort.
 * 
 *  This is the only perspective camera with specifications for off-axis
 *  viewing, which is desirable for scrolling.
 */

struct TQ3ViewPlaneCameraData {
    TQ3CameraData                   cameraData;
    float                           viewPlane;
    float                           halfWidthAtViewPlane;
    float                           halfHeightAtViewPlane;
    float                           centerXOnViewPlane;
    float                           centerYOnViewPlane;
};
typedef struct TQ3ViewPlaneCameraData   TQ3ViewPlaneCameraData;
/*
 *  A view angle aspect camera is a perspective camera specified in 
 *  terms of the minimum view angle and the aspect ratio of X to Y.
 *
 */

struct TQ3ViewAngleAspectCameraData {
    TQ3CameraData                   cameraData;
    float                           fov;
    float                           aspectRatioXToY;
};
typedef struct TQ3ViewAngleAspectCameraData TQ3ViewAngleAspectCameraData;
/******************************************************************************
 **                                                                          **
 **                         Generic Camera routines                          **
 **                                                                          **
 *****************************************************************************/

#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3ObjectType )
Q3Camera_GetType                (TQ3CameraObject        camera);

EXTERN_API_C( TQ3Status )
Q3Camera_SetData                (TQ3CameraObject        camera,
                                 const TQ3CameraData *  cameraData);

EXTERN_API_C( TQ3Status )
Q3Camera_GetData                (TQ3CameraObject        camera,
                                 TQ3CameraData *        cameraData);

EXTERN_API_C( TQ3Status )
Q3Camera_SetPlacement           (TQ3CameraObject        camera,
                                 const TQ3CameraPlacement * placement);

EXTERN_API_C( TQ3Status )
Q3Camera_GetPlacement           (TQ3CameraObject        camera,
                                 TQ3CameraPlacement *   placement);

EXTERN_API_C( TQ3Status )
Q3Camera_SetRange               (TQ3CameraObject        camera,
                                 const TQ3CameraRange * range);

EXTERN_API_C( TQ3Status )
Q3Camera_GetRange               (TQ3CameraObject        camera,
                                 TQ3CameraRange *       range);

EXTERN_API_C( TQ3Status )
Q3Camera_SetViewPort            (TQ3CameraObject        camera,
                                 const TQ3CameraViewPort * viewPort);

EXTERN_API_C( TQ3Status )
Q3Camera_GetViewPort            (TQ3CameraObject        camera,
                                 TQ3CameraViewPort *    viewPort);

EXTERN_API_C( TQ3Status )
Q3Camera_GetWorldToView         (TQ3CameraObject        camera,
                                 TQ3Matrix4x4 *         worldToView);

EXTERN_API_C( TQ3Status )
Q3Camera_GetWorldToFrustum      (TQ3CameraObject        camera,
                                 TQ3Matrix4x4 *         worldToFrustum);

EXTERN_API_C( TQ3Status )
Q3Camera_GetViewToFrustum       (TQ3CameraObject        camera,
                                 TQ3Matrix4x4 *         viewToFrustum);


/******************************************************************************
 **                                                                          **
 **                         Specific Camera Routines                         **
 **                                                                          **
 *****************************************************************************/
/******************************************************************************
 **                                                                          **
 **                         Orthographic Camera                              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3CameraObject )
Q3OrthographicCamera_New        (const TQ3OrthographicCameraData * orthographicData);

EXTERN_API_C( TQ3Status )
Q3OrthographicCamera_GetData    (TQ3CameraObject        camera,
                                 TQ3OrthographicCameraData * cameraData);

EXTERN_API_C( TQ3Status )
Q3OrthographicCamera_SetData    (TQ3CameraObject        camera,
                                 const TQ3OrthographicCameraData * cameraData);

EXTERN_API_C( TQ3Status )
Q3OrthographicCamera_SetLeft    (TQ3CameraObject        camera,
                                 float                  left);

EXTERN_API_C( TQ3Status )
Q3OrthographicCamera_GetLeft    (TQ3CameraObject        camera,
                                 float *                left);

EXTERN_API_C( TQ3Status )
Q3OrthographicCamera_SetTop     (TQ3CameraObject        camera,
                                 float                  top);

EXTERN_API_C( TQ3Status )
Q3OrthographicCamera_GetTop     (TQ3CameraObject        camera,
                                 float *                top);

EXTERN_API_C( TQ3Status )
Q3OrthographicCamera_SetRight   (TQ3CameraObject        camera,
                                 float                  right);

EXTERN_API_C( TQ3Status )
Q3OrthographicCamera_GetRight   (TQ3CameraObject        camera,
                                 float *                right);

EXTERN_API_C( TQ3Status )
Q3OrthographicCamera_SetBottom  (TQ3CameraObject        camera,
                                 float                  bottom);

EXTERN_API_C( TQ3Status )
Q3OrthographicCamera_GetBottom  (TQ3CameraObject        camera,
                                 float *                bottom);


/******************************************************************************
 **                                                                          **
 **                         ViewPlane Camera                                 **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3CameraObject )
Q3ViewPlaneCamera_New           (const TQ3ViewPlaneCameraData * cameraData);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_GetData       (TQ3CameraObject        camera,
                                 TQ3ViewPlaneCameraData * cameraData);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_SetData       (TQ3CameraObject        camera,
                                 const TQ3ViewPlaneCameraData * cameraData);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_SetViewPlane  (TQ3CameraObject        camera,
                                 float                  viewPlane);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_GetViewPlane  (TQ3CameraObject        camera,
                                 float *                viewPlane);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_SetHalfWidth  (TQ3CameraObject        camera,
                                 float                  halfWidthAtViewPlane);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_GetHalfWidth  (TQ3CameraObject        camera,
                                 float *                halfWidthAtViewPlane);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_SetHalfHeight (TQ3CameraObject        camera,
                                 float                  halfHeightAtViewPlane);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_GetHalfHeight (TQ3CameraObject        camera,
                                 float *                halfHeightAtViewPlane);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_SetCenterX    (TQ3CameraObject        camera,
                                 float                  centerXOnViewPlane);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_GetCenterX    (TQ3CameraObject        camera,
                                 float *                centerXOnViewPlane);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_SetCenterY    (TQ3CameraObject        camera,
                                 float                  centerYOnViewPlane);

EXTERN_API_C( TQ3Status )
Q3ViewPlaneCamera_GetCenterY    (TQ3CameraObject        camera,
                                 float *                centerYOnViewPlane);


/******************************************************************************
 **                                                                          **
 **                         View Angle Aspect Camera                         **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3CameraObject )
Q3ViewAngleAspectCamera_New     (const TQ3ViewAngleAspectCameraData * cameraData);

EXTERN_API_C( TQ3Status )
Q3ViewAngleAspectCamera_SetData (TQ3CameraObject        camera,
                                 const TQ3ViewAngleAspectCameraData * cameraData);

EXTERN_API_C( TQ3Status )
Q3ViewAngleAspectCamera_GetData (TQ3CameraObject        camera,
                                 TQ3ViewAngleAspectCameraData * cameraData);

EXTERN_API_C( TQ3Status )
Q3ViewAngleAspectCamera_SetFOV  (TQ3CameraObject        camera,
                                 float                  fov);

EXTERN_API_C( TQ3Status )
Q3ViewAngleAspectCamera_GetFOV  (TQ3CameraObject        camera,
                                 float *                fov);

EXTERN_API_C( TQ3Status )
Q3ViewAngleAspectCamera_SetAspectRatio (TQ3CameraObject  camera,
                                 float                  aspectRatioXToY);

EXTERN_API_C( TQ3Status )
Q3ViewAngleAspectCamera_GetAspectRatio (TQ3CameraObject  camera,
                                 float *                aspectRatioXToY);




#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DCAMERA__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DCAMERA__RESTORE_PACKED_ENUMS)
    #pragma options(pack_enums)
#endif

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef PRAGMA_IMPORT_OFF
#pragma import off
#elif PRAGMA_IMPORT
#pragma import reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* __QD3DCAMERA__ */

