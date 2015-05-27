/*
     File:       QD3DSet.h
 
     Contains:   Q3Set types and routines
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DSET__
#define __QD3DSET__

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
        #define __QD3DSET__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DSET__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                             Set Routines                                 **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3SetObject )
Q3Set_New                       (void);

EXTERN_API_C( TQ3ObjectType )
Q3Set_GetType                   (TQ3SetObject           theSet);

EXTERN_API_C( TQ3Status )
Q3Set_Add                       (TQ3SetObject           theSet,
                                 TQ3ElementType         theType,
                                 const void *           data);

EXTERN_API_C( TQ3Status )
Q3Set_Get                       (TQ3SetObject           theSet,
                                 TQ3ElementType         theType,
                                 void *                 data);

EXTERN_API_C( TQ3Boolean )
Q3Set_Contains                  (TQ3SetObject           theSet,
                                 TQ3ElementType         theType);

EXTERN_API_C( TQ3Status )
Q3Set_Clear                     (TQ3SetObject           theSet,
                                 TQ3ElementType         theType);

EXTERN_API_C( TQ3Status )
Q3Set_Empty                     (TQ3SetObject           target);

/*
 *  Iterating through all elements in a set
 *
 *  Pass in kQ3ElementTypeNone to get first type
 *  kQ3ElementTypeNone is returned when end of list is reached
 */
EXTERN_API_C( TQ3Status )
Q3Set_GetNextElementType        (TQ3SetObject           theSet,
                                 TQ3ElementType *       theType);


/******************************************************************************
 **                                                                          **
 **                             Attribute Types                              **
 **                                                                          **
 *****************************************************************************/
/* 
 *  For the data types listed below, pass in a pointer to it in the _Add 
 *  and _Get calls.
 *
 *  For surface shader attributes, reference counts are incremented on 
 *  the _Add and _Get 
 */
#endif  /* CALL_NOT_IN_CARBON */


enum TQ3AttributeTypes {
                                                                /* Data Type          */
    kQ3AttributeTypeNone        = 0,                            /* ---------          */
    kQ3AttributeTypeSurfaceUV   = 1,                            /* TQ3Param2D          */
    kQ3AttributeTypeShadingUV   = 2,                            /* TQ3Param2D           */
    kQ3AttributeTypeNormal      = 3,                            /* TQ3Vector3D           */
    kQ3AttributeTypeAmbientCoefficient = 4,                     /* float            */
    kQ3AttributeTypeDiffuseColor = 5,                           /* TQ3ColorRGB          */
    kQ3AttributeTypeSpecularColor = 6,                          /* TQ3ColorRGB          */
    kQ3AttributeTypeSpecularControl = 7,                        /* float            */
    kQ3AttributeTypeTransparencyColor = 8,                      /* TQ3ColorRGB          */
    kQ3AttributeTypeSurfaceTangent = 9,                         /* TQ3Tangent2D          */
    kQ3AttributeTypeHighlightState = 10,                        /* TQ3Switch           */
    kQ3AttributeTypeSurfaceShader = 11,                         /* TQ3SurfaceShaderObject */
    kQ3AttributeTypeNumTypes    = 12
};
typedef enum TQ3AttributeTypes TQ3AttributeTypes;

typedef TQ3ElementType                  TQ3AttributeType;
/******************************************************************************
 **                                                                          **
 **                             Attribute Drawing                            **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3Attribute_Submit              (TQ3AttributeType       attributeType,
                                 const void *           data,
                                 TQ3ViewObject          view);


/******************************************************************************
 **                                                                          **
 **                         AttributeSet Routines                            **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3AttributeSet )
Q3AttributeSet_New              (void);

EXTERN_API_C( TQ3Status )
Q3AttributeSet_Add              (TQ3AttributeSet        attributeSet,
                                 TQ3AttributeType       theType,
                                 const void *           data);

EXTERN_API_C( TQ3Boolean )
Q3AttributeSet_Contains         (TQ3AttributeSet        attributeSet,
                                 TQ3AttributeType       attributeType);

EXTERN_API_C( TQ3Status )
Q3AttributeSet_Get              (TQ3AttributeSet        attributeSet,
                                 TQ3AttributeType       theType,
                                 void *                 data);

EXTERN_API_C( TQ3Status )
Q3AttributeSet_Clear            (TQ3AttributeSet        attributeSet,
                                 TQ3AttributeType       theType);

EXTERN_API_C( TQ3Status )
Q3AttributeSet_Empty            (TQ3AttributeSet        target);

/*
 * Q3AttributeSet_GetNextAttributeType
 *
 * Pass in kQ3AttributeTypeNone to get first type
 * kQ3AttributeTypeNone is returned when end of list is reached
 */
EXTERN_API_C( TQ3Status )
Q3AttributeSet_GetNextAttributeType (TQ3AttributeSet    source,
                                 TQ3AttributeType *     theType);

EXTERN_API_C( TQ3Status )
Q3AttributeSet_Submit           (TQ3AttributeSet        attributeSet,
                                 TQ3ViewObject          view);

/*
 * Inherit from parent->child into result
 *  Result attributes are:
 *      all child attributes + all parent attributes NOT in the child
 */
EXTERN_API_C( TQ3Status )
Q3AttributeSet_Inherit          (TQ3AttributeSet        parent,
                                 TQ3AttributeSet        child,
                                 TQ3AttributeSet        result);


