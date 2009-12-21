/*
     File:       QD3D.h
 
     Contains:   Base types for Quickdraw 3D
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3D__
#define __QD3D__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif


#if TARGET_OS_MAC
#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#endif  /* TARGET_OS_MAC */


#define ushort FREDDIE
#include <stdio.h>


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
        #define __QD3D__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3D__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

#if TARGET_OS_MAC
#define OS_MACINTOSH 1
#define OS_WIN32 0
#define OS_UNIX 0
#define OS_NEXT 0
#define WINDOW_SYSTEM_MACINTOSH 1
#define WINDOW_SYSTEM_WIN32 0
#define WINDOW_SYSTEM_X11 0
#define WINDOW_SYSTEM_NEXT 0
#endif  /* TARGET_OS_MAC */

#if TARGET_OS_WIN32
#define OS_MACINTOSH 0
#define OS_WIN32 1
#define OS_UNIX 0
#define OS_NEXT 0
#define WINDOW_SYSTEM_MACINTOSH 0
#define WINDOW_SYSTEM_WIN32 1
#define WINDOW_SYSTEM_X11 0
#define WINDOW_SYSTEM_NEXT 0
#endif  /* TARGET_OS_WIN32 */

#if TARGET_OS_UNIX
#define OS_MACINTOSH 0
#define OS_WIN32 0
#define WINDOW_SYSTEM_MACINTOSH 0
#define WINDOW_SYSTEM_WIN32 0
#if NeXT
#define OS_UNIX 0
#define OS_NEXT 1
#define WINDOW_SYSTEM_X11 0
#define WINDOW_SYSTEM_NEXT 1
#else
#define OS_UNIX 1
#define OS_NEXT 0
#define WINDOW_SYSTEM_X11 1
#define WINDOW_SYSTEM_NEXT 0
#endif  /* NeXT */

#endif  /* TARGET_OS_UNIX */


/******************************************************************************
 **                                                                             **
 **                             Export Control                               **
 **                                                                          **
 *****************************************************************************/
#if TARGET_OS_WIN32
    #if defined(WIN32_EXPORTING)    /* define when building DLL */
     #define QD3D_EXPORT __declspec( dllexport )  
      #define QD3D_CALL   
       #define QD3D_CALLBACK   
   #else
      #define QD3D_EXPORT __declspec( dllimport ) 
       #define QD3D_CALL   __cdecl
        #define QD3D_CALLBACK   __cdecl 
   #endif /* WIN32_EXPORTING */
#else
 #define QD3D_EXPORT
    #define QD3D_CALL   
   #define QD3D_CALLBACK   
#endif  /*  TARGET_OS_WIN32  */


/******************************************************************************
 **                                                                          **
 **                             NULL definition                              **
 **                                                                          **
 *****************************************************************************/
 
#ifndef NULL
 #error /*   NULL is undefined?  */
#endif /* NULL */

/******************************************************************************
 **                                                                          **
 **                                 Objects                                  **
 **                                                                          **
 *****************************************************************************/
/*
 * Everything in QuickDraw 3D is an OBJECT: a bunch of data with a type,
 * deletion, duplication, and i/o methods.
 */
typedef long                            TQ3ObjectType;
typedef struct OpaqueTQ3Object*         TQ3Object;
/* */
/*
 * There are four subclasses of OBJECT:
 *  an ELEMENT, which is data that is placed in a SET
 *  a SHAREDOBJECT, which is reference-counted data that is shared
 *  VIEWs, which maintain state information for an image
 *  a PICK, which used to query a VIEW
 */
typedef TQ3Object                       TQ3ElementObject;
typedef TQ3Object                       TQ3SharedObject;
typedef TQ3Object                       TQ3ViewObject;
typedef TQ3Object                       TQ3PickObject;
/*
 * There are several types of SharedObjects:
 *  RENDERERs, which paint to a drawContext
 *  DRAWCONTEXTs, which are an interface to a device 
 *  SETs, which maintains "mathematical sets" of ELEMENTs
 *  FILEs, which maintain state information for a metafile
 *  SHAPEs, which affect the state of the View
 *  SHAPEPARTs, which contain geometry-specific data about a picking hit
 *  CONTROLLERSTATEs, which hold state of the output channels for a CONTROLLER
 *  TRACKERs, which represent a position and orientation in the user interface
 *  STRINGs, which are abstractions of text string data.
 *  STORAGE, which is an abstraction for stream-based data storage (files, memory)
 *  TEXTUREs, for sharing bitmap information for TEXTURESHADERS
 *  VIEWHINTs, which specifies viewing preferences in FILEs
 */
typedef TQ3SharedObject                 TQ3RendererObject;
typedef TQ3SharedObject                 TQ3DrawContextObject;
typedef TQ3SharedObject                 TQ3SetObject;
typedef TQ3SharedObject                 TQ3FileObject;
typedef TQ3SharedObject                 TQ3ShapeObject;
typedef TQ3SharedObject                 TQ3ShapePartObject;
typedef TQ3SharedObject                 TQ3ControllerStateObject;
typedef TQ3SharedObject                 TQ3TrackerObject;
typedef TQ3SharedObject                 TQ3StringObject;
typedef TQ3SharedObject                 TQ3StorageObject;
typedef TQ3SharedObject                 TQ3TextureObject;
typedef TQ3SharedObject                 TQ3ViewHintsObject;
/*
 * There is one types of SET:
 *  ATTRIBUTESETs, which contain ATTRIBUTEs which are inherited 
 */
typedef TQ3SetObject                    TQ3AttributeSet;
/*
 * There are many types of SHAPEs:
 *  LIGHTs, which affect how the RENDERER draws 3-D cues
 *  CAMERAs, which affects the location and orientation of the RENDERER in space
 *  GROUPs, which may contain any number of SHARED OBJECTS
 *  GEOMETRYs, which are representations of three-dimensional data
 *  SHADERs, which affect how colors are drawn on a geometry
 *  STYLEs, which affect how the RENDERER paints to the DRAWCONTEXT
 *  TRANSFORMs, which affect the coordinate system in the VIEW
 *  REFERENCEs, which are references to objects in FILEs
 *  UNKNOWN, which hold unknown objects read from a metafile.
 */
