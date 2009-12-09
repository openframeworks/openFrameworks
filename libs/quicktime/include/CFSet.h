/*
     File:       CFSet.h
 
     Contains:   CoreFoundation set collection
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFSET__
#define __CFSET__

#ifndef __CFBASE__
#include <CFBase.h>
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
    #pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

typedef CALLBACK_API_C( const void *, CFSetRetainCallBack )(CFAllocatorRef allocator, const void *ptr);
typedef CALLBACK_API_C( void , CFSetReleaseCallBack )(CFAllocatorRef allocator, const void *ptr);
typedef CALLBACK_API_C( CFStringRef , CFSetCopyDescriptionCallBack )(const void *ptr);
typedef CALLBACK_API_C( Boolean , CFSetEqualCallBack )(const void *ptr1, const void *ptr2);
typedef CALLBACK_API_C( CFHashCode , CFSetHashCallBack )(const void *ptr);

struct CFSetCallBacks {
    CFIndex                         version;
    CFSetRetainCallBack             retain;
    CFSetReleaseCallBack            release;
    CFSetCopyDescriptionCallBack    copyDescription;
    CFSetEqualCallBack              equal;
    CFSetHashCallBack               hash;
};
typedef struct CFSetCallBacks           CFSetCallBacks;
extern const CFSetCallBacks kCFTypeSetCallBacks;
extern const CFSetCallBacks kCFCopyStringSetCallBacks;
typedef CALLBACK_API_C( void , CFSetApplierFunction )(const void *val, void *context);
typedef const struct __CFSet * CFSetRef;
typedef struct __CFSet * CFMutableSetRef;

EXTERN_API_C( CFTypeID )
CFSetGetTypeID                  (void);


EXTERN_API_C( CFSetRef )
CFSetCreate                     (CFAllocatorRef         allocator,
                                 const void **          values,
                                 CFIndex                numValues,
                                 const CFSetCallBacks * callBacks);

EXTERN_API_C( CFSetRef )
CFSetCreateCopy                 (CFAllocatorRef         allocator,
                                 CFSetRef               theSet);

EXTERN_API_C( CFMutableSetRef )
CFSetCreateMutable              (CFAllocatorRef         allocator,
                                 CFIndex                capacity,
                                 const CFSetCallBacks * callBacks);

EXTERN_API_C( CFMutableSetRef )
CFSetCreateMutableCopy          (CFAllocatorRef         allocator,
                                 CFIndex                capacity,
                                 CFSetRef               theSet);


EXTERN_API_C( CFIndex )
CFSetGetCount                   (CFSetRef               theSet);

EXTERN_API_C( CFIndex )
CFSetGetCountOfValue            (CFSetRef               theSet,
                                 const void *           value);

EXTERN_API_C( Boolean )
CFSetContainsValue              (CFSetRef               theSet,
                                 const void *           value);

EXTERN_API_C( const void *)
CFSetGetValue                   (CFSetRef               theSet,
                                 const void *           candidate);

EXTERN_API_C( Boolean )
CFSetGetValueIfPresent          (CFSetRef               theSet,
                                 const void *           candidate,
                                 const void **          value);

EXTERN_API_C( void )
CFSetGetValues                  (CFSetRef               theSet,
                                 const void **          values);

EXTERN_API_C( void )
CFSetApplyFunction              (CFSetRef               theSet,
                                 CFSetApplierFunction   applier,
                                 void *                 context);


EXTERN_API_C( void )
CFSetAddValue                   (CFMutableSetRef        theSet,
                                 const void *           value);

EXTERN_API_C( void )
CFSetReplaceValue               (CFMutableSetRef        theSet,
                                 const void *           value);

EXTERN_API_C( void )
CFSetSetValue                   (CFMutableSetRef        theSet,
                                 const void *           value);

EXTERN_API_C( void )
CFSetRemoveValue                (CFMutableSetRef        theSet,
                                 const void *           value);

EXTERN_API_C( void )
CFSetRemoveAllValues            (CFMutableSetRef        theSet);


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

#endif /* __CFSET__ */

