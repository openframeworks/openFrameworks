/*
     File:       ASDebugging.h
 
     Contains:   AppleScript Debugging Interfaces.
 
     Version:    Technology: AppleScript 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1992-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ASDEBUGGING__
#define __ASDEBUGGING__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __FILES__
#include <Files.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
#endif

#ifndef __APPLESCRIPT__
#include <AppleScript.h>
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
    Mode Flags
**************************************************************************/
/*    This mode flag can be passed to OSASetProperty or OSASetHandler
    and will prevent properties or handlers from being defined in a context
    that doesn't already have bindings for them. An error is returned if
    a current binding doesn't already exist. 
*/
enum {
    kOSAModeDontDefine          = 0x0001
};

/**************************************************************************
    Component Selectors
**************************************************************************/
enum {
    kASSelectSetPropertyObsolete = 0x1101,
    kASSelectGetPropertyObsolete = 0x1102,
    kASSelectSetHandlerObsolete = 0x1103,
    kASSelectGetHandlerObsolete = 0x1104,
    kASSelectGetAppTerminologyObsolete = 0x1105,
    kASSelectSetProperty        = 0x1106,
    kASSelectGetProperty        = 0x1107,
    kASSelectSetHandler         = 0x1108,
    kASSelectGetHandler         = 0x1109,
    kASSelectGetAppTerminology  = 0x110A,
    kASSelectGetSysTerminology  = 0x110B,
    kASSelectGetPropertyNames   = 0x110C,
    kASSelectGetHandlerNames    = 0x110D
};

enum {
    kASSelectFlushTerminology   = 0x110E
};

/**************************************************************************
    Context Accessors
**************************************************************************/
EXTERN_API( OSAError )
OSASetProperty                  (ComponentInstance      scriptingComponent,
                                 long                   modeFlags,
                                 OSAID                  contextID,
                                 const AEDesc *         variableName,
                                 OSAID                  scriptValueID)                      FIVEWORDINLINE(0x2F3C, 0x0010, 0x1106, 0x7000, 0xA82A);

EXTERN_API( OSAError )
OSAGetProperty                  (ComponentInstance      scriptingComponent,
                                 long                   modeFlags,
                                 OSAID                  contextID,
                                 const AEDesc *         variableName,
                                 OSAID *                resultingScriptValueID)             FIVEWORDINLINE(0x2F3C, 0x0010, 0x1107, 0x7000, 0xA82A);

EXTERN_API( OSAError )
OSAGetPropertyNames             (ComponentInstance      scriptingComponent,
                                 long                   modeFlags,
                                 OSAID                  contextID,
                                 AEDescList *           resultingPropertyNames)             FIVEWORDINLINE(0x2F3C, 0x000C, 0x110C, 0x7000, 0xA82A);

EXTERN_API( OSAError )
OSASetHandler                   (ComponentInstance      scriptingComponent,
                                 long                   modeFlags,
                                 OSAID                  contextID,
                                 const AEDesc *         handlerName,
                                 OSAID                  compiledScriptID)                   FIVEWORDINLINE(0x2F3C, 0x0010, 0x1108, 0x7000, 0xA82A);

EXTERN_API( OSAError )
OSAGetHandler                   (ComponentInstance      scriptingComponent,
                                 long                   modeFlags,
                                 OSAID                  contextID,
                                 const AEDesc *         handlerName,
                                 OSAID *                resultingCompiledScriptID)          FIVEWORDINLINE(0x2F3C, 0x0010, 0x1109, 0x7000, 0xA82A);

EXTERN_API( OSAError )
OSAGetHandlerNames              (ComponentInstance      scriptingComponent,
                                 long                   modeFlags,
                                 OSAID                  contextID,
                                 AEDescList *           resultingHandlerNames)              FIVEWORDINLINE(0x2F3C, 0x000C, 0x110D, 0x7000, 0xA82A);

EXTERN_API( OSAError )
OSAGetAppTerminology            (ComponentInstance      scriptingComponent,
                                 long                   modeFlags,
                                 FSSpec *               fileSpec,
                                 short                  terminologyID,
                                 Boolean *              didLaunch,
                                 AEDesc *               terminologyList)                    FIVEWORDINLINE(0x2F3C, 0x0012, 0x110A, 0x7000, 0xA82A);

/* Errors:
       errOSASystemError        operation failed
    */
EXTERN_API( OSAError )
OSAGetSysTerminology            (ComponentInstance      scriptingComponent,
                                 long                   modeFlags,
                                 short                  terminologyID,
                                 AEDesc *               terminologyList)                    FIVEWORDINLINE(0x2F3C, 0x000A, 0x110B, 0x7000, 0xA82A);

/* Errors:
       errOSASystemError        operation failed
    */
/* Notes on terminology ID

    A terminology ID is derived from script code and language code
    as follows;

        terminologyID = ((scriptCode & 0x7F) << 8) | (langCode & 0xFF)
*/
/**************************************************************************
    Obsolete versions provided for backward compatibility:
*/
EXTERN_API( OSAError )
ASSetProperty                   (ComponentInstance      scriptingComponent,
                                 OSAID                  contextID,
                                 const AEDesc *         variableName,
                                 OSAID                  scriptValueID)                      FIVEWORDINLINE(0x2F3C, 0x000C, 0x1101, 0x7000, 0xA82A);

EXTERN_API( OSAError )
ASGetProperty                   (ComponentInstance      scriptingComponent,
                                 OSAID                  contextID,
                                 const AEDesc *         variableName,
                                 OSAID *                resultingScriptValueID)             FIVEWORDINLINE(0x2F3C, 0x000C, 0x1102, 0x7000, 0xA82A);

EXTERN_API( OSAError )
ASSetHandler                    (ComponentInstance      scriptingComponent,
                                 OSAID                  contextID,
                                 const AEDesc *         handlerName,
                                 OSAID                  compiledScriptID)                   FIVEWORDINLINE(0x2F3C, 0x000C, 0x1103, 0x7000, 0xA82A);

EXTERN_API( OSAError )
ASGetHandler                    (ComponentInstance      scriptingComponent,
                                 OSAID                  contextID,
                                 const AEDesc *         handlerName,
                                 OSAID *                resultingCompiledScriptID)          FIVEWORDINLINE(0x2F3C, 0x000C, 0x1104, 0x7000, 0xA82A);

EXTERN_API( OSAError )
ASGetAppTerminology             (ComponentInstance      scriptingComponent,
                                 FSSpec *               fileSpec,
                                 short                  terminologID,
                                 Boolean *              didLaunch,
                                 AEDesc *               terminologyList)                    FIVEWORDINLINE(0x2F3C, 0x000E, 0x1105, 0x7000, 0xA82A);

/* Errors:
        errOSASystemError       operation failed
    */
/**************************************************************************/



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

#endif /* __ASDEBUGGING__ */

