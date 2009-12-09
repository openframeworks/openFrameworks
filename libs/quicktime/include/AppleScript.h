/*
     File:       AppleScript.h
 
     Contains:   AppleScript Specific Interfaces.
 
     Version:    Technology: AppleScript 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1992-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __APPLESCRIPT__
#define __APPLESCRIPT__

#ifndef __MACERRORS__
#include <MacErrors.h>
#endif

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
#endif

#ifndef __OSA__
#include <OSA.h>
#endif

#ifndef __TEXTEDIT__
#include <TextEdit.h>
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
/*
    The specific type for the AppleScript instance of the
    Open Scripting Architecture type.
*/
enum {
    typeAppleScript             = FOUR_CHAR_CODE('ascr'),
    kAppleScriptSubtype         = typeAppleScript,
    typeASStorage               = typeAppleScript
};

/**************************************************************************
    Component Selectors
**************************************************************************/

enum {
    kASSelectInit               = 0x1001,
    kASSelectSetSourceStyles    = 0x1002,
    kASSelectGetSourceStyles    = 0x1003,
    kASSelectGetSourceStyleNames = 0x1004
};


/**************************************************************************
    OSAGetScriptInfo Selectors
**************************************************************************/
enum {
    kASHasOpenHandler           = FOUR_CHAR_CODE('hsod')
};

/*
        This selector is used to query a context as to whether it contains
        a handler for the kAEOpenDocuments event. This allows "applets" to be 
        distinguished from "droplets."  OSAGetScriptInfo returns false if
        there is no kAEOpenDocuments handler, and returns the error value 
        errOSAInvalidAccess if the input is not a context.
    */
/**************************************************************************
    Initialization
**************************************************************************/
EXTERN_API( OSAError )
ASInit                          (ComponentInstance      scriptingComponent,
                                 long                   modeFlags,
                                 long                   minStackSize,
                                 long                   preferredStackSize,
                                 long                   maxStackSize,
                                 long                   minHeapSize,
                                 long                   preferredHeapSize,
                                 long                   maxHeapSize)                        FIVEWORDINLINE(0x2F3C, 0x001C, 0x1001, 0x7000, 0xA82A);

/*
        ComponentCallNow(kASSelectInit, 28);
        This call can be used to explicitly initialize AppleScript.  If it is
        not called, the a scripting size resource is looked for and used. If
        there is no scripting size resource, then the constants listed below
        are used.  If at any stage (the init call, the size resource, the 
        defaults) any of these parameters are zero, then parameters from the
        next stage are used.  ModeFlags are not currently used.
        Errors:
        errOSASystemError       initialization failed
    */
/*
    These values will be used if ASInit is not called explicitly, or if any
    of ASInit's parameters are zero:
*/
enum {
    kASDefaultMinStackSize      = 4 * 1024,
    kASDefaultPreferredStackSize = 16 * 1024,
    kASDefaultMaxStackSize      = 16 * 1024,
    kASDefaultMinHeapSize       = 4 * 1024,
    kASDefaultPreferredHeapSize = 16 * 1024,
    kASDefaultMaxHeapSize       = 32L * 1024 * 1024
};

/**************************************************************************
    Source Styles
**************************************************************************/
EXTERN_API( OSAError )
ASSetSourceStyles               (ComponentInstance      scriptingComponent,
                                 STHandle               sourceStyles)                       FIVEWORDINLINE(0x2F3C, 0x0004, 0x1002, 0x7000, 0xA82A);

/*
        ComponentCallNow(kASSelectSetSourceStyles, 4);
        Errors:
        errOSASystemError       operation failed
    */
EXTERN_API( OSAError )
ASGetSourceStyles               (ComponentInstance      scriptingComponent,
                                 STHandle *             resultingSourceStyles)              FIVEWORDINLINE(0x2F3C, 0x0004, 0x1003, 0x7000, 0xA82A);

/*
        ComponentCallNow(kASSelectGetSourceStyles, 4);
        Errors:
        errOSASystemError       operation failed
    */
EXTERN_API( OSAError )
ASGetSourceStyleNames           (ComponentInstance      scriptingComponent,
                                 long                   modeFlags,
                                 AEDescList *           resultingSourceStyleNamesList)      FIVEWORDINLINE(0x2F3C, 0x0008, 0x1004, 0x7000, 0xA82A);

/*
        ComponentCallNow(kASSelectGetSourceStyleNames, 8);
        This call returns an AEList of styled text descriptors the names of the
        source styles in the current dialect.  The order of the names corresponds
        to the order of the source style constants, below.  The style of each
        name is the same as the styles returned by ASGetSourceStyles.
        
        Errors:
        errOSASystemError       operation failed
    */
/*
    Elements of STHandle correspond to following categories of tokens, and
    accessed through following index constants:
*/
enum {
    kASSourceStyleUncompiledText = 0,
    kASSourceStyleNormalText    = 1,
    kASSourceStyleLanguageKeyword = 2,
    kASSourceStyleApplicationKeyword = 3,
    kASSourceStyleComment       = 4,
    kASSourceStyleLiteral       = 5,
    kASSourceStyleUserSymbol    = 6,
    kASSourceStyleObjectSpecifier = 7,
    kASNumberOfSourceStyles     = 8
};



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

#endif /* __APPLESCRIPT__ */

