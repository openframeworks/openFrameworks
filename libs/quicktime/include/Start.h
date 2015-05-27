/*
     File:       Start.h
 
     Contains:   Start Manager Interfaces.
 
     Version:    Technology: Mac OS 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1987-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __START__
#define __START__

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
    Important: When the major version number of kExtensionTableVersion and the value
    returned by gestaltExtensionTableVersion change, it indicates that the Extension
    Table startup mechanism has radically changed and code that doesn't know about
    the new major version must not attempt to use the Extension Table startup
    mechanism.
    
    Changes to the minor version number of kExtensionTableVersion indicate that the
    definition of the ExtensionElement structure has been extended, but the fields
    defined for previous minor versions of kExtensionTableVersion have not changed.
*/
enum {
    kExtensionTableVersion      = 0x00000100                    /* current ExtensionTable version (1.0.0) */
};

/* ExtensionNotification message codes */
enum {
    extNotificationBeforeFirst  = 0,                            /* Before any extensions have loaded */
    extNotificationAfterLast    = 1,                            /* After all extensions have loaded */
    extNotificationBeforeCurrent = 2,                           /* Before extension at extElementIndex is loaded */
    extNotificationAfterCurrent = 3                             /* After extension at extElementIndex is loaded */
};


struct ExtensionElement {
    Str31                           fileName;                   /* The file name */
    long                            parentDirID;                /* the file's parent directory ID */
                                                                /* and everything after ioNamePtr in the HParamBlockRec.fileParam variant */
    short                           ioVRefNum;                  /* always the real volume reference number (not a drive, default, or working dirID) */
    short                           ioFRefNum;
    SInt8                           ioFVersNum;
    SInt8                           filler1;
    short                           ioFDirIndex;                /* always 0 in table */
    SInt8                           ioFlAttrib;
    SInt8                           ioFlVersNum;
    FInfo                           ioFlFndrInfo;
    long                            ioDirID;
    unsigned short                  ioFlStBlk;
    long                            ioFlLgLen;
    long                            ioFlPyLen;
    unsigned short                  ioFlRStBlk;
    long                            ioFlRLgLen;
    long                            ioFlRPyLen;
    unsigned long                   ioFlCrDat;
    unsigned long                   ioFlMdDat;
};
typedef struct ExtensionElement         ExtensionElement;
typedef ExtensionElement *              ExtensionElementPtr;

struct ExtensionTableHeader {
    UInt32                          extTableHeaderSize;         /* size of ExtensionTable header ( equal to offsetof(ExtensionTable, extElements[0]) ) */
    UInt32                          extTableVersion;            /* current ExtensionTable version (same as returned by gestaltExtTableVersion Gestalt selector) */
    UInt32                          extElementIndex;            /* current index into ExtensionElement records (zero-based) */
    UInt32                          extElementSize;             /* size of ExtensionElement */
    UInt32                          extElementCount;            /* number of ExtensionElement records in table (1-based) */
};
typedef struct ExtensionTableHeader     ExtensionTableHeader;

struct ExtensionTable {
    ExtensionTableHeader            extTableHeader;             /* the ExtensionTableHeader */
    ExtensionElement                extElements[1];             /* one element for each extension to load */
};
typedef struct ExtensionTable           ExtensionTable;
typedef ExtensionTable *                ExtensionTablePtr;
typedef ExtensionTablePtr *             ExtensionTableHandle;
typedef CALLBACK_API( void , ExtensionNotificationProcPtr )(UInt32 message, void *param, ExtensionElementPtr extElement);
typedef CALLBACK_API( void , ExtensionTableHandlerProcPtr )(UInt32 message, void *param, ExtensionTableHandle extTableHandle);
typedef STACK_UPP_TYPE(ExtensionNotificationProcPtr)            ExtensionNotificationUPP;
typedef STACK_UPP_TYPE(ExtensionTableHandlerProcPtr)            ExtensionTableHandlerUPP;
#if OPAQUE_UPP_TYPES
#if CALL_NOT_IN_CARBON
    EXTERN_API(ExtensionNotificationUPP)
    NewExtensionNotificationUPP    (ExtensionNotificationProcPtr userRoutine);

    EXTERN_API(ExtensionTableHandlerUPP)
    NewExtensionTableHandlerUPP    (ExtensionTableHandlerProcPtr userRoutine);

    EXTERN_API(void)
    DisposeExtensionNotificationUPP    (ExtensionNotificationUPP userUPP);

    EXTERN_API(void)
    DisposeExtensionTableHandlerUPP    (ExtensionTableHandlerUPP userUPP);

    EXTERN_API(void)
    InvokeExtensionNotificationUPP    (UInt32               message,
                                    void *                  param,
                                    ExtensionElementPtr     extElement,
                                    ExtensionNotificationUPP userUPP);

    EXTERN_API(void)
    InvokeExtensionTableHandlerUPP    (UInt32               message,
                                    void *                  param,
                                    ExtensionTableHandle    extTableHandle,
                                    ExtensionTableHandlerUPP userUPP);

