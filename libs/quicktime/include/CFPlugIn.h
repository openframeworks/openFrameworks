/*
     File:       CFPlugIn.h
 
     Contains:   CoreFoundation plugins
 
     Version:    Technology: Mac OS X
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1999-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CFPLUGIN__
#define __CFPLUGIN__

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

#ifndef __CFBUNDLE__
#include <CFBundle.h>
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

typedef struct __CFPlugInInstanceRef * CFPlugInInstanceRef;
extern const CFStringRef kCFPlugInDynamicRegistrationKey;
extern const CFStringRef kCFPlugInDynamicRegisterFunctionKey;
extern const CFStringRef kCFPlugInUnloadFunctionKey;
extern const CFStringRef kCFPlugInFactoriesKey;
extern const CFStringRef kCFPlugInTypesKey;
/* ================= Function prototypes for various callbacks ================= */
typedef CALLBACK_API_C( void , CFPlugInDynamicRegisterFunction )(CFPlugInRef plugIn);
typedef CALLBACK_API_C( void , CFPlugInUnloadFunction )(CFPlugInRef plugIn);
typedef CALLBACK_API_C( CFPlugInInstanceRef , CFPlugInFactoryFunction )(CFAllocatorRef allocator, CFStringRef typeName);
typedef CALLBACK_API_C( Boolean , CFPlugInInstanceGetInterfaceFunction )(CFPlugInInstanceRef instance, CFStringRef interfaceName, void **ftbl);
typedef CALLBACK_API_C( void , CFPlugInInstanceDeallocateInstanceDataFunction )(void *instanceData);
/* ================= Creating PlugIns ================= */
EXTERN_API_C( UInt32 )
CFPlugInGetTypeID               (void);

EXTERN_API_C( CFPlugInRef )
CFPlugInCreate                  (CFAllocatorRef         allocator,
                                 CFURLRef               plugInURL);

/* Might return an existing instance with the ref-count bumped. */
EXTERN_API_C( CFBundleRef )
CFPlugInGetBundle               (CFPlugInRef            plugIn);

/* ================= Controlling load on demand ================= */
/* For plugIns. */
/* A dynamic registration function can call CFPlugInSetLoadOnDemand. */
EXTERN_API_C( void )
CFPlugInSetLoadOnDemand         (CFPlugInRef            plugIn,
                                 Boolean                flag);

EXTERN_API_C( Boolean )
CFPlugInIsLoadOnDemand          (CFPlugInRef            plugIn);

/* ================= Finding factories and creating instances ================= */
/* For plugIn hosts. */
/* Functions for finding factories to create specific types and actually creating instances of a type. */
EXTERN_API_C( CFArrayRef )
CFPlugInFindFactoriesForPlugInType (CFStringRef         typeName);

EXTERN_API_C( CFPlugInInstanceRef )
CFPlugInInstanceCreate          (CFAllocatorRef         allocator,
                                 CFStringRef            factoryName,
                                 CFStringRef            typeName);

/* ================= Using instances ================= */
/* For plugIn hosts and/or plugIns. */
/* Functions for dealing with instances. */
EXTERN_API_C( Boolean )
CFPlugInInstanceGetInterfaceFunctionTable (CFPlugInInstanceRef  instance,
                                 CFStringRef            interfaceName,
                                 void **                ftbl);

EXTERN_API_C( CFStringRef )
CFPlugInInstanceGetFactoryName  (CFPlugInInstanceRef    instance);

EXTERN_API_C( void *)
CFPlugInInstanceGetInstanceData (CFPlugInInstanceRef    instance);

/* ================= Registering factories and types ================= */
/* For plugIn writers who must dynamically register things. */
/* Functions to register factory functions and to associate factories with types. */
EXTERN_API_C( Boolean )
CFPlugInRegisterFactoryFunction (CFStringRef            factoryName,
                                 CFPlugInFactoryFunction  func);

EXTERN_API_C( Boolean )
CFPlugInRegisterFactoryFunctionByName (CFStringRef      factoryName,
                                 CFPlugInRef            plugIn,
                                 CFStringRef            functionName);

EXTERN_API_C( Boolean )
CFPlugInUnregisterFactory       (CFStringRef            factoryName);

EXTERN_API_C( Boolean )
CFPlugInRegisterPlugInType      (CFStringRef            factoryName,
                                 CFStringRef            typeName);

EXTERN_API_C( Boolean )
CFPlugInUnregisterPlugInType    (CFStringRef            factoryName,
                                 CFStringRef            typeName);

/* ================= Primitive instance creation ================= */
/* For plugIns. */
/* Factory function implementations should use this to actually create an instance. */
EXTERN_API_C( UInt32 )
CFPlugInInstanceGetTypeID       (void);

EXTERN_API_C( CFPlugInInstanceRef )
CFPlugInInstanceCreateWithInstanceDataSize (CFAllocatorRef  allocator,
                                 CFIndex                instanceDataSize,
                                 CFPlugInInstanceDeallocateInstanceDataFunction  deallocateInstanceFunction,
                                 CFStringRef            factoryName,
                                 CFPlugInInstanceGetInterfaceFunction  getInterfaceFunction);


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

#endif /* __CFPLUGIN__ */

