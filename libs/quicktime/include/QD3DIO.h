/*
     File:       QD3DIO.h
 
     Contains:   QuickDraw 3D IO API
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DIO__
#define __QD3DIO__

#ifndef __QD3D__
#include <QD3D.h>
#endif


#ifndef __QD3DDRAWCONTEXT__
#include <QD3DDrawContext.h>
#endif

#ifndef __QD3DVIEW__
#include <QD3DView.h>
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
        #define __QD3DIO__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DIO__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                                 Basic Types                              **                                                 
 **                                                                          **
 *****************************************************************************/
typedef unsigned char                   TQ3Uns8;
typedef signed char                     TQ3Int8;
typedef unsigned short                  TQ3Uns16;
typedef signed short                    TQ3Int16;
typedef unsigned long                   TQ3Uns32;
typedef signed long                     TQ3Int32;
#if TARGET_RT_BIG_ENDIAN

struct TQ3Uns64 {
    unsigned long                   hi;
    unsigned long                   lo;
};
typedef struct TQ3Uns64                 TQ3Uns64;

struct TQ3Int64 {
    signed long                     hi;
    unsigned long                   lo;
};
typedef struct TQ3Int64                 TQ3Int64;
#else

struct TQ3Uns64 {
    unsigned long                   lo;
    unsigned long                   hi;
};
typedef struct TQ3Uns64                 TQ3Uns64;

struct TQ3Int64 {
    unsigned long                   lo;
    signed long                     hi;
};
typedef struct TQ3Int64                 TQ3Int64;
#endif  /* TARGET_RT_BIG_ENDIAN */


typedef float                           TQ3Float32;
typedef double                          TQ3Float64;
typedef TQ3Uns32                        TQ3Size;
/******************************************************************************
 **                                                                          **
 **                                 File Types                               **
 **                                                                          **
 *****************************************************************************/

enum TQ3FileModeMasks {
    kQ3FileModeNormal           = 0,
    kQ3FileModeStream           = 1 << 0,
    kQ3FileModeDatabase         = 1 << 1,
    kQ3FileModeText             = 1 << 2
};
typedef enum TQ3FileModeMasks TQ3FileModeMasks;

typedef unsigned long                   TQ3FileMode;
/******************************************************************************
 **                                                                          **
 **                                 Method Types                             **
 **                                                                          **
 *****************************************************************************/
/*
 *  IO Methods
 *
 *  The IO system treats all objects as groups of typed information.
 *  When you register your element or attribute, the "elementType" is the 
 *  binary type of your object, the "elementName" the ascii type.
 *  
 *  All objects in the metafile are made up of a "root" or parent object which
 *  defines the instantiated object type. You may define the format of your 
 *  data any way you wish as long as you use the primitives types above and the
 *  routines below.
 *
 *  Root Objects are often appended with additional child objects, called 
 *  subobjects. You may append your object with other QuickDraw 3D objects.
 *  
 *  Writing is straightforward: an object traverses itself any other objects 
 *  that make it up, then writes its own data. Writing uses two methods: 
 *  TQ3XObjectTraverseMethod and TQ3XObjectWriteMethod.
 *
 *  The TQ3XObjectTraverseMethod method should:
 *  + First, Determine if the data should be written 
 *      - if you don't want to write out your object after examining your
 *          data, return kQ3Success in your Traverse method without calling
 *          any other submit calls.
 *  + Next, calculate the size of your object on disk
 *  + Gather whatever state from the view you need to preserve
 *      - you may access the view state NOW, as the state of the
 *          view duing your TQ3XObjectWriteMethod will not be valid. You may
 *          pass a temporary buffer to your write method.
 *  + Submit your view write data using Q3View_SubmitWriteData
 *      - note that you MUST call this before any other "_Submit" call.
 *      - you may pass in a "deleteMethod" for your data. This method
 *          will be called whether or not your write method succeeds or fails.
 *  + Submit your subobjects to the view
 *  
 *  The TQ3XObjectWriteMethod method should:
 *  + Write your data format to the file using the primitives routines below.
 *      - If you passed a "deleteMethod" in your Q3View_SubmitWriteData, that
 *          method will be called upon exit of your write method.
 *
 *  Reading is less straightforward because your root object and
 *  any subobjects must be read inside of your TQ3XObjectReadDataMethod. There 
 *  is an implicit state contained in the file while reading, which you must 
 *  be aware of. When you first enter the read method, you must physically 
 *  read in your data format using the primitives routines until
 *  
 *  Q3File_IsEndOfData(file) == kQ3True
 *  
 *  Generally, your data format should be self-descriptive such that you do not
 *  need to call Q3File_IsEndOfData to determine if you are done reading. 
 *  However, this call is useful for determining zero-sized object or 
 *  determining the end of an object's data.
 *  
 *  Once you have read in all the data, you may collect subobjects. A metafile
 *  object ONLY has subobjects if it is in a container. The call
 *  
 *  Q3File_IsEndOfContainer(file)
 *  
 *  returns kQ3False if subobjects exist, and kQ3True if subobjects do not 
 *  exist.
 *  
 *  At this point, you may use
 *  
 *  Q3File_GetNextObjectType
 *  Q3File_IsNextObjectOfType
 *  Q3File_ReadObject
 *  Q3File_SkipObject
 *  
 *  to iterate through the subobjects until Q3File_IsEndOfContainer(file) 
 *  is kQ3True.
 * 
 */