#endif  /* CALL_NOT_IN_CARBON */

#else
    enum { uppExtensionNotificationProcInfo = 0x00000FC0 };         /* pascal no_return_value Func(4_bytes, 4_bytes, 4_bytes) */
    enum { uppExtensionTableHandlerProcInfo = 0x00000FC0 };         /* pascal no_return_value Func(4_bytes, 4_bytes, 4_bytes) */
    #define NewExtensionNotificationUPP(userRoutine)                (ExtensionNotificationUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppExtensionNotificationProcInfo, GetCurrentArchitecture())
    #define NewExtensionTableHandlerUPP(userRoutine)                (ExtensionTableHandlerUPP)NewRoutineDescriptor((ProcPtr)(userRoutine), uppExtensionTableHandlerProcInfo, GetCurrentArchitecture())
    #define DisposeExtensionNotificationUPP(userUPP)                DisposeRoutineDescriptor(userUPP)
    #define DisposeExtensionTableHandlerUPP(userUPP)                DisposeRoutineDescriptor(userUPP)
    #define InvokeExtensionNotificationUPP(message, param, extElement, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppExtensionNotificationProcInfo, (message), (param), (extElement))
    #define InvokeExtensionTableHandlerUPP(message, param, extTableHandle, userUPP)  CALL_THREE_PARAMETER_UPP((userUPP), uppExtensionTableHandlerProcInfo, (message), (param), (extTableHandle))
#endif
/* support for pre-Carbon UPP routines: NewXXXProc and CallXXXProc */
#define NewExtensionNotificationProc(userRoutine)               NewExtensionNotificationUPP(userRoutine)
#define NewExtensionTableHandlerProc(userRoutine)               NewExtensionTableHandlerUPP(userRoutine)
#define CallExtensionNotificationProc(userRoutine, message, param, extElement) InvokeExtensionNotificationUPP(message, param, extElement, userRoutine)
#define CallExtensionTableHandlerProc(userRoutine, message, param, extTableHandle) InvokeExtensionTableHandlerUPP(message, param, extTableHandle, userRoutine)

union DefStartRec {
    struct {
        SignedByte                      sdExtDevID;
        SignedByte                      sdPartition;
        SignedByte                      sdSlotNum;
        SignedByte                      sdSRsrcID;
    }                                 slotDev;
    struct {
        SignedByte                      sdReserved1;
        SignedByte                      sdReserved2;
        short                           sdRefNum;
    }                                 scsiDev;
};
typedef union DefStartRec               DefStartRec;

typedef DefStartRec *                   DefStartPtr;

struct DefVideoRec {
    SignedByte                      sdSlot;
    SignedByte                      sdsResource;
};
typedef struct DefVideoRec              DefVideoRec;

typedef DefVideoRec *                   DefVideoPtr;

struct DefOSRec {
    SignedByte                      sdReserved;
    SignedByte                      sdOSType;
};
typedef struct DefOSRec                 DefOSRec;

typedef DefOSRec *                      DefOSPtr;
#if CALL_NOT_IN_CARBON
                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter GetDefaultStartup(__A0)
                                                                                            #endif
EXTERN_API( void )
GetDefaultStartup               (DefStartPtr            paramBlock)                         ONEWORDINLINE(0xA07D);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetDefaultStartup(__A0)
                                                                                            #endif
EXTERN_API( void )
SetDefaultStartup               (DefStartPtr            paramBlock)                         ONEWORDINLINE(0xA07E);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter GetVideoDefault(__A0)
                                                                                            #endif
EXTERN_API( void )
GetVideoDefault                 (DefVideoPtr            paramBlock)                         ONEWORDINLINE(0xA080);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetVideoDefault(__A0)
                                                                                            #endif
EXTERN_API( void )
SetVideoDefault                 (DefVideoPtr            paramBlock)                         ONEWORDINLINE(0xA081);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter GetOSDefault(__A0)
                                                                                            #endif
EXTERN_API( void )
GetOSDefault                    (DefOSPtr               paramBlock)                         ONEWORDINLINE(0xA084);

                                                                                            #if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
                                                                                            #pragma parameter SetOSDefault(__A0)
                                                                                            #endif