typedef TQ3ShapeObject                  TQ3GroupObject;
typedef TQ3ShapeObject                  TQ3GeometryObject;
typedef TQ3ShapeObject                  TQ3ShaderObject;
typedef TQ3ShapeObject                  TQ3StyleObject;
typedef TQ3ShapeObject                  TQ3TransformObject;
typedef TQ3ShapeObject                  TQ3LightObject;
typedef TQ3ShapeObject                  TQ3CameraObject;
typedef TQ3ShapeObject                  TQ3UnknownObject;
typedef TQ3ShapeObject                  TQ3ReferenceObject;
typedef TQ3ShapeObject                  TQ3StateOperatorObject;
/*
 * For now, there is only one type of SHAPEPARTs:
 *  MESHPARTs, which describe some part of a mesh
 */
typedef TQ3ShapePartObject              TQ3MeshPartObject;
/*
 * There are three types of MESHPARTs:
 *  MESHFACEPARTs, which describe a face of a mesh
 *  MESHEDGEPARTs, which describe a edge of a mesh
 *  MESHVERTEXPARTs, which describe a vertex of a mesh
 */
typedef TQ3MeshPartObject               TQ3MeshFacePartObject;
typedef TQ3MeshPartObject               TQ3MeshEdgePartObject;
typedef TQ3MeshPartObject               TQ3MeshVertexPartObject;
/*
 * A DISPLAY Group can be drawn to a view
 */
typedef TQ3GroupObject                  TQ3DisplayGroupObject;
/*
 * There are many types of SHADERs:
 *  SURFACESHADERs, which affect how the surface of a geometry is painted
 *  ILLUMINATIONSHADERs, which affect how lights affect the color of a surface
 */
typedef TQ3ShaderObject                 TQ3SurfaceShaderObject;
typedef TQ3ShaderObject                 TQ3IlluminationShaderObject;
/*
 * A handle to an object in a group
 */
typedef struct OpaqueTQ3GroupPosition*  TQ3GroupPosition;
/* 
 * TQ3ObjectClassNameString is used for the class name of an object
 */
enum {
    kQ3StringMaximumLength      = 1024
};


   typedef char TQ3ObjectClassNameString[kQ3StringMaximumLength];
 
   
/******************************************************************************
 **                                                                          **
 **                         Client/Server Things                             **
 **                                                                          **
 *****************************************************************************/
typedef void *                          TQ3ControllerRef;
/******************************************************************************
 **                                                                          **
 **                         Flags and Switches                               **
 **                                                                          **
 *****************************************************************************/

enum TQ3Boolean {
    kQ3False                    = 0,
    kQ3True                     = 1
};
typedef enum TQ3Boolean TQ3Boolean;


enum TQ3Switch {
    kQ3Off                      = 0,
    kQ3On                       = 1
};
typedef enum TQ3Switch TQ3Switch;


enum TQ3Status {
    kQ3Failure                  = 0,
    kQ3Success                  = 1
};
typedef enum TQ3Status TQ3Status;


enum TQ3Axis {
    kQ3AxisX                    = 0,
    kQ3AxisY                    = 1,
    kQ3AxisZ                    = 2
};
typedef enum TQ3Axis TQ3Axis;


enum TQ3PixelType {
    kQ3PixelTypeRGB32           = 0,                            /* Alpha:8 (ignored), R:8, G:8, B:8   */
    kQ3PixelTypeARGB32          = 1,                            /* Alpha:8, R:8, G:8, B:8          */
    kQ3PixelTypeRGB16           = 2,                            /* Alpha:1 (ignored), R:5, G:5, B:5   */
    kQ3PixelTypeARGB16          = 3,                            /* Alpha:1, R:5, G:5, B:5          */
    kQ3PixelTypeRGB16_565       = 4,                            /* Win32 only: 16 bits/pixel, R:5, G:6, B:5     */
    kQ3PixelTypeRGB24           = 5                             /* Win32 only: 24 bits/pixel, R:8, G:8, B:8     */
};
typedef enum TQ3PixelType TQ3PixelType;


enum TQ3Endian {
    kQ3EndianBig                = 0,
    kQ3EndianLittle             = 1
};
typedef enum TQ3Endian TQ3Endian;


enum TQ3EndCapMasks {
    kQ3EndCapNone               = 0,
    kQ3EndCapMaskTop            = 1 << 0,
    kQ3EndCapMaskBottom         = 1 << 1,
    kQ3EndCapMaskInterior       = 1 << 2
};
typedef enum TQ3EndCapMasks TQ3EndCapMasks;

typedef unsigned long                   TQ3EndCap;

   enum {
     kQ3ArrayIndexNULL = ~0  
   };
 
   
/******************************************************************************
 **                                                                          **
 **                     Point and Vector Definitions                         **
 **                                                                          **
 *****************************************************************************/

struct TQ3Vector2D {
    float                           x;
    float                           y;
};
typedef struct TQ3Vector2D              TQ3Vector2D;

struct TQ3Vector3D {
    float                           x;
    float                           y;
    float                           z;
};
typedef struct TQ3Vector3D              TQ3Vector3D;

struct TQ3Point2D {
    float                           x;
    float                           y;
};
typedef struct TQ3Point2D               TQ3Point2D;

struct TQ3Point3D {
    float                           x;
    float                           y;
    float                           z;
};
typedef struct TQ3Point3D               TQ3Point3D;

struct TQ3RationalPoint4D {
    float                           x;
    float                           y;
    float                           z;
    float                           w;
};
typedef struct TQ3RationalPoint4D       TQ3RationalPoint4D;

struct TQ3RationalPoint3D {
    float                           x;
    float                           y;
    float                           w;
};
typedef struct TQ3RationalPoint3D       TQ3RationalPoint3D;
/******************************************************************************
 **                                                                          **
 **                             Quaternion                                   **
 **                                                                          **
 *****************************************************************************/

struct TQ3Quaternion {
    float                           w;
    float                           x;
    float                           y;
    float                           z;
};
typedef struct TQ3Quaternion            TQ3Quaternion;
/******************************************************************************
 **                                                                          **
 **                             Ray Definition                               **
 **                                                                          **
 *****************************************************************************/

struct TQ3Ray3D {
    TQ3Point3D                      origin;
    TQ3Vector3D                     direction;
};
typedef struct TQ3Ray3D                 TQ3Ray3D;
/******************************************************************************
 **                                                                          **
 **                     Parameterization Data Structures                     **
 **                                                                          **
 *****************************************************************************/

struct TQ3Param2D {
    float                           u;
    float                           v;
};
typedef struct TQ3Param2D               TQ3Param2D;

