/*
     File:       CodeFragments.h
 
     Contains:   Public Code Fragment Manager Interfaces.
 
     Version:    Technology: Mac OS 8.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1992-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
/*
   o
   ===========================================================================================
   The Code Fragment Manager API
   =============================
*/


#ifndef __CODEFRAGMENTS__
#define __CODEFRAGMENTS__

#ifndef __MACTYPES__
#include <MacTypes.h>
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

/*
   .
   ===========================================================================================
   General Types and Constants
   ===========================
*/


enum {
    kCFragResourceType          = FOUR_CHAR_CODE('cfrg'),
    kCFragResourceID            = 0,
    kCFragLibraryFileType       = FOUR_CHAR_CODE('shlb'),
    kCFragAllFileTypes          = (long)0xFFFFFFFF
};


typedef OSType                          CFragArchitecture;
enum {
    kPowerPCCFragArch           = FOUR_CHAR_CODE('pwpc'),
    kMotorola68KCFragArch       = FOUR_CHAR_CODE('m68k'),
    kAnyCFragArch               = 0x3F3F3F3F
};

#if TARGET_CPU_PPC
enum {
    kCompiledCFragArch          = kPowerPCCFragArch
};

#endif  /* TARGET_CPU_PPC */

#if TARGET_CPU_68K
enum {
    kCompiledCFragArch          = kMotorola68KCFragArch
};

#endif  /* TARGET_CPU_68K */

typedef UInt32                          CFragVersionNumber;
enum {
    kNullCFragVersion           = 0,
    kWildcardCFragVersion       = (long)0xFFFFFFFF
};


typedef UInt8                           CFragUsage;
enum {
    kImportLibraryCFrag         = 0,                            /* Standard CFM import library.*/
    kApplicationCFrag           = 1,                            /* MacOS application.*/
    kDropInAdditionCFrag        = 2,                            /* Application or library private extension/plug-in*/
    kStubLibraryCFrag           = 3,                            /* Import library used for linking only*/
    kWeakStubLibraryCFrag       = 4                             /* Import library used for linking only and will be automatically weak linked*/
};


enum {
    kIsCompleteCFrag            = 0,                            /* A "base" fragment, not an update.*/
    kFirstCFragUpdate           = 1                             /* The first update, others are numbered 2, 3, ...*/
};


enum {
    kCFragGoesToEOF             = 0
};




typedef UInt8                           CFragLocatorKind;
enum {
    kMemoryCFragLocator         = 0,                            /* Container is already addressable.*/
    kDataForkCFragLocator       = 1,                            /* Container is in a file's data fork.*/
    kResourceCFragLocator       = 2,                            /* Container is in a file's resource fork.*/
    kNamedFragmentCFragLocator  = 4                             /* ! Reserved for possible future use!*/
};


/*
   --------------------------------------------------------------------------------------
   A 'cfrg' resource consists of a header followed by a sequence of variable length
   members.  The constant kDefaultCFragNameLen only provides for a legal ANSI declaration
   and for a reasonable display in a debugger.  The actual name field is cut to fit.
   There may be "extensions" after the name, the memberSize field includes them.  The
   general form of an extension is a 16 bit type code followed by a 16 bit size in bytes.
   Only one standard extension type is defined at present, it is used by SOM's searching
   mechanism.
*/



union CFragUsage1Union {                                        /* ! Meaning differs depending on value of "usage".*/
    UInt32                          appStackSize;               /* If the fragment is an application. (Not used by CFM!)*/
};
typedef union CFragUsage1Union          CFragUsage1Union;

union CFragUsage2Union {                                        /* ! Meaning differs depending on value of "usage".*/
    SInt16                          appSubdirID;                /* If the fragment is an application.*/
    UInt16                          libFlags;                   /* If the fragment is an import library.*/
};
typedef union CFragUsage2Union          CFragUsage2Union;
enum {
                                                                /* Bit masks for the CFragUsage2Union libFlags variant.*/
    kCFragLibUsageMapPrivatelyMask = 0x0001                     /* Put container in app heap if necessary.*/
};