EXTERN_API( void )
SetOSDefault                    (DefOSPtr               paramBlock)                         ONEWORDINLINE(0xA083);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API( void )
SetTimeout                      (short                  count);

EXTERN_API( void )
GetTimeout                      (short *                count);

#endif  /* CALL_NOT_IN_CARBON */

/*
    InstallExtensionNotificationProc

    Installs an ExtensionNotificationUPP.

    Parameters:
        extNotificationProc The ExtensionNotificationUPP to install.

    Results:
        noErr       0       The ExtensionNotificationUPP was installed.
        paramErr    -50     This ExtensionNotificationUPP has already been installed.
        memFullErr  -108    Not enough memory to install the ExtensionNotificationUPP.
*/
#if CALL_NOT_IN_CARBON
EXTERN_API( OSErr )
InstallExtensionNotificationProc (ExtensionNotificationUPP  extNotificationProc)            TWOWORDINLINE(0x7000, 0xAA7D);


/*
    RemoveExtensionNotificationProc

    Removes an ExtensionNotificationUPP.
    
    Note:   ExtensionNotificationUPPs can't call RemoveExtensionNotificationProc.

    Parameters:
        extNotificationProc The ExtensionNotificationUPP to remove.

    Results:
        noErr       0       The ExtensionNotificationUPP was removed.
        paramErr    -50     The ExtensionNotificationUPP was not found, or
                            RemoveExtensionNotificationProc was called from within
                            a ExtensionNotificationUPP (ExtensionNotificationUPPs can't
                            call RemoveExtensionNotificationProc).
*/
EXTERN_API( OSErr )
RemoveExtensionNotificationProc (ExtensionNotificationUPP  extNotificationProc)             TWOWORDINLINE(0x7001, 0xAA7D);


/*
    InstallExtensionTableHandlerProc

    Installs an ExtensionTableHandlerUPP. Control is taken away from the system's default
    handler and the ExtensionTableHandlerUPP is responsible for all changes to the
    ExtensionTable (except for incrementing extElementIndex between extensions). This is
    always the first handler called with extNotificationBeforeFirst and
    extNotificationBeforeCurrent messages and the last handler called with
    extNotificationAfterLast and extNotificationAfterCurrent messages. extElementIndex
    is always incremented immediately after the ExtensionTableHandlerUPP is called with
    the extNotificationAfterCurrent message.
    
    There can only be one ExtensionTableHandler installed.
    
    Warning:    The only safe time to change what ExtensionElement is at
                ExtensionTable.extElements[extElementIndex] is when your
                ExtensionTableHandlerUPP is called with the extNotificationAfterCurrent
                message. You may change the ExtensionTable or the extElementIndex at other
                times, but you must ensure that the ExtensionElement at
                ExtensionTable.extElements[extElementIndex] stays the same.
                
    Note:       If the ExtensionTable or the contents of the folders included in the
                ExtensionTable are changed after installing an ExtensionTableHandler,
                RemoveExtensionTableHandlerProc cannot be called.

    Parameters:
        extMgrProc          The ExtensionTableHandlerUPP to install.
        extTable            A pointer to an ExtensionTableHandle where
                            InstallExtensionTableHandlerProc will return the current
                            ExtensionTableHandle. You don't own the handle itself and
                            must not dispose of it, but you can change the extElementIndex.
                            the extElementCount, and the ExtensionElements in the table.

    Results:
        noErr       0       The ExtensionTableHandlerUPP was installed.
        paramErr    -50     Another ExtensionTableHandlerUPP has already been installed.
        memFullErr  -108    Not enough memory to install the ExtensionTableHandlerUPP.
*/
EXTERN_API( OSErr )
InstallExtensionTableHandlerProc (ExtensionTableHandlerUPP  extMgrProc,
                                 ExtensionTableHandle * extTable)                           TWOWORDINLINE(0x7002, 0xAA7D);


/*
    RemoveExtensionTableHandlerProc

    Remove an ExtensionTableUPP. Control is passed back to the default handler.

    Parameters:
        extMgrProc          The ExtensionTableUPP to remove.

    Results:
        noErr       0       The ExtensionTableUPP was removed.
        paramErr    -50     This ExtensionTableUPP was not installed,
                            or the ExtensionTable no longer matches the
                            original boot ExtensionTable.
*/
EXTERN_API( OSErr )
RemoveExtensionTableHandlerProc (ExtensionTableHandlerUPP  extMgrProc)                      TWOWORDINLINE(0x7003, 0xAA7D);

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

#endif /* __START__ */