/*
 * IO Methods
 */
enum {
    kQ3XMethodTypeObjectFileVersion = FOUR_CHAR_CODE('vers'),   /* version */
    kQ3XMethodTypeObjectTraverse = FOUR_CHAR_CODE('trvs'),      /* byte count */
    kQ3XMethodTypeObjectTraverseData = FOUR_CHAR_CODE('trvd'),  /* byte count */
    kQ3XMethodTypeObjectWrite   = FOUR_CHAR_CODE('writ'),       /* Dump info to file */
    kQ3XMethodTypeObjectReadData = FOUR_CHAR_CODE('rddt'),      /* Read info from file into buffer or, attach read data to parent */
    kQ3XMethodTypeObjectRead    = FOUR_CHAR_CODE('read'),
    kQ3XMethodTypeObjectAttach  = FOUR_CHAR_CODE('attc')
};

/*
 *  TQ3XObjectTraverseMethod
 *
 *  For "elements" (meaning "attributes, too), you will be passed NULL for 
 *  object. Sorry, custom objects will be available in the next major revision.
 *
 *  The "data" is a pointer to your internal element data.
 *
 *  The view is the current traversal view.
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XObjectTraverseMethod )(TQ3Object object, void *data, TQ3ViewObject view);
/*
 *  TQ3XObjectTraverseDataMethod
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XObjectTraverseDataMethod )(TQ3Object object, void *data, TQ3ViewObject view);
/*
 *  TQ3XObjectWriteMethod
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XObjectWriteMethod )(const void *object, TQ3FileObject theFile);
/*
 *  Custom object writing 
 */
typedef CALLBACK_API_C( void , TQ3XDataDeleteMethod )(void *data);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3XView_SubmitWriteData         (TQ3ViewObject          view,
                                 TQ3Size                size,
                                 void *                 data,
                                 TQ3XDataDeleteMethod   deleteData);

EXTERN_API_C( TQ3Status )
Q3XView_SubmitSubObjectData     (TQ3ViewObject          view,
                                 TQ3XObjectClass        objectClass,
                                 unsigned long          size,
                                 void *                 data,
                                 TQ3XDataDeleteMethod   deleteData);

/*
 *  TQ3XObjectReadMethod
 */
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( TQ3Object , TQ3XObjectReadMethod )(TQ3FileObject theFile);
/*
 *  TQ3XObjectReadDataMethod
 *
 *  For "elements" (meaning "attributes", too), you must allocate stack space 
 *  and call Q3Set_Add on "parentObject", which is an TQ3SetObject.
 *
 *  Otherwise, parentObject is whatever object your element is a subobject of...
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XObjectReadDataMethod )(TQ3Object parentObject, TQ3FileObject theFile);
/*
 *  TQ3XObjectAttachMethod
 */
