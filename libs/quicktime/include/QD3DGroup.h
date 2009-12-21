/*
     File:       QD3DGroup.h
 
     Contains:   Q3Group methods
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DGROUP__
#define __QD3DGROUP__

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
        #define __QD3DGROUP__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DGROUP__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                         Group Typedefs                                   **
 **                                                                          **
 *****************************************************************************/
/*
 * These flags affect how a group is traversed
 * They apply to when a group is "drawn", "picked", "bounded", "written"
 */

enum TQ3DisplayGroupStateMasks {
    kQ3DisplayGroupStateNone    = 0,
    kQ3DisplayGroupStateMaskIsDrawn = 1 << 0,
    kQ3DisplayGroupStateMaskIsInline = 1 << 1,
    kQ3DisplayGroupStateMaskUseBoundingBox = 1 << 2,
    kQ3DisplayGroupStateMaskUseBoundingSphere = 1 << 3,
    kQ3DisplayGroupStateMaskIsPicked = 1 << 4,
    kQ3DisplayGroupStateMaskIsWritten = 1 << 5
};
typedef enum TQ3DisplayGroupStateMasks TQ3DisplayGroupStateMasks;

typedef unsigned long                   TQ3DisplayGroupState;
 

/******************************************************************************
 **                                                                          **
 **                 Group Routines (apply to all groups)                     **
 **                                                                          **
 *****************************************************************************/
/* May contain any shared object */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3GroupObject )
Q3Group_New                     (void);

EXTERN_API_C( TQ3ObjectType )
Q3Group_GetType                 (TQ3GroupObject         group);

EXTERN_API_C( TQ3GroupPosition )
Q3Group_AddObject               (TQ3GroupObject         group,
                                 TQ3Object              object);

EXTERN_API_C( TQ3GroupPosition )
Q3Group_AddObjectBefore         (TQ3GroupObject         group,
                                 TQ3GroupPosition       position,
                                 TQ3Object              object);

EXTERN_API_C( TQ3GroupPosition )
Q3Group_AddObjectAfter          (TQ3GroupObject         group,
                                 TQ3GroupPosition       position,
                                 TQ3Object              object);

EXTERN_API_C( TQ3Status )
Q3Group_GetPositionObject       (TQ3GroupObject         group,
                                 TQ3GroupPosition       position,
                                 TQ3Object *            object);

EXTERN_API_C( TQ3Status )
Q3Group_SetPositionObject       (TQ3GroupObject         group,
                                 TQ3GroupPosition       position,
                                 TQ3Object              object);

EXTERN_API_C( TQ3Object )
Q3Group_RemovePosition          (TQ3GroupObject         group,
                                 TQ3GroupPosition       position);