struct TQ3Param3D {
    float                           u;
    float                           v;
    float                           w;
};
typedef struct TQ3Param3D               TQ3Param3D;

struct TQ3Tangent2D {
    TQ3Vector3D                     uTangent;
    TQ3Vector3D                     vTangent;
};
typedef struct TQ3Tangent2D             TQ3Tangent2D;

struct TQ3Tangent3D {
    TQ3Vector3D                     uTangent;
    TQ3Vector3D                     vTangent;
    TQ3Vector3D                     wTangent;
};
typedef struct TQ3Tangent3D             TQ3Tangent3D;
/******************************************************************************
 **                                                                          **
 **                     Polar and Spherical Coordinates                      **
 **                                                                          **
 *****************************************************************************/

struct TQ3PolarPoint {
    float                           r;
    float                           theta;
};
typedef struct TQ3PolarPoint            TQ3PolarPoint;

struct TQ3SphericalPoint {
    float                           rho;
    float                           theta;
    float                           phi;
};
typedef struct TQ3SphericalPoint        TQ3SphericalPoint;
/******************************************************************************
 **                                                                          **
 **                         Color Definition                                 **
 **                                                                          **
 *****************************************************************************/

struct TQ3ColorRGB {
    float                           r;
    float                           g;
    float                           b;
};
typedef struct TQ3ColorRGB              TQ3ColorRGB;

struct TQ3ColorARGB {
    float                           a;
    float                           r;
    float                           g;
    float                           b;
};
typedef struct TQ3ColorARGB             TQ3ColorARGB;
/******************************************************************************
 **                                                                          **
 **                                 Vertices                                 **
 **                                                                          **
 *****************************************************************************/

struct TQ3Vertex3D {
    TQ3Point3D                      point;
    TQ3AttributeSet                 attributeSet;
};
typedef struct TQ3Vertex3D              TQ3Vertex3D;
/******************************************************************************
 **                                                                          **
 **                                 Matrices                                 **
 **                                                                          **
 *****************************************************************************/

struct TQ3Matrix3x3 {
    float                           value[3][3];
};
typedef struct TQ3Matrix3x3             TQ3Matrix3x3;

struct TQ3Matrix4x4 {
    float                           value[4][4];
};
typedef struct TQ3Matrix4x4             TQ3Matrix4x4;
/******************************************************************************
 **                                                                          **
 **                             Bitmap/Pixmap                                **
 **                                                                          **
 *****************************************************************************/

struct TQ3Pixmap {
    void *                          image;
    unsigned long                   width;
    unsigned long                   height;
    unsigned long                   rowBytes;
    unsigned long                   pixelSize;                  /* MUST be 16 or 32 to use with the   Interactive Renderer on Mac OS*/
    TQ3PixelType                    pixelType;
    TQ3Endian                       bitOrder;
    TQ3Endian                       byteOrder;
};
typedef struct TQ3Pixmap                TQ3Pixmap;

struct TQ3StoragePixmap {
    TQ3StorageObject                image;
    unsigned long                   width;
    unsigned long                   height;
    unsigned long                   rowBytes;
    unsigned long                   pixelSize;                  /* MUST be 16 or 32 to use with the   Interactive Renderer on Mac OS*/
    TQ3PixelType                    pixelType;
    TQ3Endian                       bitOrder;
    TQ3Endian                       byteOrder;
};
typedef struct TQ3StoragePixmap         TQ3StoragePixmap;

struct TQ3Bitmap {
    unsigned char *                 image;
    unsigned long                   width;
    unsigned long                   height;
    unsigned long                   rowBytes;
    TQ3Endian                       bitOrder;
};
typedef struct TQ3Bitmap                TQ3Bitmap;

struct TQ3MipmapImage {                                         /* An image for use as a texture mipmap  */
    unsigned long                   width;                      /* Width of mipmap, must be power of 2   */
    unsigned long                   height;                     /* Height of mipmap, must be power of 2  */
    unsigned long                   rowBytes;                   /* Rowbytes of mipmap                    */
    unsigned long                   offset;                     /* Offset from image base to this mipmap */
};
typedef struct TQ3MipmapImage           TQ3MipmapImage;

struct TQ3Mipmap {
    TQ3StorageObject                image;                      /* Data containing the texture map and     */
                                                                /* if (useMipmapping==kQ3True) the       */
                                                                /* mipmap data                  */
    TQ3Boolean                      useMipmapping;              /* True if mipmapping should be used   */
                                                                /* and all mipmaps have been provided    */
    TQ3PixelType                    pixelType;
    TQ3Endian                       bitOrder;
    TQ3Endian                       byteOrder;
    unsigned long                   reserved;                   /* leave NULL for next version          */
    TQ3MipmapImage                  mipmaps[32];                /* The actual number of mipmaps is determined from the size of the first mipmap */
};
typedef struct TQ3Mipmap                TQ3Mipmap;



struct TQ3CompressedPixmap {
    TQ3StorageObject                compressedImage;            /* storage obj containing compressed image data */

    TQ3Endian                       imageDescByteOrder;         /* endianness of the data in the imageDesc */
    TQ3StorageObject                imageDesc;                  /* storage obj containing image description created by Quicktime to store info about compressed image */

    TQ3Boolean                      makeMipmaps;

    unsigned long                   width;
    unsigned long                   height;
    unsigned long                   pixelSize;
    TQ3PixelType                    pixelType;
};
typedef struct TQ3CompressedPixmap      TQ3CompressedPixmap;

/******************************************************************************
 **                                                                          **
 **                     Higher dimension quantities                          **
 **                                                                          **
 *****************************************************************************/

struct TQ3Area {
    TQ3Point2D                      min;
    TQ3Point2D                      max;
};
typedef struct TQ3Area                  TQ3Area;

struct TQ3PlaneEquation {
    TQ3Vector3D                     normal;
    float                           constant;
};
typedef struct TQ3PlaneEquation         TQ3PlaneEquation;

struct TQ3BoundingBox {
    TQ3Point3D                      min;
    TQ3Point3D                      max;
    TQ3Boolean                      isEmpty;
};
typedef struct TQ3BoundingBox           TQ3BoundingBox;