union CFragWhere1Union {                                        /* ! Meaning differs depending on value of "where".*/
    UInt32                          spaceID;                    /* If the fragment is in memory.  (Actually an AddressSpaceID.)*/
    OSType                          forkKind;                   /* If the fragment is in an arbitrary byte stream fork.*/
};
typedef union CFragWhere1Union          CFragWhere1Union;

union CFragWhere2Union {                                        /* ! Meaning differs depending on value of "where".*/
    UInt16                          forkInstance;               /* If the fragment is in an arbitrary byte stream fork.*/
};
typedef union CFragWhere2Union          CFragWhere2Union;
enum {
    kDefaultCFragNameLen        = 16
};



struct CFragResourceMember {
    CFragArchitecture               architecture;
    UInt16                          reservedA;                  /* ! Must be zero!*/
    UInt8                           reservedB;                  /* ! Must be zero!*/
    UInt8                           updateLevel;
    CFragVersionNumber              currentVersion;
    CFragVersionNumber              oldDefVersion;
    CFragUsage1Union                uUsage1;
    CFragUsage2Union                uUsage2;
    CFragUsage                      usage;
    CFragLocatorKind                where;
    UInt32                          offset;
    UInt32                          length;
    CFragWhere1Union                uWhere1;
    CFragWhere2Union                uWhere2;
    UInt16                          extensionCount;             /* The number of extensions beyond the name.*/
    UInt16                          memberSize;                 /* Size in bytes, includes all extensions.*/
    unsigned char                   name[16];                   /* ! Actually a sized PString.*/
};
typedef struct CFragResourceMember      CFragResourceMember;
typedef CFragResourceMember *           CFragResourceMemberPtr;

struct CFragResourceExtensionHeader {
    UInt16                          extensionKind;
    UInt16                          extensionSize;
};
typedef struct CFragResourceExtensionHeader CFragResourceExtensionHeader;
typedef CFragResourceExtensionHeader *  CFragResourceExtensionHeaderPtr;

struct CFragResourceSearchExtension {
    CFragResourceExtensionHeader    header;
    OSType                          libKind;
    unsigned char                   qualifiers[1];              /* ! Actually four PStrings.*/
};
typedef struct CFragResourceSearchExtension CFragResourceSearchExtension;
typedef CFragResourceSearchExtension *  CFragResourceSearchExtensionPtr;
enum {
    kCFragResourceSearchExtensionKind = 0x30EE
};



struct CFragResource {
    UInt32                          reservedA;                  /* ! Must be zero!*/
    UInt32                          reservedB;                  /* ! Must be zero!*/
    UInt16                          reservedC;                  /* ! Must be zero!*/
    UInt16                          version;
    UInt32                          reservedD;                  /* ! Must be zero!*/
    UInt32                          reservedE;                  /* ! Must be zero!*/
    UInt32                          reservedF;                  /* ! Must be zero!*/
    UInt32                          reservedG;                  /* ! Must be zero!*/
    UInt16                          reservedH;                  /* ! Must be zero!*/
    UInt16                          memberCount;
    CFragResourceMember             firstMember;
};
typedef struct CFragResource            CFragResource;
typedef CFragResource *                 CFragResourcePtr;
typedef CFragResourcePtr *              CFragResourceHandle;
enum {
    kCurrCFragResourceVersion   = 1
};


#define AlignToFour(aValue) (((aValue) + 3) & ~3)
#define kBaseCFragResourceMemberSize    (offsetof ( CFragResourceMember, name ) )
#define kBaseCFragResourceSize          (offsetof ( CFragResource, firstMember.name ) )
#define NextCFragResourceMemberPtr(aMemberPtr)  \
        ((CFragResourceMemberPtr) ((BytePtr)aMemberPtr + aMemberPtr->memberSize))
