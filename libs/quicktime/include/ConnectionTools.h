/*
     File:       ConnectionTools.h
 
     Contains:   Communications Toolbox Connection Tools Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1988-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CONNECTIONTOOLS__
#define __CONNECTIONTOOLS__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __CONNECTIONS__
#include <Connections.h>
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

#if CALL_NOT_IN_CARBON
enum {
                                                                /* messages for DefProc */
    cmInitMsg                   = 0,
    cmDisposeMsg                = 1,
    cmSuspendMsg                = 2,
    cmResumeMsg                 = 3,
    cmMenuMsg                   = 4,
    cmEventMsg                  = 5,
    cmActivateMsg               = 6,
    cmDeactivateMsg             = 7,
    cmIdleMsg                   = 50,
    cmResetMsg                  = 51,
    cmAbortMsg                  = 52,
    cmReadMsg                   = 100,
    cmWriteMsg                  = 101,
    cmStatusMsg                 = 102,
    cmListenMsg                 = 103,
    cmAcceptMsg                 = 104,
    cmCloseMsg                  = 105,
    cmOpenMsg                   = 106,
    cmBreakMsg                  = 107,
    cmIOKillMsg                 = 108,
    cmEnvironsMsg               = 109,                          /* new connection tool messages for ctb 1.1 */
    cmNewIOPBMsg                = 110,
    cmDisposeIOPBMsg            = 111,
    cmGetErrorStringMsg         = 112,
    cmPBReadMsg                 = 113,
    cmPBWriteMsg                = 114,
    cmPBIOKillMsg               = 115
};

enum {
                                                                /*    messages for validate DefProc  */
    cmValidateMsg               = 0,
    cmDefaultMsg                = 1
};

enum {
                                                                /*    messages for Setup DefProc */
    cmSpreflightMsg             = 0,
    cmSsetupMsg                 = 1,
    cmSitemMsg                  = 2,
    cmSfilterMsg                = 3,
    cmScleanupMsg               = 4
};

enum {
                                                                /*    messages for scripting defProc */
    cmMgetMsg                   = 0,
    cmMsetMsg                   = 1
};

enum {
                                                                /*    messages for localization defProc  */
    cmL2English                 = 0,
    cmL2Intl                    = 1
};

enum {
                                                                /* private data constants */
    cdefType                    = FOUR_CHAR_CODE('cdef'),       /* main connection definition procedure */
    cvalType                    = FOUR_CHAR_CODE('cval'),       /* validation definition procedure */
    csetType                    = FOUR_CHAR_CODE('cset'),       /* connection setup definition procedure */
    clocType                    = FOUR_CHAR_CODE('cloc'),       /* connection configuration localization defProc */
    cscrType                    = FOUR_CHAR_CODE('cscr'),       /* connection scripting defProc interfaces */
    cbndType                    = FOUR_CHAR_CODE('cbnd'),       /* bundle type for connection */
    cverType                    = FOUR_CHAR_CODE('vers')
};


struct CMDataBuffer {
    Ptr                             thePtr;
    long                            count;
    CMChannel                       channel;
    CMFlags                         flags;
};
typedef struct CMDataBuffer             CMDataBuffer;
typedef CMDataBuffer *                  CMDataBufferPtr;

struct CMCompletorRecord {
    Boolean                         async;
    SInt8                           filler;
    ConnectionCompletionUPP         completionRoutine;
};
typedef struct CMCompletorRecord        CMCompletorRecord;

typedef CMCompletorRecord *             CMCompletorPtr;
/*  Private Data Structure  */

struct CMSetupStruct {
    DialogPtr                       theDialog;
    short                           count;
    Ptr                             theConfig;
    short                           procID;                     /* procID of the tool */
};
typedef struct CMSetupStruct            CMSetupStruct;

typedef CMSetupStruct *                 CMSetupPtr;
#endif  /* CALL_NOT_IN_CARBON */


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

#endif /* __CONNECTIONTOOLS__ */