struct TQ3BoundingSphere {
    TQ3Point3D                      origin;
    float                           radius;
    TQ3Boolean                      isEmpty;
};
typedef struct TQ3BoundingSphere        TQ3BoundingSphere;
/*
 *  The TQ3ComputeBounds flag passed to StartBoundingBox or StartBoundingSphere
 *  calls in the View. It's a hint to the system as to how it should 
 *  compute the bbox of a shape:
 *
 *  kQ3ComputeBoundsExact:  
 *      Vertices of shapes are transformed into world space and
 *      the world space bounding box is computed from them.  Slow!
 *  
 *  kQ3ComputeBoundsApproximate: 
 *      A local space bounding box is computed from a shape's
 *      vertices.  This bbox is then transformed into world space,
 *      and its bounding box is taken as the shape's approximate
 *      bbox.  Fast but the bbox is larger than optimal.
 */

enum TQ3ComputeBounds {
    kQ3ComputeBoundsExact       = 0,
    kQ3ComputeBoundsApproximate = 1
};
typedef enum TQ3ComputeBounds TQ3ComputeBounds;


/******************************************************************************
 **                                                                          **
 **                         Object System Types                              **
 **                                                                          **
 *****************************************************************************/

typedef struct OpaqueTQ3XObjectClass*   TQ3XObjectClass;

typedef unsigned long                   TQ3XMethodType;
/*
 * Object methods
 */
#define kQ3XMethodTypeObjectUnregister      Q3_METHOD_TYPE('u','n','r','g')
/* 
 *  Return true from the metahandler if this 
 *  object can be submitted in a rendering loop 
 */
#define kQ3XMethodTypeObjectIsDrawable      Q3_METHOD_TYPE('i','s','d','r') /* return true from the metahandler if this object can be submitted in a rendering loop */
typedef CALLBACK_API_C( void , TQ3XFunctionPointer )(void );
typedef CALLBACK_API_C( TQ3XFunctionPointer , TQ3XMetaHandler )(TQ3XMethodType methodType);
/*
 * MetaHandler:
 *      When you give a metahandler to QuickDraw 3D, it is called multiple 
 *      times to build method tables, and then is thrown away. You are 
 *      guaranteed that your metahandler will never be called again after a 
 *      call that was passed a metahandler returns.
 *
 *      Your metahandler should contain a switch on the methodType passed to it
 *      and should return the corresponding method as an TQ3XFunctionPointer.
 *
 *      IMPORTANT: A metaHandler MUST always "return" a value. If you are
 *      passed a methodType that you do not understand, ALWAYS return NULL.
 *
 *      These types here are prototypes of how your functions should look.
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XObjectUnregisterMethod )(TQ3XObjectClass objectClass);
/*
 * See QD3DIO.h for the IO method types: 
 *      ObjectReadData, ObjectTraverse, ObjectWrite
 */

/******************************************************************************
 **                                                                          **
 **                             Set Types                                    **
 **                                                                          **
 *****************************************************************************/
typedef long                            TQ3ElementType;
enum {
    kQ3ElementTypeNone          = 0,
    kQ3ElementTypeUnknown       = 32,
    kQ3ElementTypeSet           = 33
};


/* 
 *  kQ3ElementTypeUnknown is a TQ3Object. 
 *  
 *      Do Q3Set_Add(s, ..., &obj) or Q3Set_Get(s, ..., &obj);
 *      
 *      Note that the object is always referenced when copying around. 
 *      
 *      Generally, it is an Unknown object, a Group of Unknown objects, or a 
 *      group of other "objects" which have been found in the metafile and
 *      have no attachment method to their parent. Be prepared to handle
 *      any or all of these cases if you actually access the set on a shape.
 *
 *  kQ3ElementTypeSet is a TQ3SetObject. 
 *  
 *      Q3Shape_GetSet(s,&o) is eqivalent to 
 *          Q3Shape_GetElement(s, kQ3ElementTypeSet, &o)
 *          
 *      Q3Shape_SetSet(s,o)  is eqivalent to 
 *          Q3Shape_SetElement(s, kQ3ElementTypeSet, &o)
 *  
 *      Note that the object is always referenced when copying around. 
 *      
 *  See the note below about the Set and Shape changes.
 */

/******************************************************************************
 **                                                                          **
 **                         Object System Macros                             **
 **                                                                          **
 *****************************************************************************/

#define Q3_FOUR_CHARACTER_CONSTANT(a,b,c,d)        \
          ((const unsigned long)                  \
          ((const unsigned long) (a) << 24) |     \
          ((const unsigned long) (b) << 16) |     \
          ((const unsigned long) (c) << 8)  |     \
          ((const unsigned long) (d)))

#define Q3_OBJECT_TYPE(a,b,c,d) \
    ((TQ3ObjectType) Q3_FOUR_CHARACTER_CONSTANT(a,b,c,d))

#define Q3_METHOD_TYPE(a,b,c,d) \
   ((TQ3XMethodType) Q3_FOUR_CHARACTER_CONSTANT(a,b,c,d))

/******************************************************************************
 **                                                                          **
 **                             Object Types                                 **
 **                                                                          **
 *****************************************************************************/
/*
 * Note:    a call to Q3Foo_GetType will return a value kQ3FooTypeBar
 *          e.g. Q3Shared_GetType(object) returns kQ3SharedTypeShape, etc.
 */

#define kQ3ObjectTypeInvalid                           0L
#define kQ3ObjectTypeView                                ((TQ3ObjectType)FOUR_CHAR_CODE('view'))
#define kQ3ObjectTypeElement                            ((TQ3ObjectType)FOUR_CHAR_CODE('elmn'))
    #define kQ3ElementTypeAttribute                     ((TQ3ObjectType)FOUR_CHAR_CODE('eatt'))
#define kQ3ObjectTypePick                               ((TQ3ObjectType)FOUR_CHAR_CODE('pick'))
    #define kQ3PickTypeWindowPoint                      ((TQ3ObjectType)FOUR_CHAR_CODE('pkwp'))
    #define kQ3PickTypeWindowRect                       ((TQ3ObjectType)FOUR_CHAR_CODE('pkwr'))
    #define kQ3PickTypeWorldRay                         ((TQ3ObjectType)FOUR_CHAR_CODE('pkry'))