#define FirstCFragResourceExtensionPtr(aMemberPtr)                                          \
        ((CFragResourceExtensionHeaderPtr) ((BytePtr)aMemberPtr +                           \
                                            AlignToFour ( kBaseCFragResourceMemberSize +    \
                                                          aMemberPtr->name[0] + 1 ) ))
#define NextCFragResourceExtensionPtr(anExtensionPtr)                   \
        ((CFragResourceExtensionHeaderPtr) ((BytePtr)anExtensionPtr +   \
                                            ((CFragResourceExtensionHeaderPtr)anExtensionPtr)->extensionSize ))
#define FirstCFragResourceSearchQualifier(searchExtensionPtr)                   \
        ((StringPtr) ((BytePtr)searchExtensionPtr +                             \
                      offsetof ( CFragResourceSearchExtension, qualifiers ) ))
#define NextCFragResourceSearchQualifier(searchQualifierPtr)    \
        ((StringPtr) ((BytePtr)searchQualifierPtr + searchQualifierPtr[0] + 1))

typedef struct OpaqueCFragConnectionID*  CFragConnectionID;
typedef struct OpaqueCFragClosureID*    CFragClosureID;
typedef struct OpaqueCFragContainerID*  CFragContainerID;
typedef struct OpaqueCFragContextID*    CFragContextID;

typedef UInt32                          CFragLoadOptions;
enum {
    kReferenceCFrag             = 0x0001,                       /* Try to use existing copy, increment reference counts.*/
    kFindCFrag                  = 0x0002,                       /* Try find an existing copy, do not increment reference counts.*/
    kPrivateCFragCopy           = 0x0005                        /* Prepare a new private copy.  (kReferenceCFrag | 0x0004)*/
};


enum {
    kUnresolvedCFragSymbolAddress = 0
};


typedef UInt8                           CFragSymbolClass;
enum {
    kCodeCFragSymbol            = 0,
    kDataCFragSymbol            = 1,
    kTVectorCFragSymbol         = 2,
    kTOCCFragSymbol             = 3,
    kGlueCFragSymbol            = 4
};


/*
   .
   ===========================================================================================
   Macros and Functions
   ====================
*/


#define CFragHasFileLocation(where) \
        ( ((where) == kDataForkCFragLocator) || ((where) == kResourceCFragLocator) )
EXTERN_API( OSErr )
GetSharedLibrary                (ConstStr63Param        libName,
                                 CFragArchitecture      archType,
                                 CFragLoadOptions       loadFlags,
                                 CFragConnectionID *    connID, /* can be NULL */
                                 Ptr *                  mainAddr, /* can be NULL */
                                 Str255                 errMessage) /* can be NULL */       THREEWORDINLINE(0x3F3C, 0x0001, 0xAA5A);

EXTERN_API( OSErr )
GetDiskFragment                 (const FSSpec *         fileSpec,
                                 UInt32                 offset,
                                 UInt32                 length,
                                 ConstStr63Param        fragName, /* can be NULL */
                                 CFragLoadOptions       loadFlags,
                                 CFragConnectionID *    connID, /* can be NULL */
                                 Ptr *                  mainAddr, /* can be NULL */
                                 Str255                 errMessage) /* can be NULL */       THREEWORDINLINE(0x3F3C, 0x0002, 0xAA5A);

EXTERN_API( OSErr )
GetMemFragment                  (void *                 memAddr,
                                 UInt32                 length,
                                 ConstStr63Param        fragName, /* can be NULL */
                                 CFragLoadOptions       loadFlags,
                                 CFragConnectionID *    connID, /* can be NULL */
                                 Ptr *                  mainAddr, /* can be NULL */
                                 Str255                 errMessage) /* can be NULL */       THREEWORDINLINE(0x3F3C, 0x0003, 0xAA5A);

EXTERN_API( OSErr )
CloseConnection                 (CFragConnectionID *    connID)                             THREEWORDINLINE(0x3F3C, 0x0004, 0xAA5A);