EXTERN_API_C( TQ3Status )
Q3Group_GetFirstPosition        (TQ3GroupObject         group,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_GetLastPosition         (TQ3GroupObject         group,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_GetNextPosition         (TQ3GroupObject         group,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_GetPreviousPosition     (TQ3GroupObject         group,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_CountObjects            (TQ3GroupObject         group,
                                 unsigned long *        nObjects);

EXTERN_API_C( TQ3Status )
Q3Group_EmptyObjects            (TQ3GroupObject         group);

/*
 *  Typed Access
 */
EXTERN_API_C( TQ3Status )
Q3Group_GetFirstPositionOfType  (TQ3GroupObject         group,
                                 TQ3ObjectType          isType,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_GetLastPositionOfType   (TQ3GroupObject         group,
                                 TQ3ObjectType          isType,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_GetNextPositionOfType   (TQ3GroupObject         group,
                                 TQ3ObjectType          isType,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_GetPreviousPositionOfType (TQ3GroupObject       group,
                                 TQ3ObjectType          isType,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_CountObjectsOfType      (TQ3GroupObject         group,
                                 TQ3ObjectType          isType,
                                 unsigned long *        nObjects);

EXTERN_API_C( TQ3Status )
Q3Group_EmptyObjectsOfType      (TQ3GroupObject         group,
                                 TQ3ObjectType          isType);

/*
 *  Determine position of objects in a group
 */
EXTERN_API_C( TQ3Status )
Q3Group_GetFirstObjectPosition  (TQ3GroupObject         group,
                                 TQ3Object              object,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_GetLastObjectPosition   (TQ3GroupObject         group,
                                 TQ3Object              object,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_GetNextObjectPosition   (TQ3GroupObject         group,
                                 TQ3Object              object,
                                 TQ3GroupPosition *     position);

EXTERN_API_C( TQ3Status )
Q3Group_GetPreviousObjectPosition (TQ3GroupObject       group,
                                 TQ3Object              object,
                                 TQ3GroupPosition *     position);


/******************************************************************************
 **                                                                          **
 **                         Group Subclasses                                 **
 **                                                                          **
 *****************************************************************************/
/* Must contain only lights */
EXTERN_API_C( TQ3GroupObject )
Q3LightGroup_New                (void);

/* Must contain only strings */
EXTERN_API_C( TQ3GroupObject )
Q3InfoGroup_New                 (void);

/******************************************************************************
 **                                                                          **
 **                     Display Group Routines                               **
 **                                                                          **
 *****************************************************************************/
/* May contain only drawables */
EXTERN_API_C( TQ3GroupObject )
Q3DisplayGroup_New              (void);

EXTERN_API_C( TQ3ObjectType )
Q3DisplayGroup_GetType          (TQ3GroupObject         group);

EXTERN_API_C( TQ3Status )
Q3DisplayGroup_GetState         (TQ3GroupObject         group,
                                 TQ3DisplayGroupState * state);

EXTERN_API_C( TQ3Status )
Q3DisplayGroup_SetState         (TQ3GroupObject         group,
                                 TQ3DisplayGroupState   state);

EXTERN_API_C( TQ3Status )
Q3DisplayGroup_Submit           (TQ3GroupObject         group,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3DisplayGroup_SetAndUseBoundingBox (TQ3GroupObject     group,
                                 TQ3BoundingBox *       bBox);

EXTERN_API_C( TQ3Status )
Q3DisplayGroup_GetBoundingBox   (TQ3GroupObject         group,
                                 TQ3BoundingBox *       bBox);

EXTERN_API_C( TQ3Status )
Q3DisplayGroup_RemoveBoundingBox (TQ3GroupObject        group);

EXTERN_API_C( TQ3Status )
Q3DisplayGroup_CalcAndUseBoundingBox (TQ3GroupObject    group,
                                 TQ3ComputeBounds       computeBounds,
                                 TQ3ViewObject          view);


/******************************************************************************
 **                                                                          **
 **     Ordered Display Group                                                **
 **                                                                          **
 **     Ordered display groups keep objects in order by the type of object:  **
 **                                                                          **
 **     1   kQ3ShapeTypeTransform                                            **
 **     2   kQ3ShapeTypeStyle                                                **
 **     3   kQ3SetTypeAttribute                                              **
 **     4   kQ3ShapeTypeShader                                               **
 **     5   kQ3ShapeTypeCamera                                               **
 **     6   kQ3ShapeTypeLight                                                **
 **     7   kQ3ShapeTypeGeometry                                             **
 **     8   kQ3ShapeTypeGroup                                                **         
 **     9   kQ3ShapeTypeUnknown                                              **
 **                                                                          **
 **     Within a type, you are responsible for keeping things in order.      **
 **                                                                          **
 **     You may access and/or manipulate the group using the above types     **
 **     (fast), or you may use any parent or leaf class types (slower).      **
 **                                                                          **
 **     Additional types will be added as functionality grows.               **
 **                                                                          **
 **     The group calls which access by type are much faster for ordered     ** 
 **     display group for the types above.                                   **
 **                                                                          **
 **     N.B. Lights and Cameras in groups are a no-op when drawn and will    **
 **          post an error with the debug libraries.                         **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3GroupObject )
Q3OrderedDisplayGroup_New       (void);

/******************************************************************************
 **                                                                          **
 **     IO Proxy Display Group                                               **
 **                                                                          **
 **     IO Proxy display groups are used to place more than one              **
 **     representation of an object in a metafile. For example, if you know  **
 **     another program does not understand NURBPatches but does understand  **
 **     Meshes, you may place a mesh and a NURB Patch in an IO Proxy Group,  **
 **     and the reading program will select the desired representation.      **
 **                                                                          **
 **     Objects in an IO Proxy Display Group are placed in their preferencial**
 **     order, with the FIRST object being the MOST preferred, the LAST      **
 **     object the least preferred.                                          **
 **                                                                          **
 **     The behavior of an IO Proxy Display Group is that when drawn/picked/ **
 **     bounded, the first object in the group that is not "Unknown" is used,**
 **     and the other objects ignored.                                       **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3GroupObject )
Q3IOProxyDisplayGroup_New       (void);

/******************************************************************************
 **                                                                          **
 **                     Group Extension Definitions                          **
 **                                                                          **
 *****************************************************************************/
/*
 *  Searching methods - OPTIONAL
 */
#endif  /* CALL_NOT_IN_CARBON */

enum {
    kQ3XMethodType_GroupAcceptObject = FOUR_CHAR_CODE('gaco')
};

typedef CALLBACK_API_C( TQ3Boolean , TQ3XGroupAcceptObjectMethod )(TQ3GroupObject group, TQ3Object object);
enum {
    kQ3XMethodType_GroupAddObject = FOUR_CHAR_CODE('gado')
};

typedef CALLBACK_API_C( TQ3GroupPosition , TQ3XGroupAddObjectMethod )(TQ3GroupObject group, TQ3Object object);
enum {
    kQ3XMethodType_GroupAddObjectBefore = FOUR_CHAR_CODE('gaob')
};

typedef CALLBACK_API_C( TQ3GroupPosition , TQ3XGroupAddObjectBeforeMethod )(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object);
enum {
    kQ3XMethodType_GroupAddObjectAfter = FOUR_CHAR_CODE('gaoa')
};

typedef CALLBACK_API_C( TQ3GroupPosition , TQ3XGroupAddObjectAfterMethod )(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object);
enum {
    kQ3XMethodType_GroupSetPositionObject = FOUR_CHAR_CODE('gspo')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupSetPositionObjectMethod )(TQ3GroupObject group, TQ3GroupPosition gPos, TQ3Object obj);
enum {
    kQ3XMethodType_GroupRemovePosition = FOUR_CHAR_CODE('grmp')
};

typedef CALLBACK_API_C( TQ3Object , TQ3XGroupRemovePositionMethod )(TQ3GroupObject group, TQ3GroupPosition position);
 

/*
 *  Searching methods - OPTIONAL - default uses above methods
 */
enum {
    kQ3XMethodType_GroupGetFirstPositionOfType = FOUR_CHAR_CODE('gfrt')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupGetFirstPositionOfTypeMethod )(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *gPos);
enum {
    kQ3XMethodType_GroupGetLastPositionOfType = FOUR_CHAR_CODE('glst')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupGetLastPositionOfTypeMethod )(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *gPos);
enum {
    kQ3XMethodType_GroupGetNextPositionOfType = FOUR_CHAR_CODE('gnxt')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupGetNextPositionOfTypeMethod )(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *gPos);
enum {
    kQ3XMethodType_GroupGetPrevPositionOfType = FOUR_CHAR_CODE('gpvt')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupGetPrevPositionOfTypeMethod )(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *gPos);
enum {
    kQ3XMethodType_GroupCountObjectsOfType = FOUR_CHAR_CODE('gcnt')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupCountObjectsOfTypeMethod )(TQ3GroupObject group, TQ3ObjectType isType, unsigned long *nObjects);
enum {
    kQ3XMethodType_GroupEmptyObjectsOfType = FOUR_CHAR_CODE('geot')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupEmptyObjectsOfTypeMethod )(TQ3GroupObject group, TQ3ObjectType isType);
enum {
    kQ3XMethodType_GroupGetFirstObjectPosition = FOUR_CHAR_CODE('gfop')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupGetFirstObjectPositionMethod )(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *gPos);
enum {
    kQ3XMethodType_GroupGetLastObjectPosition = FOUR_CHAR_CODE('glop')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupGetLastObjectPositionMethod )(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *gPos);
enum {
    kQ3XMethodType_GroupGetNextObjectPosition = FOUR_CHAR_CODE('gnop')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupGetNextObjectPositionMethod )(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *gPos);
enum {
    kQ3XMethodType_GroupGetPrevObjectPosition = FOUR_CHAR_CODE('gpop')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupGetPrevObjectPositionMethod )(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *gPos);
 

/*
 *  Group Position Methods
 */
enum {
    kQ3XMethodType_GroupPositionSize = FOUR_CHAR_CODE('ggpz')
};

typedef unsigned long                   TQ3XMethodTypeGroupPositionSize;
enum {
    kQ3XMethodType_GroupPositionNew = FOUR_CHAR_CODE('ggpn')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupPositionNewMethod )(void *gPos, TQ3Object object, const void *initData);
enum {
    kQ3XMethodType_GroupPositionCopy = FOUR_CHAR_CODE('ggpc')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupPositionCopyMethod )(void *srcGPos, void *dstGPos);
enum {
    kQ3XMethodType_GroupPositionDelete = FOUR_CHAR_CODE('ggpd')
};

typedef CALLBACK_API_C( void , TQ3XGroupPositionDeleteMethod )(void *gPos);
 

/*
 *  View Drawing Helpers
 *  
 *  TQ3XGroupStartIterateMethod
 *
 *      Pass back *object = NULL to NOT call EndIterate iterate
 *      Pass back *object != NULL to draw object
 *       (other side will pass it to EndIterate for deletion!)
 *
 *      *iterator is uninitialized, use for iteration state. Caller should 
 *       ignore it.
 *  
 *  TQ3XGroupEndIterateMethod
 *  
 *      *object is previous object, dispose it or play with it.
 *      Pass back NULL when last iteration has occurred
 *      *iterator is previous value, use for iteration state Caller should 
 *      ignore it.
 */
enum {
    kQ3XMethodType_GroupStartIterate = FOUR_CHAR_CODE('gstd')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupStartIterateMethod )(TQ3GroupObject group, TQ3GroupPosition *iterator, TQ3Object *object, TQ3ViewObject view);
enum {
    kQ3XMethodType_GroupEndIterate = FOUR_CHAR_CODE('gitd')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupEndIterateMethod )(TQ3GroupObject group, TQ3GroupPosition *iterator, TQ3Object *object, TQ3ViewObject view);
 

/*
 *  IO  Helpers
 *  
 *  TQ3XGroupEndReadMethod
 *      Called when a group has been completely read. Group should perform
 *      validation and clean up any reading caches.
 */
enum {
    kQ3XMethodType_GroupEndRead = FOUR_CHAR_CODE('gerd')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XGroupEndReadMethod )(TQ3GroupObject group);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void *)
Q3XGroup_GetPositionPrivate     (TQ3GroupObject         group,
                                 TQ3GroupPosition       position);



#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DGROUP__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DGROUP__RESTORE_PACKED_ENUMS)
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

#endif /* __QD3DGROUP__ */