#define kQ3ObjectTypeShared                             ((TQ3ObjectType)FOUR_CHAR_CODE('shrd'))
    #define kQ3SharedTypeRenderer                       ((TQ3ObjectType)FOUR_CHAR_CODE('rddr'))
        #define kQ3RendererTypeWireFrame                ((TQ3ObjectType)FOUR_CHAR_CODE('wrfr'))
        #define kQ3RendererTypeGeneric                  ((TQ3ObjectType)FOUR_CHAR_CODE('gnrr'))
        #define kQ3RendererTypeInteractive              ((TQ3ObjectType)FOUR_CHAR_CODE('ctwn'))
    #define kQ3SharedTypeShape                          ((TQ3ObjectType)FOUR_CHAR_CODE('shap'))

            
       #define kQ3ShapeTypeGeometry                    ((TQ3ObjectType)FOUR_CHAR_CODE('gmtr'))
            #define kQ3GeometryTypeBox                  ((TQ3ObjectType)FOUR_CHAR_CODE('box '))
            #define kQ3GeometryTypeGeneralPolygon       ((TQ3ObjectType)FOUR_CHAR_CODE('gpgn'))
            #define kQ3GeometryTypeLine                 ((TQ3ObjectType)FOUR_CHAR_CODE('line'))
            #define kQ3GeometryTypeMarker               ((TQ3ObjectType)FOUR_CHAR_CODE('mrkr'))
            #define kQ3GeometryTypePixmapMarker         ((TQ3ObjectType)FOUR_CHAR_CODE('mrkp'))
            #define kQ3GeometryTypeMesh                 ((TQ3ObjectType)FOUR_CHAR_CODE('mesh'))
            #define kQ3GeometryTypeNURBCurve            ((TQ3ObjectType)FOUR_CHAR_CODE('nrbc'))
            #define kQ3GeometryTypeNURBPatch            ((TQ3ObjectType)FOUR_CHAR_CODE('nrbp'))
            #define kQ3GeometryTypePoint                ((TQ3ObjectType)FOUR_CHAR_CODE('pnt '))
            #define kQ3GeometryTypePolygon              ((TQ3ObjectType)FOUR_CHAR_CODE('plyg'))
            #define kQ3GeometryTypePolyLine             ((TQ3ObjectType)FOUR_CHAR_CODE('plyl'))
            #define kQ3GeometryTypeTriangle             ((TQ3ObjectType)FOUR_CHAR_CODE('trng'))
            #define kQ3GeometryTypeTriGrid              ((TQ3ObjectType)FOUR_CHAR_CODE('trig'))
            #define kQ3GeometryTypeCone                 ((TQ3ObjectType)FOUR_CHAR_CODE('cone'))
            #define kQ3GeometryTypeCylinder             ((TQ3ObjectType)FOUR_CHAR_CODE('cyln'))
            #define kQ3GeometryTypeDisk                 ((TQ3ObjectType)FOUR_CHAR_CODE('disk'))
            #define kQ3GeometryTypeEllipse              ((TQ3ObjectType)FOUR_CHAR_CODE('elps'))
            #define kQ3GeometryTypeEllipsoid            ((TQ3ObjectType)FOUR_CHAR_CODE('elpd'))
            #define kQ3GeometryTypePolyhedron           ((TQ3ObjectType)FOUR_CHAR_CODE('plhd'))
            #define kQ3GeometryTypeTorus                ((TQ3ObjectType)FOUR_CHAR_CODE('tors'))
            #define kQ3GeometryTypeTriMesh              ((TQ3ObjectType)FOUR_CHAR_CODE('tmsh'))

            
       #define kQ3ShapeTypeShader                      ((TQ3ObjectType)FOUR_CHAR_CODE('shdr'))
            #define kQ3ShaderTypeSurface                ((TQ3ObjectType)FOUR_CHAR_CODE('sush'))
                #define kQ3SurfaceShaderTypeTexture     ((TQ3ObjectType)FOUR_CHAR_CODE('txsu'))
            #define kQ3ShaderTypeIllumination           ((TQ3ObjectType)FOUR_CHAR_CODE('ilsh'))
                #define kQ3IlluminationTypePhong        ((TQ3ObjectType)FOUR_CHAR_CODE('phil'))
                #define kQ3IlluminationTypeLambert      ((TQ3ObjectType)FOUR_CHAR_CODE('lmil'))
                #define kQ3IlluminationTypeNULL         ((TQ3ObjectType)FOUR_CHAR_CODE('nuil'))
        #define kQ3ShapeTypeStyle                       ((TQ3ObjectType)FOUR_CHAR_CODE('styl'))
            #define kQ3StyleTypeBackfacing              ((TQ3ObjectType)FOUR_CHAR_CODE('bckf'))
            #define kQ3StyleTypeInterpolation           ((TQ3ObjectType)FOUR_CHAR_CODE('intp'))
            #define kQ3StyleTypeFill                    ((TQ3ObjectType)FOUR_CHAR_CODE('fist'))
            #define kQ3StyleTypePickID                  ((TQ3ObjectType)FOUR_CHAR_CODE('pkid'))
            #define kQ3StyleTypeReceiveShadows          ((TQ3ObjectType)FOUR_CHAR_CODE('rcsh'))
            #define kQ3StyleTypeHighlight               ((TQ3ObjectType)FOUR_CHAR_CODE('high'))
            #define kQ3StyleTypeSubdivision             ((TQ3ObjectType)FOUR_CHAR_CODE('sbdv'))
            #define kQ3StyleTypeOrientation             ((TQ3ObjectType)FOUR_CHAR_CODE('ofdr'))
            #define kQ3StyleTypePickParts               ((TQ3ObjectType)FOUR_CHAR_CODE('pkpt'))
            #define kQ3StyleTypeAntiAlias               ((TQ3ObjectType)FOUR_CHAR_CODE('anti'))
            #define kQ3StyleTypeFog                     ((TQ3ObjectType)FOUR_CHAR_CODE('fogg'))

            
       #define kQ3ShapeTypeTransform                   ((TQ3ObjectType)FOUR_CHAR_CODE('xfrm'))
            #define kQ3TransformTypeMatrix              ((TQ3ObjectType)FOUR_CHAR_CODE('mtrx'))
            #define kQ3TransformTypeScale               ((TQ3ObjectType)FOUR_CHAR_CODE('scal'))
            #define kQ3TransformTypeTranslate           ((TQ3ObjectType)FOUR_CHAR_CODE('trns'))
            #define kQ3TransformTypeRotate              ((TQ3ObjectType)FOUR_CHAR_CODE('rott'))
            #define kQ3TransformTypeRotateAboutPoint    ((TQ3ObjectType)FOUR_CHAR_CODE('rtap'))
            #define kQ3TransformTypeRotateAboutAxis     ((TQ3ObjectType)FOUR_CHAR_CODE('rtaa'))
            #define kQ3TransformTypeQuaternion          ((TQ3ObjectType)FOUR_CHAR_CODE('qtrn'))
            #define kQ3TransformTypeReset               ((TQ3ObjectType)FOUR_CHAR_CODE('rset'))
        #define kQ3ShapeTypeLight                       ((TQ3ObjectType)FOUR_CHAR_CODE('lght'))
            #define kQ3LightTypeAmbient                 ((TQ3ObjectType)FOUR_CHAR_CODE('ambn'))
            #define kQ3LightTypeDirectional             ((TQ3ObjectType)FOUR_CHAR_CODE('drct'))
            #define kQ3LightTypePoint                   ((TQ3ObjectType)FOUR_CHAR_CODE('pntl'))
            #define kQ3LightTypeSpot                    ((TQ3ObjectType)FOUR_CHAR_CODE('spot'))

            
       #define kQ3ShapeTypeCamera                      ((TQ3ObjectType)FOUR_CHAR_CODE('cmra'))
            #define kQ3CameraTypeOrthographic           ((TQ3ObjectType)FOUR_CHAR_CODE('orth'))
            #define kQ3CameraTypeViewPlane              ((TQ3ObjectType)FOUR_CHAR_CODE('vwpl'))
            #define kQ3CameraTypeViewAngleAspect        ((TQ3ObjectType)FOUR_CHAR_CODE('vana'))
        #define kQ3ShapeTypeStateOperator               ((TQ3ObjectType)FOUR_CHAR_CODE('stop'))
            #define kQ3StateOperatorTypePush            ((TQ3ObjectType)FOUR_CHAR_CODE('push'))
            #define kQ3StateOperatorTypePop             ((TQ3ObjectType)FOUR_CHAR_CODE('pop '))
        #define kQ3ShapeTypeGroup                       ((TQ3ObjectType)FOUR_CHAR_CODE('grup'))
            #define kQ3GroupTypeDisplay                 ((TQ3ObjectType)FOUR_CHAR_CODE('dspg'))
                #define kQ3DisplayGroupTypeOrdered      ((TQ3ObjectType)FOUR_CHAR_CODE('ordg'))
                #define kQ3DisplayGroupTypeIOProxy      ((TQ3ObjectType)FOUR_CHAR_CODE('iopx'))
            #define kQ3GroupTypeLight                   ((TQ3ObjectType)FOUR_CHAR_CODE('lghg'))
            #define kQ3GroupTypeInfo                    ((TQ3ObjectType)FOUR_CHAR_CODE('info'))

            
       #define kQ3ShapeTypeUnknown                     ((TQ3ObjectType)FOUR_CHAR_CODE('unkn'))
            #define kQ3UnknownTypeText                  ((TQ3ObjectType)FOUR_CHAR_CODE('uktx'))
            #define kQ3UnknownTypeBinary                ((TQ3ObjectType)FOUR_CHAR_CODE('ukbn'))
        #define kQ3ShapeTypeReference                   ((TQ3ObjectType)FOUR_CHAR_CODE('rfrn'))
            #define kQ3ReferenceTypeExternal            ((TQ3ObjectType)FOUR_CHAR_CODE('rfex'))
    #define kQ3SharedTypeSet                            ((TQ3ObjectType)FOUR_CHAR_CODE('set '))
        #define kQ3SetTypeAttribute                     ((TQ3ObjectType)FOUR_CHAR_CODE('attr'))
    #define kQ3SharedTypeDrawContext                    ((TQ3ObjectType)FOUR_CHAR_CODE('dctx'))
        #define kQ3DrawContextTypePixmap                ((TQ3ObjectType)FOUR_CHAR_CODE('dpxp'))
        #define kQ3DrawContextTypeMacintosh             ((TQ3ObjectType)FOUR_CHAR_CODE('dmac'))
        #define kQ3DrawContextTypeWin32DC               ((TQ3ObjectType)FOUR_CHAR_CODE('dw32'))
        #define kQ3DrawContextTypeDDSurface             ((TQ3ObjectType)FOUR_CHAR_CODE('ddds'))
        #define kQ3DrawContextTypeX11                   ((TQ3ObjectType)FOUR_CHAR_CODE('dx11'))
    #define kQ3SharedTypeTexture                        ((TQ3ObjectType)FOUR_CHAR_CODE('txtr'))
        #define kQ3TextureTypePixmap                    ((TQ3ObjectType)FOUR_CHAR_CODE('txpm')) 
       #define kQ3TextureTypeMipmap                    ((TQ3ObjectType)FOUR_CHAR_CODE('txmm')) 
       #define kQ3TextureTypeCompressedPixmap          ((TQ3ObjectType)FOUR_CHAR_CODE('txcp'))

            
   #define kQ3SharedTypeFile                           ((TQ3ObjectType)FOUR_CHAR_CODE('file'))
    #define kQ3SharedTypeStorage                        ((TQ3ObjectType)FOUR_CHAR_CODE('strg'))
        #define kQ3StorageTypeMemory                    ((TQ3ObjectType)FOUR_CHAR_CODE('mems'))
        #define kQ3MemoryStorageTypeHandle              ((TQ3ObjectType)FOUR_CHAR_CODE('hndl'))
        #define kQ3StorageTypeUnix                      ((TQ3ObjectType)FOUR_CHAR_CODE('uxst'))
        #define kQ3UnixStorageTypePath                  ((TQ3ObjectType)FOUR_CHAR_CODE('unix'))
        #define kQ3StorageTypeMacintosh                 ((TQ3ObjectType)FOUR_CHAR_CODE('macn'))
        #define kQ3MacintoshStorageTypeFSSpec           ((TQ3ObjectType)FOUR_CHAR_CODE('macp'))                 
       #define kQ3StorageTypeWin32                     ((TQ3ObjectType)FOUR_CHAR_CODE('wist'))
    #define kQ3SharedTypeString                         ((TQ3ObjectType)FOUR_CHAR_CODE('strn'))
        #define kQ3StringTypeCString                    ((TQ3ObjectType)FOUR_CHAR_CODE('strc'))
    #define kQ3SharedTypeShapePart                      ((TQ3ObjectType)FOUR_CHAR_CODE('sprt'))
        #define kQ3ShapePartTypeMeshPart                ((TQ3ObjectType)FOUR_CHAR_CODE('spmh'))
            #define kQ3MeshPartTypeMeshFacePart         ((TQ3ObjectType)FOUR_CHAR_CODE('mfac'))
            #define kQ3MeshPartTypeMeshEdgePart         ((TQ3ObjectType)FOUR_CHAR_CODE('medg'))
            #define kQ3MeshPartTypeMeshVertexPart       ((TQ3ObjectType)FOUR_CHAR_CODE('mvtx'))
    #define kQ3SharedTypeControllerState                ((TQ3ObjectType)FOUR_CHAR_CODE('ctst'))
    #define kQ3SharedTypeTracker                        ((TQ3ObjectType)FOUR_CHAR_CODE('trkr'))
    #define kQ3SharedTypeViewHints                      ((TQ3ObjectType)FOUR_CHAR_CODE('vwhn'))
    #define kQ3SharedTypeEndGroup                       ((TQ3ObjectType)FOUR_CHAR_CODE('endg'))

