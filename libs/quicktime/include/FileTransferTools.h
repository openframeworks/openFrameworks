/*
     File:       FileTransferTools.h
 
     Contains:   CommToolbox File Transfer Tools Interfaces.
 
     Version:    Technology: System 7.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1988-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FILETRANSFERTOOLS__
#define __FILETRANSFERTOOLS__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __DIALOGS__
#include <Dialogs.h>
#endif

#ifndef __FILETRANSFERS__
#include <FileTransfers.h>
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
                                                                /* DEFs */
    fdefType                    = FOUR_CHAR_CODE('fdef'),
    fsetType                    = FOUR_CHAR_CODE('fset'),
    fvalType                    = FOUR_CHAR_CODE('fval'),
    flocType                    = FOUR_CHAR_CODE('floc'),
    fscrType                    = FOUR_CHAR_CODE('fscr'),
    fbndType                    = FOUR_CHAR_CODE('fbnd'),
    fverType                    = FOUR_CHAR_CODE('vers')
};

enum {
                                                                /* control */
    ftInitMsg                   = 0,
    ftDisposeMsg                = 1,
    ftSuspendMsg                = 2,
    ftResumeMsg                 = 3,
    ftMenuMsg                   = 4,
    ftEventMsg                  = 5,
    ftActivateMsg               = 6,
    ftDeactivateMsg             = 7,
    ftGetErrorStringMsg         = 8,
    ftAbortMsg                  = 52,
    ftStartMsg                  = 100,
    ftExecMsg                   = 102,
    ftSendMsg                   = 103,
    ftReceiveMsg                = 104
};

enum {
                                                                /* setup */
    ftSpreflightMsg             = 0,
    ftSsetupMsg                 = 1,
    ftSitemMsg                  = 2,
    ftSfilterMsg                = 3,
    ftScleanupMsg               = 4
};

enum {
                                                                /* validate */
    ftValidateMsg               = 0,
    ftDefaultMsg                = 1
};

enum {
                                                                /* scripting */
    ftMgetMsg                   = 0,
    ftMsetMsg                   = 1
};

enum {
                                                                /* localization */
    ftL2English                 = 0,
    ftL2Intl                    = 1
};


struct FTSetupStruct {
    DialogPtr                       theDialog;                  /* the dialog form the application */
    short                           count;                      /* first appended item */
    Ptr                             theConfig;                  /* the config record to setup */
    short                           procID;                     /* procID of the tool */
};
typedef struct FTSetupStruct            FTSetupStruct;

typedef FTSetupStruct *                 FTSetupPtr;
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

#endif /* __FILETRANSFERTOOLS__ */

