/*
     File:       OSAComp.h
 
     Contains:   AppleScript Component Implementor's Interfaces.
 
     Version:    Technology: AppleScript 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1992-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __OSACOMP__
#define __OSACOMP__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __AEDATAMODEL__
#include <AEDataModel.h>
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

/**************************************************************************
    Types and Constants
**************************************************************************/
/**************************************************************************
    Routines for Associating a Storage Type with a Script Data Handle 
**************************************************************************/
EXTERN_API( OSErr )
OSAGetStorageType               (AEDataStorage          scriptData,
                                 DescType *             dscType);

EXTERN_API( OSErr )
OSAAddStorageType               (AEDataStorage          scriptData,
                                 DescType               dscType);

EXTERN_API( OSErr )
OSARemoveStorageType            (AEDataStorage          scriptData);


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

#endif /* __OSACOMP__ */