/******************************************************************************
 **                                                                          **
 **                         QuickDraw 3D System Routines                     **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3Initialize                    (void);

EXTERN_API_C( TQ3Status )
Q3Exit                          (void);

EXTERN_API_C( TQ3Boolean )
Q3IsInitialized                 (void);

EXTERN_API_C( TQ3Status )
Q3GetVersion                    (unsigned long *        majorRevision,
                                 unsigned long *        minorRevision);

/*
 *  Q3GetReleaseVersion returns the release version number,
 *  in the format of the first four bytes of a 'vers' resource
 *  (e.g. 0x01518000 ==> 1.5.1 release).
 */
EXTERN_API_C( TQ3Status )
Q3GetReleaseVersion             (unsigned long *        releaseRevision);


/******************************************************************************
 **                                                                          **
 **                         ObjectClass Routines                             **
 **                                                                          **
 *****************************************************************************/
/* 
 *  New object system calls to query the object system.
 *
 *  These comments to move to the stubs file before final release, they 
 *  are here for documentation for developers using early seeds.
 */
/*
 *  Given a class name as a string return the associated class type for the 
 *  class, may return kQ3Failure if the string representing the class is 
 *  invalid.
 */
EXTERN_API_C( TQ3Status )
Q3ObjectHierarchy_GetTypeFromString (TQ3ObjectClassNameString  objectClassString,
                                 TQ3ObjectType *        objectClassType);