EXTERN_API( OSErr )
FindSymbol                      (CFragConnectionID      connID,
                                 ConstStr255Param       symName,
                                 Ptr *                  symAddr, /* can be NULL */
                                 CFragSymbolClass *     symClass) /* can be NULL */         THREEWORDINLINE(0x3F3C, 0x0005, 0xAA5A);

EXTERN_API( OSErr )
CountSymbols                    (CFragConnectionID      connID,
                                 long *                 symCount)                           THREEWORDINLINE(0x3F3C, 0x0006, 0xAA5A);

EXTERN_API( OSErr )
GetIndSymbol                    (CFragConnectionID      connID,
                                 long                   symIndex,
                                 Str255                 symName, /* can be NULL */
                                 Ptr *                  symAddr, /* can be NULL */
                                 CFragSymbolClass *     symClass) /* can be NULL */         THREEWORDINLINE(0x3F3C, 0x0007, 0xAA5A);


/*
   .
   ===========================================================================================
   Initialization & Termination Routines
   =====================================
*/


/*
   -----------------------------------------------------------------------------------------
   A fragment's initialization and termination routines are called when a new incarnation of
   the fragment is created or destroyed, respectively.  Exactly when this occurs depends on
   what kinds of section sharing the fragment has and how the fragment is prepared.  Import
   libraries have at most one incarnation per process.  Fragments prepared with option
   kPrivateCFragCopy may have many incarnations per process.
   The initialization function is passed a pointer to an initialization information structure
   and returns an OSErr.  If an initialization function returns a non-zero value the entire
   closure of which it is a part fails.  The C prototype for an initialization function is:
        OSErr   CFragInitFunction   ( const CFragInitBlock *    initBlock );
   The termination procedure takes no parameters and returns nothing.  The C prototype for a
   termination procedure is:
        void    CFragTermProcedure  ( void );
   Note that since the initialization and termination routines are themselves "CFM"-style
   routines whether or not they have the "pascal" keyword is irrelevant.
*/


/*
   -----------------------------------------------------------------------------------------
   ! Note:
   ! The "System7" portion of these type names was introduced during the evolution towards
   ! the now defunct Copland version of Mac OS.  Copland was to be called System 8 and there
   ! were slightly different types for System 7 and System 8.  The "generic" type names were
   ! conditionally defined for the desired target system.
   ! Always use the generic types, e.g. CFragInitBlock!  The "System7" names have been kept
   ! only to avoid perturbing code that (improperly) used the target specific type.
*/



struct CFragSystem7MemoryLocator {
    LogicalAddress                  address;
    UInt32                          length;
    Boolean                         inPlace;
    UInt8                           reservedA;                  /* ! Must be zero!*/
    UInt16                          reservedB;                  /* ! Must be zero!*/
};
typedef struct CFragSystem7MemoryLocator CFragSystem7MemoryLocator;

struct CFragSystem7DiskFlatLocator {
    FSSpecPtr                       fileSpec;
    UInt32                          offset;
    UInt32                          length;
};
typedef struct CFragSystem7DiskFlatLocator CFragSystem7DiskFlatLocator;
/* ! This must have a file specification at the same offset as a disk flat locator!*/

struct CFragSystem7SegmentedLocator {
    FSSpecPtr                       fileSpec;
    OSType                          rsrcType;
    SInt16                          rsrcID;
    UInt16                          reservedA;                  /* ! Must be zero!*/
};
typedef struct CFragSystem7SegmentedLocator CFragSystem7SegmentedLocator;

struct CFragSystem7Locator {
    SInt32                          where;
    union {
        CFragSystem7DiskFlatLocator     onDisk;
        CFragSystem7MemoryLocator       inMem;
        CFragSystem7SegmentedLocator    inSegs;
    }                                 u;
};
typedef struct CFragSystem7Locator      CFragSystem7Locator;
typedef CFragSystem7Locator *           CFragSystem7LocatorPtr;

