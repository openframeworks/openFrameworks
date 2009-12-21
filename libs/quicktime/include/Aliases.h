/*
     File:       Aliases.h
 
     Contains:   Alias Manager Interfaces.
 
     Version:    Technology: Mac OS 8.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1989-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ALIASES__
#define __ALIASES__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __APPLETALK__
#include <AppleTalk.h>
#endif

#ifndef __FILES__
#include <Files.h>
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

enum {
    rAliasType                  = FOUR_CHAR_CODE('alis')        /* Aliases are stored as resources of this type */
};

enum {
                                                                /* define alias resolution action rules mask */
    kARMMountVol                = 0x00000001,                   /* mount the volume automatically */
    kARMNoUI                    = 0x00000002,                   /* no user interface allowed during resolution */
    kARMMultVols                = 0x00000008,                   /* search on multiple volumes */
    kARMSearch                  = 0x00000100,                   /* search quickly */
    kARMSearchMore              = 0x00000200,                   /* search further */
    kARMSearchRelFirst          = 0x00000400                    /* search target on a relative path first */
};

enum {
                                                                /* define alias record information types */
    asiZoneName                 = -3,                           /* get zone name */
    asiServerName               = -2,                           /* get server name */
    asiVolumeName               = -1,                           /* get volume name */
    asiAliasName                = 0,                            /* get aliased file/folder/volume name */
    asiParentName               = 1                             /* get parent folder name */
};

/* ResolveAliasFileWithMountFlags options */
enum {
    kResolveAliasFileNoUI       = 0x00000001                    /* no user interaction during resolution */
};

/* define the alias record that will be the blackbox for the caller */

struct AliasRecord {
    OSType                          userType;                   /* appl stored type like creator type */
    unsigned short                  aliasSize;                  /* alias record size in bytes, for appl usage */
};
typedef struct AliasRecord              AliasRecord;

typedef AliasRecord *                   AliasPtr;
typedef AliasPtr *                      AliasHandle;
/* alias record information type */
typedef short                           AliasInfoType;
/*  create a new alias between fromFile-target and return alias record handle  */
EXTERN_API( OSErr )
NewAlias                        (const FSSpec *         fromFile, /* can be NULL */
                                 const FSSpec *         target,
                                 AliasHandle *          alias)                              TWOWORDINLINE(0x7002, 0xA823);

/* create a minimal new alias for a target and return alias record handle */
EXTERN_API( OSErr )
NewAliasMinimal                 (const FSSpec *         target,
                                 AliasHandle *          alias)                              TWOWORDINLINE(0x7008, 0xA823);

/* create a minimal new alias from a target fullpath (optional zone and server name) and return alias record handle  */
EXTERN_API( OSErr )
NewAliasMinimalFromFullPath     (short                  fullPathLength,
                                 const void *           fullPath,
                                 ConstStr32Param        zoneName,
                                 ConstStr31Param        serverName,
                                 AliasHandle *          alias)                              TWOWORDINLINE(0x7009, 0xA823);

/* given an alias handle and fromFile, resolve the alias, update the alias record and return aliased filename and wasChanged flag. */
EXTERN_API( OSErr )
ResolveAlias                    (const FSSpec *         fromFile, /* can be NULL */
                                 AliasHandle            alias,
                                 FSSpec *               target,
                                 Boolean *              wasChanged)                         TWOWORDINLINE(0x7003, 0xA823);

/* given an alias handle and an index specifying requested alias information type, return the information from alias record as a string. */
EXTERN_API( OSErr )
GetAliasInfo                    (AliasHandle            alias,
                                 AliasInfoType          index,
                                 Str63                  theString)                          TWOWORDINLINE(0x7007, 0xA823);


EXTERN_API( OSErr )
IsAliasFile                     (const FSSpec *         fileFSSpec,
                                 Boolean *              aliasFileFlag,
                                 Boolean *              folderFlag)                         TWOWORDINLINE(0x702A, 0xA823);

EXTERN_API( OSErr )
ResolveAliasWithMountFlags      (const FSSpec *         fromFile,
                                 AliasHandle            alias,
                                 FSSpec *               target,
                                 Boolean *              wasChanged,
                                 unsigned long          mountFlags)                         TWOWORDINLINE(0x702B, 0xA823);

/* 
  Given a file spec, return target file spec if input file spec is an alias.
  It resolves the entire alias chain or one step of the chain.  It returns
  info about whether the target is a folder or file; and whether the input
  file spec was an alias or not. 
*/
EXTERN_API( OSErr )
ResolveAliasFile                (FSSpec *               theSpec,
                                 Boolean                resolveAliasChains,
                                 Boolean *              targetIsFolder,
                                 Boolean *              wasAliased)                         TWOWORDINLINE(0x700C, 0xA823);