/*
 *  Given a class type as return the associated string for the class name, 
 *  may return kQ3Failure if the type representing the class is invalid.
 */
EXTERN_API_C( TQ3Status )
Q3ObjectHierarchy_GetStringFromType (TQ3ObjectType      objectClassType,
                                 TQ3ObjectClassNameString  objectClassString);

/* 
 *  Return true if the class with this type is registered, false if not 
 */
EXTERN_API_C( TQ3Boolean )
Q3ObjectHierarchy_IsTypeRegistered (TQ3ObjectType       objectClassType);

/* 
 *  Return true if the class with this name is registered, false if not 
 */
EXTERN_API_C( TQ3Boolean )
Q3ObjectHierarchy_IsNameRegistered (const char *        objectClassName);

/*
 * TQ3SubClassData is used when querying the object system for
 * the subclasses of a particular parent type:
 */
#endif  /* CALL_NOT_IN_CARBON */


struct TQ3SubClassData {
    unsigned long                   numClasses;                 /* the # of subclass types found for a parent class */
    TQ3ObjectType *                 classTypes;                 /* an array containing the class types */
};
typedef struct TQ3SubClassData          TQ3SubClassData;
/*
 *  Given a parent type and an instance of the TQ3SubClassData struct fill
 *  it in with the number and class types of all of the subclasses immediately
 *  below the parent in the class hierarchy.  Return kQ3Success to indicate no
 *  errors occurred, else kQ3Failure.
 *
 *  NOTE:  This function will allocate memory for the classTypes array.  Be 
 *  sure to call Q3ObjectClass_EmptySubClassData to free this memory up.
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3ObjectHierarchy_GetSubClassData (TQ3ObjectType        objectClassType,
                                 TQ3SubClassData *      subClassData);

/*
 *  Given an instance of the TQ3SubClassData struct free all memory allocated 
 *  by the Q3ObjectClass_GetSubClassData call.
 *
 *  NOTE: This call MUST be made after a call to Q3ObjectClass_GetSubClassData
 *  to avoid memory leaks.
 */
EXTERN_API_C( TQ3Status )
Q3ObjectHierarchy_EmptySubClassData (TQ3SubClassData *  subClassData);


/******************************************************************************
 **                                                                          **
 **                             Object Routines                              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3Object_Dispose                (TQ3Object              object);

EXTERN_API_C( TQ3Object )
Q3Object_Duplicate              (TQ3Object              object);

EXTERN_API_C( TQ3Status )
Q3Object_Submit                 (TQ3Object              object,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Boolean )
Q3Object_IsDrawable             (TQ3Object              object);

EXTERN_API_C( TQ3Boolean )
Q3Object_IsWritable             (TQ3Object              object,
                                 TQ3FileObject          theFile);


/******************************************************************************
 **                                                                          **
 **                         Object Type Routines                             **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3ObjectType )
Q3Object_GetType                (TQ3Object              object);

EXTERN_API_C( TQ3ObjectType )
Q3Object_GetLeafType            (TQ3Object              object);

EXTERN_API_C( TQ3Boolean )
Q3Object_IsType                 (TQ3Object              object,
                                 TQ3ObjectType          theType);


/******************************************************************************
 **                                                                          **
 **                         Shared Object Routines                           **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3ObjectType )
Q3Shared_GetType                (TQ3SharedObject        sharedObject);

EXTERN_API_C( TQ3SharedObject )
Q3Shared_GetReference           (TQ3SharedObject        sharedObject);

/* 
 *  Q3Shared_IsReferenced
 *      Returns kQ3True if there is more than one reference to sharedObject.
 *      Returns kQ3False if there is ONE reference to sharedObject.
 *  
 *  This call is intended to allow applications and plug-in objects to delete
 *  objects of which they hold THE ONLY REFERENCE. This is useful when
 *  caching objects, etc.
 *  
 *  Although many may be tempted, DO NOT DO THIS:
 *      while (Q3Shared_IsReferenced(foo)) { Q3Object_Dispose(foo); }
 *  
 *  Your application will crash and no one will buy it. Chapter 11 is 
 *  never fun (unless you short the stock). Thanks.
 */