/******************************************************************************
 **                                                                          **
 **                         Custom Element Registration                      **
 **                                                                          **
 *****************************************************************************/
/*
 * Element Methods - 
 *
 *      When you create a custom element, you control what structures are 
 *      passed around the API. For example, you may allow the Q3Set_Add call 
 *      take one type of argument, store your element internally in some 
 *      abstract data type, and have the Q3Set_Get call take a different 
 *      argument.
 *
 *      For example:
 *          
 *      There are four calls which at some point will copy an element:
 *
 *      Q3Set_Add (copied from Application memory to QuickDraw3D memory)
 *      Q3Set_Get (copied from QuickDraw3D memory to Application memory)
 *      Q3Object_Duplicate (all elements are copied internally)
 *      Q3AttributeSet_Inherit (all elements are copied internally)
 *
 *      Either CopyAdd or CopyReplace is called during the "_Add" call.
 *          - CopyAdd is destructive and should assume "toElement" is garbage
 *          - CopyReplace is replacing an existing element.
 *
 *      CopyGet is called during the "_Get" call.
 *
 *      CopyDuplicate is called to duplicate an element's internal structure.
 *
 * Attributes Methods - 
 *
 *      For copying data while Inheriting. Element methods are used
 *      at all other times.
 *  
 *      CopyInherit is called to duplicate an element's internal structure 
 *          during inheritance. You should make this as fast as possible.
 *          (for example, if your custom element contains objects, you
 *           should do a Q3Shared_GetReference instead of a Q3Object_Duplicate)
 *          
 *      The ElementDelete method will be called for all of your elements 
 *      copied around via CopyAdd, CopyReplace, CopyDuplicate, and 
 *      CopyInherit.
 *      If CopyGet allocates any memory in it's destination, it is up to the 
 *      application to delete it on its side.
 */
#endif  /* CALL_NOT_IN_CARBON */

#define kQ3XMethodTypeElementCopyAdd            Q3_METHOD_TYPE('e','c','p','a')
#define kQ3XMethodTypeElementCopyReplace        Q3_METHOD_TYPE('e','c','p','r')
#define kQ3XMethodTypeElementCopyGet            Q3_METHOD_TYPE('e','c','p','g')
#define kQ3XMethodTypeElementCopyDuplicate      Q3_METHOD_TYPE('e','c','p','d')
#define kQ3XMethodTypeElementDelete             Q3_METHOD_TYPE('e','d','e','l')
typedef CALLBACK_API_C( TQ3Status , TQ3XElementCopyAddMethod )(const void *fromAPIElement, void *toInternalElement);
typedef CALLBACK_API_C( TQ3Status , TQ3XElementCopyReplaceMethod )(const void *fromAPIElement, void *ontoInternalElement);
typedef CALLBACK_API_C( TQ3Status , TQ3XElementCopyGetMethod )(const void *fromInternalElement, void *toAPIElement);
typedef CALLBACK_API_C( TQ3Status , TQ3XElementCopyDuplicateMethod )(const void *fromInternalElement, void *toInternalElement);
typedef CALLBACK_API_C( TQ3Status , TQ3XElementDeleteMethod )(void *internalElement);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3XObjectClass )
Q3XElementClass_Register        (TQ3ElementType *       elementType,
                                 const char *           name,
                                 unsigned long          sizeOfElement,
                                 TQ3XMetaHandler        metaHandler);

EXTERN_API_C( TQ3Status )
Q3XElementType_GetElementSize   (TQ3ElementType         elementType,
                                 unsigned long *        sizeOfElement);


/******************************************************************************
 **                                                                          **
 **                     Custom Attribute Registration                        **
 **                                                                          **
 *****************************************************************************/
#define kQ3XMethodTypeAttributeInherit      Q3_METHOD_TYPE('i','n','h','t')
#endif  /* CALL_NOT_IN_CARBON */

typedef TQ3Boolean                      TQ3XAttributeInheritMethod;
/* return kQ3True or kQ3False in your metahandler */
#define kQ3XMethodTypeAttributeCopyInherit  Q3_METHOD_TYPE('a','c','p','i')
typedef CALLBACK_API_C( TQ3Status , TQ3XAttributeCopyInheritMethod )(const void *fromInternalAttribute, void *toInternalAttribute);
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3XObjectClass )
Q3XAttributeClass_Register      (TQ3AttributeType *     attributeType,
                                 const char *           creatorName,
                                 unsigned long          sizeOfElement,
                                 TQ3XMetaHandler        metaHandler);

/*
 *  Version 1.5
 */
#define kQ3XMethodTypeAttributeDefault      Q3_METHOD_TYPE('a','s','d','f')
#endif  /* CALL_NOT_IN_CARBON */

typedef CALLBACK_API_C( TQ3Status , TQ3XAttributeDefaultMethod )(void *internalElement);
#define kQ3XMethodTypeAttributeIsDefault        Q3_METHOD_TYPE('a','i','d','f')
typedef CALLBACK_API_C( TQ3Boolean , TQ3XAttributeIsDefaultMethod )(void *internalElement);







#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DSET__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DSET__RESTORE_PACKED_ENUMS)
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

#endif /* __QD3DSET__ */

