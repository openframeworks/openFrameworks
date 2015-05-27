/*
     File:       QD3DExtension.h
 
     Contains:   QuickDraw 3D Plug-in Architecture	 Interface File.
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DEXTENSION__
#define __QD3DEXTENSION__

#ifndef __QD3D__
#include <QD3D.h>
#endif

#ifndef __QD3DERRORS__
#include <QD3DErrors.h>
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
        #define __QD3DEXTENSION__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DEXTENSION__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                             Constants                                    **
 **                                                                          **
 *****************************************************************************/
#if TARGET_OS_MAC
enum {
    kQ3XExtensionMacCreatorType = FOUR_CHAR_CODE('Q3XT'),
    kQ3XExtensionMacFileType    = FOUR_CHAR_CODE('shlb')
};

#endif  /* TARGET_OS_MAC */


/******************************************************************************
 **                                                                             **
 **                                 Macros                                   **
 **                                                                          **
 *****************************************************************************/

/*
 * Use this Macro to pack the version number for your class.  This would most
 * likely get used in the  kQ3XMethodTypeObjectClassVersion to return the 
 * version for your class.  This method is set up in your meta handler.
 */

#define Q3_OBJECT_CLASS_VERSION(major, minor)         \
  (unsigned long) (((major) << 16) | (minor))

/*
 * Convenience macros to unpack a version number, accessing the major and the
 * minor version numbers
 */
#define Q3_OBJECT_CLASS_GET_MAJOR_VERSION(version)       \
  (unsigned long) ((version) >> 16)

#define Q3_OBJECT_CLASS_GET_MINOR_VERSION(version)       \
  (unsigned long) ((version) & 0x0000ffff)


/******************************************************************************
 **                                                                          **
 **                             Object Method types                          **
 **                                                                          **
 *****************************************************************************/
enum {
    kQ3XMethodTypeObjectClassVersion = FOUR_CHAR_CODE('vrsn')
};

typedef unsigned long                   TQ3XObjectClassVersion;
enum {
    kQ3XMethodTypeObjectClassRegister = FOUR_CHAR_CODE('rgst')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XObjectClassRegisterMethod )(TQ3XObjectClass objectClass, void *classPrivate);
enum {
    kQ3XMethodTypeObjectClassReplace = FOUR_CHAR_CODE('rgrp')
};

typedef CALLBACK_API_C( void , TQ3XObjectClassReplaceMethod )(TQ3XObjectClass oldObjectClass, void *oldClassPrivate, TQ3XObjectClass newObjectClass, void *newClassPrivate);
enum {
    kQ3XMethodTypeObjectClassUnregister = FOUR_CHAR_CODE('unrg')
};

typedef CALLBACK_API_C( void , TQ3XObjectClassUnregisterMethod )(TQ3XObjectClass objectClass, void *classPrivate);
enum {
    kQ3XMethodTypeObjectNew     = FOUR_CHAR_CODE('newo')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XObjectNewMethod )(TQ3Object object, void *privateData, void *parameters);
enum {
    kQ3XMethodTypeObjectDelete  = FOUR_CHAR_CODE('dlte')
};

typedef CALLBACK_API_C( void , TQ3XObjectDeleteMethod )(TQ3Object object, void *privateData);
enum {
    kQ3XMethodTypeObjectDuplicate = FOUR_CHAR_CODE('dupl')
};

typedef CALLBACK_API_C( TQ3Status , TQ3XObjectDuplicateMethod )(TQ3Object fromObject, const void *fromPrivateData, TQ3Object toObject, const void *toPrivateData);
typedef CALLBACK_API_C( TQ3Status , TQ3XSharedLibraryRegister )(void );
/******************************************************************************
 **                                                                          **
 **                         Object Hierarchy Registration                    **
 **                                                                          **
 *****************************************************************************/
/*
 *  Q3XObjectHierarchy_RegisterClass
 *  
 *  Register an object class in the QuickDraw 3D hierarchy.
 *  
 *  parentType          - an existing type in the hierarchy, or 0 to subclass
 *                          TQ3Object
 *  objectType          - the new object class type, used in the binary 
 *                        metafile.  This is assigned at run time and returned
 *                        to you.
 *  objectName          - the new object name, used in the text metafile
 *  metaHandler         - a TQ3XMetaHandler (may be NULL for some classes) 
 *                        which returns non-virtual methods
 *  virtualMetaHandler  - a TQ3XMetaHandler (may be NULL as well) which returns
 *                          virtual methods a child would inherit
 *  methodsSize         - the size of the class data needed (see 
 *                          GetClassPrivate calls below)
 *  instanceSize        - the size of the object instance data needed (see 
 *                          GetPrivate calls below)
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3XObjectClass )
Q3XObjectHierarchy_RegisterClass (TQ3ObjectType         parentType,
                                 TQ3ObjectType *        objectType,
                                 char *                 objectName,
                                 TQ3XMetaHandler        metaHandler,
                                 TQ3XMetaHandler        virtualMetaHandler,
                                 unsigned long          methodsSize,
                                 unsigned long          instanceSize);

/*
 *  Q3XObjectHierarchy_UnregisterClass
 *  
 *  Returns kQ3Failure if the objectClass still has objects 
 * around; the class remains registered.
 */