typedef CALLBACK_API_C( TQ3Status , TQ3XObjectAttachMethod )(TQ3Object childObject, TQ3Object parentObject);


/******************************************************************************
 **                                                                          **
 **                             Versioning                                   **
 **                                                                          **
 *****************************************************************************/

#define Q3FileVersion(majorVersion, minorVersion)  (TQ3FileVersion) \
 ((((TQ3Uns32) majorVersion & 0xFFFF) << 16) | ((TQ3Uns32) minorVersion & 0xFFFF))

typedef unsigned long                   TQ3FileVersion;
#define kQ3FileVersionCurrent   Q3FileVersion(1,6)

/******************************************************************************
 **                                                                          **
 **                             File Routines                                **
 **                                                                          **
 *****************************************************************************/
/*
 * Creation and accessors
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3FileObject )
Q3File_New                      (void);

EXTERN_API_C( TQ3Status )
Q3File_GetStorage               (TQ3FileObject          theFile,
                                 TQ3StorageObject *     storage);

EXTERN_API_C( TQ3Status )
Q3File_SetStorage               (TQ3FileObject          theFile,
                                 TQ3StorageObject       storage);

/*
 * Opening, and accessing "open" state, closing/cancelling
 */
EXTERN_API_C( TQ3Status )
Q3File_OpenRead                 (TQ3FileObject          theFile,
                                 TQ3FileMode *          mode);

EXTERN_API_C( TQ3Status )
Q3File_OpenWrite                (TQ3FileObject          theFile,
                                 TQ3FileMode            mode);

EXTERN_API_C( TQ3Status )
Q3File_IsOpen                   (TQ3FileObject          theFile,
                                 TQ3Boolean *           isOpen);

EXTERN_API_C( TQ3Status )
Q3File_GetMode                  (TQ3FileObject          theFile,
                                 TQ3FileMode *          mode);

EXTERN_API_C( TQ3Status )
Q3File_GetVersion               (TQ3FileObject          theFile,
                                 TQ3FileVersion *       version);

