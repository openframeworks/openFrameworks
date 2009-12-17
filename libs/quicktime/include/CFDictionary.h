/*
     File:       CFDictionary.h
 
     Contains:   CoreFoundation dictionary collection
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFDICTIONARY__
#define __CFDICTIONARY__

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

typedef CALLBACK_API_C( const void *, CFDictionaryRetainCallBack )(CFAllocatorRef allocator, const void *ptr);
typedef CALLBACK_API_C( void , CFDictionaryReleaseCallBack )(CFAllocatorRef allocator, const void *ptr);
typedef CALLBACK_API_C( CFStringRef , CFDictionaryCopyDescriptionCallBack )(const void *ptr);
typedef CALLBACK_API_C( Boolean , CFDictionaryEqualCallBack )(const void *ptr1, const void *ptr2);
typedef CALLBACK_API_C( CFHashCode , CFDictionaryHashCallBack )(const void *ptr);

struct CFDictionaryKeyCallBacks {
    CFIndex                         version;
    CFDictionaryRetainCallBack      retain;
    CFDictionaryReleaseCallBack     release;
    CFDictionaryCopyDescriptionCallBack  copyDescription;
    CFDictionaryEqualCallBack       equal;
    CFDictionaryHashCallBack        hash;
};
typedef struct CFDictionaryKeyCallBacks CFDictionaryKeyCallBacks;
extern const CFDictionaryKeyCallBacks kCFTypeDictionaryKeyCallBacks;
extern const CFDictionaryKeyCallBacks kCFCopyStringDictionaryKeyCallBacks;

struct CFDictionaryValueCallBacks {
    CFIndex                         version;
    CFDictionaryRetainCallBack      retain;
    CFDictionaryReleaseCallBack     release;
    CFDictionaryCopyDescriptionCallBack  copyDescription;
    CFDictionaryEqualCallBack       equal;
};
typedef struct CFDictionaryValueCallBacks CFDictionaryValueCallBacks;
extern const CFDictionaryValueCallBacks kCFTypeDictionaryValueCallBacks;
typedef CALLBACK_API_C( void , CFDictionaryApplierFunction )(const void *key, const void *val, void *context);
typedef const struct __CFDictionary * CFDictionaryRef;
typedef struct __CFDictionary * CFMutableDictionaryRef;
EXTERN_API_C( CFTypeID )
CFDictionaryGetTypeID           (void);


EXTERN_API_C( CFDictionaryRef )
CFDictionaryCreate              (CFAllocatorRef         allocator,
                                 const void **          keys,
                                 const void **          values,
                                 CFIndex                numValues,
                                 const CFDictionaryKeyCallBacks * keyCallBacks,
                                 const CFDictionaryValueCallBacks * valueCallBacks);

EXTERN_API_C( CFDictionaryRef )
CFDictionaryCreateCopy          (CFAllocatorRef         allocator,
                                 CFDictionaryRef        dict);

EXTERN_API_C( CFMutableDictionaryRef )
CFDictionaryCreateMutable       (CFAllocatorRef         allocator,
                                 CFIndex                capacity,
                                 const CFDictionaryKeyCallBacks * keyCallBacks,
                                 const CFDictionaryValueCallBacks * valueCallBacks);

EXTERN_API_C( CFMutableDictionaryRef )
CFDictionaryCreateMutableCopy   (CFAllocatorRef         allocator,
                                 CFIndex                capacity,
                                 CFDictionaryRef        dict);


EXTERN_API_C( CFIndex )
CFDictionaryGetCount            (CFDictionaryRef        dict);

EXTERN_API_C( CFIndex )
CFDictionaryGetCountOfKey       (CFDictionaryRef        dict,
                                 const void *           key);

EXTERN_API_C( CFIndex )
CFDictionaryGetCountOfValue     (CFDictionaryRef        dict,
                                 const void *           value);

EXTERN_API_C( Boolean )
CFDictionaryContainsKey         (CFDictionaryRef        dict,
                                 const void *           key);

EXTERN_API_C( Boolean )
CFDictionaryContainsValue       (CFDictionaryRef        dict,
                                 const void *           value);

EXTERN_API_C( const void *)
CFDictionaryGetValue            (CFDictionaryRef        dict,
                                 const void *           key);

EXTERN_API_C( Boolean )
CFDictionaryGetValueIfPresent   (CFDictionaryRef        dict,
                                 const void *           key,
                                 const void **          value);

EXTERN_API_C( void )
CFDictionaryGetKeysAndValues    (CFDictionaryRef        dict,
                                 const void **          keys,
                                 const void **          values);

EXTERN_API_C( void )
CFDictionaryApplyFunction       (CFDictionaryRef        dict,
                                 CFDictionaryApplierFunction  applier,
                                 void *                 context);


EXTERN_API_C( void )
CFDictionaryAddValue            (CFMutableDictionaryRef  dict,
                                 const void *           key,
                                 const void *           value);

EXTERN_API_C( void )
CFDictionarySetValue            (CFMutableDictionaryRef  dict,
                                 const void *           key,
                                 const void *           value);

EXTERN_API_C( void )
CFDictionaryReplaceValue        (CFMutableDictionaryRef  dict,
                                 const void *           key,
                                 const void *           value);

EXTERN_API_C( void )
CFDictionaryRemoveValue         (CFMutableDictionaryRef  dict,
                                 const void *           key);

EXTERN_API_C( void )
CFDictionaryRemoveAllValues     (CFMutableDictionaryRef  dict);



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

#endif /* __CFDICTIONARY__ */