EXTERN_API_C( TQ3Status )
Q3XObjectHierarchy_UnregisterClass (TQ3XObjectClass     objectClass);

/*
 *  Q3XObjectHierarchy_GetMethod
 *  
 *  For use in TQ3XObjectClassRegisterMethod call
 */
EXTERN_API_C( TQ3XFunctionPointer )
Q3XObjectClass_GetMethod        (TQ3XObjectClass        objectClass,
                                 TQ3XMethodType         methodType);

/*
 *  Q3XObjectHierarchy_NewObject
 *  
 *  To create a new object. Parameters is passed into the 
 *  TQ3XObjectNewMethod as the "parameters" parameter.
 */
EXTERN_API_C( TQ3Object )
Q3XObjectHierarchy_NewObject    (TQ3XObjectClass        objectClass,
                                 void *                 parameters);

/*
 *  Q3XObjectClass_GetLeafType
 *  
 *  Return the leaf type of a class.
 */
EXTERN_API_C( TQ3ObjectType )
Q3XObjectClass_GetLeafType      (TQ3XObjectClass        objectClass);

/*
 *  Q3XObjectClass_GetVersion
 *  This routine obtains the the version of a class, referenced by an
 *  object class type.  Functions for getting the type are in QD3D.h,
 *  if you have the class name.
 */
EXTERN_API_C( TQ3Status )
Q3XObjectHierarchy_GetClassVersion (TQ3ObjectType       objectClassType,
                                 TQ3XObjectClassVersion * version);

/*
 *  Q3XObjectClass_GetType 
 *
 *  This can be used to get the type, given a reference 
 *  to a class.  This is most useful in the instance where you register a 
 *  an element/attribute and need to get the type.  When you register an
 *  element, QD3D will take the type you pass in and modify it (to avoid
 *  namespace clashes).  Many object system calls require an object type
 *  so this API call allows you to get the type from the class referernce
 *  that you will ordinarily store when you register the class.
 */
EXTERN_API_C( TQ3Status )
Q3XObjectClass_GetType          (TQ3XObjectClass        objectClass,
                                 TQ3ObjectType *        theType);


EXTERN_API_C( TQ3XObjectClass )
Q3XObjectHierarchy_FindClassByType (TQ3ObjectType       theType);



/*
 *  Q3XObjectClass_GetPrivate
 *  
 *  Return a pointer to private instance data, a block of instanceSize bytes, 
 *  from the Q3XObjectHierarchy_RegisterClass call.
 *  
 *  If instanceSize was zero, NULL is always returned.
 */
EXTERN_API_C( void *)
Q3XObjectClass_GetPrivate       (TQ3XObjectClass        objectClass,
                                 TQ3Object              targetObject);

/*
 * Return the "TQ3XObjectClass" of an object
 */
EXTERN_API_C( TQ3XObjectClass )
Q3XObject_GetClass              (TQ3Object              object);



/******************************************************************************
 **                                                                          **
 **                 Shared Library Registration Entry Point                  **
 **                                                                          **
 *****************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


struct TQ3XSharedLibraryInfo {
    TQ3XSharedLibraryRegister       registerFunction;
    unsigned long                   sharedLibrary;
};
typedef struct TQ3XSharedLibraryInfo    TQ3XSharedLibraryInfo;
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3Status )
Q3XSharedLibrary_Register       (TQ3XSharedLibraryInfo * sharedLibraryInfo);

EXTERN_API_C( TQ3Status )
Q3XSharedLibrary_Unregister     (unsigned long          sharedLibrary);


/******************************************************************************
 **                                                                          **
 **                             Posting Errors                               **
 **                                                                          **
 **         You may only call these functions from within an extension       **
 **                                                                          **
 *****************************************************************************/
/*
 *  Q3XError_Post
 *  
 *  Post a QuickDraw 3D Error from an extension.
 */
EXTERN_API_C( void )
Q3XError_Post                   (TQ3Error               error);

/*
 *  Q3XWarning_Post
 *  
 *  Post a QuickDraw 3D Warning, from an extension.  Note the warning code you
 *  pass into this routine must already be defined in the table above.
 */
EXTERN_API_C( void )
Q3XWarning_Post                 (TQ3Warning             warning);

/*
 *  Q3XNotice_Post
 *  
 *  Post a QuickDraw 3D Notice, from an extension.  Note the notice code you
 *  pass into this routine must already be defined in the table above.
 */
EXTERN_API_C( void )
Q3XNotice_Post                  (TQ3Notice              notice);


#endif  /* CALL_NOT_IN_CARBON */

#if TARGET_OS_MAC
/*
 *  Q3XMacintoshError_Post
 *  
 *  Post the QuickDraw 3D Error, kQ3ErrorMacintoshError, and the Macintosh
 *  OSErr macOSErr. (Retrieved with Q3MacintoshError_Get)
 */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( void )
Q3XMacintoshError_Post          (OSErr                  macOSErr);

#endif  /* CALL_NOT_IN_CARBON */

#endif  /* TARGET_OS_MAC */



#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DEXTENSION__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DEXTENSION__RESTORE_PACKED_ENUMS)
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

#endif /* __QD3DEXTENSION__ */

