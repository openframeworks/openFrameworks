/*
     File:       CFArray.h
 
     Contains:   CoreFoundation array collection
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFARRAY__
#define __CFARRAY__

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

typedef CALLBACK_API_C( const void *, CFArrayRetainCallBack )(CFAllocatorRef allocator, const void *ptr);
typedef CALLBACK_API_C( void , CFArrayReleaseCallBack )(CFAllocatorRef allocator, const void *ptr);
typedef CALLBACK_API_C( CFStringRef , CFArrayCopyDescriptionCallBack )(const void *ptr);
typedef CALLBACK_API_C( Boolean , CFArrayEqualCallBack )(const void *ptr1, const void *ptr2);

struct CFArrayCallBacks {
    CFIndex                         version;
    CFArrayRetainCallBack           retain;
    CFArrayReleaseCallBack          release;
    CFArrayCopyDescriptionCallBack  copyDescription;
    CFArrayEqualCallBack            equal;
};
typedef struct CFArrayCallBacks         CFArrayCallBacks;
extern const CFArrayCallBacks kCFTypeArrayCallBacks;
typedef CALLBACK_API_C( void , CFArrayApplierFunction )(const void *val, void *context);
typedef const struct __CFArray * CFArrayRef;
typedef struct __CFArray * CFMutableArrayRef;
EXTERN_API_C( CFTypeID )
CFArrayGetTypeID                (void);


EXTERN_API_C( CFArrayRef )
CFArrayCreate                   (CFAllocatorRef         allocator,
                                 const void **          values,
                                 CFIndex                numValues,
                                 const CFArrayCallBacks * callBacks);

EXTERN_API_C( CFArrayRef )
CFArrayCreateCopy               (CFAllocatorRef         allocator,
                                 CFArrayRef             srcArray);

EXTERN_API_C( CFMutableArrayRef )
CFArrayCreateMutable            (CFAllocatorRef         allocator,
                                 CFIndex                capacity,
                                 const CFArrayCallBacks * callBacks);

EXTERN_API_C( CFMutableArrayRef )
CFArrayCreateMutableCopy        (CFAllocatorRef         allocator,
                                 CFIndex                capacity,
                                 CFArrayRef             srcArray);


EXTERN_API_C( CFIndex )
CFArrayGetCount                 (CFArrayRef             theArray);

EXTERN_API_C( CFIndex )
CFArrayGetCountOfValue          (CFArrayRef             theArray,
                                 CFRange                range,
                                 const void *           value);

EXTERN_API_C( Boolean )
CFArrayContainsValue            (CFArrayRef             theArray,
                                 CFRange                range,
                                 const void *           value);

EXTERN_API_C( const void *)
CFArrayGetValueAtIndex          (CFArrayRef             theArray,
                                 CFIndex                idx);

EXTERN_API_C( void )
CFArrayGetValues                (CFArrayRef             theArray,
                                 CFRange                range,
                                 const void **          values);

EXTERN_API_C( void )
CFArrayApplyFunction            (CFArrayRef             theArray,
                                 CFRange                range,
                                 CFArrayApplierFunction  applier,
                                 void *                 context);

EXTERN_API_C( CFIndex )
CFArrayGetFirstIndexOfValue     (CFArrayRef             theArray,
                                 CFRange                range,
                                 const void *           value);

EXTERN_API_C( CFIndex )
CFArrayGetLastIndexOfValue      (CFArrayRef             theArray,
                                 CFRange                range,
                                 const void *           value);

EXTERN_API_C( CFIndex )
CFArrayBSearchValues            (CFArrayRef             theArray,
                                 CFRange                range,
                                 const void *           value,
                                 CFComparatorFunction   comparator,
                                 void *                 context);


EXTERN_API_C( void )
CFArrayAppendValue              (CFMutableArrayRef      theArray,
                                 const void *           value);

EXTERN_API_C( void )
CFArrayInsertValueAtIndex       (CFMutableArrayRef      theArray,
                                 CFIndex                idx,
                                 const void *           value);

EXTERN_API_C( void )
CFArraySetValueAtIndex          (CFMutableArrayRef      theArray,
                                 CFIndex                idx,
                                 const void *           value);

EXTERN_API_C( void )
CFArrayRemoveValueAtIndex       (CFMutableArrayRef      theArray,
                                 CFIndex                idx);

EXTERN_API_C( void )
CFArrayRemoveAllValues          (CFMutableArrayRef      theArray);

EXTERN_API_C( void )
CFArrayReplaceValues            (CFMutableArrayRef      theArray,
                                 CFRange                range,
                                 const void **          newValues,
                                 CFIndex                newCount);

EXTERN_API_C( void )
CFArrayExchangeValuesAtIndices  (CFMutableArrayRef      theArray,
                                 CFIndex                idx1,
                                 CFIndex                idx2);

EXTERN_API_C( void )
CFArraySortValues               (CFMutableArrayRef      theArray,
                                 CFRange                range,
                                 CFComparatorFunction   comparator,
                                 void *                 context);


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

#endif /* __CFARRAY__ */

