/*
     File:       CFBundle.h
 
     Contains:   CoreFoundation bundle
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFBUNDLE__
#define __CFBUNDLE__

#ifndef __CFBASE__
#include <CFBase.h>
#endif

#ifndef __CFARRAY__
#include <CFArray.h>
#endif

#ifndef __CFSTRING__
#include <CFString.h>
#endif

#ifndef __CFURL__
#include <CFURL.h>
#endif

#ifndef __CFDICTIONARY__
#include <CFDictionary.h>
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

typedef struct __CFBundle * CFBundleRef;
typedef struct __CFBundle * CFPlugInRef;
extern const CFStringRef kCFBundleInfoDictionaryVersionKey;
extern const CFStringRef kCFBundleExecutableKey;
extern const CFStringRef kCFBundleIdentifierKey;
extern const CFStringRef kCFBundleVersionKey;
extern const CFStringRef kCFBundleDevelopmentRegionKey;
extern const CFStringRef kCFBundleNameKey;
/* ===================== Finding Bundles ===================== */
EXTERN_API_C( CFBundleRef )
CFBundleGetMainBundle           (void);

EXTERN_API_C( CFBundleRef )
CFBundleGetBundleWithIdentifier (CFStringRef            bundleID);

/* A bundle can name itself by providing a key in the info dictionary. */
/* This facility is meant to allow bundle-writers to get hold of their */
/* bundle from their code without having to know where it was on the disk. */
/* This is meant to be a replacement mechanism for +bundleForClass: users. */
/* ===================== Creating Bundles ===================== */
EXTERN_API_C( UInt32 )
CFBundleGetTypeID               (void);

EXTERN_API_C( CFBundleRef )
CFBundleCreate                  (CFAllocatorRef         allocator,
                                 CFURLRef               bundleURL);

/* Might return an existing instance with the ref-count bumped. */
EXTERN_API_C( CFArrayRef )
CFBundleCreateBundlesFromDirectory (CFAllocatorRef      allocator,
                                 CFURLRef               directoryURL,
                                 CFStringRef            bundleType);

/* Create instances for all bundles in the given directory matching the given */
/* type (or all of them if bundleType is NULL) */
/* ==================== Basic Bundle Info ==================== */
EXTERN_API_C( CFURLRef )
CFBundleCopyBundleURL           (CFBundleRef            bundle);

EXTERN_API_C( CFDictionaryRef )
CFBundleGetInfoDictionary       (CFBundleRef            bundle);

EXTERN_API_C( CFStringRef )
CFBundleGetIdentifier           (CFBundleRef            bundle);

EXTERN_API_C( CFStringRef )
CFBundleGetDevelopmentRegion    (CFBundleRef            bundle);

EXTERN_API_C( CFURLRef )
CFBundleCopySupportFilesDirectoryURL (CFBundleRef       bundle);

EXTERN_API_C( CFURLRef )
CFBundleCopyResourcesDirectoryURL (CFBundleRef          bundle);

/* ------------- Basic Bundle Info without a CFBundle instance ------------- */
/* This API is provided to enable developers to use access a bundle Info */
/* dictionary without having to create an instance of CFBundle. */
EXTERN_API_C( CFDictionaryRef )
CFBundleCopyInfoDictionaryInDirectory (CFURLRef         bundleURL);

/* ==================== Resource Handling API ==================== */
EXTERN_API_C( CFURLRef )
CFBundleCopyResourceURL         (CFBundleRef            bundle,
                                 CFStringRef            resourceName,
                                 CFStringRef            resourceType,
                                 CFStringRef            subDirName);

EXTERN_API_C( CFArrayRef )
CFBundleCopyResourceURLsOfType  (CFBundleRef            bundle,
                                 CFStringRef            resourceType,
                                 CFStringRef            subDirName);

EXTERN_API_C( CFStringRef )
CFBundleCopyLocalizedString     (CFBundleRef            bundle,
                                 CFStringRef            key,
                                 CFStringRef            value,
                                 CFStringRef            tableName);

#define CFCopyLocalizedString(key, comment) CFBundleCopyLocalizedString(CFBundleGetMainBundle(), (key), (key), NULL)
#define CFCopyLocalizedStringFromTable(key, tbl, comment) CFBundleCopyLocalizedString(CFBundleGetMainBundle(), (key), (key), (tbl))
#define CFCopyLocalizedStringFromTableInBundle(key, tbl, bundle, comment) CFBundleCopyLocalizedString((bundle), (key), (key), (tbl))
/* ------------- Resource Handling without a CFBundle instance ------------- */
/* This API is provided to enable developers to use the CFBundle resource */
/* searching policy without having to create an instance of CFBundle. */
/* Because of caching behavior when a CFBundle instance exists, it will be faster */
/* to actually create a CFBundle if you need to access several resources. */
EXTERN_API_C( CFURLRef )
CFBundleCopyResourceURLInDirectory (CFURLRef            bundleURL,
                                 CFStringRef            resourceName,
                                 CFStringRef            resourceType,
                                 CFStringRef            subDirName);

EXTERN_API_C( CFArrayRef )
CFBundleCopyResourceURLsOfTypeInDirectory (CFURLRef     bundleURL,
                                 CFStringRef            resourceType,
                                 CFStringRef            subDirName);

/* ==================== Primitive Code Loading API ==================== */
/* This API abstracts the variosu different executable formats supported on various platforms. */
/* It can load DYLD, CFM, or DLL shared libraries (on their appropriate platforms) and gives a */
/* uniform API for looking up functions. */
EXTERN_API_C( CFURLRef )
CFBundleCopyExecutableURL       (CFBundleRef            bundle);

EXTERN_API_C( Boolean )
CFBundleIsExecutableLoaded      (CFBundleRef            bundle);

EXTERN_API_C( Boolean )
CFBundleLoadExecutable          (CFBundleRef            bundle);

EXTERN_API_C( void )
CFBundleUnloadExecutable        (CFBundleRef            bundle);

EXTERN_API_C( void *)
CFBundleGetFunctionPointerForName (CFBundleRef          bundle,
                                 CFStringRef            functionName);

EXTERN_API_C( void )
CFBundleGetFunctionPointersForNames (CFBundleRef        bundle,
                                 CFArrayRef             functionNames,
                                 void *                 ftbl[]);

/* ==================== Getting a bundles plugIn ==================== */
EXTERN_API_C( CFPlugInRef )
CFBundleGetPlugIn               (CFBundleRef            bundle);


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

#endif /* __CFBUNDLE__ */