EXTERN_API_C( TQ3Status )
Q3File_Close                    (TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3File_Cancel                   (TQ3FileObject          theFile);

/*
 * Writing (Application)
 */
EXTERN_API_C( TQ3Status )
Q3View_StartWriting             (TQ3ViewObject          view,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3ViewStatus )
Q3View_EndWriting               (TQ3ViewObject          view);

/*
 * Reading (Application)
 */
EXTERN_API_C( TQ3ObjectType )
Q3File_GetNextObjectType        (TQ3FileObject          theFile);

EXTERN_API_C( TQ3Boolean )
Q3File_IsNextObjectOfType       (TQ3FileObject          theFile,
                                 TQ3ObjectType          ofType);

EXTERN_API_C( TQ3Object )
Q3File_ReadObject               (TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3File_SkipObject               (TQ3FileObject          theFile);

EXTERN_API_C( TQ3Boolean )
Q3File_IsEndOfData              (TQ3FileObject          theFile);

EXTERN_API_C( TQ3Boolean )
Q3File_IsEndOfContainer         (TQ3FileObject          theFile,
                                 TQ3Object              rootObject);

EXTERN_API_C( TQ3Boolean )
Q3File_IsEndOfFile              (TQ3FileObject          theFile);

/*    
 *  External file references
 */
EXTERN_API_C( TQ3Status )
Q3File_MarkAsExternalReference  (TQ3FileObject          theFile,
                                 TQ3SharedObject        sharedObject);

EXTERN_API_C( TQ3GroupObject )
Q3File_GetExternalReferences    (TQ3FileObject          theFile);

/*    
 *  Tracking editing in read-in objects with custom elements
 */
EXTERN_API_C( TQ3Status )
Q3Shared_ClearEditTracking      (TQ3SharedObject        sharedObject);

EXTERN_API_C( TQ3Boolean )
Q3Shared_GetEditTrackingState   (TQ3SharedObject        sharedObject);

/*    
 *  Reading objects inside a group one-by-one
 */
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3FileReadGroupStateMasks {
    kQ3FileReadWholeGroup       = 0,
    kQ3FileReadObjectsInGroup   = 1 << 0,
    kQ3FileCurrentlyInsideGroup = 1 << 1
};
typedef enum TQ3FileReadGroupStateMasks TQ3FileReadGroupStateMasks;

typedef unsigned long                   TQ3FileReadGroupState;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3File_SetReadInGroup           (TQ3FileObject          theFile,
                                 TQ3FileReadGroupState  readGroupState);

EXTERN_API_C( TQ3Status )
Q3File_GetReadInGroup           (TQ3FileObject          theFile,
                                 TQ3FileReadGroupState * readGroupState);


/*
 * Idling
 */
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( TQ3Status , TQ3FileIdleMethod )(TQ3FileObject theFile, const void *idlerData);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3File_SetIdleMethod            (TQ3FileObject          theFile,
                                 TQ3FileIdleMethod      idle,
                                 const void *           idleData);


/******************************************************************************
 **                                                                          **
 **                             Primitives Routines                          **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3NewLine_Write                 (TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Uns8_Read                     (TQ3Uns8 *              data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Uns8_Write                    (TQ3Uns8                data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Uns16_Read                    (TQ3Uns16 *             data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Uns16_Write                   (TQ3Uns16               data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Uns32_Read                    (TQ3Uns32 *             data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Uns32_Write                   (TQ3Uns32               data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Int8_Read                     (TQ3Int8 *              data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Int8_Write                    (TQ3Int8                data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Int16_Read                    (TQ3Int16 *             data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Int16_Write                   (TQ3Int16               data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Int32_Read                    (TQ3Int32 *             data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Int32_Write                   (TQ3Int32               data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Uns64_Read                    (TQ3Uns64 *             data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Uns64_Write                   (TQ3Uns64               data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Int64_Read                    (TQ3Int64 *             data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Int64_Write                   (TQ3Int64               data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Float32_Read                  (TQ3Float32 *           data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Float32_Write                 (TQ3Float32             data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Float64_Read                  (TQ3Float64 *           data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Float64_Write                 (TQ3Float64             data,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Size )
Q3Size_Pad                      (TQ3Size                size);

/*
 * Pass a pointer to a buffer of kQ3StringMaximumLength bytes
 */
EXTERN_API_C( TQ3Status )
Q3String_Read                   (char *                 data,
                                 unsigned long *        length,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3String_Write                  (const char *           data,
                                 TQ3FileObject          theFile);

/* 
 * This call will read Q3Size_Pad(size) bytes,
 *  but only place size bytes into data.
 */
EXTERN_API_C( TQ3Status )
Q3RawData_Read                  (unsigned char *        data,
                                 unsigned long          size,
                                 TQ3FileObject          theFile);

/* 
 * This call will write Q3Size_Pad(size) bytes,
 *  adding 0's to pad to the nearest 4 byte boundary.
 */
EXTERN_API_C( TQ3Status )
Q3RawData_Write                 (const unsigned char *  data,
                                 unsigned long          size,
                                 TQ3FileObject          theFile);

/******************************************************************************
 **                                                                          **
 **                     Convenient Primitives Routines                       **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3Status )
Q3Point2D_Read                  (TQ3Point2D *           point2D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Point2D_Write                 (const TQ3Point2D *     point2D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Point3D_Read                  (TQ3Point3D *           point3D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Point3D_Write                 (const TQ3Point3D *     point3D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3RationalPoint3D_Read          (TQ3RationalPoint3D *   point3D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3RationalPoint3D_Write         (const TQ3RationalPoint3D * point3D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3RationalPoint4D_Read          (TQ3RationalPoint4D *   point4D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3RationalPoint4D_Write         (const TQ3RationalPoint4D * point4D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Vector2D_Read                 (TQ3Vector2D *          vector2D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Vector2D_Write                (const TQ3Vector2D *    vector2D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Vector3D_Read                 (TQ3Vector3D *          vector3D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Vector3D_Write                (const TQ3Vector3D *    vector3D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Matrix4x4_Read                (TQ3Matrix4x4 *         matrix4x4,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Matrix4x4_Write               (const TQ3Matrix4x4 *   matrix4x4,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Tangent2D_Read                (TQ3Tangent2D *         tangent2D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Tangent2D_Write               (const TQ3Tangent2D *   tangent2D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Tangent3D_Read                (TQ3Tangent3D *         tangent3D,
                                 TQ3FileObject          theFile);

EXTERN_API_C( TQ3Status )
Q3Tangent3D_Write               (const TQ3Tangent3D *   tangent3D,
                                 TQ3FileObject          theFile);

/*  This call affects only text Files - it is a no-op in binary files */
EXTERN_API_C( TQ3Status )
Q3Comment_Write                 (char *                 comment,
                                 TQ3FileObject          theFile);

/******************************************************************************
 **                                                                          **
 **                             Unknown Object                               **
 **                                                                          **
 **     Unknown objects are generated when reading files which contain       **
 **     custom data which has not been registered in the current             **
 **     instantiation of QuickDraw 3D.                                       **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3ObjectType )
Q3Unknown_GetType               (TQ3UnknownObject       unknownObject);

EXTERN_API_C( TQ3Status )
Q3Unknown_GetDirtyState         (TQ3UnknownObject       unknownObject,
                                 TQ3Boolean *           isDirty);

EXTERN_API_C( TQ3Status )
Q3Unknown_SetDirtyState         (TQ3UnknownObject       unknownObject,
                                 TQ3Boolean             isDirty);


/******************************************************************************
 **                                                                          **
 **                         Unknown Text Routines                            **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


struct TQ3UnknownTextData {
    char *                          objectName;                 /* '\0' terminated */
    char *                          contents;                   /* '\0' terminated */
};
typedef struct TQ3UnknownTextData       TQ3UnknownTextData;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3UnknownText_GetData           (TQ3UnknownObject       unknownObject,
                                 TQ3UnknownTextData *   unknownTextData);

EXTERN_API_C( TQ3Status )
Q3UnknownText_EmptyData         (TQ3UnknownTextData *   unknownTextData);


/******************************************************************************
 **                                                                          **
 **                         Unknown Binary Routines                          **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


struct TQ3UnknownBinaryData {
    TQ3ObjectType                   objectType;
    unsigned long                   size;
    TQ3Endian                       byteOrder;
    char *                          contents;
};
typedef struct TQ3UnknownBinaryData     TQ3UnknownBinaryData;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3UnknownBinary_GetData         (TQ3UnknownObject       unknownObject,
                                 TQ3UnknownBinaryData * unknownBinaryData);

EXTERN_API_C( TQ3Status )
Q3UnknownBinary_EmptyData       (TQ3UnknownBinaryData * unknownBinaryData);


#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3UnknownBinary_GetTypeString   (TQ3UnknownObject       unknownObject,
                                 char **                typeString);

EXTERN_API_C( TQ3Status )
Q3UnknownBinary_EmptyTypeString (char **                typeString);

#endif  /* CALL_NOT_IN_CARBON */

/******************************************************************************
 **                                                                          **
 **                         ViewHints routines                               **
 **                                                                          **
 **     ViewHints are an object in a metafile to give you some hints on how  **
 **     to render a scene.  You may create a view with any of the objects    **
 **     retrieved from it, or you can just throw it away.                    **
 **                                                                          **
 **     To write a view hints to a file, create a view hints object from a   **
 **     view and write the view hints.                                       **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3ViewHintsObject )
Q3ViewHints_New                 (TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3ViewHints_SetRenderer         (TQ3ViewHintsObject     viewHints,
                                 TQ3RendererObject      renderer);

EXTERN_API_C( TQ3Status )
Q3ViewHints_GetRenderer         (TQ3ViewHintsObject     viewHints,
                                 TQ3RendererObject *    renderer);

EXTERN_API_C( TQ3Status )
Q3ViewHints_SetCamera           (TQ3ViewHintsObject     viewHints,
                                 TQ3CameraObject        camera);

EXTERN_API_C( TQ3Status )
Q3ViewHints_GetCamera           (TQ3ViewHintsObject     viewHints,
                                 TQ3CameraObject *      camera);

EXTERN_API_C( TQ3Status )
Q3ViewHints_SetLightGroup       (TQ3ViewHintsObject     viewHints,
                                 TQ3GroupObject         lightGroup);

EXTERN_API_C( TQ3Status )
Q3ViewHints_GetLightGroup       (TQ3ViewHintsObject     viewHints,
                                 TQ3GroupObject *       lightGroup);

EXTERN_API_C( TQ3Status )
Q3ViewHints_SetAttributeSet     (TQ3ViewHintsObject     viewHints,
                                 TQ3AttributeSet        attributeSet);

EXTERN_API_C( TQ3Status )
Q3ViewHints_GetAttributeSet     (TQ3ViewHintsObject     viewHints,
                                 TQ3AttributeSet *      attributeSet);

EXTERN_API_C( TQ3Status )
Q3ViewHints_SetDimensionsState  (TQ3ViewHintsObject     viewHints,
                                 TQ3Boolean             isValid);

EXTERN_API_C( TQ3Status )
Q3ViewHints_GetDimensionsState  (TQ3ViewHintsObject     viewHints,
                                 TQ3Boolean *           isValid);

EXTERN_API_C( TQ3Status )
Q3ViewHints_SetDimensions       (TQ3ViewHintsObject     viewHints,
                                 unsigned long          width,
                                 unsigned long          height);

EXTERN_API_C( TQ3Status )
Q3ViewHints_GetDimensions       (TQ3ViewHintsObject     viewHints,
                                 unsigned long *        width,
                                 unsigned long *        height);

EXTERN_API_C( TQ3Status )
Q3ViewHints_SetMaskState        (TQ3ViewHintsObject     viewHints,
                                 TQ3Boolean             isValid);

EXTERN_API_C( TQ3Status )
Q3ViewHints_GetMaskState        (TQ3ViewHintsObject     viewHints,
                                 TQ3Boolean *           isValid);

EXTERN_API_C( TQ3Status )
Q3ViewHints_SetMask             (TQ3ViewHintsObject     viewHints,
                                 const TQ3Bitmap *      mask);

EXTERN_API_C( TQ3Status )
Q3ViewHints_GetMask             (TQ3ViewHintsObject     viewHints,
                                 TQ3Bitmap *            mask);

/* Call Q3Bitmap_Empty when done with the mask  */
EXTERN_API_C( TQ3Status )
Q3ViewHints_SetClearImageMethod (TQ3ViewHintsObject     viewHints,
                                 TQ3DrawContextClearImageMethod  clearMethod);

EXTERN_API_C( TQ3Status )
Q3ViewHints_GetClearImageMethod (TQ3ViewHintsObject     viewHints,
                                 TQ3DrawContextClearImageMethod * clearMethod);

EXTERN_API_C( TQ3Status )
Q3ViewHints_SetClearImageColor  (TQ3ViewHintsObject     viewHints,
                                 const TQ3ColorARGB *   color);

EXTERN_API_C( TQ3Status )
Q3ViewHints_GetClearImageColor  (TQ3ViewHintsObject     viewHints,
                                 TQ3ColorARGB *         color);



#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DIO__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DIO__RESTORE_PACKED_ENUMS)
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

#endif /* __QD3DIO__ */