EXTERN_API_C( TQ3Boolean )
Q3Shared_IsReferenced           (TQ3SharedObject        sharedObject);

/*
 *  Q3Shared_GetEditIndex
 *      Returns the "serial number" of sharedObject. Usefuly for caching 
 *      object information. Returns 0 on error.
 *      
 *      Hold onto this number to determine if an object has changed since you
 *      last built your caches... To validate, do:
 *      
 *      if (Q3Shared_GetEditIndex(foo) == oldFooEditIndex) {
 *          // Cache is valid
 *      } else {
 *          // Cache is invalid
 *          RebuildSomeSortOfCache(foo);
 *          oldFooEditIndex = Q3Shared_GetEditIndex(foo);
 *      }
 */
EXTERN_API_C( unsigned long )
Q3Shared_GetEditIndex           (TQ3SharedObject        sharedObject);

/*
 *  Q3Shared_Edited
 *      Bumps the "serial number" of sharedObject to a different value. This
 *      call is intended to be used solely from a plug-in object which is 
 *      shared. Call this whenever your private instance data changes.
 *      
 *      Returns kQ3Failure iff sharedObject is not a shared object, OR
 *          QuickDraw 3D isn't initialized.
 */
EXTERN_API_C( TQ3Status )
Q3Shared_Edited                 (TQ3SharedObject        sharedObject);


/******************************************************************************
 **                                                                          **
 **                             Shape Routines                               **
 **                                                                          **
 *****************************************************************************/
/*
 *  QuickDraw 3D 1.5 Note:
 *
 *  Shapes and Sets are now (sort of) polymorphic.
 *
 *      You may call Q3Shape_Foo or Q3Set_Foo on a shape or a set.
 *      The following calls are identical, in implementation:
 *
 *          Q3Shape_GetElement          =   Q3Set_Get
 *          Q3Shape_AddElement          =   Q3Set_Add
 *          Q3Shape_ContainsElement     =   Q3Set_Contains
 *          Q3Shape_GetNextElementType  =   Q3Set_GetNextElementType
 *          Q3Shape_EmptyElements       =   Q3Set_Empty
 *          Q3Shape_ClearElement        =   Q3Set_Clear
 *
 *  All of these calls accept a shape or a set as their first parameter.
 *
 *  The Q3Shape_GetSet and Q3ShapeSetSet calls are implemented via a new
 *  element type kQ3ElementTypeSet. See the note above about 
 *  kQ3ElementTypeSet;
 *
 *  It is important to note that the new Q3Shape_...Element... calls do not
 *  create a set on a shape and then add the element to it. This data is
 *  attached directly to the shape. Therefore, it is possible for an element
 *  to exist on a shape without a set existing on it as well. 
 *
 *  In your application, if you attach an element to a shape like this:
 *      (this isn't checking for errors for simplicity)
 *
 *      set = Q3Set_New();
 *      Q3Set_AddElement(set, kMyElemType, &data);
 *      Q3Shape_SetSet(shape, set);
 *
 *  You should retrieve it in the same manner:
 *
 *      Q3Shape_GetSet(shape, &set);
 *      if (Q3Set_Contains(set, kMyElemType) == kTrue) {
 *          Q3Set_Get(set, kMyElemType, &data);
 *      }
 *
 *  Similarly, if you attach data to a shape with the new calls:
 *
 *      Q3Shape_AddElement(shape, kMyElemType, &data);
 *
 *  You should retrieve it in the same manner:
 *
 *      if (Q3Shape_ContainsElement(set, kMyElemType) == kTrue) {
 *          Q3Shape_GetElement(set, kMyElemType, &data);
 *      }
 *
 *  This really becomes an issue when dealing with version 1.0 and version 1.1 
 *  metafiles.
 *
 *  When attempting to find a particular element on a shape, you should
 *  first check with Q3Shape_GetNextElementType or Q3Shape_GetElement, then,
 *  Q3Shape_GetSet(s, &set) (or Q3Shape_GetElement(s, kQ3ElementTypeSet, &set))
 *  and then Q3Shape_GetElement(set, ...).
 *
 *  In terms of implementation, Q3Shape_SetSet and Q3Shape_GetSet should only be
 *  used for sets of information that are shared among multiple shapes.
 *  
 *  Q3Shape_AddElement, Q3Shape_GetElement, etc. calls should only be used
 *  for elements that are unique for a particular shape.
 *  
 */
EXTERN_API_C( TQ3ObjectType )
Q3Shape_GetType                 (TQ3ShapeObject         shape);

EXTERN_API_C( TQ3Status )
Q3Shape_GetSet                  (TQ3ShapeObject         shape,
                                 TQ3SetObject *         theSet);

EXTERN_API_C( TQ3Status )
Q3Shape_SetSet                  (TQ3ShapeObject         shape,
                                 TQ3SetObject           theSet);

EXTERN_API_C( TQ3Status )
Q3Shape_AddElement              (TQ3ShapeObject         shape,
                                 TQ3ElementType         theType,
                                 const void *           data);

EXTERN_API_C( TQ3Status )
Q3Shape_GetElement              (TQ3ShapeObject         shape,
                                 TQ3ElementType         theType,
                                 void *                 data);

EXTERN_API_C( TQ3Boolean )
Q3Shape_ContainsElement         (TQ3ShapeObject         shape,
                                 TQ3ElementType         theType);

EXTERN_API_C( TQ3Status )
Q3Shape_GetNextElementType      (TQ3ShapeObject         shape,
                                 TQ3ElementType *       theType);

EXTERN_API_C( TQ3Status )
Q3Shape_EmptyElements           (TQ3ShapeObject         shape);

EXTERN_API_C( TQ3Status )
Q3Shape_ClearElement            (TQ3ShapeObject         shape,
                                 TQ3ElementType         theType);


/******************************************************************************
 **                                                                          **
 **                         Color Table Routines                             **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3Bitmap_Empty                  (TQ3Bitmap *            bitmap);

EXTERN_API_C( unsigned long )
Q3Bitmap_GetImageSize           (unsigned long          width,
                                 unsigned long          height);



#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3D__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3D__RESTORE_PACKED_ENUMS)
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

#endif /* __QD3D__ */