struct CFragSystem7InitBlock {
    CFragContextID                  contextID;
    CFragClosureID                  closureID;
    CFragConnectionID               connectionID;
    CFragSystem7Locator             fragLocator;
    StringPtr                       libName;
    UInt32                          reservedA;                  /* ! Must be zero!*/
};
typedef struct CFragSystem7InitBlock    CFragSystem7InitBlock;
typedef CFragSystem7InitBlock *         CFragSystem7InitBlockPtr;

typedef CFragSystem7InitBlock           CFragInitBlock;
typedef CFragSystem7InitBlockPtr        CFragInitBlockPtr;
/* These init/term routine types are only of value to CFM itself.*/
typedef CALLBACK_API_C( OSErr , CFragInitFunction )(const CFragInitBlock *initBlock);
typedef CALLBACK_API_C( void , CFragTermProcedure )(void );
/*
   .
   ===========================================================================================
   Old Name Spellings
   ==================
*/


/*
   -------------------------------------------------------------------------------------------
   We've tried to reduce the risk of name collisions in the future by introducing the phrase
   "CFrag" into constant and type names.  The old names are defined below in terms of the new.
*/


enum {
    kLoadCFrag                  = kReferenceCFrag
};


#if OLDROUTINENAMES
#define IsFileLocation      CFragHasFileLocation
typedef CFragConnectionID               ConnectionID;
typedef CFragLoadOptions                LoadFlags;
typedef CFragSymbolClass                SymClass;
typedef CFragInitBlock                  InitBlock;
typedef CFragInitBlockPtr               InitBlockPtr;
typedef CFragSystem7MemoryLocator       MemFragment;
typedef CFragSystem7DiskFlatLocator     DiskFragment;
typedef CFragSystem7SegmentedLocator    SegmentedFragment;
typedef CFragSystem7Locator             FragmentLocator;
typedef CFragSystem7LocatorPtr          FragmentLocatorPtr;
typedef CFragSystem7MemoryLocator       CFragHFSMemoryLocator;
typedef CFragSystem7DiskFlatLocator     CFragHFSDiskFlatLocator;
typedef CFragSystem7SegmentedLocator    CFragHFSSegmentedLocator;
typedef CFragSystem7Locator             CFragHFSLocator;
typedef CFragSystem7LocatorPtr          CFragHFSLocatorPtr;
enum {
    kPowerPCArch                = kPowerPCCFragArch,
    kMotorola68KArch            = kMotorola68KCFragArch,
    kAnyArchType                = kAnyCFragArch,
    kNoLibName                  = 0,
    kNoConnectionID             = 0,
    kLoadLib                    = kLoadCFrag,
    kFindLib                    = kFindCFrag,
    kNewCFragCopy               = kPrivateCFragCopy,
    kLoadNewCopy                = kPrivateCFragCopy,
    kUseInPlace                 = 0x80,
    kCodeSym                    = kCodeCFragSymbol,
    kDataSym                    = kDataCFragSymbol,
    kTVectSym                   = kTVectorCFragSymbol,
    kTOCSym                     = kTOCCFragSymbol,
    kGlueSym                    = kGlueCFragSymbol,
    kInMem                      = kMemoryCFragLocator,
    kOnDiskFlat                 = kDataForkCFragLocator,
    kOnDiskSegmented            = kResourceCFragLocator,
    kIsLib                      = kImportLibraryCFrag,
    kIsApp                      = kApplicationCFrag,
    kIsDropIn                   = kDropInAdditionCFrag,
    kFullLib                    = kIsCompleteCFrag,
    kUpdateLib                  = kFirstCFragUpdate,
    kWholeFork                  = kCFragGoesToEOF,
    kCFMRsrcType                = kCFragResourceType,
    kCFMRsrcID                  = kCFragResourceID,
    kSHLBFileType               = kCFragLibraryFileType,
    kUnresolvedSymbolAddress    = kUnresolvedCFragSymbolAddress
};

enum {
    kPowerPC                    = kPowerPCCFragArch,
    kMotorola68K                = kMotorola68KCFragArch
};

#endif  /* OLDROUTINENAMES */






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

#endif /* __CODEFRAGMENTS__ */