EXTERN_API( OSErr )
ResolveAliasFileWithMountFlags  (FSSpec *               theSpec,
                                 Boolean                resolveAliasChains,
                                 Boolean *              targetIsFolder,
                                 Boolean *              wasAliased,
                                 unsigned long          mountFlags)                         TWOWORDINLINE(0x7029, 0xA823);

EXTERN_API( OSErr )
FollowFinderAlias               (ConstFSSpecPtr         fromFile,
                                 AliasHandle            alias,
                                 Boolean                logon,
                                 FSSpec *               target,
                                 Boolean *              wasChanged)                         TWOWORDINLINE(0x700F, 0xA823);

/* 
   Low Level Routines 
*/
/* given a fromFile-target pair and an alias handle, update the lias record pointed to by alias handle to represent target as the new alias. */
EXTERN_API( OSErr )
UpdateAlias                     (const FSSpec *         fromFile, /* can be NULL */
                                 const FSSpec *         target,
                                 AliasHandle            alias,
                                 Boolean *              wasChanged)                         TWOWORDINLINE(0x7006, 0xA823);


typedef CALLBACK_API( Boolean , AliasFilterProcPtr )(CInfoPBPtr cpbPtr, Boolean *quitFlag, Ptr myDataPtr);
typedef STACK_UPP_TYPE(AliasFilterProcPtr)                      AliasFilterUPP;
#if OPAQUE_UPP_TYPES
    EXTERN_API(AliasFilterUPP)
    NewAliasFilterUPP              (AliasFilterProcPtr      userRoutine);

    EXTERN_API(void)
    DisposeAliasFilterUPP          (AliasFilterUPP          userUPP);

    EXTERN_API(Boolean)
    InvokeAliasFilterUPP           (CInfoPBPtr              cpbPtr,
                                    Boolean *               quitFlag,
                                    Ptr                     myDataPtr,
                                    AliasFilterUPP          userUPP);

#else
    enum { uppAliasFilterProcInfo = 0x00000FD0 };                   /* pascal 1_byte Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewAliasFilterUPP(userRoutine)                          (AliasFilterUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppAliasFilterProcInfo, GetCurrentArchitecture())
    #define DisposeAliasFilterUPP(userUPP)                          DisposeRoutineDescriptor(userUPP)
    #define InvokeAliasFilterUPP(cpbPtr, quitFlag, myDataPtr, userUPP)  (Boolean)CALL_THREE_PARAMETER_UPP((userUPP), uppAliasFilterProcInfo, (cpbPtr), (quitFlag), (myDataPtr))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewAliasFilterProc(userRoutine)                         NewAliasFilterUPP(userRoutine)
#define CallAliasFilterProc(userRoutine, cpbPtr, quitFlag, myDataPtr) InvokeAliasFilterUPP(cpbPtr, quitFlag, myDataPtr, userRoutine)

/*  Given an alias handle and fromFile, match the alias and return aliased filename(s) and needsUpdate flag */
EXTERN_API( OSErr )
MatchAlias                      (const FSSpec *         fromFile, /* can be NULL */
                                 unsigned long          rulesMask,
                                 AliasHandle            alias,
                                 short *                aliasCount,
                                 FSSpecArrayPtr         aliasList,
                                 Boolean *              needsUpdate,
                                 AliasFilterUPP         aliasFilter,
                                 void *                 yourDataPtr)                        TWOWORDINLINE(0x7005, 0xA823);



/* The follow two calls are for Carbon only.  They allow tools and background apps to have
    AliasMgr functionality without ever having to deal with UI */
EXTERN_API( OSErr )
ResolveAliasFileWithMountFlagsNoUI (FSSpec *            theSpec,
                                 Boolean                resolveAliasChains,
                                 Boolean *              targetIsFolder,
                                 Boolean *              wasAliased,
                                 unsigned long          mountFlags);

EXTERN_API( OSErr )
MatchAliasNoUI                  (const FSSpec *         fromFile, /* can be NULL */
                                 unsigned long          rulesMask,
                                 AliasHandle            alias,
                                 short *                aliasCount,
                                 FSSpecArrayPtr         aliasList,
                                 Boolean *              needsUpdate,
                                 AliasFilterUPP         aliasFilter,
                                 void *                 yourDataPtr);




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

#endif /* __ALIASES__ */

